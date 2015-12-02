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
//ԭ�ļ�����ringmdiframewnd.cpp
//
//���ļ�˵����RingMDIFrameWnd��ʵ�ִ���
//
//��⹦�ܣ����ĵ����ڵķ�װ
//
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdows.h"

//extern ringFile rfc;
//extern char cc[80];

RingMDIFrameWnd::RingMDIFrameWnd()
{
   m_nChildid = 0;
	m_menuBar = NULL;
	m_windowInfo.hCursor = LoadCursor(NULL,IDC_ARROW);
   m_windowInfo.hIconBig = NULL;
   m_windowInfo.hIconSmall = NULL;
   m_windowInfo.lpszMenu = NULL;
   m_windowType = ITIS_MDIFRAME;
}

RingMDIFrameWnd::~RingMDIFrameWnd()
{
	//delete m_client;
}

/*****************************************************************
//
//֪ͨ�Ӵ��ڴ�����Ϣ����WM_SIZE�ȣ��Ӵ��ڿ��Ը��游���ڵ����ߴ�
//
//����Ĵ�����Ҫ�Ȱ���������˳����֪ͨDockSite������������Զ�����
//�ߴ���Ӵ�������DockSite��������ΪDockSite��δ�����ߴ磬��GetClientRect
//���ز���ȷ�����������Ӵ��ڴ�����Ϻ�ô��ڳߴ粻��ȷ
//
*****************************************************************/
void RingMDIFrameWnd::NotifyChild(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	
	RingWnd::NotifyChild(hWnd,uMsg,wParam,lParam);
	if(uMsg == WM_SIZE || uMsg == WM_DOCKSITESIZECHANGE)
	{
		if (wParam != SIZE_MINIMIZED)
		{
			RECT rc;
			GetClientRect(&rc);
			MoveWindow(m_client.Handle(),
				rc.left,
				rc.top,
				rc.right-rc.left,
				rc.bottom-rc.top,
				TRUE);
    	}
	}
}

void RingMDIFrameWnd::OnCreate(HWND hWnd)
{
   CLIENTCREATESTRUCT ccs = {0};

   RingWnd::OnCreate(hWnd);
	
	FindWindowMenu(&ccs);

	m_client.SetParentObj(this);
	
   ::ShowWindow(m_client.Create(&ccs),SW_SHOW);
}

void RingMDIFrameWnd::FindWindowMenu(LPCLIENTCREATESTRUCT lpccs)
{
	HMENU hMenu = GetMenu();
   int numItems = GetMenuItemCount(hMenu) - 2;

   if(numItems < 0)
   	numItems = 0;

   lpccs->hWindowMenu = GetSubMenu(hMenu,numItems);

   numItems = GetMenuItemCount((HMENU)lpccs->hWindowMenu);

   lpccs->idFirstChild = GetMenuItemID((HMENU)lpccs->hWindowMenu,numItems-1) + 10;
}

HWND RingMDIFrameWnd::Create(LPCTSTR szTitle,LPCTSTR szClass,
									  DWORD dwStyle,DWORD dwExStyle,
									  int x,int y,int width,int height,
									  HWND hParentWnd/*=NULL*/,HMENU hMenu/*=NULL*/,
									  LPARAM lParam/*=0*/)
{
   if(!Register(szClass,
            (HBRUSH)(COLOR_WINDOW + 1),
            m_windowInfo.lpszMenu,
            m_windowInfo.hIconBig,
            m_windowInfo.hCursor,
            m_windowInfo.hIconSmall))
   	return NULL;

   wsprintf(m_szChildClass,"%s_RingSDKMDIChild\0",szClass);

	Register(m_szChildClass,
            (HBRUSH)(COLOR_WINDOW + 1),
            NULL,//..m_windowInfo.lpszMenu,
            m_windowInfo.hIconBig,
            m_windowInfo.hCursor,
            m_windowInfo.hIconSmall,
				ITIS_MDICHILD);

   CreateEx(szTitle,szClass,dwStyle | WS_CLIPCHILDREN,dwExStyle,hParentWnd,hMenu,
				x,y,width,height,lParam);
   return m_hWnd;
}

