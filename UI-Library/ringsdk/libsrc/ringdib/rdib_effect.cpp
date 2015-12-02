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
//ԭ�ļ�����rdib_effect.cpp
//
//˵����ͼ��Ч������
//
//Ч����GrayScale -- ��ͼ��ת��Ϊ�Ҷ�ͼ��(ע�⣺һ��ת��ͼ����޷��ظ���ɫ��
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"
   
BOOL dibFilterEFFECT::GrayScale()
{
	if(!m_rdib->Data())
   	return FALSE;

   LPBYTE pRed, pGrn, pBlu,pBuf=(LPBYTE)m_rdib->Data();
	UINT loop = m_rdib->Width() * m_rdib->Height();
   int lum;

   pRed=pBuf++;pGrn=pBuf++;pBlu=pBuf;

	for (UINT i=0;i<loop;i++)
   {
		// luminance
		lum = (int)(.299 * (double)(*pRed) + .587 * (double)(*pGrn) + .114 * (double)(*pBlu));

		*pRed = (BYTE)lum;
		*pGrn = (BYTE)lum;
		*pBlu = (BYTE)lum;
      pRed+=4;
      pGrn+=4;
      pBlu+=4;
	}
	return TRUE;
}

BOOL dibFilterEFFECT::Negative()
{
	if(!m_rdib->Data())
   	return FALSE;

   LPBYTE pRed, pGrn, pBlu,pBuf=(LPBYTE)m_rdib->Data();
	UINT loop = m_rdib->Width() * m_rdib->Height();
   
   pRed=pBuf++;pGrn=pBuf++;pBlu=pBuf;

	for (UINT i=0;i<loop;i++)
   {
		*pRed ^= 0xFF;
		*pGrn ^= 0xFF;
		*pBlu ^= 0xFF;
      pRed+=4;
      pGrn+=4;
      pBlu+=4;
	}
	return TRUE;
}

BOOL dibFilterEFFECT::AdjustContrast(int Percent)
{
	if(!m_rdib->Data())
   	return FALSE;
	
	if(Percent == 100)
		return TRUE;	//δ��������ֱ�ӷ���

   LPBYTE pRed, pGrn, pBlu,pBuf=(LPBYTE)m_rdib->Data();
	UINT loop = m_rdib->Width() * m_rdib->Height();
   
   pRed=pBuf++;pGrn=pBuf++;pBlu=pBuf;
	int r,g,b;

	for (UINT i=0;i<loop;i++)
	{
		r = min(128 + ((*pRed - 128) * Percent/100), 255);
      g = min(128 + ((*pGrn - 128) * Percent/100), 255);
      b = min(128 + ((*pBlu - 128) * Percent/100), 255);
      r = max(r, 0);
      g = max(g, 0);
      b = max(b, 0);
      
		*pRed = (BYTE)r;
		*pGrn = (BYTE)g;
		*pBlu = (BYTE)b;
		
		pRed+=4;
      pGrn+=4;
      pBlu+=4;
	}
	return TRUE;
}

BOOL dibFilterEFFECT::AdjustBright(int step)
{
	if(!m_rdib->Data())
   	return FALSE;
	
	if(step == 0)
		return TRUE;	//δ��������ֱ�ӷ���

   LPBYTE pRed, pGrn, pBlu,pBuf=(LPBYTE)m_rdib->Data();
	UINT loop = m_rdib->Width() * m_rdib->Height();
   
   pRed=pBuf++;pGrn=pBuf++;pBlu=pBuf;
	int r,g,b;

	for (UINT i=0;i<loop;i++)
	{
		r = min(*pRed + step, 255);
      g = min(*pGrn + step, 255);
      b = min(*pBlu + step, 255);
      r = max(r, 0);
      g = max(g, 0);
      b = max(b, 0);
      
		*pRed = (BYTE)r;
		*pGrn = (BYTE)g;
		*pBlu = (BYTE)b;
		
		pRed+=4;
      pGrn+=4;
      pBlu+=4;
	}
	return TRUE;
}

BOOL dibFilterEFFECT::AdjustBright_Contrast(int bright,int contrast)
{
	if(!m_rdib->Data())
   	return FALSE;
	
	if(contrast == 100 && bright == 0)
		return TRUE;	//δ��������ֱ�ӷ���

   LPBYTE pRed, pGrn, pBlu,pBuf=(LPBYTE)m_rdib->Data();
	UINT loop = m_rdib->Width() * m_rdib->Height();
   
   pRed=pBuf++;pGrn=pBuf++;pBlu=pBuf;
	int r,g,b;

	for (UINT i=0;i<loop;i++)
	{
		r = min(bright + 128 + ((*pRed - 128) * contrast/100), 255);
      g = min(bright + 128 + ((*pGrn - 128) * contrast/100), 255);
      b = min(bright + 128 + ((*pBlu - 128) * contrast/100), 255);
      r = max(r, 0);
      g = max(g, 0);
      b = max(b, 0);
      
		*pRed = (BYTE)r;
		*pGrn = (BYTE)g;
		*pBlu = (BYTE)b;
		
		pRed+=4;
      pGrn+=4;
      pBlu+=4;
	}
	return TRUE;
}

