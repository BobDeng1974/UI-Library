/**********************************************************************
//
//
//        ##########                          ######  #########   # ######    #
//      #############                      ########### ######### #########  ###
//     ######## # ###  ##                 ############# ##    ## #####  # ####
//     ####  ##    ## ###                 ###     ### # #      #####   #####
//          #     ###  #            #     ##       ##  ##      ##     ###
//         ## ###### ##      ##  ####    ####          #       #     ##
//       #########  ###  ## ### #######   ######      ##      ##    ###
//      ######      ##  ######  ##  ##       ####     #      ##     ####
//     #######     ##   ###### ##  ###          ##   ##     ###    ######
//     #########   ##  ###### ## ######         ### ##    ###      #  #####
//    ##    ###### ####### ### #### ##  ## #######  ########      ##    ####
//    ##      #### ###  #  ### ### ##  ##########   ######       ##      ####
//   ##         ##                ##   #########    ####         #         ##
//               #              ###
//                              ##
//                             ###
//                             ##
//
//
//							RingSDK��ý����� ringdib.lib
//���ߣ��ٷ�
//
//�汾��1.0
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����gif.cpp
//
//˵����GIFͼ�������ʵ�֡�
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"

#pragma pack(push,1)
typedef struct _gifhead{
	char szID[6];		//GIF87a or GIF89a
   WORD wWidth;
   WORD wHeight;
   BYTE bFlag;
   BYTE bgColor;		//����ɫ
	//char w_mod_h;     //ͼ���ݺ�ߴ��,=(w_mod_h+15)/64
//w_mod_h������δ�õ�����Ϊ�ֽڶ�������⣬�ṹ�м��ϸ��ֶ�
//�������ݽ���������ȥ�����ֶΣ����򵥽���������ȡ������
//ָ��+1�������ֶΡ�
}GIFHEADER,*LPGIFHEADER;

typedef struct _localimageinfo{	//ͼ�����ݿ���Ϣ
   WORD wLeft;                   //ͼ����λ������
   WORD wTop;                    //ͼ����λ������
	WORD wWidth;                  //ͼ���
   WORD wHeight;                 //ͼ���
   BYTE bFlag;                   //ѹ���ֶ�
}LOCALINFO,*LPLOCALINFO;

typedef void(*OUTDATA)();

OUTDATA outdataFunc;					//LZW���������������ָ�룬������������������

LPLOCALINFO lInfo;
LPBYTE m_SBits=NULL;					//����ͼ������

LPBYTE gPal=NULL,lPal=NULL;		//gPal:ȫ�ֵ�ɫ�壬lPal:�ֲ���ɫ��
LPBYTE lpGifData=NULL,lpDelData; //lpDelData,=�����lpGifData,lpGifData��Ҫ�䶯��so ����ʱDel(lpDelData)

DWORD aniGifThread(RingDIB* rdib);
void DrawLocalPic(RingDIB *rdib,COLORREF* dest,COLORREF* bits,LPANIGIFPIC lpAniGifPic);

////////////////-----���¶���ΪLZW��������---------////////////////////////////
typedef struct tagLZW_DATA
{
	short base;
	BYTE newcode;
	short length;
}LZW_DATA,*LPLZW_DATA;
#pragma pack(pop)

int first_code, curr_bit_size, bit_overflow;
int bit_pos, data_pos, data_len, entire, code;
int clear_code, data_length, cnt, bit_size;
LPLZW_DATA lpstr=NULL;
LPBYTE lpLzw_Data=NULL;
int interlace,trans_index=-1;				//����ɨ������͸��ɫ����
int curr_scan;		//��ǰɨ�����
int nLeft,nTop,nWidth,nHeight;

//ringFile x;

void clear_table(void);
void get_code(void);
void get_data(int num);
void output_data(void);					//��̬GIF���������������
void output_localdata(void);        //��̬GIF����ͼ����������������
void DecodeLZW(void);
void resetVar();		//����ȫ�ֱ���
////////////////-----LZW���붨�����---------////////////////////////////

BOOL LoadGIF(LPCTSTR szFilename,RingDIB* lprd,int iFlag,LPSTR lpszFormat)
{
	if(iFlag == DIB_GETFORMAT)
	{
		if(lpszFormat)
		{
			wsprintf(lpszFormat,"gif");
			return TRUE;
		}
		else
			return FALSE;
	}
	
	if(szFilename == NULL || lprd == NULL)
		return FALSE;

	GIFHEADER gifHeader;
	LPANIGIFPIC lpAniGifPic = NULL;
   LPLOCALGIFINFO lpGifInfo = lprd->GetLocalGifInfo();
	if(lpGifInfo == NULL)
		return FALSE;
	
//x="c:\\dibdebug.txt";
//x.Create(RF_NEW);

	ringFile rf(szFilename);
   rf.Open();
	outdataFunc=output_data;

   if(!rf.Read(&gifHeader,sizeof(GIFHEADER)) || gifHeader.szID[0] != 'G' || 
		gifHeader.szID[1] != 'I' || gifHeader.szID[2] != 'F' || 
		gifHeader.szID[3] != '8' || gifHeader.szID[5] != 'a')
      return FALSE;
	else if(iFlag == DIB_QUERYSUPPORT)
		return TRUE;

   if(!lprd->Create(gifHeader.wWidth,gifHeader.wHeight))
   {
     	//Errmsg("�ڴ������󣬲��ܴ���ͼ��");
      rf.Close();
	   return FALSE;
   }
	
   nWidth=gifHeader.wWidth;
   nHeight=gifHeader.wHeight;

   DWORD dwFileSize=rf.Size()-sizeof(GIFHEADER);
   lpGifData=(LPBYTE)New(dwFileSize);
   if(lpGifData==NULL)
   {
   	rf.Close();
   	return FALSE;
   }

   if(!rf.Read(lpGifData,dwFileSize))
   {
   	rf.Close();
   	return FALSE;
   }
   rf.Close();
	
	lpDelData=lpGifData;

	lpGifData ++;
//int ps;
   
   if((gifHeader.bFlag&0x80))	//��ȫ�ֵ�ɫ��
   {
   	int palsize=1<<((gifHeader.bFlag&0x07)+1);
		palsize=(palsize<<1)+palsize;       //palsize*3
      gPal=(LPBYTE)lpGifData;
      lpGifData+=palsize;
//ps=palsize;
   }

   if(gPal!=NULL)
   {
   	int bg=gifHeader.bgColor*3+2;
   	lprd->Fill(gPal[bg],gPal[bg-1],gPal[bg-2]);
      lpGifInfo->bgC = lprd->Data()[0];
   }

   while((lpGifData-lpDelData)<=(int)dwFileSize)
   {
   	switch(*lpGifData)
      {
         case 0x2C:			//ͼ��������
         {
         	lpGifData++;
            lInfo=(LPLOCALINFO)lpGifData;

         	lpGifData+=9;
            if((lInfo->bFlag&0x80))	//�оֲ���ɫ��
			   {
			   	int lpalsize=1<<((lInfo->bFlag&0x07)+1);
					lpalsize=(lpalsize<<1)+lpalsize;       //palsize*3
			      lPal=(LPBYTE)lpGifData;
			      lpGifData+=lpalsize;
			   }
            else if(gPal!=NULL)
            	lPal=NULL;

            if(lInfo->bFlag&0x40)
            	interlace = 8;
				else
					interlace = 1;

            curr_scan=1;

            m_SBits=(LPBYTE)lprd->Data();

            if(lpAniGifPic)
            {
            	lpAniGifPic->x = lInfo->wLeft;
               lpAniGifPic->y = lInfo->wTop;
               lpAniGifPic->width = lInfo->wWidth;
               lpAniGifPic->height = lInfo->wHeight;
               if(lpAniGifPic->curr!=NULL)			//��һ��ͼ��������,lprd->m_aniGifPicʼ��ָ����ͼ��currӦΪNULL
               {
						Del(lpDelData);
		            resetVar();
      		      return TRUE;
               }
               lpAniGifPic->curr = (COLORREF*)New(lInfo->wWidth*lInfo->wHeight*4);
               if(lpAniGifPic->curr == NULL)
               {
						Del(lpDelData);
		            resetVar();
      		      return TRUE;
               }
               m_SBits = (LPBYTE)lpAniGifPic->curr;
               outdataFunc = output_localdata;

               if(lpAniGifPic->keycolor != 0xFFFFFFFF)
               {
                  LPBYTE lpPal;
               	if(lPal!=NULL)
						 	lpPal = lPal;
						else
							lpPal = gPal;

                  if(lpAniGifPic->keycolor == 1)
                     lprd->SetColorKey((lpPal[trans_index]<<16)+(lpPal[trans_index-1]<<8)+lpPal[trans_index-2]);

                  lpAniGifPic->keycolor=(lpPal[trans_index]<<16)+(lpPal[trans_index-1]<<8)+lpPal[trans_index-2];
               }
            }
            DecodeLZW();

				//Del(lpDelData);
            //resetVar();
            //return TRUE;
            break;
			}
         case 0x21:
         {
         	lpGifData++;
            switch(*lpGifData)
            {
               case 0xFF:				//Ӧ�ó�����չ
               	lpGifData+=13;
                  break;
            	case 0xF9:				//ͼ�������չ
               {
                  BYTE docase;
                  BOOL trans;
                  WORD delay;

                  lpGifData+=2;
                  docase=(BYTE)(((*lpGifData)>>2)&0x07);
                  if(docase == 0x3)
						   lpGifInfo->bNeedBak = TRUE;

                  trans=(*lpGifData)&1;
                  lpGifData++;
                  delay=*(WORD*)lpGifData;
                  lpGifData+=2;

                  if(trans)
						   trans_index=(int)*lpGifData*3+2;

                  if(delay==0 && lpAniGifPic == NULL)		//����ʱ������ͼ��
	               {
                     if(trans && gPal != NULL)
                       	lprd->SetColorKey((gPal[trans_index]<<16)+(gPal[trans_index-1]<<8)+gPal[trans_index-2]);
                  }
                  else
                  {
		         		lpAniGifPic = lprd->AddAniGifPic(lpAniGifPic,docase,delay,trans);
							if(lpAniGifPic == NULL)
                  	{
		                 	if(lpAniGifPic == NULL)
                     	{
                      		lpGifData+=2;
                       		break;
                     	}                     	
                     }                     
						}
                  lpGifData+=2;
						break;
               }
               case 0xFE:				//ע�Ϳ�
               	for(int i=0;i<254;i++)
                  {
                     lpGifData++;
                  	if(*lpGifData==0)
                     {
                     	lpGifData++;
                        break;
                     }
                  }
                  break;
            }
            break;
         }
         case 0x3B:						//����������
         {
         	Del(lpDelData);
            resetVar();

            if(lpAniGifPic)
            {
            	//������ͷβ����
					lpAniGifPic->next = lprd->GetAniGifPic();
					lprd->GetAniGifPic()->prev = lpAniGifPic;
					if(lpGifInfo->AniThread == NULL)
						lpGifInfo->AniThread = new RingThread;
					if(lpGifInfo->AniThread)
						lpGifInfo->AniThread->Start((THREADPROC)aniGifThread,(LONG)lprd);
            }
		      return TRUE;
         }
         default:
         	lpGifData++;
            break;
      }
   }
   Del(lpDelData);
   resetVar();
	return FALSE;
}

/////////////////////----  LZW����  -------///////////////////////////////
void clear_table(void)
{
	first_code = clear_code + 2;
	curr_bit_size = bit_size + 1;
	bit_overflow = 0;
}

void get_code(void)
{
	if(bit_pos+curr_bit_size>8)
   {
		if(data_pos>=data_len)
      {
      	data_len=*lpGifData;
         lpGifData++;
         data_pos=0;
      }
		entire=(*lpGifData<<8)+entire;
      lpGifData++;
		data_pos ++;
	}
	if(bit_pos+curr_bit_size>16)
   {
		if(data_pos>=data_len)
      {
      	data_len=*lpGifData;
         lpGifData++;
         data_pos=0;
      }
		entire=(*lpGifData<<16)+entire;
      lpGifData++;
		data_pos ++;
	}

	code=(entire>>bit_pos)&((1<<curr_bit_size)-1);

	if(bit_pos+curr_bit_size>8)
		entire>>=8;
	if(bit_pos+curr_bit_size>16)
		entire>>=8;
	bit_pos=(bit_pos+curr_bit_size)%8;
	if(bit_pos==0)
   {
		if(data_pos>=data_len)
      {
      	data_len=*lpGifData;
         lpGifData++;
         data_pos=0;
      }
		entire=*lpGifData;
      lpGifData++;
		data_pos ++;
	}
}

void get_data(int num)
{
	if(num<clear_code)
	{
		data_length = 1;
		lpLzw_Data[0]=lpstr[num].newcode;
	}
	else
	{
		cnt=lpstr[num].length;
		data_length=cnt;
		while(cnt>0)
		{
			cnt--;
			lpLzw_Data[cnt]=lpstr[num].newcode;
			num=lpstr[num].base;
		}
	}
}

void output_data(void)
{
   LPBYTE lpPal;
   int pos;
   int temp;

   if(lPal!=NULL)
	 	lpPal = lPal;
   else
		lpPal = gPal;

   pos=(nLeft+nTop*nWidth)<<2;

//x.Write(lpPal,256*3);

	for(int i=0; i<data_length;i++)
	{
      temp=lpLzw_Data[i]*3+2;
      //if(temp!=trans_index)
      //{
			m_SBits[pos]=lpPal[temp];
   	   pos++;temp--;
      	m_SBits[pos]=lpPal[temp];
	      pos++;temp--;
   	   m_SBits[pos]=lpPal[temp];
      	pos++;
	      m_SBits[pos]=0;
   	   pos++;
      //}
      //else
      	//pos+=4;

		nLeft++;

		if(nLeft>=lInfo->wLeft+lInfo->wWidth)
		{
			nLeft = lInfo->wLeft;
			nTop-=interlace;
         pos=(nLeft+nTop*nWidth)<<2;

			if(nTop<lInfo->wTop)
			{
            switch(interlace)
            {
              	case 8:
                  if(curr_scan==1)
                  {
                    	nTop=lInfo->wTop+nHeight-5;		//-5=-1-4
                     pos=(nLeft+nTop*nWidth)<<2;
                     curr_scan++;
                  }
                  else
                  {
                     interlace=4;
                   	nTop=lInfo->wTop+nHeight-3;		//-3=-1-2
                     pos=(nLeft+nTop*nWidth)<<2;
                  }
						break;
               case 4:
                 	interlace=2;
                 	nTop=lInfo->wTop+nHeight-2;		//-2=-1-1
                  pos=(nLeft+nTop*nWidth)<<2;
                	break;
            }
         }
		}
	}
//x.Close();
}

void output_localdata(void)
{
   LPBYTE lpPal;
   int pos;
   int temp;

   if(lPal!=NULL)
	 	lpPal = lPal;
   else
		lpPal = gPal;

   pos=(nLeft+nTop*lInfo->wWidth)<<2;

	for(int i=0; i<data_length;i++)
	{
      temp=lpLzw_Data[i]*3+2;
		m_SBits[pos]=lpPal[temp];
      pos++;temp--;
      m_SBits[pos]=lpPal[temp];
      pos++;temp--;
      m_SBits[pos]=lpPal[temp];
      pos++;
      m_SBits[pos]=0;
      pos++;

		nLeft++;

		if(nLeft>=lInfo->wWidth)
		{
			nLeft = 0;
			nTop-=interlace;
         pos=(nTop*lInfo->wWidth)<<2;

			if(nTop<0)
			{
            switch(interlace)
            {
              	case 8:
                  if(curr_scan==1)
                  {
                    	nTop=lInfo->wHeight-5;	//-5=-1-4
                     pos=(nTop*lInfo->wWidth)<<2;
                     curr_scan++;
                  }
                  else
                  {
                     interlace=4;
                   	nTop=lInfo->wHeight-3;
                     pos=(nTop*lInfo->wWidth)<<2;
                  }
						break;
               case 4:
                 	interlace=2;
                 	nTop=lInfo->wHeight-2;
                  pos=(nTop*lInfo->wWidth)<<2;
                	break;
            }
         }
		}
	}
}

void DecodeLZW(void)
{
	int old;
   BYTE btData[4096];		//LZW����볤12BIT��=0xFFF=4095
   LZW_DATA lzw_data[4096];

   lpstr=lzw_data;
   lpLzw_Data=btData;

	bit_size=*lpGifData;
   lpGifData++;
	clear_code=1<<bit_size;

	//��ʼ���������ݱ�
   for(short i=0; i<clear_code;i++)
	{
		lzw_data[i].base=-1;
		lzw_data[i].newcode=(unsigned char)i;
		lzw_data[i].length=1;
   }

	bit_pos=0;
	data_len=*lpGifData;
   lpGifData++;
   data_pos=0;
   entire=*lpGifData;
   lpGifData++;
   data_pos ++;
	data_length=0;

   if(outdataFunc==output_data)
   {
	   nLeft=lInfo->wLeft;
   	nTop=lInfo->wTop+nHeight-1;
   }
   else
   {
	   nLeft=0;
   	nTop=lInfo->wHeight-1;
   }

   //���뿪ʼ
   clear_table();
	get_code();
   if(code==clear_code)
		get_code();
   get_data(code);
	(*outdataFunc)();
	old = code;

	while(TRUE)
	{
		get_code();

		if(code == clear_code)        //�����
		{
			clear_table();
			get_code();
			get_data(code);
         (*outdataFunc)();

			old = code;
		}
		else if(code==clear_code+1)	//������
      {
      	lpGifData--;
      	break;
      }
      else if(code < first_code)
		{
			get_data(code);
			(*outdataFunc)();

			if(bit_overflow == 0)
         {
				lpstr[first_code].base = (short)old;
				lpstr[first_code].newcode = lpLzw_Data[0];
            lpstr[first_code].length = (short)(lpstr[old].length+1);
            first_code++;
            if(first_code==(1<<curr_bit_size))
            	curr_bit_size ++;
            if(curr_bit_size==13)
            {
					curr_bit_size = 12;
					bit_overflow = 1;
				}
			}

			old = code;
      }
		else
		{
			get_data(old);
			lpLzw_Data[lpstr[old].length]=lpLzw_Data[0];
         data_length++;

         if(bit_overflow==0)
         {
				lpstr[first_code].base=(short)old;
            lpstr[first_code].newcode=lpLzw_Data[0];
            lpstr[first_code].length=(short)(lpstr[old].length+1);
            first_code++;
            if(first_code==(1<<curr_bit_size))
            	curr_bit_size++;
            if(curr_bit_size==13)
            {
					curr_bit_size = 12;
					bit_overflow = 1;
				}
         }
			(*outdataFunc)();
			old = code;
      }
   }
}
//////////////////////--------LZW �������-------------//////////////////////

void resetVar()		//����ȫ�ֱ���
{
	lInfo=NULL;
	m_SBits=NULL;

	gPal=NULL;
   lPal=NULL;
	lpGifData=NULL;
   lpDelData=NULL;

   trans_index=-1;
   outdataFunc=output_data;

	lpstr=NULL;
	lpLzw_Data=NULL;
}

DWORD aniGifThread(RingDIB* rdib)			//����GIF��ʾ��������̣߳�
{
   COLORREF *ani_Bits = NULL;
   RingMSTimer timer;
	LPLOCALGIFINFO lpGifInfo = rdib->GetLocalGifInfo();
	LPANIGIFPIC lpAniGifPic = rdib->GetAniGifPic();
   if(lpGifInfo == NULL)
		return 0;

	if(lpGifInfo->bNeedBak)
   {
   	ani_Bits = (COLORREF*)New(rdib->Width() * rdib->Height() * sizeof(COLORREF));
      if(ani_Bits)
         DrawLocalPic(rdib,ani_Bits,rdib->Data(),lpAniGifPic);
   }

   lpGifInfo->bDibReady = TRUE;

	while(rdib->GetAniGifPic() && lpGifInfo->AniThread && lpGifInfo->AniThread->IsRunning())
   {
      if(lpGifInfo->bActive && lpGifInfo->bDibReady)
	   {
   	   DrawLocalPic(rdib,rdib->Data(),NULL,lpAniGifPic);
         lpGifInfo->funcFilter();
   	      
			if(rdib->GetDestDIB())
			{
				rdib->DrawTo(rdib->GetDestDIB());
				rdib->GetDestDIB()->Draw();
			}
			else
				rdib->Draw();

     		timer.Reset();
	      lpGifInfo->bDibReady = FALSE;
   	}
      else if(timer.PassedTime() >= 10 * lpAniGifPic->delay)
	   {
   	   switch(lpAniGifPic->docase)
      	{
            case 0:                //����ͼ�󣬼�¼Ϊ����ͼ��
            case 1:
              	if(ani_Bits)
	              	DrawLocalPic(rdib,ani_Bits,NULL,lpAniGifPic);
               break;
         	case 2:                //�ָ��ɱ���ɫ
           		rdib->FillRect(lpGifInfo->bgC,lpAniGifPic->x,lpAniGifPic->y,lpAniGifPic->width,lpAniGifPic->height);
					break;
   	      case 3:						//�ָ�ǰһͼ��
               if(ani_Bits)
                  DrawLocalPic(rdib,rdib->Data(),ani_Bits,lpAniGifPic);
               break;
      	   default:
              	break;
	      }
   	   if(lpAniGifPic->next)
      	{
          	lpAniGifPic = lpAniGifPic->next;
            lpGifInfo->bDibReady = TRUE;
	         timer.Reset();   	      
      	}
	      timer.Count();
   	}
      Sleep(1);
   }
   Del(ani_Bits);
   return 100;
}

/////////////////////////
//
//��bitsͼ����Ƶ�dest,bits=NULL,��Դͼ��Ϊ����ͼ���е�ǰͼ��
//����dest��bitsΪ����ͬ��Сͼ������֮һΪ��¼����ͼ��(docase=0 or 1)
//
/////////////////////////
void DrawLocalPic(RingDIB *rdib,COLORREF* dest,COLORREF* bits,LPANIGIFPIC lpAniGifPic)
{
	int i,j,y;
   COLORREF *lpmb,*lpl,*lpmbb,*lplb;

   //����þ���
  	y = lpAniGifPic->y + lpAniGifPic->height;

   lpmb = lpmbb = dest + (rdib->Height() - y) * rdib->Width() + lpAniGifPic->x;

   if(bits == NULL)
   {
      lpl = lplb = lpAniGifPic->curr;

      for(j=0;j<lpAniGifPic->height;j++)
	   {
			for(i=0;i<lpAniGifPic->width;i++)
			{
        		if(lpAniGifPic->keycolor==0xFFFFFFFF || *lpl != lpAniGifPic->keycolor)
					*lpmb = *lpl;
	     		lpmb++;lpl++;
   	   }
			lplb += lpAniGifPic->width;
			lpmbb += rdib->Width();
	  		lpl = lplb;
         lpmb = lpmbb;
  		}
   }
   else
   {
   	lpl = lplb = bits + (rdib->Height() - y) * rdib->Width() + lpAniGifPic->x;
      for(j=0;j<lpAniGifPic->height;j++)
		{
      	for(i=0;i<lpAniGifPic->width;i++)
         {
				*lpmb++ = *lpl++;	     		
         }
         lplb += rdib->Width();
			lpmbb += rdib->Width();
	  		lpl = lplb;
         lpmb = lpmbb;
      }
   }
}

void NoneFilter()
{
}


