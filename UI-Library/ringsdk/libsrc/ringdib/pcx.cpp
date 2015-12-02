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
//ԭ�ļ�����pcx.cpp
//
//˵����PCXͼ�����빦�ܵ�ʵ�֡�����ɫ��16ɫͼ��Ľ�����δʵ�֣�
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"

#pragma pack(push,1)
typedef struct tagPCXHEAD
{
	char manufacture;	//always 0x0A
   char version;		//version number
   char encoding;		//should be 1
   char bits_per_pixel;	//color depth
   short xmin,ymin;	//ͼ�����Ͻ�����
   short xmax,ymax;	//ͼ����=(pcxHead.xmax-pcxHead.xmin)+1;�߶�ͬ��
   short hres,vres;	//�豸�ֱ��ʣ��ɺ���
   char palette[48];	//16ɫ�µ�ɫ��
   char reserved;
   char color_planes;	//��ɫ�档EGA 16:4,else ,include ��ɫ,=1
   short bytes_per_line;	//ɨ�����ֽ���������ż������!=xmax-xmin
   short palette_type;	//�Ҷ�ͼ��Ϊ1��ȫ��:2 Paintbrush����汾����
   short hscreensize;   //ΪPaintbrush IV/IV+������
   short vscreensize;	//����汾ӦΪNULL
   char filler[54];		//null
}PCXHEAD,*LPPCXHEADER;
#pragma pack(pop)

int xWidth,xHeight;
DWORD dwDataSize;
LPBYTE lpData;
BOOL bOK=FALSE;

int Uncode1(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph);
int Uncode16(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph);
int Uncode24(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph);
int Uncode256(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph);

BOOL LoadPCX(LPCTSTR szFilename,RingDIB* lprd,int iFlag,LPSTR lpszFormat)
{
	if(iFlag == DIB_GETFORMAT)
	{
		if(lpszFormat)
		{
			wsprintf(lpszFormat,"pcx");
			return TRUE;
		}
		else
			return FALSE;
	}
   PCXHEAD pcxHead;
   ringFile rf(szFilename);

   rf.Open();
   if(!rf.Read(&pcxHead,sizeof(PCXHEAD)) || pcxHead.manufacture != 0x0A || 
		pcxHead.version < 0x01 || pcxHead.version > 0x05)
      return FALSE;
	else if(iFlag == DIB_QUERYSUPPORT)
		return TRUE;

   xWidth=pcxHead.xmax-pcxHead.xmin+1;
   xHeight=pcxHead.ymax-pcxHead.ymin+1;

   dwDataSize = rf.Size()-sizeof(PCXHEAD);

   lpData=(LPBYTE)New(dwDataSize);
   if(lpData==NULL)
   {
   	//Errmsg("�ڴ�������.");
      rf.Close();
      return FALSE;
   }

   if(!rf.Read(lpData,dwDataSize))
   {
      //Errmsg("���󣺲��ܶ�ȡ�ļ�");
      lpData=(LPBYTE)Del(lpData);
      rf.Close();
      return FALSE;
   }
   rf.Close();
   switch(pcxHead.version)
   {
   	case 0x00:
      	Errmsg("�汾̫�ɣ��Ѳ�֧��.");
         lpData=(LPBYTE)Del(lpData);
         return FALSE;
      case 0x02:
      	bOK=Uncode16(lpData,(LPBYTE)lprd->Data(),&pcxHead);
         lpData=(LPBYTE)Del(lpData);
         return bOK;
      case 0x03:
      	bOK=Uncode1(lpData,(LPBYTE)lprd->Data(),&pcxHead);
         lpData=(LPBYTE)Del(lpData);
         return bOK;
      case 0x04:
      case 0x05:
      	if(!lprd->Create(xWidth,xHeight))
         {
         	//Errmsg("�ڴ�������.");
		      return FALSE;
         }

      	if(pcxHead.color_planes==3)
         	bOK=Uncode24(lpData,(LPBYTE)lprd->Data(),&pcxHead);
         else if(pcxHead.color_planes==1)
         	bOK=Uncode256(lpData,(LPBYTE)lprd->Data(),&pcxHead);
         lpData=(LPBYTE)Del(lpData);
         return bOK;
      default:
         //Errmsg("�汾����");
         lpData=(LPBYTE)Del(lpData);
         return FALSE;
   }
}

