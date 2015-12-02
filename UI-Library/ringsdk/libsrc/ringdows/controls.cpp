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
//ԭ�ļ�����controls.cpp
//
//���ļ�˵�����ؼ�����ʵ�֡�
//
**********************************************************************/
#define MAKE_SELF_LIB
//#ifndef __BORLANDC__
#include "ringdows.h"
//#else
//#include "include\ringdows.h"
//#endif

//RingOwnerDrawer* RingControls::m_drawer = NULL;

RingCtrlWnd::~RingCtrlWnd()
{
	//RingBaseWnd�������Զ��ж��Ƿ���ҪDetach	
}

HWND RingCtrlWnd::Create(LPCTSTR szTitle,LPCTSTR szClass,DWORD dwStyle,HWND hParentWnd/*=NULL*/,DWORD dwExStyle,HMENU hMenu,int x,int y,int width,int height)
{
	if(IsWindow())
		return m_hWnd;

	if(hParentWnd == NULL)
		hParentWnd = m_parent->Handle();
	//else
	//	SetParentObj(GetWindowPtr(hParentWnd));

	m_hWnd = CreateWindowEx(
				  		dwExStyle,
                  szClass,
                  szTitle,
                  dwStyle,
                  x,
                  y,
                  width,
                  height,
                  hParentWnd,
                  hMenu,
                  GetInstance(),
                  NULL
                  );
	RingBaseWnd::Attach(m_hWnd);
	
   m_ID = (UINT)hMenu;

   return m_hWnd;
}


LRESULT RingCtrlWnd::DefaultProc(RINGPARAMS& param)
{
	if(param.uMsg == WM_NCDESTROY)
	{
		CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
		//����ʱ����Detach,RingBaseWnd����ʱ��Detach
		OnNcDestroy();
		return 0;
	}

	return CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
}

RingControls::RingControls()
{
	m_brush = NULL;//(HBRUSH)GetStockObject(NULL_BRUSH/*DC_BRUSH*/);
	m_bNotifiParentKeyEvent = FALSE;
	m_buf = NULL;
	m_bufsize = 0;
	m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	m_crBkgColor = CLR_INVALID;
	m_windowType = ITIS_DLGITEM;
	m_bOwnerDraw = FALSE;
	m_drawer = NULL;
}

RingControls::~RingControls()
{
	m_buf=(LPSTR)Del(m_buf);
	if(m_brush && m_brush != (HBRUSH)CLR_INVALID)
		DeleteObject(m_brush);
}

void RingControls::SetAutoSize(BOOL bAuto)
{
	if(bAuto)
		m_ExStyle |= RTV_FITPARENTSIZE;
	else
		m_ExStyle &= ~RTV_FITPARENTSIZE;
}

LRESULT RingControls::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	if(m_drawer)
	{
		LRESULT res = m_drawer->DrawProc(this,param);
		if(res != 0)
			return res;
	}
	if(m_bNotifiParentKeyEvent)
	{
		if(param.uMsg == WM_KEYDOWN ||
			param.uMsg == WM_KEYUP ||
			param.uMsg == WM_CHAR)
			//�����������ȡ����Ϣ���ÿؼ�����������뷵�ط�0ֵ
			if(SendMessage(m_parent->Handle(),param))
				return 0;
	}
	switch(param.uMsg)
	{
		case WM_DESTROY:
			return OnDestroy(param);
		case WM_DOCKSTATECHANGE:
			return OnDockStateChange(param.shortval.low,param.shortval.high);
		case WM_GETDRAGRECTS:
			if(param.lpnmhdr->hwndFrom == m_hWnd)
				return OnGetDragRects(param.lpnmhdr->code,(LPRECT)param.wParam);
	}
	return DefaultProc(param);
}

int RingControls::OnDockStateChange(int stateOld,int stateNew)
{
	return SendMessage(m_parent->Handle(),WM_DOCKSTATECHANGE,MAKELONG(stateOld,stateNew),(LPARAM)m_hWnd);
}

BOOL RingControls::OnGetDragRects(int state,LPRECT lprc)
{
	//return SendMessage(m_parent->Handle(),WM_GETDRAGRECTS,(WPARAM)lprc,(LPARAM)m_hWnd);
	//���򸸴��ڲ�ѯʧ��Ȼ�󷢹�������Ϣ��û��Ҫ�ٴβ�ѯ
	return 0;
}

