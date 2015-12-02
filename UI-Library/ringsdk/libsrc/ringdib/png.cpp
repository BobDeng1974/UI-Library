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
//ԭ�ļ�����png.cpp
//
//˵��������PNGͼ��
// 		PNG�Ľ���/�����õ��������ֳɵ����png.lib��zlib.lib���ļ�λ�ã�
//			free_lib\png��free_lib\zlib.ԭʼ��Ȩ��Ϣ������������Ŀ¼��
//			�Ĵ���Ͱ�Ȩ��Ϣ��
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"
#include "png.h"

void ImportRGB(LPBYTE lpd,LPBYTE* lpb,int width,int height);
void ImportRGBA(LPBYTE lpd,LPBYTE* lpb,int width,int height);
void ImportGray(LPBYTE lpd,LPBYTE* lpb,int width,int height);
void ImportGrayA(LPBYTE lpd,LPBYTE* lpb,int width,int height);

BOOL LoadPNG(LPCTSTR szFilename,RingDIB* rdib,int iFlag,LPSTR lpszFormat)
{
	if(iFlag == DIB_GETFORMAT)
	{
		if(lpszFormat)
		{
			wsprintf(lpszFormat,"png");
			return TRUE;
		}
		else
			return FALSE;
	}
	
	ringFile rf(szFilename);
   rf.Open();

	int nread = 8;
	BYTE buffer[8];
	
	if(!rf.Read(buffer,nread,TRUE))
		return FALSE;	
	
	//����ļ�ͷ
	if(png_sig_cmp(buffer,0,nread) != 0)
		return FALSE;
	else if(iFlag == DIB_QUERYSUPPORT)
		return TRUE;

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if(png_ptr == NULL)
		return FALSE;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	
	if(info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr,0,0);
		return FALSE;
	}
	
	//�����ջ�ֳ�
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,0);
		return FALSE;
	}
	
	png_ptr->io_ptr = (png_voidp)rf.Handle();
	//�ָ��ļ�ָ��
	rf.Seek(0);
	//ͼ�����
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	
	//���������ͼ������
	if(rdib->Create(png_get_image_width(png_ptr,info_ptr),png_get_image_height(png_ptr,info_ptr)))
	{
		LPBYTE* lpb = png_get_rows(png_ptr,info_ptr);
		LPBYTE lpd = (LPBYTE)rdib->Data();

		//��ȡ��ɫ��Ϣ
		switch(png_get_color_type(png_ptr,info_ptr))
		{
		case PNG_COLOR_TYPE_RGB:
			ImportRGB(lpd,lpb,rdib->Width(),rdib->Height());
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			ImportRGBA(lpd,lpb,rdib->Width(),rdib->Height());
			break;
		case PNG_COLOR_TYPE_GRAY:
			ImportGray(lpd,lpb,rdib->Width(),rdib->Height());
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			ImportGrayA(lpd,lpb,rdib->Width(),rdib->Height());
			break;
		}
	}
	//�ͷ��ڴ�
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	
	rf.Close();
	return TRUE;
}

BOOL SavePNG(LPCTSTR szFilename,RingDIB* rdib)
{
	if(rdib)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void ImportRGB(LPBYTE lpd,LPBYTE* lpb,int width,int height)
{
	int nwCnt = width * sizeof(COLORREF);
	int nw = width * 3;
	LPBYTE lpLine = lpd + (height - 1) * nwCnt;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < nw; j += 3)
		{
			*lpLine ++ = lpb[i][j + 2];	// blue
			*lpLine ++ = lpb[i][j + 1];	// green
			*lpLine ++ = lpb[i][j];		// red
			*lpLine ++;
		}
		
		lpLine -= (nwCnt << 1);
	}
}

void ImportRGBA(LPBYTE lpd,LPBYTE* lpb,int width,int height)
{
	int nwCnt = width * sizeof(COLORREF);
	LPBYTE lpLine = lpd + (height - 1) * nwCnt;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < nwCnt; j += 4)
		{
			*lpLine ++ = lpb[i][j + 2];	// blue
			*lpLine ++ = lpb[i][j + 1];	// green
			*lpLine ++ = lpb[i][j];		// red
			*lpLine ++ = lpb[i][j + 3];	// alpha
		}
		
		lpLine -= (nwCnt << 1);
	}
}

void ImportGray(LPBYTE lpd,LPBYTE* lpb,int width,int height)
{
	BYTE cr;
	COLORREF* lpLine = (COLORREF*)lpd;
	lpLine += (height - 1) * width;
	
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j ++)
		{
			cr = lpb[i][j];
			*lpLine ++ = RGB(cr,cr,cr);			
		}		
		lpLine -= (width << 1);
	}
}

void ImportGrayA(LPBYTE lpd,LPBYTE* lpb,int width,int height)
{
	BYTE cr;
	int nwCnt = width << 1;
	COLORREF* lpLine = (COLORREF*)lpd;
	lpLine += (height - 1) * width;
	
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < nwCnt; j += 2)
		{
			cr = lpb[i][j];
			*lpLine ++ = (RGB(cr,cr,cr) | (((COLORREF)lpb[i][j+1]) << 24));			
		}		
		lpLine -= (width << 1);
	}
}
