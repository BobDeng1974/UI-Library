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
//ԭ�ļ�����jpg.cpp
//
//˵��������JPEGͼ��
// 		JPEG�Ľ���/�����õ���һ���ֳɵ����JPEG.LIB���ļ�λ�ã�
//			free_lib\jpeg��������Ҫ��лJPEG ����С�顣
//			special thanks to the work of Tom Lane, Philip Gladstone,
//			Luis Ortiz, Jim Boucher, Lee Crocker, Julian Minguillon,
//			George Phillips, Davide Rossi,Ge' Weijers, and other members
//			of the Independent JPEG Group.
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"

#ifdef __cplusplus
extern "C" {
#endif
extern BYTE * JpegFileToRGB(LPSTR fileName,UINT *width,UINT *height);
extern BOOL GetJPGSize(LPSTR fileName,UINT *width,UINT *height);
extern BOOL BGRFromRGB(LPBYTE dest,BYTE *src, UINT widthPix, UINT height);
extern BOOL RGBToJpegFile(LPSTR fileName,BYTE *dataBuf,UINT widthPix,UINT height,BOOL bRGB,int quality);
#ifdef __cplusplus
}
#endif

BOOL LoadJPG(LPCTSTR szFilename,RingDIB* rdib,int iFlag,LPSTR lpszFormat)
{
	if(iFlag == DIB_GETFORMAT)
	{
		if(lpszFormat)
		{
			wsprintf(lpszFormat,"jpeg");
			return TRUE;
		}
		else
			return FALSE;
	}

	LPBYTE tmp;
   UINT w,h;

	tmp=JpegFileToRGB((LPSTR)szFilename,&w,&h);
   if(tmp==NULL)
   	return FALSE;
	else if(iFlag == DIB_QUERYSUPPORT)
	{
		delete []tmp;
		return TRUE;
	}

   rdib->Create(w,h);
   BGRFromRGB((LPBYTE)rdib->Data(),tmp,w,h);

   delete []tmp;

   rdib->SetSrc(0,0);
   rdib->SetDest(0,0);

   return TRUE;
}

BOOL SaveJPG(LPCTSTR szFilename,RingDIB* rdib,BOOL bRGB/*=NULL*/,int quality/*=50*/)
{
	if(rdib)
	{
		int w = rdib->Width(),h = rdib->Height();
		LPBYTE lpb = (LPBYTE)New(w * h * 3);
		//int size = (rdib->Width() * rdib->Height()) << 2;
		LPBYTE lpd,lps = (LPBYTE)rdib->Data();

		if(lpb)
		{
		   lpd = lpb + w * 3 * (h - 1);
         int height_plus= w * 6;
         
			for(int j=0;j<h;j++)
			{
   			for(int i=0;i<w;i++)
				{
					lpd[0] = lps[2];
					lpd[1] = lps[1];
					lpd[2] = lps[0];
					lpd += 3;
					lps += 4;
				}
				lpd -= height_plus;
			}      
		}
		BOOL bok = RGBToJpegFile((LPSTR)szFilename,lpb,rdib->Width(),rdib->Height(),bRGB,quality);

		Del(lpb);
		return bok;
	}
	else
		return FALSE;
}
