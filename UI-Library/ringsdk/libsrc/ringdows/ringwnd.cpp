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
//ԭ�ļ�����ringwnd.cpp
//
//���ļ�˵����Ӧ�ó��򴰿���ʵ�ִ��롣
//					RingWnd�� -- ��ʵ�������࣬��Ҫ�û��Զ���ʵ�ִ�����Ϣ��
//											 ��������ɸ������������ര��һ�����
//											 ��һ������ʵ����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

/*******************************************************
//
//	Ӧ�ó��򴰿���
//
*******************************************************/
RingWnd::RingWnd()
{
   m_hWnd = NULL;

	//m_windowInfo.lpfnWndProc = InitProc;
   m_windowInfo.hCursor = LoadCursor(NULL,IDC_ARROW);
   m_windowInfo.hIconBig = NULL;
   m_windowInfo.hIconSmall = NULL;
   m_windowInfo.lpszMenu = NULL;
   m_windowType = ITIS_WINDOW;

	m_pInterMenu = NULL;
	m_msgs = NULL;
	m_dockSite = NULL;	
	RingBaseWnd *w = NULL;

	//�Զ���⸸���ڶ���
	HWND hParentWnd = GetActiveWindow();
	if(hParentWnd)
		if(::IsWindow(hParentWnd))
		{
			w = GetWindowPtr(hParentWnd);
			//�����FLYBAR����Ҫ�����丸���ڶ���
			if(w && GetWindowLong(hParentWnd,GWL_ID) == ITIS_FLYBAR)
				w = w->Parent();
		}
	SetParentObj(w);
}

BOOL RingWnd::Register(LPCTSTR szClass,
                       HBRUSH hbkg,
                       LPCTSTR lpszMenu,
                       HICON hBigIcon,
                       HCURSOR hCur,
                       HICON hSmallIcon,
							  int windowType)
{
	if(szClass == NULL || szClass[0] == '\0')
		return FALSE;

	WNDCLASSEX wndclsex =
   {
   	sizeof(WNDCLASSEX),
		CS_DBLCLKS,// | CS_HREDRAW | CS_VREDRAW,
		(windowType == ITIS_MDICHILD)?(RingMDIChildWnd::InitChildProc):InitWndProc,
		0,//sizeof(RingBaseWnd*),
   	0,//sizeof(RingBaseWnd*),
   	GetInstance(),
      hBigIcon,
   	hCur,
		hbkg,
   	lpszMenu,
   	szClass,
   	hSmallIcon
   };
	if(m_windowType == ITIS_MDICHILD)
		wndclsex.style = 0;
   return RegRingdowClass(&wndclsex);
}

HICON RingWnd::SetIcon(HICON hIconBig,HICON hIconSmall)
{
   if(m_hWnd)
	{
		HICON hicon = (HICON)SendMessage(m_hWnd,WM_SETICON,ICON_BIG,(LONG)hIconBig);
		if(hIconSmall)
  			return (HICON)SendMessage(m_hWnd,WM_SETICON,ICON_SMALL,(LONG)hIconSmall);
		else
     	{
			SendMessage(m_hWnd,WM_SETICON,ICON_SMALL,(LONG)hIconBig);
			return hicon;
		}
	}
	else
   {
		m_windowInfo.hIconBig = hIconBig;
      m_windowInfo.hIconSmall = hIconSmall;
		return NULL;
   }
}

BOOL RingWnd::SetMenu(HMENU hMenu)
{
	return ::SetMenu(m_hWnd,hMenu);
}