BOOL RingControls::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	switch(uMsg)
	{
		case WM_DOCKSITESIZECHANGE:	
			hWnd = NULL;	 //�����WM_DOCKSITESIZECHANGE��Ϣ��DOCKSITE�����ߴ�
		case WM_SIZE:		
			return OnFitSize(hWnd,wParam,lParam);
		case WM_DRAWITEM:
			//m_bOwnerDraw=TRUE��ʾ�ɸ����ڴ���
			if(!m_bOwnerDraw)
				if(m_ID == wParam)
					return OnDrawItem((LPDRAWITEMSTRUCT)lParam);
			return FALSE;
	}
	return FALSE;
}

BOOL RingControls::OnFitSize(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	if(m_ExStyle & RTV_FITPARENTSIZE)
	{
		if(wParam == -1)	//-1��ʾ����lParam��ֵ���ô�С��δ��֧�֣���ʱ�����õ�
			SetPos(0,0,LOWORD(lParam),HIWORD(lParam),NULL,SWP_NOMOVE|SWP_NOZORDER);//SWP_NOACTIVATE|SWP_NOREDRAW);
		else
		{
			RECT rc;
			//����m_parent->GetClientRect�ŷ�����ȷ��RECT
			RingBaseWnd* parent = GetWindowPtr(GetParent());
			if(parent)
				parent->GetClientRect(&rc);
			else
				::GetClientRect(GetParent(),&rc);
			
			MoveWindow(m_hWnd,rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,TRUE);
			if(hWnd == NULL)
			{
				//WM_DOCKSITESIZECHANGE��Ϣ��DockSite�ߴ�䶯���п��ܸ��ǿؼ���
				//ĳЩ�ؼ���ListView�Ȳ���ˢ�µ��½���Ứ�������ˢ��
				InvalidateRect(m_hWnd,NULL,TRUE);
			}
		}		
		return TRUE;
	}
	return FALSE;
}

int RingControls::GetInt(int index/*=INDEX_CURSEL*/)
{
	return (int)GetDlgItemInt(m_parent->Handle(),m_ID,NULL,TRUE);
}

LPCTSTR RingControls::GetText(int index/*=INDEX_CURSEL*/,
										int size/*=RBUF_SIZE*/,
										LPTSTR lpBuf/*=NULL*/)
{
   LPTSTR lpstr = lpBuf;
	if(lpstr == NULL && GetBuffer(size))
		lpstr = m_buf;

	if(lpstr)
	{
		memset(lpstr,0,size);
		GetItemText(index,lpstr,size);
		//GetItemText�п������·���m_buf������������ж�
   	if(lpstr != m_buf && lpBuf == NULL)
			return m_buf;
		else
			return lpstr;
	}
	else
   	return "";	
}

int RingControls::GetItemText(int,LPTSTR lpBuf,int size)
{
	return GetWindowText(m_hWnd,lpBuf,size);
}

BOOL RingControls::SetValue(int val,int index/*=INDEX_CURSEL*/)
{
	return SetDlgItemInt(GetParent(),m_ID,val,TRUE);
}

BOOL RingControls::SetValue(LPCTSTR lpszVal,int index/*=INDEX_CURSEL*/)
{
	return SetDlgItemText(GetParent(),m_ID,lpszVal);
}

int RingControls::GetBuffer(int size)
{
   //�����ڴ棬������ߴ��4�Է������������¼��ʵ�ʳߴ�
	//m_bufsize��Ϊ����ߴ�
	if(m_buf == NULL && size > 0)
   {
   	m_buf = (LPSTR)New(size + 4);
      if(m_buf)
      	m_bufsize = size;
   }
   else
   	if(size > m_bufsize)
      {
      	LPSTR temp = (LPSTR)New(size + 4);
         if(temp)
         {
         	m_bufsize = size;
            Del(m_buf);
            m_buf = temp;
         }
      }
   return m_bufsize;
}

BOOL RingControls::TypeMatched(UINT id)
{
	HWND hwnd = GetDlgItem(m_parent->Handle(),id);
	if(hwnd)
		return TypeMatched(hwnd);
	return FALSE;
}