BOOL RingMDIFrameWnd::CreateChild(LPCTSTR lpszTitle,RingWnd* child,BOOL bOpen/*=FALSE*/,
											 DWORD dwStyle/*=0*/,LPARAM lParam/*=0*/,
											 LPSTR szClass/*=NULL*/)
{
   m_nChildid ++;
   MDICREATESTRUCT mcs;

   LPTSTR str = (LPTSTR)New(lstrlen(lpszTitle) + 8);
   if(str)
   {
	   int nCmdShow = SW_SHOW;
		BOOL bMax = FALSE;
		if(GetCurrChildWnd(&bMax) && bMax)
			nCmdShow = SW_SHOWMAXIMIZED;

		if(bOpen)
			lstrcpy(str,lpszTitle);
		else
			wsprintf(str,"%s%d",lpszTitle,m_nChildid);

		if(szClass == NULL)
			mcs.szClass = m_szChildClass;
		else
			mcs.szClass = szClass;

		RINGWNDPARAM wndParam = {child,lParam};
	   mcs.szTitle = str;
      mcs.hOwner  = GetInstance();
      mcs.x       = CW_USEDEFAULT;    // x position
      mcs.y       = CW_USEDEFAULT;    // y position
      mcs.cx      = CW_USEDEFAULT;    // width
      mcs.cy      = CW_USEDEFAULT;    // height
		mcs.style   = dwStyle;// & ~(WS_MAXIMIZE | WS_VISIBLE);                // window style
      mcs.lParam  = (LPARAM)&wndParam;            // lparam

      HWND hwndChild = (HWND)SendMessage(m_client.Handle(),WM_MDICREATE,0,(LPARAM)&mcs);

      if(hwndChild)
      {
			::ShowWindow(hwndChild,nCmdShow);
			SendMessage(m_client.Handle(),WM_MDIREFRESHMENU,0,0);
			::SetFocus(hwndChild);
		}
		Del(str);

      return TRUE;
   }	
   return FALSE;
}

RingMDIChildWnd* RingMDIFrameWnd::GetCurrChild(LPBOOL bMax/*=NULL*/)
{
	HWND hwnd = (HWND)SendMessage(GetClient(),WM_MDIGETACTIVE,0,(LPARAM)bMax);
	if(hwnd)
		return (RingMDIChildWnd*)GetWindowPtr(hwnd);
	else
		return NULL;
}

HWND RingMDIFrameWnd::GetCurrChildWnd(LPBOOL bMax/*=NULL*/)
{
	return (HWND)SendMessage(GetClient(),WM_MDIGETACTIVE,0,(LPARAM)bMax);
}

RingMDIChildWnd* RingMDIFrameWnd::GetMDIChild()
{
	return (RingMDIChildWnd*)GetWindowPtr(m_client.GetChild());
}

RingMDIChildWnd* RingMDIFrameWnd::GetNextMDIChild(RingMDIChildWnd* child)
{
	if(child)
		return (RingMDIChildWnd*)GetWindowPtr(m_client.GetNextChild(child->Handle()));
	else
		return (RingMDIChildWnd*)GetWindowPtr(m_client.GetChild());
}

void RingMDIFrameWnd::CascadeChild(UINT flag/*=0*/)
{
	SendMessage(GetClient(),WM_MDICASCADE,0,0);
}

void RingMDIFrameWnd::TileChild(UINT flag/*=MDITILE_VERTICAL*/)
{
	SendMessage(GetClient(),WM_MDITILE,flag,0);
}

void RingMDIFrameWnd::CloseChild(HWND hWnd/*=NULL*/)
{
	if(hWnd == NULL)
		hWnd = GetCurrChildWnd();
	SendMessage(GetClient(),WM_MDIDESTROY,(WPARAM)hWnd,0L);
}

void RingMDIFrameWnd::ActiveChild(HWND hWnd)
{
	SendMessage(GetClient(),WM_MDIACTIVATE,(WPARAM)hWnd,0L);
}

void RingMDIFrameWnd::ArrangeIcon(UINT event)
{
	SendMessage(GetClient(),WM_MDIICONARRANGE,0,0L);
}

void RingMDIFrameWnd::MaxChild(UINT event)
{
	SendMessage(GetClient(),WM_MDIMAXIMIZE,SendMessage(GetClient(),WM_MDIGETACTIVE,0,0),0L);
}
	
