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
//							�ٷ���������� ringdows.lib
//���ߣ��ٷ�
//
//�汾��0.2
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����ringoutlookbar.cpp
//
//���ļ�˵����RingOutLookBar��ʵ�ִ���
//
//��⹦�ܣ�OutlookBar�ؼ���װ
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingOutLookBar::RingOutLookBar()
{
	SetAutoDelete();
}

RingOutLookBar::~RingOutLookBar()
{
}

BOOL RingOutLookBar::Create()
{
	if(m_hWnd)
		return TRUE;
	
	WNDCLASSEX wndclsex =
   {
		sizeof(WNDCLASSEX),
			CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
			InitWndProc,
			0,0,
			GetInstance(),
			NULL,
			NULL,
			(HBRUSH)(COLOR_WINDOW + 1),
			NULL,
			"RingOutLookBar",
			NULL
   };
	
	if(!RegRingdowClass(&wndclsex))
		return NULL;
	
   CreateEx("","RingOutLookBar", 
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		WS_EX_TOOLWINDOW,NULL,NULL,0,0,0,0);
	
	Show(SW_SHOW);
	
	return (BOOL)m_hWnd;
}

BOOL RingOutLookBar::AddBarItem(LPCTSTR lpszTitle,UINT uId,UINT style)
{
	return TRUE;
}

LRESULT RingOutLookBar::RingdowProc(HWND,RINGPARAMS param)
{
	switch(param.uMsg)
	{
		case WM_COMMAND:
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(m_hWnd,&ps);
			TextOut(ps.hdc,0,0,"hello",5);
			EndPaint(m_hWnd,&ps);
			break;
		}
	}
	return DefaultProc(param);
}

BOOL RingOutLookBar::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	switch(uMsg)
	{
		case WM_SIZE:
			SetPos(0,0,LOWORD(lParam),HIWORD(lParam));
			NotifyChild(hWnd,uMsg,wParam,lParam);
	}
	return FALSE;
}
