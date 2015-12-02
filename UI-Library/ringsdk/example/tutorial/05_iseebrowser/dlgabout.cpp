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
//							RingSDK ��ʾ����(ISeeͼ�������)
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
//˵������Ȩ��Ϣ�Ի���ˮ��Ч����ʵ��
//
**********************************************************************/
#include "global.h"

BEGIN_REGMSG(DlgAbout)
	REGMSG(WM_MOUSEMOVE)
	REGMSG(WM_LBUTTONDOWN)
	REGMSG(WM_CLOSE)
	REGMSG(WM_INITDIALOG)
END_REGMSG

RINGMSG(DlgAbout,WM_INITDIALOG)
{
	m_dib = new RingDIB(m_hWnd);
	//����ͼ��
	m_dib->Load(IDB_ABOUT);
	//��ȡͼ��ߴ�
	int nSize = m_dib->Width() * m_dib->Height();
	//�������λ�����
	m_buf1 = (short*)New(nSize*sizeof(short));
	m_buf2 = (short*)New(nSize*sizeof(short));
	//������������ͼ���С��ͬ��
	m_dib->CreateCanvas();
	//����ͼ��������
	m_dib->DrawToCanvas();

	HDC hdc = GetDC(m_hWnd);
   int cxScreen = GetDeviceCaps(hdc,HORZRES);
   int cyScreen = GetDeviceCaps(hdc,VERTRES);
   ReleaseDC(m_hWnd,hdc);
	//������ʾ
	SetPos((cxScreen-m_dib->Width())/2,(cyScreen-m_dib->Height())/2,m_dib->Width(),m_dib->Height());
	
	//����������ˮ�������߳�
	m_thread = new RingThread;
	m_thread->Start(this,(THREADFUNC)RippleThread);

	return FALSE;
}

RINGMSG(DlgAbout,WM_LBUTTONDOWN)
{
	//����������˳���Ϣ�������Ի���
	SendExit();
	return FALSE;
}

RINGMSG(DlgAbout,WM_CLOSE)
{
	//�ر�ʱ��������
	delete m_thread;
	Del(m_buf1);
	Del(m_buf2);
	delete m_dib;
	Close();
	return FALSE;
}

RINGMSG(DlgAbout,WM_MOUSEMOVE)
{
	//����ƶ�ʱ��ˮ���ӡ�ʯͷ�������ڲ��λ�����ָ��λ�ð뾶Ϊ4��Χ�ڳ�ʼ������Ϊ96
	DropStone(param.mousept.x,param.mousept.y,4,96);
	return FALSE;
}

DWORD RINGMETHOD(DlgAbout,RippleThread)(LONG)
{
	while(m_thread && m_thread->IsRunning())
	{
		RippleSpread();					//���㲨�����ݻ�����
		RenderRipple();					//ҳ����Ⱦ		
		m_dib->GetCanvas()->Draw();	//����
		Sleep(10);
	}
	return 0xDEAD;
}

void RINGMETHOD(DlgAbout,RippleSpread)()
{
	int s = m_dib->Width();
	int e = m_dib->Width() * (m_dib->Height() - 1);

	for(int i=s;i<e;i++)
	{
      if((i%s-1) < 0 || (i%s+1) > s)
			continue;
		//������ɢ,���������ĸ����������/2-�м������
		m_buf2[i] = (short)(((m_buf1[i-1]+m_buf1[i+1]+m_buf1[i-s]+m_buf1[i+s])>>1)	- m_buf2[i]);
		//����˥��������ˮ������������ȥ
		m_buf2[i] -= (short)(m_buf2[i]>>5);
	}
	//�����������ݻ�����
	short *ptmp = m_buf1;
	m_buf1 = m_buf2;
	m_buf2 = ptmp;
}

void RINGMETHOD(DlgAbout,RenderRipple)()
{
	//����ҳ����Ⱦ������ÿ�����ص�ƫ��λ��,�����ƶ����������λ��
	int xoff, yoff;
	int k = m_dib->Width();
	int h = m_dib->Height();
	int ix,iy,w = k;
	int pos1, pos2;
	RingDIB* dib = m_dib->GetCanvas();
	COLORREF *src,*dst;
	for(int i=1;i<h-1;i++)
	{
		for(int j=0;j<w;j++)
		{
			//����ƫ����
			xoff = m_buf1[k-1] - m_buf1[k+1];
			yoff = m_buf1[k-w] - m_buf1[k + w];
			
			ix = j + xoff;
			iy = i + yoff;
			//�ж������Ƿ��ڴ��ڷ�Χ��
			if(ix < 0 || ix >= w || iy < 0 || iy >= h)
			{
				k++;
				continue;
			}
			
			//�����ƫ�����غ�ԭʼ���ص��ڴ��ַƫ����
			pos1 = m_dib->Width() * iy + ix;
			pos2 = w * i + j;
			
			src = m_dib->Data() + pos1;
			dst = dib->Data() + pos2;

			//��������			
			*dst = *src;
			k++;
		}
	}
}

void RINGMETHOD(DlgAbout,DropStone)(int x,int y,int stonesize,int stoneweight)
{
	//�ж������Ƿ�����Ļ��Χ��
	if((x + stonesize) > m_dib->Width() ||
		(y + stonesize) > m_dib->Height() ||
		(x - stonesize) < 0 || (y-stonesize) < 0)
		return;

	//�������
	for(int posx=x-stonesize;posx<x+stonesize;posx++)
		for(int posy=y-stonesize;posy<y+stonesize;posy++)
			if((posx-x)*(posx-x) + (posy-y)*(posy-y) < stonesize*stonesize)	//����Բ�ܷ�Χ�ж�
				m_buf1[m_dib->Width() * (m_dib->Height() - posy) + posx] = (short)(-stoneweight);
}