void RingMDIFrameWnd::MinChild(UINT event)
{
	RingMDIChildWnd *child = GetCurrChild();
	if(child)
		child->Show(SW_MINIMIZE);
}

void RingMDIFrameWnd::RestoreChild(UINT event)
{
	SendMessage(GetClient(),WM_MDIRESTORE,SendMessage(GetClient(),WM_MDIGETACTIVE,0,0),0L);
}

BOOL RingMDIFrameWnd::KeepChildActive()
{
	HWND hwnd = GetClient();
	
	if(hwnd)
	{		
		HWND hwndChild = GetWindow(hwnd,GW_CHILD);
		if(hwndChild)
		{
			SendMessage(hwndChild,WM_CHILDACTIVATE,0,0);
			return TRUE;
		}
	}
	return FALSE;
}
/*
BOOL RingMDIFrameWnd::IsChildMax()
{
	HWND hwnd = (HWND)SendMessage(GetClient(),WM_MDIGETACTIVE,0,0);
	if(hwnd)
		return IsZoomed(hwnd);
	else
		return FALSE;
}
*/

RingMenuBar* RingMDIFrameWnd::GetMenuBar()
{
	if(m_menuBar)// && IsWindowPtr(m_menuBar))
		return m_menuBar;
	else
		return NULL;
/*	
	HWND hwnd;
	RingBaseWnd* obj;

	if(m_dockSite)
	{
		for(int i=0;i<4;i++)
			if(m_dockSite[i])
			{
				hwnd = m_dockSite[i]->GetChild();
				while(hwnd)
				{
					obj = GetWindowPtr(hwnd);
					if(obj && obj->GetWindowType() == ITIS_MENUBAR)
						return (RingMenuBar*)obj;
					
					hwnd = m_dockSite[i]->GetNextChild(hwnd);
				}
			}
	}
	return NULL;
*/
}

HWND RingMDIClient::Create(CLIENTCREATESTRUCT* lpccs)
{	
	m_nFirstChildid = lpccs->idFirstChild;
	m_parentMenu = (HMENU)lpccs->hWindowMenu;

	m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,"MDICLIENT",
                         NULL,
								 WS_VISIBLE | WS_CHILD | 
								 WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
								 MDIS_ALLCHILDSTYLES |
                         WS_VSCROLL | WS_HSCROLL,
                         0,0,0,0,
                         m_parent->Handle(),
                         (HMENU)0xCAC,
                         GetInstance(),
                         (LPVOID)lpccs);
	if(m_hWnd)
		SubClassWindow(m_hWnd);		
	
   //m_ID = 0xCAC;

   return m_hWnd;
}

LRESULT RingMDIClient::RingdowProc(HWND,RINGPARAMS param)
{	
	switch(param.uMsg)	
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
		case WM_KEYUP:
			return SendMessage(m_parent->Handle(),param.uMsg,param.wParam,param.lParam);
		case WM_NCDESTROY:
			if(m_extWnd)
				*m_extWnd = NULL;
			CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
			OnNcDestroy();
			m_hWnd = NULL;
			return 0;
		//case WM_MDIMAXIMIZE:
		//case WM_MDIMIN
		//case WM_MDIRESTORE:
		case WM_MDICREATE:
		//case WM_MDIDESTROY:
		case WM_MDINEXT:
		case WM_MDIACTIVATE:
			{
				RingCoolMenu* rm = ((RingMDIFrameWnd*)m_parent)->GetCoolMenu();
				if(m_parent->GetMenu() == NULL && rm)
				{
					LRESULT res = CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
					RefreshMenu(rm,NULL);//(HWND)param.wParam);
					//if(param.uMsg == WM_MDINEXT)
					//	UpdateMenuBar((HWND)param.lParam);
					//else
					UpdateMenuBar(NULL);//(HWND)param.wParam);
					return res;				
				}				
				break;
			}
		//case WM_MDISETMENU:
		//	return DefaultProc(param);
		case WM_MDIREFRESHMENU:
			{
				LRESULT res = CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
				RingCoolMenu* rm = ((RingMDIFrameWnd*)m_parent)->GetCoolMenu();
				if(rm)
				{
					int cnt = rm->GetSubMenuCount();
					while((--cnt) >= 0)
					{
						if(rm->SubMenu(cnt)->GetMenu() == m_parentMenu)
						{
							rm->SubMenu(cnt)->ReScan();
							break;
						}
					}
					UpdateMenuBar(NULL);
					//int cnt = rm->GetItemCount();
					//cnt -= 2;
					//if(cnt < 0)
					//	cnt = 0;
					//rm->SubMenu(cnt)->ReScan();
				}
				return res;
			}
		//*/
		/*case WM_MDIDESTROY:
		case WM_MDIGETACTIVE:
		case WM_MDICREATE:
		case WM_MDIREFRESHMENU:
		{
			LRESULT res = CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
			RingCoolMenu* rm = ((RingMDIFrameWnd*)m_parent)->GetCoolMenu();
			if(rm)
			{
				int cnt = rm->GetItemCount();
				cnt -= 2;
				if(cnt < 0)
					cnt = 0;
				rm->SubMenu(cnt)->ReScan();
			}
			//DrawMenuBar(m_parent->Handle());
//*/
//			return res;
//		}
	}
	return CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
}

