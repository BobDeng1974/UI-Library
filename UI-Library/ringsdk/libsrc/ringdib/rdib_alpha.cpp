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
//ԭ�ļ�����rdib_alpha.cpp
//
//˵����RingDIB�Ĳ���ࣨALPHA��ϣ�ʵ��
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"

dibFilterALPHA::~dibFilterALPHA()
{
}

void dibFilterALPHA::Fill(int r,int g,int b,int a)
{
   if(!m_rdib->Data())
   	return;

	BYTE *dst = (BYTE*)m_rdib->Data();
	int nSize = m_rdib->Width() * m_rdib->Height();

	while ( nSize-- )
	{
		dst[0]=(BYTE)(((b-dst[0])*a+(dst[0]<<8))>>8);
		dst[1]=(BYTE)(((g-dst[1])*a+(dst[1]<<8))>>8);
		dst[2]=(BYTE)(((r-dst[2])*a+(dst[2]<<8))>>8);
		dst+=4;
	}
}

void dibFilterALPHA::FillGlass(COLORREF color)
{
   if(!m_rdib->Data())
   	return;

	COLORREF* crColor = m_rdib->Data();
   COLORREF* crBak = crColor;
	int w = m_rdib->Width();
	int h = m_rdib->Height(),i,j;

	for(j=0;j<h;j++ )
	{
		for(i=0;i<w;i++)
		{
         if((i+j)&0x1)
				*crColor = color;
         crColor ++;
		}
      crBak += w;
      crColor = crBak;
	}
}

void dibFilterALPHA::Blend(RingDIB *dib,int A)
{
	if(!m_rdib->Data() || !dib->Data() || m_rdib->Width()!=dib->Width() || m_rdib->Height()!=dib->Height())
		return;

	BYTE *src=(BYTE*)dib->Data();
	BYTE *dst=(BYTE*)m_rdib->Data();
	int nSize=m_rdib->Width()*m_rdib->Height();

	while ( nSize-- )
	{
		dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
		dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
		dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
		dst+=4;
		src+=4;
	}
}

void dibFilterALPHA::Darken(RingDIB *dib)
{
	if(!m_rdib->Data() || !dib->Data() || m_rdib->Width()!=dib->Width() || m_rdib->Height()!=dib->Height())
		return;

	BYTE *src=(BYTE*)dib->Data();
	BYTE *dst=(BYTE*)m_rdib->Data();
	int nSize=m_rdib->Width()*m_rdib->Height();

	while(nSize--)
	{
		dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
		dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
		dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
		dst+=4;
		src+=4;
	}
}

void dibFilterALPHA::Difference(RingDIB *dib)
{
	if(!m_rdib->Data() || !dib->Data() || m_rdib->Width()!=dib->Width() || m_rdib->Height()!=dib->Height())
		return;

	BYTE *src=(BYTE*)dib->Data();
	BYTE *dst=(BYTE*)m_rdib->Data();
	int nSize=m_rdib->Width()*m_rdib->Height();

	while ( nSize-- )
	{
		int Difference;
		Difference=src[0]-dst[0];
		dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
		Difference=src[1]-dst[1];
		dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
		Difference=src[2]-dst[2];
		dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
		dst+=4;
		src+=4;
	}
}

void dibFilterALPHA::Lighten (RingDIB *dib)
{
	if (!m_rdib->Data() || !dib->Data() || m_rdib->Width()!=dib->Width() && m_rdib->Height()!=dib->Height())
		return;

	BYTE *src=(BYTE*)dib->Data();
	BYTE *dst=(BYTE*)m_rdib->Data();
	int nSize=m_rdib->Width()*m_rdib->Height();

	while ( nSize-- )
	{
		dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
		dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
		dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
		dst+=4;
		src+=4;
	}
}

void dibFilterALPHA::Multiply (RingDIB *dib)
{
	if(!m_rdib->Data() || !dib->Data() || m_rdib->Width()!=dib->Width() && m_rdib->Height()!=dib->Height())
		return;

	BYTE *src=(BYTE*)dib->Data();
	BYTE *dst=(BYTE*)m_rdib->Data();
	int nSize=m_rdib->Width()*m_rdib->Height();

	while ( nSize-- )
	{
		dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
		dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
		dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
		dst+=4;
		src+=4;
	}
}

void dibFilterALPHA::Screen(RingDIB *dib)
{
	if(!m_rdib->Data() || !dib->Data() || m_rdib->Width()!=dib->Width() && m_rdib->Height()!=dib->Height())
		return;

	BYTE *src=(BYTE*)dib->Data();
	BYTE *dst=(BYTE*)m_rdib->Data();
	int nSize=m_rdib->Width()*m_rdib->Height();

	while ( nSize-- )
	{
		dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
		dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
		dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
		dst+=4;
		src+=4;
	}
}

