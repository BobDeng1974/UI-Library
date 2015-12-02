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
//�汾��1.0
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����ringtoolbar.cpp
//
//���ļ�˵������ʾ��ʵ�ִ��롣
//
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdows.h"

RingToolTip::RingToolTip()
{
	InitBarControl();
	m_tiptext = NULL;
	SetTipStyle(TTF_SUBCLASS);
}

RingToolTip::~RingToolTip()
{
	LPRINGTIPTEXT curr,save;
	curr = save = m_tiptext;
	while(curr)
	{
		Del((LPSTR)curr->lpszTip);
		save = curr;
		curr = curr->m_next;
		Del(save);
	}
}

//����RingControls��Create��RingControls::Createʼ�մ����Ӵ��ڣ���������ҪWS_POPUP
BOOL RingToolTip::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,
								 int x,int y,int width,int height)
{
	if(m_hWnd && IsWindow())
		return TRUE;
	
	m_ExStyle = dwExStyle & ~RTV_FITPARENTSIZE;
	
	m_hWnd = RingCtrlWnd::Create(
		szInitText,
		WndClassName(),
		WS_POPUP | dwStyle,
		0,dwExStyle,
		(HMENU)uId,x,y,width,height);
	
	return (BOOL)m_hWnd;
}

BOOL RingToolTip::Create(DWORD dwStyle/*=TTS_NOPREFIX*/)
{
   return Create(dwStyle,WS_EX_TOPMOST,NULL,0,0,0,0,0);
}

BOOL RingToolTip::Attach(HWND hWnd)
{
	TOOLINFO ti;
	
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = m_dwTipStyle|TTF_IDISHWND;
	ti.hwnd   = m_parent->Handle();
	ti.uId    = (UINT)hWnd;
	ti.hinst  = GetInstance();
	ti.lpszText  = LPSTR_TEXTCALLBACK;
	::GetClientRect(hWnd,&ti.rect);
	//MapWindowPoints(hWnd,m_parent->Handle(),(LPPOINT)&ti.rect,2);
	return ::SendMessage(m_hWnd,TTM_ADDTOOL,0,(LPARAM)&ti);
}

BOOL RingToolTip::SetPos(int x,int y,int width/*=-999*/,int height/*=-999*/,
								 HWND hPos/* = NULL*/,UINT uFlags/* = SWP_NOZORDER*/)
{
	return ::SendMessage(m_hWnd,TTM_TRACKPOSITION,0,MAKELONG(x,y));
}

BOOL RingToolTip::SetValue(int val,int index/*=INDEX_CURSEL*/)
{
	HWND hwnd = GetDlgItem(m_parent->Handle(),index);
	TOOLINFO ti;
	
	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd   = hwnd;
	ti.uId    = (UINT)hwnd;
	ti.hinst  = GetInstance();
	ti.lpszText  = MAKEINTRESOURCE(val);
	
	return ::SendMessage(m_hWnd,TTM_UPDATETIPTEXT,0,(LPARAM)&ti);
}

BOOL RingToolTip::SetValue(LPCTSTR lpszVal,int index/*=INDEX_CURSEL*/)
{
	HWND hwnd = GetDlgItem(m_parent->Handle(),index);
	TOOLINFO ti;
	
	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd   = hwnd;
	ti.uId    = (UINT)hwnd;
	ti.hinst  = NULL;
	ti.lpszText = (LPSTR)lpszVal;
	
	return ::SendMessage(m_hWnd,TTM_UPDATETIPTEXT,0,(LPARAM)&ti);
}

LPCTSTR RingToolTip::AddTipText(UINT uId,LPCTSTR lpszText)
{
	if(IS_INTRESOURCE(lpszText) || (LPSTR)lpszText == LPSTR_TEXTCALLBACK)
		return lpszText;

	LPRINGTIPTEXT lpTip;

	if(m_tiptext == NULL)
		m_tiptext = (LPRINGTIPTEXT)New(sizeof(RINGTIPTEXT));

	lpTip = m_tiptext;
	while(lpTip)
	{
		if(lpTip->m_nSize == 0 && lpTip->lpszTip == NULL)
		{
			//�ҵ��յ�
			lpTip->m_nSize = (lstrlen(lpszText) + 2) * sizeof(TCHAR);
			lpTip->lpszTip = (LPCTSTR)New(lpTip->m_nSize);
			if(lpTip->lpszTip)
			{
				lstrcpy((LPSTR)lpTip->lpszTip,lpszText);
				lpTip->m_id = uId;
				return lpTip->lpszTip;
			}
			else
			{
				lpTip->m_nSize = 0;
				return NULL;
			}
		}
		else
		{
			if(lpTip->m_next == NULL)
				lpTip->m_next = (LPRINGTIPTEXT)New(sizeof(RINGTIPTEXT));
			lpTip = lpTip->m_next;
		}
	}
	return NULL;
}

BOOL RingToolTip::AddTool(HWND hWnd,LPCTSTR lpszTip,LPRECT lprc)
{
	TOOLINFO ti;
	
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = m_dwTipStyle|TTF_IDISHWND;
	ti.hwnd   = m_parent->Handle();
	ti.uId    = (UINT)hWnd;
	ti.hinst  = GetInstance();
	if(IS_INTRESOURCE(lpszTip) || lpszTip == LPSTR_TEXTCALLBACK)
		ti.lpszText = (LPSTR)lpszTip;
	else
	{
		//���Ʊ���
		ti.lpszText = (LPSTR)AddTipText((UINT)hWnd,lpszTip);
	}
	if(lprc == NULL)
		::GetClientRect(hWnd,&ti.rect);
	else
		CopyRect(&ti.rect,lprc);
	
	return ::SendMessage(m_hWnd,TTM_ADDTOOL,0,(LPARAM)&ti);
}

BOOL RingToolTip::AddTool(UINT uId,LPCTSTR lpszTip,LPRECT lprc)
{
	return AddTool(GetDlgItem(m_parent->Handle(),uId),lpszTip,lprc);
}