BOOL RingControls::TypeMatched(HWND hWnd)
{
	char szClass[80];
	memset(szClass,0,80);
	GetClassName(hWnd,szClass,80);
	return (strcmpi(WndClassName(),szClass) == 0);
}

BOOL RingControls::Attach(UINT uId,RingBaseWnd* parent/*=NULL*/)
{
	if(m_parent == this && parent)
		m_parent = parent;

	m_hWnd = GetDlgItem(m_parent->Handle(),uId);
	return Attach(m_hWnd);	
}

BOOL RingControls::Attach(HWND hWnd)
{
	if(RingBaseWnd::Attach(hWnd))
	{		
		m_ID = GetWindowLong(hWnd,GWL_ID);
		//m_ExStyle�п��ܱ�SetAutoSize���ù������������
		m_ExStyle = GetWindowLong(m_hWnd,GWL_EXSTYLE) | m_ExStyle;
		//m_brush = (HBRUSH)GetClassLong(m_hWnd,GCL_HBRBACKGROUND);
		return TRUE;
	}
	return FALSE;
}

BOOL RingControls::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height)
{
	if(m_hWnd && IsWindow())
		return TRUE;

	m_ExStyle = dwExStyle;
	
	if((dwExStyle & RTV_FITPARENTSIZE))
	{
		RECT rc;
		m_parent->GetClientRect(&rc);
		x = y = 0;
		width = rc.right;
		height = rc.bottom;
		dwExStyle &= ~RTV_FITPARENTSIZE;
	}
	m_hWnd = RingCtrlWnd::Create(
		szInitText,
		WndClassName(),
		WS_CHILD | WS_VISIBLE | dwStyle,
		0,dwExStyle,
		(HMENU)uId,x,y,width,height);

	if(m_hWnd)
	{
		//m_brush = (HBRUSH)GetClassLong(m_hWnd,GCL_HBRBACKGROUND);
		ringFont rf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		rf.SetWindowFont(m_hWnd);
	}
   return (BOOL)m_hWnd;
}

void RingControls::SetOwnerDraw(BOOL bParentDraw,UINT uItemStyle)
{
	m_bOwnerDraw = bParentDraw;

	if(uItemStyle != 0xFFFFFFFF && (GetStyle() & uItemStyle) != uItemStyle)
		ReplaceStyle(0,uItemStyle);	
}

BOOL RingControls::SetBkColor(COLORREF crColor)
{	
	m_crBkgColor = crColor;
	HDC hDC = GetDC(m_hWnd);
	::SetBkColor(hDC,crColor);
	ReleaseDC(m_hWnd,hDC);
	
	if(m_brush)
		DeleteObject(m_brush);

	if(crColor == CLR_INVALID)
		m_brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	else
		m_brush = CreateSolidBrush(crColor);
	return (BOOL)m_brush;
}

void RingControls::SetFont(LPCTSTR szFontName,int size,int nStyle,COLORREF crText)
{
	m_crTextColor = crText;
	m_font.SetWindowFont(m_hWnd,szFontName,size,nStyle);
}

void RingControls::SetTextColor(COLORREF crColor)
{
	m_crTextColor = crColor;
}

LRESULT RingControls::SetColors(HDC hDC)
{
	//if(m_crBkgColor != 0xFFFFFFFF)
	{
		::SetTextColor(hDC,m_crTextColor);
		//SelectObject(hDC,m_brush);
		SetBkMode(hDC,TRANSPARENT);
		//::SetBkColor(param.hdc,0);//((GetSysColor(COLOR_WINDOW));
		//SetROP2(param.hdc,R2_BLACK);
		//if(m_brush == NULL)
		//	SetBkColor(GetSysColor(COLOR_WINDOW));
	}
	//SetBrushOrgEx(hDC,0,0,NULL);
	return (LONG)m_brush;
}

void RingControls::SetMouseTrack(RINGPARAMS& param)
{
	if((param.uMsg >= WM_MOUSEFIRST) && (param.uMsg <= WM_MOUSELAST))
	{
		TRACKMOUSEEVENT tme;
		
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 10;//HOVER_DEFAULT;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		
		TrackMouseEvent(&tme);         
	}
}