void dibFilterALPHA::Xor(RingDIB *dib)
{
	if(!m_rdib->Data() || !dib->Data() || m_rdib->Width()!=dib->Width() && m_rdib->Height()!=dib->Height())
		return;
	
	LPDWORD src=(LPDWORD)dib->Data();
	LPDWORD dst=(LPDWORD)m_rdib->Data();
	int nSize=m_rdib->Width()*m_rdib->Height();
	
	while(nSize--)
	{
		*dst++ ^= *src++;		
	}
}
//////////////////////////////////////////////////////////////////////
// Line Functions
//////////////////////////////////////////////////////////////////////

/*void Dib::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
{
	int dx, dy, k1, k2, d, x, y;
	COLORREF Color=RGB ( B, G, R );

	dx=x2-x1;
	dy=y2-y1;
	d=(dy<<1)-dx;
	k1=dy<<1;
	k2=(dy-dx)<<1;
	x=x1;
	y=y1;

	m_Bits[y*m_rdib->Width()+x]=Color;
	while (x<dx)
	{
		if (d<=0)
		{
			d+=k1;
			x++;
		}
		else
		{
			d+=k2;
			x++;
			y++;
		}
		m_Bits[y*m_rdib->Width()+x]=Color;
	}
}*/
/*
void Dib::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
{
	int d, x, y, ax, ay, sx, sy, dx, dy;
	COLORREF Color=RGB ( B, G, R );

	dx=x2-x1;
	ax=abs ( dx )<<1;
	sx=(dx<0) ? -1 : 1;
	dy=y2-y1;
	ay=abs ( dy )<<1;
	sy=(dy<0) ? -1 : 1;
	x=x1;
	y=y1;

	if ( ax>ay )
	{
		d=ay-(ax>>1);
		while ( x!=x2 )
		{
			m_Bits[y*m_rdib->Width()+x]=Color;
			if ( d>=0 )
			{
				y+=sy;
				d-=ax;
			}
			x+=sx;
			d+=ay;
		}
	}
	else
	{
		d=ax-(ay>>1);
		while ( y!=y2 )
		{
			m_Bits[y*m_rdib->Width()+x]=Color;
			if ( d>=0 )
			{
				x+=sx;
				d-=ay;
			}
			y+=sy;
			d+=ax;
		}
	}
}

void Dib::LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A )
{
	int d, x, y, ax, ay, sx, sy, dx, dy;
	//COLORREF Color=RGB ( B, G, R );
	BYTE *dst=(BYTE *)m_Bits;

	dx=x2-x1;
	ax=abs ( dx )<<1;
	sx=(dx<0) ? -1 : 1;
	dy=y2-y1;
	ay=abs ( dy )<<1;
	sy=(dy<0) ? -1 : 1;
	x=x1;
	y=y1;

	if ( ax>ay )
	{
		d=ay-(ax>>1);
		while ( x!=x2 )
		{
			dst[(y*m_rdib->Width()+x)<<2]=(BYTE)(((B-dst[(y*m_rdib->Width()+x)<<2])*A+(dst[(y*m_rdib->Width()+x)<<2]<<8))>>8);
			dst[((y*m_rdib->Width()+x)<<2)+1]=(BYTE)(((G-dst[((y*m_rdib->Width()+x)<<2)+1])*A+(dst[((y*m_rdib->Width()+x)<<2)+1]<<8))>>8);
			dst[((y*m_rdib->Width()+x)<<2)+2]=(BYTE)(((R-dst[((y*m_rdib->Width()+x)<<2)+2])*A+(dst[((y*m_rdib->Width()+x)<<2)+2]<<8))>>8);
			if ( d>=0 )
			{
				y+=sy;
				d-=ax;
			}
			x+=sx;
			d+=ay;
		}
	}
	else
	{
		d=ax-(ay>>1);
		while ( y!=y2 )
		{
			dst[(y*m_rdib->Width()+x)<<2]=(BYTE)(((B-dst[(y*m_rdib->Width()+x)<<2])*A+(dst[(y*m_rdib->Width()+x)<<2]<<8))>>8);
			dst[((y*m_rdib->Width()+x)<<2)+1]=(BYTE)(((G-dst[((y*m_rdib->Width()+x)<<2)+1])*A+(dst[((y*m_rdib->Width()+x)<<2)+1]<<8))>>8);
			dst[((y*m_rdib->Width()+x)<<2)+2]=(BYTE)(((R-dst[((y*m_rdib->Width()+x)<<2)+2])*A+(dst[((y*m_rdib->Width()+x)<<2)+2]<<8))>>8);
			if ( d>=0 )
			{
				x+=sx;
				d-=ay;
			}
			y+=sy;
			d+=ax;
		}
	}
}
*/