BOOL dibFilterEFFECT::AdjustHSL(int degHue,int perSaturation,int perLuminosity)
{
	if(!m_rdib->Data())
   	return FALSE;

	if(perSaturation < 0 || perLuminosity < 0)
		return FALSE;

	if(degHue == 0 && perSaturation == 100 && perLuminosity == 100)
		return TRUE;	//δ��������ֱ�ӷ���
		
   LPBYTE pRed, pGrn, pBlu,pBuf=(LPBYTE)m_rdib->Data();
	UINT loop = m_rdib->Width() * m_rdib->Height();
	COLORREF *cr = (COLORREF*)m_rdib->Data();
   
   pRed=pBuf++;pGrn=pBuf++;pBlu=pBuf;	
	float H,S,L;

	for (UINT i=0;i<loop;i++)
	{
		RGBtoHSL(*pRed,*pGrn,*pBlu,&H,&S,&L);

		H += degHue;
      S = (S*perSaturation/100.0f);
		L = (L*perLuminosity/100.0f);

      *cr = HSLtoRGB(H,S,L);

		pRed+=4;
      pGrn+=4;
      pBlu+=4;
		cr ++;
	}
	return TRUE;
}


void dibFilterEFFECT::RGBtoHSL(BYTE R,BYTE G,BYTE B,float* H,float* S,float* L)
{
	BYTE minval = min(R,min(G,B));
   BYTE maxval = max(R,max(G,B));
   float mdiff = float(maxval) - float(minval);
   float msum  = float(maxval) + float(minval);
   
   *L = msum / 510.0f;

   if (maxval == minval) 
   {
		*S = 0.0f;
      *H = 0.0f; 
   }   
   else 
   { 
      float rnorm = (maxval - R) / mdiff;      
      float gnorm = (maxval - G) / mdiff;
      float bnorm = (maxval - B) / mdiff;   

      *S = (*L <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

      if(R == maxval) 
			*H = 60.0f * (6.0f + bnorm - gnorm);
      if(G == maxval) 
			*H = 60.0f * (2.0f + rnorm - bnorm);
      if(B == maxval) 
			*H = 60.0f * (4.0f + gnorm - rnorm);
      if (*H > 360.0f) 
			*H -= 360.0f;
   }
}

COLORREF dibFilterEFFECT::HSLtoRGB(float H,float S,float L)
{
	BYTE r,g,b;
	
	L = min(1,L);
	S = min(1,S);

	if(S == 0.0)
	{
		r = g = b = (BYTE)(255 * L);
	} 
	else 
	{
		float rm1, rm2;
         
      if (L <= 0.5f) 
			rm2 = L + L * S;
      else
			rm2 = L + S - L * S;
      rm1 = 2.0f * L - rm2;   
      
		r = HueToRGB(rm1, rm2, H + 120.0f);
      g = HueToRGB(rm1, rm2, H);
      b = HueToRGB(rm1, rm2, H - 120.0f);
   }
   return RGB(r,g,b);
}

BYTE dibFilterEFFECT::HueToRGB(float rm1,float rm2,float rh)
{
	if(rh > 360.0f)
		rh -= 360.0f;
	else if(rh < 0.0f)
		rh += 360.f;
 
	if(rh < 60.0f)
		rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;
	else if(rh < 180.0f)
		rm1 = rm2;
	else if(rh < 240.0f)
		rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;
   
	float n = rm1*255;
	int m = min((int)n,255);
	m = max(0,m);
	return (BYTE)m;//(rm1 * 255);
}

/*
//new math,my thinking
//RGB -> HSL����
//L = ���ɫֵ / 255
//S = 1 - ��Сɫֵ / ���ɫֵ
//H = 60 * ���м�ɫֵ - ��Сɫֵ��/�����ɫֵ - ��Сɫֵ��
//		+ ��0[��ɫ���]��120[��ɫ���]��240[��ɫ���]��
//�����һ��Hʹ��ֵ��Χ��0~360֮��

void dibFilterEFFECT::RGBtoHSL(BYTE R,BYTE G,BYTE B,float* H,float* S,float* L)
{
	int delta;
	int r = R;
	int g = G;
	int b = B;
	BYTE cmax = max(r,max(g,b));
	BYTE cmin = min(r,min(g,b));
	*L = (float)cmax / 255.0f;//* 0.39f;	//0.39 = 100 / 255;
  
	if(cmax == cmin) 
	{
		*S = 0;
		*H = 0;
	} 
	else 
	{
		*S = 1.0f - (float)cmin / (float)cmax;
	  
		delta = cmax - cmin;
		if (r == cmax)
			*H = (g-b) * 60.0f / delta;
		else if (g==cmax)
			*H = 120.0f + (b-r) * 60.0f /delta;
		else
			*H = 240.0f + (r-g) * 60.0f /delta;
		
		if (*H < 0.0f)
			*H += 360.0f;
	}
//if(xz && !Ask("H=%d,S=%d,L=%d\nR=%x,G=%x,B=%x\n",(int)*H,(int)(*S*100),(int)(*L*100),r,g,b))
//	xz = FALSE;
}

COLORREF dibFilterEFFECT::HLStoRGB(float H,float L,float S)
{
	BYTE r,g,b;

	L = min(1,L);
	S = min(1,S);

	if(S == 0.0f)
	{
		r = g = b = (int)(255 * L);
	} 
	else
	{
		float max = 255.0f * L;
		float min = max * (1.0f - S);
				
		if(H < 0.0)
			H += 360.0f;
		if(H > 360.0f)
			H -= 360.0f;

		float h = H;

		while(h >= 60.0f)
			h -= 60.0f;

		float mid = (max - min) * h /60.0f + min;

		max = min(max,255);
		max = max(0,max);
		min = min(min,255);
		min = max(0,min);
		mid = min(mid,255);
		mid = max(0,mid);

		if(H <= 60.0f)
		{
			r = (BYTE)max;
			b = (BYTE)min;
			g = (BYTE)mid;
		}
		else if(H <= 180.0f)
		{
			g = (BYTE)max;
			if(H <= 120.0f)
			{
				b = (BYTE)min;
				r = (BYTE)mid;
			}
			else
			{
				r = (BYTE)min;
				b = (BYTE)mid;
			}
		}
		else if(H <= 300.0f)
		{
			b = (BYTE)max;
			if(H <= 240.0f)
			{
				r = (BYTE)min;
				g = (BYTE)mid;
			}
			else
			{
				g = (BYTE)min;
				r = (BYTE)mid;
			}
		}
		else
		{
			r = (BYTE)max;
			g = (BYTE)min;
			b = (BYTE)mid;
		}
	}		
//if(xz && !Ask("R=%x,G=%x,B=%x\nH=%d,S=%d,L=%d",r,g,b,(int)H,(int)(S*100),(int)L*100))
//	xz = FALSE;
   return RGB(r,g,b);
}

/*
BYTE dibFilterEFFECT::HueToRGB(int rm1, int rm2, int rh)
{
	if(rh > 360)
		rh -= 360;
	else if(rh < 0)
		rh += 360;
 
	if(rh < 60)
		rm1 = rm1 + (rm2 - rm1) * rh / 60;
	else if(rh < 180)
		rm1 = rm2;
	else if(rh < 240)
		rm1 = rm1 + (rm2 - rm1) * (240 - rh) / 60;
                   
	return (rm1 * 255 / 10000);
}
*/
//�ο�/������룬��ת
/*************************************************************************
 * �������ƣ�
 *   RotateDIB()
 * ����:
 *   LPSTR lpDIB        - ָ��ԴDIB��ָ��
 *   int iRotateAngle    - ��ת�ĽǶȣ�0-360�ȣ�
 * ����ֵ:
 *   HGLOBAL            - ��ת�ɹ�������DIB��������򷵻�NULL��
 * ˵��:
 *   �ú���������ͼ������Ϊ������תDIBͼ�񣬷���������DIB�ľ����
 * ���øú������Զ�����ͼ������ʾ���е����ء������в������ڽ���
 * ֵ�㷨���в�ֵ��
 ************************************************************************/
/*HGLOBAL CDibImage::RotateDIB(LPSTR lpDIB, int iRotateAngle)
{    
    LONG    lWidth;                // Դͼ��Ŀ��
    LONG    lHeight;            // Դͼ��ĸ߶�    
    LONG    lNewWidth;            // ��ת��ͼ��Ŀ��
    LONG    lNewHeight;            // ��ת��ͼ��ĸ߶�    
    LONG    lLineBytes;            // ͼ��ÿ�е��ֽ���    
    LONG    lNewLineBytes;        // ��ת��ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����    
    LPSTR    lpDIBBits;            // ָ��Դͼ���ָ��    
    LPSTR    lpSrc;                // ָ��Դ���ص�ָ��    
    HDIB    hDIB;                // ��ת����DIB���    
    LPSTR    lpDst;                // ָ����תͼ���Ӧ���ص�ָ��    
    LPSTR    lpNewDIB;            // ָ����תͼ���ָ��
    LPSTR    lpNewDIBBits;    
    LPBITMAPINFOHEADER lpbmi;    // ָ��BITMAPINFO�ṹ��ָ�루Win3.0��    
    LPBITMAPCOREHEADER lpbmc;    // ָ��BITMAPCOREINFO�ṹ��ָ��
        
    LONG    i;                    // ѭ����������������DIB�е����꣩
    LONG    j;    
    LONG    i0;                    // ������ԴDIB�е�����
    LONG    j0;    
    
    float    fRotateAngle;        // ��ת�Ƕȣ����ȣ�    
    float    fSina, fCosa;        // ��ת�Ƕȵ����Һ�����    
    // Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
    float    fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;    
    // ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
    float    fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
    float    f1,f2;
        
    lpDIBBits = FindDIBBits(lpDIB);        // �ҵ�ԴDIBͼ��������ʼλ��    
    lWidth = DIBWidth(lpDIB);            // ��ȡͼ���"���"��4�ı�����    
    lLineBytes = WIDTHBYTES(lWidth * 8);// ����ͼ��ÿ�е��ֽ���    
    lHeight = DIBHeight(lpDIB);            // ��ȡͼ��ĸ߶�
    
    // ����ת�ǶȴӶ�ת��������
    fRotateAngle = (float) RADIAN(iRotateAngle);    
    fSina = (float) sin((double)fRotateAngle);    // ������ת�Ƕȵ�����
    fCosa = (float) cos((double)fRotateAngle);    // ������ת�Ƕȵ�����
    
    // ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
    fSrcX1 = (float) (- (lWidth  - 1) / 2);
    fSrcY1 = (float) (  (lHeight - 1) / 2);
    fSrcX2 = (float) (  (lWidth  - 1) / 2);
    fSrcY2 = (float) (  (lHeight - 1) / 2);
    fSrcX3 = (float) (- (lWidth  - 1) / 2);
    fSrcY3 = (float) (- (lHeight - 1) / 2);
    fSrcX4 = (float) (  (lWidth  - 1) / 2);
    fSrcY4 = (float) (- (lHeight - 1) / 2);
    
    // ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
    fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
    fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
    fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
    fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
    fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
    fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
    fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
    fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
    
    // ������ת���ͼ��ʵ�ʿ��
    lNewWidth  = (LONG)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
    
    // ������ͼ��ÿ�е��ֽ���
    lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
    
    // ������ת���ͼ��߶�
    lNewHeight = (LONG)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);
    
    // �������������������Ժ�ÿ�ζ�������
    f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
        + 0.5 * (lWidth  - 1));
    f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
        + 0.5 * (lHeight - 1));
    
    // �����ڴ棬�Ա�����DIB
    hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + 
        *(LPDWORD)lpDIB + PaletteSize(lpDIB));
    if (hDIB == NULL)
    {
        return NULL;
    }

    lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);
    
    // ����DIB��Ϣͷ�͵�ɫ��
    memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + PaletteSize(lpDIB));
    
    // �ҵ���DIB������ʼλ��
    lpNewDIBBits = FindDIBBits(lpNewDIB);
    
    lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
    lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

    // ����DIB��ͼ��ĸ߶ȺͿ��
    if (IS_WIN30_DIB(lpNewDIB))
    {
        // ����Windows 3.0 DIB
        lpbmi->biWidth = lNewWidth;
        lpbmi->biHeight = lNewHeight;
    }
    else
    {
        // ����������ʽ��DIB
        lpbmc->bcWidth = (unsigned short) lNewWidth;
        lpbmc->bcHeight = (unsigned short) lNewHeight;
    }
        
    for(i = 0; i < lNewHeight; i++)        // ���ͼ��ÿ�н��в���
    {        
        for(j = 0; j < lNewWidth; j++)    // ���ͼ��ÿ�н��в���
        {
            // ָ����DIB��i�У���j�����ص�ָ��
            // ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
            lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;
            
            // �����������ԴDIB�е�����
            i0 = (LONG) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
            j0 = (LONG) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);
            
            // �ж��Ƿ���Դͼ��Χ��
            if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
            {
                // ָ��ԴDIB��i0�У���j0�����ص�ָ��
                lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
                
                // ��������
                *lpDst = *lpSrc;
            }
            else
            {
                // ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
                * ((unsigned char*)lpDst) = 255;
            }            
        }        
    }
    
    return hDIB;
}
��ɫ��ת��
          cos(r),sin(r),0.0f, 
[R,G,B]x[ sin(r),cos(r),0.0f, ] =[R*cos(r)+G*sin(r),R*sin(r)+G*cos(r),B]
          0.0f  ,0.0f  ,1.0f

  
*/