BOOL RingWnd::SetMenu(LPCTSTR lpszMenu,HINSTANCE hInst/*=GetInstance()*/)
{
	if(m_hWnd == NULL)	//������δ����
		m_windowInfo.lpszMenu = lpszMenu;
	else
		return SetMenu(LoadMenu(hInst,lpszMenu));
	return TRUE;
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
void RingWnd::NotifyChild(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	
	HWND hChildWnd = GetChild();
	RingBaseWnd* tmp;
	
	if(m_dockSite)
	{
		for(int i=0;i<4;i++)
		{
			tmp = m_dockSite[i];
			if(tmp)
				tmp->OnParentNotify(hWnd,uMsg,wParam,lParam);
		}
	}
	//if(uMsg == WM_MOVE)	//WM_MOVE��Ϣ��֪ͨDockSite
	//	return;

	while(hChildWnd)
	{
		tmp = GetWindowPtr(hChildWnd);
		if(tmp && tmp->GetWindowType() != ITIS_DOCKSITE)// && tmp->Parent() == this )//&& tmp->IsControls())
			tmp->OnParentNotify(hWnd,uMsg/* + MYWM_BASEEX*/,wParam,lParam);

		hChildWnd = GetNextChild(hChildWnd);
	}
}

LRESULT RingWnd::NotifyChildProc(HWND hWnd,UINT uMsg,UINT wParam,LPARAM lParam)
{
   switch(uMsg)
   {
   	//case WM_NOTIFY:			
		case WM_ACTIVATE:
		case WM_SIZE:
      case WM_ENABLE:
		case WM_MOVE:
		//case WM_LBUTTONDOWN:
		//case WM_RBUTTONDOWN:
			NotifyChild(hWnd,uMsg,wParam,lParam);
         break;      
      case WM_MEASUREITEM:
      	OnMeasureCoolMenu((LPMEASUREITEMSTRUCT)lParam);
			NotifyChild(hWnd,uMsg,wParam,lParam);
			return TRUE;
      case WM_DRAWITEM:
     		OnDrawCoolMenu((LPDRAWITEMSTRUCT)lParam);			
			return NotifyChildDrawItem(hWnd,((LPDRAWITEMSTRUCT)lParam)->hwndItem,uMsg,wParam,lParam);			
   }
   return 0;
}

BOOL RingWnd::IsClipBoradFormatText()
{
	BOOL bOK = FALSE;
	if(OpenClipboard(m_hWnd))
	{
		if(IsClipboardFormatAvailable(CF_TEXT) ||
			IsClipboardFormatAvailable(CF_OEMTEXT))
		{
			bOK = TRUE;
		}
	}
	CloseClipboard();
	return bOK;
}

BOOL RingWnd::IsClipBoradFormat(UINT format)
{
	BOOL bOK = FALSE;
	if(OpenClipboard(m_hWnd))
		bOK = IsClipboardFormatAvailable(format);

	CloseClipboard();
	return bOK;
}

/************************************************************
//
//ת����Ϣ
//		����msgs���顣��Ϊmsgs�������COMMANDҪ�ɷ�����Ϣ������
//		����IDת������Ӧ����������˼�⵽��һ��COMMANDҪ�ɷ���
//		��Ϣ������COMMAND��Ϣʱת��DispCommand����
//		����REGCMDRANGE������uMsg��uId�ֶΣ����Ϊ��ֹuMsg���ú�
//		��Ϣ�ȼ۵������е�����������ж�cmdFunc�ֶ��Ƿ�Ϊ�գ���
//		���Ǵ�����Ϣ����COMMAND��Ϣ
//
************************************************************/
//RingMsgs������uMsg=��Ϣ����,uId=0,msgFunc=��Ϣ������,cmdFunc=NULL
LRESULT RingWnd::DispMessage(RINGVCLASS* obj,RingMsgs* msgs,HWND hWnd,RINGPARAMS& param)
{
   LRESULT res;
	BOOL bFind = FALSE;

	while(msgs && msgs->msgFunc)
   {		
		//REGCMD�������Ϣ
		if(msgs->cmdFunc != NULL && param.uMsg == WM_COMMAND)
			return DispCommand(obj,msgs,hWnd,param);
		else if(param.uMsg == WM_NOTIFY)//msgs->uMsg == 0 && msgs->cmdFunc == NULL &&
				//msgs->uId != 0 &&	param.uMsg == WM_NOTIFY)
			return DispNotify(obj,msgs,hWnd,param);
		else if(msgs->uMsg == param.uMsg && msgs->cmdFunc == NULL)
		{
			res = (obj->*(msgs->msgFunc))(hWnd,param);
			bFind = TRUE;
			break;
		}
		else
			msgs ++;
	}
	
	if(!bFind)
	{
		res = CtlColorProc(hWnd,param);
		if(res == RM_NOTPROCESS && m_windowType != ITIS_DIALOG)
		{
			res = DefaultProc(param);
			if(param.uMsg == WM_DESTROY && hWnd == GetMainWnd())
			{
				res = 0;
				PostQuitMessage(0);
			}			
		}
	}
		
	return res;
}

//����ת����ֻ�д�����Ϣ��WM_COMMAND��ת��ú���
//RingMsgs������uMsg=0,uId=�������,msgFunc=DispCommand,cmdFunc=�������
LRESULT RingWnd::DispCommand(RINGVCLASS* obj,RingMsgs*& msgs,HWND hWnd,RINGPARAMS& param)
{
	while(msgs->msgFunc)
	{
		if(msgs->cmdFunc)
		{
			if(param.shortval.low == msgs->uId ||
				(param.shortval.low <= msgs->uMsg && 
				 param.shortval.low >= msgs->uId))
			{
				if(msgs->uMsg == 0)
					(obj->*(msgs->cmdFunc))(param.shortval.high);
				else		//REGCMDRANGE����Ϣ�����봫����Ϣֵ
					(obj->*(msgs->cmdFunc))(param.shortval.low);
				return 0;
			}
		}
		else if(msgs->uMsg == WM_COMMAND)
			return (obj->*(msgs->msgFunc))(hWnd,param);		
		msgs ++;		
	}	
	return DefaultProc(param);
}

//֪ͨ��Ϣת����ֻ�д�����Ϣ��WM_NOTIFY��ת��ú���
//RingMsgs������uMsg=0,uId=֪ͨ����,msgFunc=������,cmdFunc=NULL
LRESULT RingWnd::DispNotify(RINGVCLASS* obj,RingMsgs*& msgs,HWND hWnd,RINGPARAMS& param)
{
	LRESULT res = -1;
	while(msgs->msgFunc)
	{
		if((msgs->uMsg == WM_NOTIFY) ||						//REGMSG(WM_NOTIFY)
			(param.lpnmhdr->code == msgs->uId && 			//REGNOTIFY(֪ͨ��Ϣ����)
			(msgs->cmdFunc == NULL && msgs->uMsg == 0))) 
		{
			res = (obj->*(msgs->msgFunc))(hWnd,param);
		}

		//��ؼ�ͬһNOTIFY��Ϣ��������ó���δȫ��������ؼ���Ҫ����,
		//���ܾʹ�return
		if(res == RM_NOTPROCESS)
		{
			res = -1;
			break;
		}
		else if(res != -1)
			return res;

		msgs ++;
	}
	//δ�ҵ���������ת�����ؼ��Լ�����
	RingBaseWnd *w = GetWindowPtr(param.lpnmhdr->hwndFrom);
	
	if(w)
		res = w->OnDefNotify(this,param);

	if(res == -1)
		return DefaultProc(param);
	else
		return res;
}

void RingWnd::OnCreate(HWND hWnd)
{
   if(m_windowType > ITIS_MAINWINDOW)
   {
		//�������ڣ�����ȫ��m_hWnd
    	InitRingLib(hWnd,GetInstance());
		m_windowType -= ITIS_MAINWINDOW;
   }
}

RingCoolMenu* RingWnd::LoadCoolMenu(LPCTSTR lpszMenu,BOOL bPopup,HINSTANCE hInst/*=GetInstance()*/)
{
	RingCoolMenu* rm;
	
   try
	{
		if(lpszMenu)
		{
			rm = new RingCoolMenu;
			if(bPopup)
				rm->LoadPopup(lpszMenu,0,hInst);
			else
				rm->Load(lpszMenu,hInst);
		}
	}
	catch(...)
	{
		return NULL;
	}
	
   return rm;
}

RingCoolMenu* RingWnd::InstallCoolMenu(LPCTSTR lpszMenu,BOOL bPopup,HINSTANCE hInst/*=GetInstance()*/)
{
   if(m_pInterMenu)
		return m_pInterMenu;

	RingCoolMenu* rm;
	
   try
	{
		if(lpszMenu)
		{
			rm = new RingCoolMenu;
			if(bPopup)
				rm->LoadPopup(lpszMenu,0,hInst);
			else
				rm->Load(lpszMenu,hInst);
		}
		else
			rm = new RingCoolMenu(::GetMenu(m_hWnd),TRUE);
	}
	catch(...)
	{
		return NULL;
	}
      
	rm->SetItemDrawFunc(FUNC_DRAWMENU_DEF,TRUE,FUNC_DRAWPARENT_DEF);
	
	m_pInterMenu = rm;

   return rm;
}

RingCoolMenu* RingWnd::InstallXPMenu(LPCTSTR lpszMenu,BOOL bPopup,HINSTANCE hInst/*=GetInstance()*/)
{
   RingCoolMenu* rm = InstallCoolMenu(lpszMenu,bPopup,hInst);
	
	if(rm)
		rm->SetXPStyle();

   return rm;
}

void RingWnd::OnMeasureCoolMenu(LPMEASUREITEMSTRUCT lParam)
{
	if(lParam->CtlType == ODT_MENU)
	{
		LPRINGMENUSTYLE lprms = (LPRINGMENUSTYLE)lParam->itemData;
		if(lprms)
		{
			try
			{
				lprms->pMenu->MeasureItem(lParam);
			}
			catch(...)
			{
			}
		}		
	}
}

void RingWnd::OnDrawCoolMenu(LPDRAWITEMSTRUCT lParam)
{
	if(lParam->CtlType == ODT_MENU)
	{
		LPRINGMENUSTYLE lprms = (LPRINGMENUSTYLE)lParam->itemData;
		if(lprms)
		{
			try
			{
				lprms->pMenu->Draw(lParam);
			}
			catch(...)
			{
			}
		}
	}
}

void RingWnd::SetCursor(HCURSOR hCur)
{
   if(m_hWnd)
   	RingBaseWnd::SetCursor(hCur);
   else
		m_windowInfo.hCursor = hCur;
}

HWND RingWnd::Create(LPCTSTR szTitle,LPCTSTR szClass,
							DWORD dwStyle,DWORD dwExStyle,							
							int x,int y,int width,int height,
							HWND hParentWnd/*=NULL*/,HMENU hMenu/*=NULL*/,LPARAM lParam/*=0*/)
{
   if( !Register( szClass,
   					(HBRUSH)(COLOR_WINDOW + 1),
                  m_windowInfo.lpszMenu,
                  m_windowInfo.hIconBig,
                  m_windowInfo.hCursor,
                  m_windowInfo.hIconSmall))
   	return NULL;

   CreateEx(szTitle,szClass,dwStyle,dwExStyle,hParentWnd,hMenu,x,y,width,height,lParam);
   return m_hWnd;
}

LRESULT RingWnd::RingdowProc(HWND hWnd,RINGPARAMS param)
{
   if(param.uMsg == WM_CREATE)
   {
		param.lpcreate->lpCreateParams = 
				(LPVOID)((LPRINGWNDPARAM)(param.lpcreate->lpCreateParams))->dwInitParam;
		OnCreate(hWnd);
	}
	
	NotifyChildProc(hWnd,param.uMsg,param.wParam,param.lParam);
   return DispMessage((RINGVCLASS*)this,m_msgs,hWnd,param);
}

RingDockSite* RingWnd::GetDockSite(int index)
{
	if(m_dockSite == NULL || index < TBS_FLAG_DOCKTOP || index > TBS_FLAG_DOCKRIGHT)
		return NULL;
	else
		return m_dockSite[index];
}

RingDockSite* RingWnd::CreateDockSite(int index)
{
	if(index < TBS_FLAG_DOCKTOP || index > TBS_FLAG_DOCKRIGHT)
		return NULL;

	if(m_dockSite == NULL)
	{
		m_dockSite = (RingDockSite**)New(sizeof(RingDockSite*) << 2);
		if(m_dockSite == NULL)
			return NULL;
	}
	if(m_dockSite[index] == NULL)
	{
		m_dockSite[index] = new RingDockSite(this,index);
		if(m_dockSite[index] == NULL)
			return NULL;
		else
			m_dockSite[index]->Create();
	}
	//m_dockSite[index]->Show(SW_SHOW);
	return m_dockSite[index];
}

BOOL RingWnd::GetClientRect(LPRECT lprc)
{
	if(lprc == NULL)
		return FALSE;
	
	BOOL bOK = ::GetClientRect(m_hWnd,lprc);
		
	if(m_dockSite)
	{
		RECT rt;
		
		for(int i=0;i<4;i++)
		{
			if(m_dockSite[i] && IsWindowVisible(m_dockSite[i]->Handle()))
			{
				GetWindowRect(m_dockSite[i]->Handle(),&rt);
				ScreenToClient(m_hWnd,(LPPOINT)&rt);
				ScreenToClient(m_hWnd,(LPPOINT)&rt+1);
				
				switch(i)
				{
					case TBS_FLAG_DOCKTOP:
						lprc->top = rt.bottom;
						break;
					case TBS_FLAG_DOCKBOTTOM:
						lprc->bottom = rt.top;
						break;
					case TBS_FLAG_DOCKLEFT:
						lprc->left = rt.right;
						break;
					case TBS_FLAG_DOCKRIGHT:
						lprc->right = rt.left;
						break;
				}
			}
		}
	}
	return TRUE;
}

/*
BOOL RingWnd::GetClientRect(LPRECT lprc,BOOL bWithScrollSize)
{
	if(lprc == NULL)
		return FALSE;
	
	BOOL bOK = ::GetClientRect(m_hWnd,lprc);
	int cxr=0,cyr=0;
	
	if(!bWithScrollSize)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask  = SIF_RANGE;
		if(GetScrollInfo(SB_HORZ,&si))
			cyr = GetSystemMetrics(SM_CYHSCROLL);
		if(GetScrollInfo(SB_VERT,&si))
			cxr = GetSystemMetrics(SM_CXVSCROLL);		
	}
	
	if(m_dockSite)
	{
		RECT rt;
		
		for(int i=0;i<4;i++)
		{
			if(m_dockSite[i] && IsWindowVisible(m_dockSite[i]->Handle()))
			{
				GetWindowRect(m_dockSite[i]->Handle(),&rt);
				ScreenToClient(m_hWnd,(LPPOINT)&rt);
				ScreenToClient(m_hWnd,(LPPOINT)&rt+1);
				
				switch(i)
				{
				case TBS_FLAG_DOCKTOP:
					lprc->top = rt.bottom;
					break;
				case TBS_FLAG_DOCKBOTTOM:
					lprc->bottom = rt.top;
					break;
				case TBS_FLAG_DOCKLEFT:
					lprc->left = rt.right;
					break;
				case TBS_FLAG_DOCKRIGHT:
					lprc->right = rt.left;
					break;
				}
			}
		}
	}
	lprc->right -= cxr;
	lprc->bottom -= cyr;
	return TRUE;
}
*/

BOOL RingWnd::SetDockBackGround(LPCTSTR lpszBmp,BOOL bSetOrg/*=FALSE*/,
										  int index/*=-1*/,HWND hStandWnd/*=0xFFFFFFFF*/,
										  HINSTANCE hInst/*=GetInstance()*/)
{
	RingDockSite* pSite;
	if(index == -1)
		pSite = GetFirstDockSite();
	else
		pSite = GetDockSite(index);

	if(pSite)
	{
		pSite->SetStandWindow(hStandWnd);
		pSite->SetBgBmp(lpszBmp,bSetOrg,hInst);
		return TRUE;
	}
	return FALSE;
}

BOOL RingWnd::SetDockBackGround(HBITMAP hbmp,BOOL bSetOrg/*=FALSE*/,
										  int index/*=-1*/,HWND hStandWnd/*=0xFFFFFFFF*/)
{
	RingDockSite* pSite;
	if(index == -1)
		pSite = GetFirstDockSite();
	else
		pSite = GetDockSite(index);
	
	if(pSite)
	{
		pSite->SetStandWindow(hStandWnd);
		pSite->SetBgBmp(hbmp,bSetOrg);
		return TRUE;
	}
	return FALSE;
}

BOOL RingWnd::SetDockBackGround(COLORREF crColor,int index/*=-1*/,HWND hStandWnd/*=0xFFFFFFFF*/)
{
	RingDockSite* pSite;
	if(index == -1)
		pSite = GetFirstDockSite();
	else
		pSite = GetDockSite(index);
	
	if(pSite)
	{
		pSite->SetStandWindow(hStandWnd);
		pSite->SetBgColor(crColor);
		return TRUE;
	}
	return FALSE;
}

RingDockSite* RingWnd::GetFirstDockSite()
{
	if(m_dockSite == NULL)
		return NULL;
	else
		for(int i=0;i<4;i++)
			if(m_dockSite[i])
				return m_dockSite[i];
	return NULL;
}

RingCoolMenu* RingWnd::GetDockMenu()
{
	CreateDockSite(TBS_FLAG_DOCKTOP);
	if(m_dockSite[TBS_FLAG_DOCKTOP])
		return m_dockSite[TBS_FLAG_DOCKTOP]->GetDockSiteMenu();
	else
		return NULL;
}

/************************************************
//
//��������ť��TIP��ʾĬ�ϴ���
//
//������Դ�ļ��������ֵΪ��ťID+MH_BASE���ַ���
//
//��ťID+MH_BASE��ΪWM_MENUSELECT�˵�ѡ��������裨����MenuHelp����
//�ַ���ID�����ǲ˵�����ID+MH_BASEֵ��Ϊ����ͳһ������Ҳ
//����MH_BASEֵ
//
//**********************************************/
LRESULT RingWnd::On_WM_RINGTOOLTIP(HWND,RINGPARAMS& param)
{
	LPTOOLTIPTEXT lptt = (LPTOOLTIPTEXT)param.lParam;
	
	lptt->hinst = GetInstance();
	lptt->lpszText = MAKEINTRESOURCE(lptt->hdr.idFrom + MH_BASE);
	
	return 0;
}

LRESULT RingWnd::On_WM_MENUSELECT(HWND,RINGPARAMS& param)
{
	//if(param.shortval.high & MF_POPUP)
	//	return 0;
	
	if(m_dockSite && m_dockSite[TBS_FLAG_DOCKBOTTOM])
	{
		HWND hchild = m_dockSite[TBS_FLAG_DOCKBOTTOM]->GetChild();
		RingBaseWnd* wnd = GetWindowPtr(hchild);
		while(wnd)
		{
			if(wnd->GetWindowType() == ITIS_STATUSBAR)
				return OnMenuSelect(wnd->Handle(),param);
			else
			{
				hchild = m_dockSite[TBS_FLAG_DOCKBOTTOM]->GetNextChild(hchild);
				wnd = GetWindowPtr(hchild);
			}
		}
	}
	return 0;
}

LRESULT RingWnd::OnMenuSelect(HWND hStatusWnd,RINGPARAMS& param,HINSTANCE hInst/*=GetInstance()*/)
{
	if(m_pInterMenu)
	{
		UINT uIds[] = {MH_BASE,MH_POPUPBASE,0,0};
		MenuHelp(WM_MENUSELECT,param.wParam,param.lParam,m_pInterMenu->GetMenu(),hInst,hStatusWnd,uIds);
	}
	return 0;
}

void RingWnd::UpdateDockSite()
{
	RECT rc;
	::GetClientRect(m_hWnd,&rc);
	NotifyChild(m_hWnd,WM_DOCKSITESIZECHANGE,0,MAKELONG(rc.right,rc.bottom));
	//SendMessage(m_hWnd,WM_SIZE,SIZE_RESTORED,MAKELONG(rc.right,rc.bottom));
}

//////////////////////////////////////////////////////////////
//
//����������
//
/////////////////////////////////////////////////////////////
int RingWnd::SetScrollPosHV(int nPosX,int nPosY,BOOL bRedraw)
{
	int x = 0;
	if(nPosX >= 0)
		x = SetScrollInfo(SB_HORZ,SIF_POS,0,0,0,nPosX,bRedraw);
	if(nPosY >= 0)
		x = SetScrollInfo(SB_VERT,SIF_POS,0,0,0,nPosY,bRedraw);
	return x;
}

int RingWnd::SetScrollRangePage(int nBar,int nMin,int nMax,int nPage,BOOL bRedraw)
{
	return SetScrollInfo(nBar,SIF_PAGE|SIF_RANGE,nMin,nMax,nPage,0,bRedraw);
}

int RingWnd::SetScrollInfo(int nBar,int nMask,int nMin,int nMax,int nPage,int nPos,
									int nTrackPos,BOOL bRedraw)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = nMask;
	si.nMin = nMin;
	si.nMax = nMax;
	si.nPage = nPage;
	si.nPos = nPos;
	si.nTrackPos = nTrackPos;
	
	return ::SetScrollInfo(m_hWnd,nBar,&si,bRedraw);
}

BOOL RingWnd::GetScrollRange(int nBar,LPSIZE lpSize)
{
	if(lpSize)
		return ::GetScrollRange(m_hWnd,SB_CTL,(LPINT)&lpSize->cx,(LPINT)&lpSize->cy);
	else
		return FALSE;
}

BOOL RingWnd::GetScrollInfo(int nBar,LPSCROLLINFO lpsi)
{
	if(lpsi)
		return ::GetScrollInfo(m_hWnd,nBar,lpsi);
	else
		return FALSE;
}

BOOL RingWnd::SetScrollInfo(int nBar,LPSCROLLINFO lpsi,BOOL bRedraw)
{
	if(lpsi)
		return ::SetScrollInfo(m_hWnd,nBar,lpsi,bRedraw);
	else
		return FALSE;
}

int RingWnd::GetScrollLimit(int nBar)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE;
	if(::GetScrollInfo(m_hWnd,nBar,&si))
		return si.nMax;
	else
		return 0;
}