/*************************************************************
//
//���¸����ڲ˵���
//
//		��Ϊ�˵����Ѹ����ڵĲ˵������������������޲˵��������
//		���������Ӵ��ڳ���10���������ڡ��˵��е��Ӵ����б�˵�
//		�������ʾ�´������Ӵ��ڲ˵�����ǵ�9���Ӵ��ڵĲ˵�
//		������ID�Ѱ󶨵��½��Ӵ��ڣ������Ҫ���øú���ʹ�����
//		��������ֻ��ı���˵�������֡�
//
//		��Ҫ���ñ������������
//			1�������Ӵ���ʱ(WM_MDICREATE)���������Զ��ж��Ƿ�
//				��10���Ӵ���
//			2���Ӵ���ͨ��ѡ��˵�����(WM_MDIACTIVATE)
//			3���Ӵ���ͨ����������(�Ӵ��ڵ�WM_ACTIVATE����
//				wParam����ΪWA_CLICKACTIVE)
//
*************************************************************/
void RingMDIClient::RefreshMenu(RingCoolMenu* rm,HWND hChildWnd)
{
	if(hChildWnd == NULL)
		hChildWnd = (HWND)SendMessage(m_hWnd,WM_MDIGETACTIVE,0,0);

	UINT id = hChildWnd?GetWindowLong(hChildWnd,GWL_ID):0;

	if(id - m_nFirstChildid == 8)
	{
		char txt[200],title[160];
		MENUITEMINFO mi;
		
		memset(title,0,160);
		GetWindowText(hChildWnd,title,160);
		wsprintf(txt,"&9 %s\0",title);
		
		mi.cbSize = sizeof(MENUITEMINFO);
		
		mi.fMask = MIIM_TYPE | MIIM_STATE;
		mi.fType = MFT_STRING;
		mi.dwTypeData = txt;
		mi.cch = strlen(txt);
		mi.fState = MFS_CHECKED;
		
		SetMenuItemInfo(m_parentMenu,id,FALSE,&mi);
		
		mi.fMask = MIIM_DATA | MIIM_TYPE;
		
		GetMenuItemInfo(m_parentMenu,id,FALSE,&mi);
		mi.fType |=	MFT_OWNERDRAW;
		SetMenuItemInfo(m_parentMenu,id,FALSE,&mi);
	}
}

void RingMDIClient::UpdateMenuBar(HWND hChildWnd)
{
	if(hChildWnd == NULL)
		hChildWnd = (HWND)SendMessage(m_hWnd,WM_MDIGETACTIVE,0,0);

	BOOL bmax;

	if(hChildWnd == NULL)
		bmax = FALSE;
	else
		bmax = IsZoomed(hChildWnd);

	if(bmax != m_bChildMax)
	{
		m_bChildMax = bmax;
		RingMenuBar *bar = ((RingMDIFrameWnd*)m_parent)->GetMenuBar();
		if(bar)
			bar->SetSysControlInfo(m_bChildMax);
	}	
}
