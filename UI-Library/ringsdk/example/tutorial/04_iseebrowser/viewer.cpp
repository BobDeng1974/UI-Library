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
//˵����MDI�Ӵ��ڣ�ͼ����ʾ���ִ���
//
**********************************************************************/
#include "global.h"

BEGIN_REGMSG(child)
	REGMSG(WM_PAINT)
	REGMSG(WM_ERASEBKGND)
	REGMSG(WM_SIZE)
	REGMSG(WM_CREATE)
END_REGMSG

RINGMSG(child,WM_CREATE)
{
	char szPic[MAX_PATH];
	GetWindowText(m_hWnd,szPic,MAX_PATH);
	m_dib.Load(szPic);
	m_dib.SetTarget(m_hWnd);
	
	SetIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_APP)));
	return DefaultProc(param);
}

RINGMSG(child,WM_SIZE)
{
	InvalidateRect(m_hWnd,NULL,TRUE);
	return DefaultProc(param);
}

RINGMSG(child,WM_ERASEBKGND)
{
	return TRUE;
}

RINGMSG(child,WM_PAINT)
{
	int w,h;
	RECT rc,rcImage;
	PAINTSTRUCT ps;
	
	GetClientRect(&rc);
	//����ͼ����ʾ���
	if(rc.right < m_dib.Width())
	{
		//������ʾ
		h = rc.right*m_dib.Height()/m_dib.Width();
		if(h > rc.bottom)
		{
			h = rc.bottom;
			w = h*m_dib.Width()/m_dib.Height();
		}
		else
			w = rc.right;
	}
	else if(rc.bottom < m_dib.Height())
	{
		//������ʾ
		w = rc.bottom*m_dib.Width()/m_dib.Height();
		if(w > rc.right)
		{
			w = rc.right;
			h = w*m_dib.Height()/m_dib.Width();
		}
		else
			h = rc.bottom;
	}
	else
	{
		w = m_dib.Width();
		h = m_dib.Height();
	}
	rcImage.left = (rc.right - w)/2;
	rcImage.right = rcImage.left + w;
	rcImage.top = (rc.bottom - h)/2;
	rcImage.bottom = rcImage.top + h;
	
	//����ͼ��
	BeginPaint(m_hWnd,&ps);
	m_dib.Draw(ps.hdc,rcImage.left,rcImage.top,0,0,w,h);
	//�ų�ͼ��������Ʊ������������������˸
	ExcludeClipRect(ps.hdc,rcImage.left,rcImage.top,rcImage.right,rcImage.bottom);
	FillRect(ps.hdc,&rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
	EndPaint(m_hWnd,&ps);
	return 0;
}