#if defined  __BORLANDC__
#pragma argsused
#endif
int Uncode1(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph)
{
	Errmsg("��ɫ�����в�֧��");
   return TRUE;
}

#if defined  __BORLANDC__
#pragma argsused
#endif
int Uncode16(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph)
{
	Errmsg("16ɫ�����в�֧��");
   return TRUE;
}

int Uncode24(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph)
{
	DWORD Index,dwScan;//fRes
   int i,j,k,l,count;

   SetCursor(LoadCursor(NULL,IDC_WAIT));

	//////////uncode date//////////
   i=j=k=Index=0;

   dwScan=(xHeight-1)*xWidth*4;
   int len=ph->bytes_per_line*4;

   while(j<xHeight)
   {
      for(l=2;l>=0;l--)
      {
         while(i<len)
         {
            if((lpData[Index]&0xC0)==0xC0)
   		   {
      				count=lpData[Index]&0x3F;
        				Index++;

            	for(k=0;k<count;k++)
	   	     	{
   	   	     	lpDest[dwScan+i+l]=lpData[Index];
      	   	  	i+=4;
	      	     	if(i>=len)
                  {
                  	Index--;
                     lpData[Index]-=(BYTE)(k+1);
                     Index--;
                     break;
                  }
		   	   }
   		   }
	   		else
	      	{
   	      	lpDest[dwScan+i+l]=lpData[Index];
	      	   i+=4;
		      }
            Index++;
            if(Index>=dwDataSize)
            	return FALSE;
         }
         i=0;
      }
      dwScan-=xWidth*4;
      i=0;
      j++;
   }

	return TRUE;
}

int Uncode256(LPBYTE lpData,LPBYTE lpDest,PCXHEAD* ph)
{
   DWORD fSize,Index,dwScan;//fRes
   LPBYTE lpPal;

   int i,j,k,count;

	fSize=dwDataSize;

   if(fSize<769)
   {
   	Errmsg("���ļ�����256ɫͼ���ļ�.--���㹻��ɫ����Ϣ");
      return FALSE;
   }

   if(lpData[fSize-769]!=0x000C)
   {
   	Errmsg("���ļ�����256ɫͼ���ļ�.--����ĵ�ɫ����Ϣ");
      return FALSE;
   }

   SetCursor(LoadCursor(NULL,IDC_WAIT));

   lpPal=lpData+fSize-768;

   //////////uncode date//////////
   i=j=k=Index=0;
   dwScan=(xHeight-1)*xWidth*4;
   int len=ph->bytes_per_line*4,s;

   while(j<xHeight)
   {
      while(i<len)
      {
	      if((lpData[Index]&0xC0)==0xC0)
  		   {
     			count=lpData[Index]&0x3F;
     			Index++;

            s=lpData[Index]*3+2;
           	for(k=0;k<count;k++)
   	     	{
     	   	  	lpDest[dwScan+i]=lpPal[s];
               i++;s--;
     	   	  	lpDest[dwScan+i]=lpPal[s];
               i++;s--;
     	   	  	lpDest[dwScan+i]=lpPal[s];
               i++;s+=2;
               lpDest[dwScan+i]=0;
               i++;
     	   	  	if(i>=len)
     	   	  		k=count;
	   	   }
  		   }
   		else
      	{
            s=lpData[Index]*3+2;
  	      	lpDest[dwScan+i]=lpPal[s];
            i++;s--;
     	   	lpDest[dwScan+i]=lpPal[s];
            i++;s--;
     	   	lpDest[dwScan+i]=lpPal[s];
            i++;
            lpDest[dwScan+i]=0;
     	   	i++;
	      }
  		   Index++;
         if(Index>=dwDataSize)
           	return FALSE;
      }
      dwScan-=xWidth*4;
      i=0;
      j++;
   }
   return TRUE;
}


