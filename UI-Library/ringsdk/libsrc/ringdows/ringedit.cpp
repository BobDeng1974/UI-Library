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
//ԭ�ļ�����edit.cpp
//
//˵����Edit�ؼ�����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingEdit::RingEdit()
{
	m_windowType = ITIS_EDIT;	
}

RingEdit::~RingEdit()
{

}

BOOL RingEdit::SetReadOnly(BOOL bReadOnly)
{
	return (BOOL)SendMessage(m_hWnd,EM_SETREADONLY,bReadOnly,0);
}

BOOL RingEdit::IsReadOnly()
{
	return (GetWindowLong(m_hWnd,GWL_STYLE) & ES_READONLY);
}

void RingEdit::LimitText(int nChars)
{
	SendMessage(m_hWnd,EM_LIMITTEXT,nChars,0);
}

void RingEdit::InsertLine(LPCTSTR lpText,BOOL bFirstLine/*=FALSE*/)
{
	if(!bFirstLine)
	{		
		SendMessage(m_hWnd,EM_SETSEL,0x7f,0x7f);
		
		char s[8];
		if(SendMessage(m_hWnd,WM_GETTEXT,4,(LPARAM)s) > 0)
			SendMessage(m_hWnd,EM_REPLACESEL,TRUE,(LPARAM)"\r\n");
		SendMessage(m_hWnd,EM_REPLACESEL,TRUE,(LPARAM)lpText);
	}
	else
	{
		SendMessage(m_hWnd,EM_SETSEL,0,0);
		SendMessage(m_hWnd,EM_REPLACESEL,TRUE,(LPARAM)lpText);
		SendMessage(m_hWnd,EM_REPLACESEL,TRUE,(LPARAM)"\r\n");
	}
}

LRESULT RingEdit::OnBkColor(UINT uMsg,HDC hDC)
{
	switch(uMsg)
	{
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			return SetColors(hDC);			
	}
	return 0;
}

LRESULT RingEdit::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	switch(param.uMsg)
	{
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_CHAR:
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
		{
			LRESULT res = RingControls::RingdowProc(m_hWnd,param);
			if(m_brush)	//�б�������ˢ�µĻ��ֻỨ
				InvalidateRect(m_hWnd,NULL,TRUE);
			//SendMessage(m_hWnd,WM_SETREDRAW,TRUE,0);
			return res;
		}
		/*case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(m_hWnd,&ps);
			SetBkMode(ps.hdc,TRANSPARENT);
			param.wParam = (WPARAM)ps.hdc;
			LRESULT res = DefaultProc(param);
			EndPaint(m_hWnd,&ps);
			return res;
		}
		*/
		case WM_ERASEBKGND:
		{			
			if(m_brush)
			{
				RECT rc;
				::GetClientRect(m_hWnd,&rc);
				FillRect(param.hdc,&rc,m_brush);
				return TRUE;
			}
			else
				break;//FillRect(param.hdc,&rc,(HBRUSH)GetStockObject(18/DC_BRUSH///));			
		}
		
	}
	return RingControls::RingdowProc(m_hWnd,param);
}
/*
void RingEdit::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	switch(uMsg)
	{
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			if(m_brush)
				return 
	}
}
*/
/*
LRESULT CALLBACK RingEdit::RingdowProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	if(uMsg == WM_ENABLE)
	{
		RingCtrlWnd::RingdowProc(hWnd,uMsg,wParam,lParam);
		currStyle = wParam?RINGITEMSTYLE_FLAT:RINGITEMSTYLE_NORMAL;
		DrawItem();
		return 0;
	}

	if(m_ExStyle == RINGITEMSTYLE_FLAT && IsWindowEnabled(hWnd))
		switch(uMsg)
   	{
			case WM_NCPAINT:
	         return OnDrawItem(hWnd,wParam,lParam);
      	case WM_TIMER:
//				rf.Write("ontimer\r\n",9);
	         return OnTimer(hWnd,wParam,lParam);			
   	}
   return RingCtrlWnd::RingdowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT RingEdit::OnDrawItem(HWND,UINT,LONG)
{
	DrawItem();

  	return 0;
}
//*/




