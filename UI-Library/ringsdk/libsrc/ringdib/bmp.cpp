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
//ԭ�ļ�����bmp.cpp
//
//˵����BMPͼ�����빦��ʵ�֡�
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"

BOOL RingDIB::LoadBmp(LPCTSTR szFilename,RingDIB* lprdib,int iFlag,LPSTR lpszFormat)
{
	if(iFlag == DIB_GETFORMAT)
	{
		if(lpszFormat)
		{
			wsprintf(lpszFormat,"bmp");
			return TRUE;
		}
		else
			return FALSE;
	}

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
   HBITMAP hbm;
   BITMAP bm;
	ringFile rf;
	
	//BMP��ʽ̫�ࣨ��ɫ����ѹ����ѹ����ɨ���е��򲻵��򣩣��������
	//ֱ�ӵ���API���룬�����Լ�д�����ˡ�
   if(szFilename && !IS_INTRESOURCE(szFilename))
	{
		if(rf.Open(szFilename) && rf.Read(&bmfh,sizeof(BITMAPFILEHEADER)) && 
			rf.Read(&bmih,sizeof(BITMAPINFOHEADER)))
		{
  	  		if(bmfh.bfType != 0x4D42 || iFlag == DIB_QUERYSUPPORT)
				return (BOOL)(bmfh.bfType == 0x4D42);
				
			lprdib->Create(bmih.biWidth,bmih.biHeight>0?bmih.biHeight:-bmih.biHeight);
			hbm = (HBITMAP)LoadImage(NULL,szFilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			if(hbm==NULL)
	   		return FALSE;
		}
		else
     		return FALSE;
   }
   else
   {
      hbm = LoadBitmap(GetInstance(),szFilename);

   	if(hbm==NULL)
     		return FALSE;
      GetObject(hbm,sizeof(BITMAP),&bm);
	   lprdib->Create(bm.bmWidth,bm.bmHeight);
   }

   lprdib->m_bmif.bmiHeader.biWidth = lprdib->m_width;
   lprdib->m_bmif.bmiHeader.biHeight = lprdib->m_height;

   HDC hDC = GetDC(lprdib->m_hWnd);
   GetDIBits(hDC,hbm,0,lprdib->m_height,lprdib->m_Bits,&lprdib->m_bmif,DIB_RGB_COLORS);
	ReleaseDC(lprdib->m_hWnd,hDC);

	lprdib->SetSrc(0,0);
	lprdib->SetDest(0,0);

   DeleteObject(hbm);

   return TRUE;
}

HBITMAP RingDIB::GetBitmap()
{
	if(m_Bits == NULL)
		return NULL;

	m_bmif.bmiHeader.biWidth=m_width;
   m_bmif.bmiHeader.biHeight=m_height;

	HDC hDC = GetDC(m_hWnd);
	HBITMAP hbm = CreateCompatibleBitmap(hDC,m_width,m_height);
	if(hbm)
		SetDIBits(hDC,hbm,0,m_height,m_Bits,&m_bmif,DIB_RGB_COLORS);
	
	ReleaseDC(m_hWnd,hDC);

	return hbm;
}

BOOL RingDIB::Import(HBITMAP hbm)
{
	BITMAP bm;

	if(hbm==NULL)
     	return FALSE;

   GetObject(hbm,sizeof(BITMAP),&bm);
	Create(bm.bmWidth,bm.bmHeight);
   
   m_bmif.bmiHeader.biWidth=m_width;
   m_bmif.bmiHeader.biHeight=m_height;

   HDC hDC = GetDC(m_hWnd);
   GetDIBits (hDC,hbm,0,m_height,m_Bits,&m_bmif,DIB_RGB_COLORS);
   ReleaseDC(m_hWnd,hDC);

	SetSrc(0,0);
   return TRUE;
}

BOOL SaveBMP(LPCTSTR szFilename,RingDIB* rdib)
{
	BITMAPFILEHEADER bmfh;
   BITMAPINFOHEADER bmih;
   ringFile rf = szFilename;
   LPBYTE lpb,lpd,lps;
   
   if(rdib == NULL)
      return FALSE;

	int w = rdib->Width(),h = rdib->Height();
   ZeroMemory(&bmfh,sizeof(BITMAPFILEHEADER));
   bmfh.bfType = 0x4d42;
	//cw������4�ı���((x+3)&0b1100[=0xC]Ϊʵ�ָù��ܵ�һ��СС���ɣ�
	int cw = (int)(((w * 3) + 3) & 0xFFFFFFFC) ;

	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + cw * h;
   bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

   if(!rf.Create(RF_NEW))
      return FALSE;

   if(!rf.Write(&bmfh,sizeof(BITMAPFILEHEADER),TRUE))
   {
      rf.Close();
      return FALSE;
   }

   ZeroMemory(&bmih,sizeof(BITMAPINFOHEADER));
   bmih.biSize = sizeof(BITMAPINFOHEADER);
   bmih.biWidth = w;
   bmih.biHeight = h;
   bmih.biPlanes = 1;
   bmih.biBitCount = 24;
   bmih.biCompression = BI_RGB;

   if(!rf.Write(&bmih,sizeof(BITMAPINFOHEADER),TRUE))
      return FALSE;

   lpb = (LPBYTE)New(cw * h);
   if(lpb == NULL)
   {
      rf.Close();
      return FALSE;
   }

   lps = (LPBYTE)rdib->Data();
	lpd = lpb;

	if(lpd)
	{
//		   lpd = lpb + w * 3 * (h - 1);
//         int height_plus= w * 6;
      int n=0;   
		for(int j=0;j<h;j++)
		{
  			for(int i=0;i<w;i++)
			{
				lpd[0] = lps[0];
				lpd[1] = lps[1];
				lpd[2] = lps[2];
				lpd += 3;
				n++;
				lps += 4;
				if(n == 4)
					n = 0;
			}
			lpd += n;//lpb + j * cw;
			n = 0;
		}      
	}	
	
   if(!rf.Write(lpb,cw * h,TRUE))
   {
      Del(lpb);
      rf.Close();
      return FALSE;
   }

   Del(lpb);

   rf.Close();
   return TRUE;
}
