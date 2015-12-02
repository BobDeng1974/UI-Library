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
//ԭ�ļ�����ctrlbar.cpp
//
//���ļ�˵����ͣ����ʵ�ִ��룬����IE���ƶ��Ĳ˵�����������
//
//���˼·��
//		RingSDK��Ƴ�����ؼ����Ӵ��ھ���ͣ����ͣ�������Ǵ��ڣ�������
//	��Щ��ͣ������ͣ�����϶�ʱ��һЩ��Ϣ�������ڵ�ͣ�����϶�������
//	���ڵ��϶����ֵȡ�������Ϊ����״̬ʱ�ᴴ�����������ڣ�����ͣ��
//	���ڡ���������������Ӧͣ�����ڳߴ硣
//
//	ͣ��ʵ�֣�
//		�������ı߻������Ҫ����ͣ����������ͣ��������ͣ����
//	����������������״̬����Ƴɱ���ͣ����Ĭ���޷��϶����������
//	EnableDock�ſ��϶�������������Ƿ�����������ڿͻ����ߴ磬�۵�
//	�ĸ�ͣ���������ߴ缴�ɡ�
//
//	����ʵ�֣�
//		ͣ�������������DockBar::Draw���Ʊ��������֣�ͣ�����߿򣬶�
//	�ڸ���״̬��FlyBar�����������øú���������ޱ߿򼰰��֡�
//
//	�϶�ʵ�֣�
//		��ʼ�϶�ʱ��Ҫ��ʼ��3�����Σ����������ͣ�����Σ��߿���Ϊ1����
//	�������Σ��߿���Ϊ3�����϶�ʱ�������λ��ƫ����Щ���β���ʱ������
//	�ж�ͣ��λ�þ�����ʾ�ĸ����Ρ�
//	
//	�����ڳߴ�䶯��Ӧ��
//		�����ڳߴ�䶯��ͣ������������ͣ�����ڵ��������У��������β�ѯ
//	ÿһͣ����ǰ���λ�ߴ磬��ȡ�����пɵ���λ�ÿռ�ߴ磬Ȼ�����
//	�������һ��ͣ������AdjustDockPos����ͣ������λ�ã�ͣ���������
//	��������ǰ�洰���ص��������ǰһ��ͣ�����ĸú�������������Ӧֱ��
//	���ڴ���ȫ��������ϡ�
//
//	ͣ�����ڳߴ���Ӧ��
//		ͣ�����ڳߴ�䶯�����������˵�����ɾ��ť�ȣ��ɹ�����֪ͨͣ������
//	ͣ�������³ߴ��֪ͨ�����ڣ���������Ӧ����������ڳߴ�䶯�¼�������
// �����ڳߴ�䶯��Ӧ��
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdows.h"

DWORD	gdwDockBarId = 39000;

RingFlyBar::RingFlyBar(RingBaseWnd* parent)
{
   SetParentObj(parent);
   m_child = NULL;
	m_dragger = NULL;
	SetAutoDelete();
   m_windowType = ITIS_FLYBAR;
}

RingFlyBar::~RingFlyBar()
{
	if(m_child && m_child->m_State == TBS_FLAG_FLY)
		delete m_child;
}

BOOL RingFlyBar::Create(LPCTSTR lpszTitle)
{
   if(m_hWnd)
		return TRUE;

	if(!Register("RingFlyBar",
		(HBRUSH)(COLOR_BTNFACE + 1),
		NULL,
		NULL,
		m_windowInfo.hCursor,
		NULL))
		return NULL;
	
   CreateEx(lpszTitle,"RingFlyBar",//0x94CC1200, 
		WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME | 
		/*WS_OVERLAPPED |*/ WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		WS_EX_TOOLWINDOW,NULL,NULL);//,0,0,0,0);
	
   RingMenu rm;
	rm.Attach(GetSystemMenu(m_hWnd,FALSE));
	//rm.Delete(SC_CLOSE);
	//rm.Delete(SC_SIZE);
	rm.Delete(SC_MINIMIZE);
	rm.Delete(SC_MAXIMIZE);
	rm.Delete(SC_MOVE);
	rm.Delete(SC_RESTORE);

	return (BOOL)m_hWnd;
}

LRESULT RingFlyBar::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	if(param.uMsg >= WM_KEYFIRST && param.uMsg <= WM_KEYLAST)
	{
		//��Ϣת�����Ӵ���
		if(m_child && m_child->GetChild())
			return SendMessage(m_child->GetChild()->Handle(),param);
		else
			return DefaultProc(param);
	}
	switch(param.uMsg)
   {
		case WM_COMMAND:
			//��Ϣת����������
			if(m_child && m_child->GetChild())
				return SendMessage(m_child->GetChild()->Parent()->Handle(),param);
			else
				return DefaultProc(param);
		case WM_CONTEXTMENU:
			//ȡ�������˵�
			//if(HIWORD(lParam))
			//PostMessage(hWnd,WM_CANCELMODE,0,0);
			return 0;
		case WM_SHOWWINDOW:
			if(param.wParam)
			{				
				Update();
			}
			return DefaultProc(param);
		case WM_PAINT:
			//����Ӵ�����ʾ״̬��������������Ԥ�������߻���ң�
			if(m_child && m_child->GetChild() && !m_child->GetChild()->IsVisible())
				m_child->GetChild()->Show();
			return DefaultProc(param);
		case WM_CLOSE:
      	Show(SW_HIDE);
			if(m_child && m_child->m_pSite)
				m_child->m_pSite->OnDockBarHide(m_child);
         break;
      case WM_ACTIVATE:
         if(IsWindowEnabled(hWnd))
         {
	         SendMessage(m_parent->Handle(),WM_NCACTIVATE,TRUE,0);
   		   SendMessage(m_hWnd,WM_NCACTIVATE,TRUE,0);
         }
         break;
		case WM_NOTIFY:
			switch(param.lpnmhdr->code)
			{
			case TTN_NEEDTEXT:
				{
					((LPTOOLTIPTEXT)param.lParam)->hinst = GetInstance();
					return SendMessage(m_parent->Handle(),WM_RINGTOOLTIP,((LPTOOLTIPTEXT)param.lParam)->hdr.idFrom,param.lParam);
				}
				
				//case TBN_DROPDOWN:
				//������DropDown��ť
				//	return SendMessage(param.lpnmhdr->hwndFrom,param);
			}
			if(IsWindowPtr(m_parent))
				return SendMessage(m_parent->Handle(),WM_NOTIFY,param.wParam,param.lParam);
			return DefaultProc(param);
      case WM_NCLBUTTONDBLCLK:
			if(param.wParam == HTCAPTION)
         {
				if(m_child)
					m_child->DockBack();
			}
			return 0;
		case WM_NCLBUTTONDOWN:
         if(param.wParam == HTCAPTION)
         {
				if(m_child)
				{
					POINT pt = {param.mousept.x,param.mousept.y};
					RingDockSite* pSite = m_child->m_pSite;
					if(pSite == NULL && m_child->m_child)
						pSite = ((RingWnd*)(m_child->GetChild()->Parent()))->CreateDockSite(TBS_FLAG_DOCKTOP);

					if(pSite == NULL)
						return 0;

					if(m_dragger == NULL)
						m_dragger = &pSite->m_dragger;
					
					ScreenToClient(m_hWnd,&pt);
					
					param.mousept.x = (short)pt.x;
					param.mousept.y = (short)pt.y;
					m_child->StartDrag(m_hWnd,m_dragger,param);
				}
            return 0;
         }
         else
         	return DefaultProc(param);
		case WM_MOUSEMOVE:
			if(m_dragger && m_dragger->IsDragging() && m_child)
				return m_child->OnDrag(m_hWnd,m_dragger,param);
			return DefaultProc(param);
		case WM_LBUTTONUP:
			if(m_dragger && m_dragger->IsDragging() && m_child)
				return m_child->StopDrag(m_child->GetChild()->Parent()->Handle(),m_dragger,param);
			return DefaultProc(param);
		/*case WM_ERASEBKGND:
			if(!DrawBkg(param.hdc))
				DefaultProc(param);
			break;
		*/
		case WM_SIZE:
			//NotifyChild(hWnd,param.uMsg,param.wParam,param.lParam);
			if(m_child)
			{
				m_child->UpdateFlySize(param.mousept.x + WINVAR_FRAME_X*2,
												param.mousept.y + BARTOOLY + WINVAR_FRAME_Y);
				/*if(m_child->m_child)
				{
					RECT rc;
					GetWindowRect(m_child->m_child->Handle(),&rc);
					OffsetRect(&rc,-rc.left,-rc.top);
					m_child->UpdateSize(rc.right,rc.bottom,FALSE);
				}
				*/
			}
			break;
		case WM_SIZING:
		{
			RECT rctmp;
			LPRECT lprc = (LPRECT)param.lParam;
						
			CopyRect(&rctmp,lprc);
			param.lParam = (LPARAM)&rctmp;
			InflateRect(&rctmp,-WINVAR_FRAME_X,-WINVAR_FRAME_Y);
			rctmp.top += (BARTOOLY);// - WINVAR_FRAME_Y);
			
			if(m_child)
				m_child->OnSizing(param);
			//SendMessage(GetChild(),param);
			InflateRect(&rctmp,WINVAR_FRAME_X,WINVAR_FRAME_Y);
			rctmp.top -= (BARTOOLY);// - WINVAR_FRAME_Y);
			param.lParam = (LPARAM)lprc;
			CopyRect(lprc,&rctmp);
			return DefaultProc(param);
			/*
			switch(param.wParam)
			{
				case WMSZ_LEFT:
					lprc->left = lprc->right - rc.right - WINVAR_FRAME_X*2;
					break;
				case WMSZ_RIGHT:
					lprc->right = lprc->left + rc.right + WINVAR_FRAME_X*2;
					break;
				case WMSZ_TOP:
					lprc->top = lprc->bottom - rc.bottom - BARTOOLY - WINVAR_FRAME_Y - 2;
					break;
				case WMSZ_TOPLEFT:
					lprc->top = lprc->bottom - rc.bottom - BARTOOLY - WINVAR_FRAME_Y - 2;
					lprc->left = lprc->right - rc.right - WINVAR_FRAME_X*2;
					break;
				case WMSZ_TOPRIGHT:
					lprc->top = lprc->bottom - rc.bottom - BARTOOLY - WINVAR_FRAME_Y - 2;
					lprc->right = lprc->left + rc.right + WINVAR_FRAME_X*2;
					break;
				case WMSZ_BOTTOM:
					lprc->bottom = lprc->top + rc.bottom + BARTOOLY + WINVAR_FRAME_Y + 2;
					break;
				case WMSZ_BOTTOMLEFT:
					lprc->left = lprc->right - rc.right - WINVAR_FRAME_X*2;
					lprc->bottom = lprc->top + rc.bottom + BARTOOLY + WINVAR_FRAME_Y + 2;
					break;
				case WMSZ_BOTTOMRIGHT:
					lprc->right = lprc->left + rc.right + WINVAR_FRAME_X*2;
					lprc->bottom = lprc->top + rc.bottom + BARTOOLY + WINVAR_FRAME_Y + 2;
					break;
			}*/			
		}
      default:
		{	
			LRESULT res = CtlColorProc(hWnd,param);
			if(res != RM_NOTPROCESS)
				return res;
			else
				return DefaultProc(param);
		}
   }
   return 0;
}
/*
BOOL RingFlyBar::DrawBkg(HDC hDC,LPRECT lprc/*=NULL/)
{
	RingDockSite* tmp = NULL;
	
	if(m_child == NULL)
		return FALSE;

	RingWnd* pa = (RingWnd*)m_child->m_parent;

	for(int i=0;i<4;i++)
	{
		tmp = pa->GetDockSite(i);
		if(tmp)
			break;
	}
	
	if(tmp)
	{
		RECT rc;
		::GetClientRect(m_hWnd,&rc);
		ClientToScreen(m_hWnd,(LPPOINT)&rc);
		ClientToScreen(m_hWnd,(LPPOINT)&rc+1);
		ScreenToClient(pa->Handle(),(LPPOINT)&rc);
		ScreenToClient(pa->Handle(),(LPPOINT)&rc+1);
		//CopyRect(&rc,(LPRECT)&m_winpos);
		if(lprc)
			CopyRect(lprc,&rc);
		//OffsetRect(&rc,tmp->m_winpos.x,tmp->m_winpos.y);
		return tmp->DrawBkg(hDC,&rc);			
	}
	return FALSE;
}
*/
BOOL RingFlyBar::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG)
{
   if(hWnd == m_parent->Handle() && IsWindowVisible(m_hWnd))
		if(uMsg == WM_ENABLE)
      {
       	EnableWindow(m_hWnd,(BOOL)wParam);
         if(wParam)
   	   	SendMessage(m_hWnd,WM_NCACTIVATE,TRUE,0);
         else
         {
         	SendMessage(m_hWnd,WM_NCACTIVATE,FALSE,0);
            SendMessage(m_parent->Handle(),WM_NCACTIVATE,FALSE,0);
         }
      }
		//else if(uMsg == WM_SIZE)
		//	InvalidateRect(m_hWnd,NULL,TRUE);

	return FALSE;
}

void RingFlyBar::AddChild(RingDockBar* child)
{
	if(child)
	{
		m_child = child;
		
		RingBaseWnd *pwnd = child->GetChild();
		if(pwnd == NULL)
			return;

		if(child->IsMenuBar())
		{
			RingMenu rm;
			rm.Attach(GetSystemMenu(m_hWnd,FALSE));
			rm.EnableItem(SC_CLOSE,FALSE);			
		}
		pwnd->SetParent(m_hWnd);
		pwnd->SetPos(0,0);
	}
}

void RingFlyBar::Update()
{
	if(m_child)
	{
		RECT rc;
		RingBaseWnd* wnd = m_child->GetChild();
		
		if(wnd)
		{
			GetWindowRect(wnd->Handle(),&rc);
			OffsetRect(&rc,-rc.left,-rc.top);
			if(m_child->IsSizeBar())
				SetPos(0,0,rc.right,rc.bottom,0,SWP_NOMOVE);
			else
				SetPos(0,0,rc.right + 2 + WINVAR_FRAME_X*2,
						rc.bottom + 2 + BARTOOLY + WINVAR_FRAME_Y,0,SWP_NOMOVE);
		}
	}
}

void RingFlyBar::UpdateDockSite()
{
	if(m_child)
	{
		RingBaseWnd* pwnd = m_child->GetChild();
		if(pwnd)
		{
			SendMessage(pwnd->Handle(),WM_DOCKSTATECHANGE,
					MAKELONG(TBS_FLAG_FLY,TBS_FLAG_FLY),(LPARAM)pwnd->Handle());			
			Update();
			InvalidateRect(pwnd->Handle(),NULL,FALSE);
		}
	}
}

//////////////////////////////////////////////////////////
//
//	RingDockBar
//
//////////////////////////////////////////////////////////
RingDockBar::RingDockBar()
{
	m_State = TBS_FLAG_NONE;
	m_uStyle = TBS_EX_NONE;
	m_id = gdwDockBarId ++;

	SetRectEmpty(&m_rcDockPos);
	SetRectEmpty(&m_rcClient);
	SetRectEmpty(&m_hrc);
	SetRectEmpty(&m_vrc);
	SetRectEmpty(&m_frc);
	SetRectEmpty(&m_lfrc);
	SetRectEmpty(&m_ldrc);

	m_pSite = NULL;
	m_next = m_prev = NULL;
	m_lineInfo = NULL;
	m_flyBar = NULL;
	m_BorderSpace = 3;
	m_GripperSpace = 9;
	m_bNCBtDown = FALSE;
	m_nSize = 0;
	m_child = NULL;
	m_szTitle = NULL;
	m_nTitleSize = 0;
	m_bVisible = TRUE;
}

RingDockBar::~RingDockBar()
{
	if(m_szTitle)
		Del(m_szTitle);
}

void RingDockBar::SetDockStyle(UINT style)
{
	m_uStyle = style;
	//SizeBar����TBS_EX_NONE��TBS_EX_CANCLOSE���棬��������ж�˳�򲻿ɸ��
	if(style & TBS_EX_NONE)
	{
		m_GripperSpace = 0;
		if(style & TBS_EX_ALLLINE)
			m_BorderSpace = 2;
		else
			m_BorderSpace = 0;
	}
	else
	{
		m_GripperSpace = 9;
		m_BorderSpace = 3;
	}		

	if((style & TBS_EX_CANCLOSE))
		m_GripperSpace = 15;
}

BOOL RingDockBar::GetDockRect(LPRECT lprc)
{
	if(lprc && m_child && m_pSite)
	{
		if(m_State == TBS_FLAG_FLY)
		{
			if(m_flyBar)
			{
				GetWindowRect(m_flyBar->Handle(),lprc);
				MapWindowPoints(m_flyBar->Handle(),m_pSite->Parent()->Handle(),(LPPOINT)lprc,2);
				return TRUE;
			}
		}
		else
		{
			CopyRect(lprc,&m_rcDockPos);
			MapWindowPoints(m_pSite->Handle(),m_pSite->Parent()->Handle(),(LPPOINT)lprc,2);
			return TRUE;
		}
	}
	return FALSE;
}

//�õ�ָ��ͣ��λ�õĿͻ�����
BOOL RingDockBar::GetClientRect(int state,LPRECT lprc)
{
	if(m_child && lprc)
	{
		LONG x = 0;
		//���򸸴��ڲ�ѯ
		NMHDR hdr;
		hdr.hwndFrom = m_child->Handle();
		hdr.code = state;
		hdr.idFrom = (UINT)m_child;

		if(m_pSite)
			x = SendMessage(m_pSite->Parent()->Handle(),WM_GETDRAGRECTS,
									(WPARAM)lprc,(LPARAM)&hdr);
		else if(m_flyBar)
			x = SendMessage(m_flyBar->Parent()->Handle(),WM_GETDRAGRECTS,
									(WPARAM)lprc,(LPARAM)&hdr);
		//��ѯʧ�ܣ���ؼ���ѯ
		if(x == 0)
			x = SendMessage(m_child->Handle(),WM_GETDRAGRECTS,(WPARAM)lprc,(LPARAM)&hdr);
		//��ʧ�ܣ���ȡ�ؼ��ߴ紦��
		if(x == 0)
		{
			if(m_flyBar && IsWindow(m_flyBar->Handle()))
				m_flyBar->GetClientRect(lprc);
			else
				GetWindowRect(m_child->Handle(),lprc);
		}
		return TRUE;
	}
	else
		return FALSE;
}

LPRECT RingDockBar::GetDragRects(int state)
{
	if(!GetClientRect(state,&m_rcCurr))
		CopyRect(&m_rcCurr,&m_rcDockPos);

	//OffsetRect(&m_rcCurr,-m_rcCurr.left,-m_rcCurr.top);

	if(state == TBS_FLAG_FLY)
	{
		if(!IsSizeBar())
		{
			m_rcCurr.top -= BARTOOLY;
			m_rcCurr.bottom += WINVAR_FRAME_Y;
			m_rcCurr.left -= WINVAR_FRAME_X;
			m_rcCurr.right += WINVAR_FRAME_X;
		}
	}
	else
	{
		InflateRect(&m_rcCurr,m_BorderSpace,m_BorderSpace);

		if(::IsVert(state))
			m_rcCurr.top -= m_GripperSpace + m_BorderSpace;
		else
			m_rcCurr.left -= m_GripperSpace + m_BorderSpace;
	}
	OffsetRect(&m_rcCurr,m_rcDockPos.left-m_rcCurr.left,m_rcDockPos.top-m_rcCurr.top);
	return &m_rcCurr;
}

BOOL RingDockBar::SetChild(RingBaseWnd* child)
{
	if(child && child->IsWindow() && child->Parent() != child && m_child == NULL)
	{
		m_child = child;
		//UpdateClientSize();
		if(m_child->GetWindowType() == ITIS_STATUSBAR)
		{
			m_BorderSpace = 0;
			m_GripperSpace = 0;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL RingDockBar::IsVisible(BOOL *lpbUpdate)
{
	if(lpbUpdate == NULL)	//����飬ֱ�ӷ���״ֵ̬
		return m_bVisible;

	if(m_child && m_child->IsVisible() != m_bVisible)
	{
		m_bVisible = m_child->IsVisible();
		*lpbUpdate = TRUE;
	}
	else
		*lpbUpdate = FALSE;

	return m_bVisible;
}

//��ȡͣ�����ڳߴ磬����m_rcClient������m_rcDockPos��ͣ��λ��ƽ�Ƶ��ʵ�λ��
void RingDockBar::UpdateClientSize()
{
	if(m_child)
	{
		RECT rc;
		GetWindowRect(m_child->Handle(),&rc);
		
		if(IsVert())
			OffsetRect(&rc,m_rcDockPos.left + m_BorderSpace - rc.left,
							m_rcDockPos.top + m_BorderSpace + m_GripperSpace - rc.top);
		else
			OffsetRect(&rc,m_rcDockPos.left + m_BorderSpace + m_GripperSpace - rc.left,
							m_rcDockPos.top + m_BorderSpace - rc.top);
		CopyRect(&m_rcClient,&rc);
	}	
}

//�ͻ����ߴ�ı䣬��ȡͣ�����ڳߴ磬����m_rcClient��m_rcDockPos(������λ����Ϣ)
void RingDockBar::UpdateSize()
{
	UpdateClientSize();

	if(IsVert())
		m_nSize = m_rcClient.right - m_rcClient.left + m_BorderSpace*2;
	else
		m_nSize = m_rcClient.bottom - m_rcClient.top + m_BorderSpace*2;
	
	m_rcDockPos.right = m_rcClient.right + m_BorderSpace;
	m_rcDockPos.bottom = m_rcClient.bottom + m_BorderSpace;

	if(m_lineInfo)
		m_lineInfo->m_maxSize = max(m_lineInfo->m_maxSize,m_nSize);
}

//���¸���״̬�µĳߴ磬��������FlyBar��WM_SIZE��Ϣ����
void RingDockBar::UpdateSize(int nWidth,int nHeight,BOOL bUpdate)
{
/*	m_winpos.cx = nWidth;
	m_nSize = m_winpos.cy = nHeight + m_BorderSpace*2;
	m_rcClient.right = m_rcClient.left + nWidth - m_GripperSpace - m_BorderSpace*2;
	m_rcClient.bottom = m_rcClient.top + nHeight;
	m_winpos.right = m_winpos.x + nWidth;
	m_winpos.bottom = m_winpos.y + m_winpos.cy;
*/
}

void RingDockBar::UpdateFlySize(int nWidth,int nHeight)
{
	m_frc.right = m_frc.left + nWidth;
	m_frc.bottom = m_frc.top + nHeight;
}

//��������DockBar�����FlyBar
void RingDockBar::UpdateAllSite()
{
	if(IsFloat())
	{
		if(m_flyBar && m_flyBar->IsVisible())
			m_flyBar->UpdateDockSite();
	}
	else if(m_pSite)
		m_pSite->UpdateAllSite();
}

BOOL RingDockBar::SetDockTitle(LPCTSTR lpszTitle)
{
	if(lpszTitle == NULL)
		return FALSE;

	LPSTR szTitle;
	int cch = strlen(lpszTitle);

	if(m_szTitle == NULL || cch > m_nTitleSize)
	{		
		m_nTitleSize = cch + 2;
		szTitle = (LPSTR)New(m_nTitleSize);
		if(szTitle)
		{
			Del(m_szTitle);
			m_szTitle = szTitle;
		}
		else
			return FALSE;
	}
	else
		memset(m_szTitle,0,m_nTitleSize);
	
	strcpy(m_szTitle,lpszTitle);

	return TRUE;
}

LRESULT RingDockBar::OnClose()
{
	if(m_child)
		m_child->Show(SW_HIDE);
	m_bVisible = FALSE;
	if(m_pSite)
		m_pSite->HideBar(this);
	return 0;
}

void RingDockBar::Show(int nState)
{
	if(nState == SW_SHOW || nState == SW_HIDE)
	{
		if(m_State == TBS_FLAG_FLY && m_flyBar)
			m_flyBar->Show(nState);
		else
		{
			if(m_child)
				m_child->Show(nState);
			m_bVisible = (nState == SW_SHOW);
			if(m_lineInfo && m_bVisible)
				m_lineInfo->isVisible = TRUE;
			UpdateAllSite();
		}
	}
}

LRESULT RingDockBar::OnSizing(RINGPARAMS& param)
{	
	RECT rc;
	LPRECT lprc = (LPRECT)param.lParam;
	//param.lParam = (LPARAM)&rc;
	CopyRect(&rc,lprc);

	if(m_child)
		SendMessage(m_child->Handle(),param.uMsg,param.wParam,param.lParam);
	
//	SetPos(0,0,lprc->right - lprc->left,lprc->bottom - lprc->top);

	AdjustSizingRect(param.wParam,lprc,rc);
	return TRUE;
}

void RingDockBar::AdjustSizingRect(int flag,LPRECT lprc,RECT& rc)
{
	switch(flag)
	{
		case WMSZ_LEFT:
		case WMSZ_BOTTOMLEFT:
			OffsetRect(lprc,rc.right - lprc->right,rc.top - lprc->top);
			break;
		case WMSZ_RIGHT:
		case WMSZ_BOTTOM:
		case WMSZ_BOTTOMRIGHT:
			OffsetRect(lprc,rc.left - lprc->left,rc.top - lprc->top);
			break;
		case WMSZ_TOP:
		case WMSZ_TOPRIGHT:
			OffsetRect(lprc,rc.left - lprc->left,rc.bottom - lprc->bottom);
			break;			
		case WMSZ_TOPLEFT:
			OffsetRect(lprc,rc.right - lprc->right,rc.bottom - lprc->bottom);
			break;
	}
}

//�ж����λ�ںη�������������ǰ�Ѿ����ж�������ڱ�DockBar��Χ��
UINT RingDockBar::HitTest(POINT* lpt)
{
	if((m_uStyle & TBS_EX_CANCLOSE) && PtInRect(&m_rcClose,*lpt))
		return HTCLOSE;
	else
		return HTCAPTION;
}

void RingDockBar::StartPushFrameControl(HWND hWnd,RingDragger* dragger,RINGPARAMS& param)
{
	if(m_CurWhere == HTCLOSE)
	{
		m_bNCBtDown = DFCS_CAPTIONCLOSE|DFCS_PUSHED;
		
		HDC hDC = GetDC(hWnd);
		DrawFrameControl(hDC,&m_rcClose,DFC_CAPTION,m_bNCBtDown);
		ReleaseDC(hWnd,hDC);
		
		SetCapture(hWnd);
	}
}

void RingDockBar::OnPushHoverFrameControl(HWND hWnd,RingDragger* dragger,LPPOINT lpt)
{
	if(m_CurWhere == HTCLOSE)
	{
		HDC hDC = GetDC(hWnd);
		
		if(PtInRect(&m_rcClose,*lpt))
		{
			if((m_bNCBtDown & DFCS_PUSHED)!= DFCS_PUSHED)
			{
				m_bNCBtDown |= DFCS_PUSHED;
				DrawFrameControl(hDC,&m_rcClose,DFC_CAPTION,m_bNCBtDown);
			}
		}
		else
		{
			if(m_bNCBtDown != DFCS_CAPTIONCLOSE)
			{
				m_bNCBtDown = DFCS_CAPTIONCLOSE;
				DrawFrameControl(hDC,&m_rcClose,DFC_CAPTION,DFCS_CAPTIONCLOSE);
			}
		}
		ReleaseDC(hWnd,hDC);
	}
}

void RingDockBar::StopPushFrameControl(HWND hWnd,RingDragger* dragger,LPPOINT lpt)
{
	if(PtInRect(&m_rcClose,*lpt) && m_CurWhere == HTCLOSE)
		OnClose();
}

//ע�⣺hWnd�п�����FlyBar��hWnd
LRESULT RingDockBar::StartDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param)
{
	POINT pt = {param.mousept.x,param.mousept.y};
	m_CurWhere = HitTest(&pt);
	
	if(m_CurWhere == HTCAPTION)
	{
		if(LOWORD(m_uStyle) <= TBS_EX_DOCKABLE)
		{
			m_bNCBtDown = 2;
			
			InitDragRects(hWnd,pt,(IsRectEmpty(&m_hrc) || !IsFloat()));
			m_currState = m_State;

			if(IsFloat())
				dragger->StartDrag(hWnd,pt.x,pt.y,&m_frc,3);
			else
			{
				if(IsVert())
					dragger->StartDrag(hWnd,pt.x,pt.y,&m_vrc,1);
				else
					dragger->StartDrag(hWnd,pt.x,pt.y,&m_hrc,1);
			}
		}
	}
	else
		StartPushFrameControl(hWnd,dragger,param);

	return 0;
}

LRESULT RingDockBar::StopDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param)
{
	if(dragger && dragger->IsDragging())
	{
		RECT rc;
		CopyRect(&rc,dragger->StopDrag(hWnd,param.mousept.x,param.mousept.y));
		Dockto(m_currState,&rc);
	}
	else
	{
		POINT pt = {param.mousept.x,param.mousept.y};

		ReleaseCapture();
		m_bNCBtDown = 0;
		StopPushFrameControl(hWnd,dragger,&pt);
	}
	return 0;
}

//�������϶�
LRESULT RingDockBar::OnDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param)
{
	POINT pt = {param.mousept.x,param.mousept.y};

	if(dragger && dragger->IsDragging())
	{
		RECT rc;
		LPRECT lprc;
		
		CopyRect(&rc,dragger->GetCurrRect());
		//�����ɾ���
		lprc = dragger->EraseRect(pt.x,pt.y);
/*
RECT rc1;
CopyRect(&rc1,&m_hrc);
MapWindowPoints(hWnd,HWND_DESKTOP,(LPPOINT)&rc1,2);
dragger->DrawRect(&rc1,1);
CopyRect(&rc1,&m_vrc);
MapWindowPoints(hWnd,HWND_DESKTOP,(LPPOINT)&rc1,2);
dragger->DrawRect(&rc1,1);
CopyRect(&rc1,&m_frc);
MapWindowPoints(hWnd,HWND_DESKTOP,(LPPOINT)&rc1,2);
dragger->DrawRect(&rc1,1);
*/
		//�����¾ɾ��ε�ƫ����
		rc.left = lprc->left - rc.left;
		rc.top = lprc->top - rc.top;
		//ƫ����������
		OffsetRect(&m_hrc,rc.left,rc.top);
		OffsetRect(&m_vrc,rc.left,rc.top);
		OffsetRect(&m_frc,rc.left,rc.top);
		
		//GetWindowRect(m_child->Parent()->Handle(),&rc);
		//MapWindowPoints(HWND_DESKTOP,hWnd,(LPPOINT)&rc,2);
		m_child->Parent()->GetClientRect(&rc);
		MapWindowPoints(m_child->Parent()->Handle(),hWnd,(LPPOINT)&rc,2);
		
		//AdjustDragRects(pt,&rc);

		ClientToScreen(hWnd,&pt);
		
		m_currState = CheckFloatRect(hWnd,lprc,pt);		
		dragger->DrawRect(SetDragRect(hWnd,dragger));
/*
CopyRect(&rc1,&m_hrc);
MapWindowPoints(hWnd,HWND_DESKTOP,(LPPOINT)&rc1,2);
dragger->DrawRect(&rc1,1);
CopyRect(&rc1,&m_vrc);
MapWindowPoints(hWnd,HWND_DESKTOP,(LPPOINT)&rc1,2);
dragger->DrawRect(&rc1,1);
CopyRect(&rc1,&m_frc);
MapWindowPoints(hWnd,HWND_DESKTOP,(LPPOINT)&rc1,2);
dragger->DrawRect(&rc1,1);
*/
	}
	else if(param.wParam == MK_LBUTTON)
		OnPushHoverFrameControl(hWnd,dragger,&pt);
	
	return 0;
}

int RingDockBar::SetDragRect(HWND hWnd,RingDragger* dragger)
{
	switch(m_currState)
	{
		case TBS_FLAG_FLY:
			dragger->SetDragRect(&m_frc,hWnd);
			return 3;
		case TBS_FLAG_DOCKLEFT:
		case TBS_FLAG_DOCKRIGHT:
			dragger->SetDragRect(&m_vrc,hWnd);
			break;
		case TBS_FLAG_DOCKTOP:
		case TBS_FLAG_DOCKBOTTOM:
			dragger->SetDragRect(&m_hrc,hWnd);
			break;			
	}
	return 1;
}

void RingDockBar::UpdateCaption()
{
	if(m_uStyle & TBS_EX_CANCLOSE)
	{
		if(IsVert())
		{
			m_rcClose.left = m_rcDockPos.right - 15;
			m_rcClose.top = m_rcDockPos.top + 3;
			m_rcClose.right = m_rcDockPos.right - 3;
			m_rcClose.bottom = m_rcDockPos.top + 15;
		}
		else
		{
			m_rcClose.left = m_rcDockPos.left + 3;
			m_rcClose.top = m_rcDockPos.top + 3;
			m_rcClose.right = m_rcDockPos.left + 15;
			m_rcClose.bottom = m_rcDockPos.top + 15;
		}
	}
}

void RingDockBar::OffsetDockPos(int x,int y)
{
	OffsetRect(&m_rcClient,x,y);
	OffsetRect(&m_rcDockPos,x,y);
}

BOOL RingDockBar::SetDockPos(int x,int y,int width/*=-999*/,int height/*=-999*/)
{
	if(m_child == NULL)
		return FALSE;

	OffsetDockPos(x - m_rcDockPos.left,y - m_rcDockPos.top);
	return TRUE;
}

void RingDockBar::UpdateFlyBkg()
{
	//SizeBarռ��FlyBar������ΪTRUE����������˸
	if(m_State == TBS_FLAG_FLY && m_flyBar && m_child)
		InvalidateRect(m_child->Handle(),NULL,!IsSizeBar());
}

BOOL RingDockBar::DrawBkg(HDC hDC,LPRECT lprc/*=NULL*/)
{	
	RingDockSite* tmp = NULL;
	RECT rc;
	SIZE offset={0,0};
	
	if(m_State == TBS_FLAG_FLY)
	{
		if(m_flyBar && m_child)
		{
			RingWnd* parent;
		
			parent = (RingWnd*)m_child->Parent();			
			if(parent)
			{
				for(int i=0;i<4;i++)
				{
					tmp = parent->GetDockSite(i);
					if(tmp)
					{
						m_flyBar->GetClientRect(&rc);
						//GetWindowRect(m_flyBar->Handle(),&rc);
						MapWindowPoints(m_flyBar->Handle(),tmp->Handle(),(LPPOINT)&rc,2);
						offset.cx = rc.left;
						offset.cy = rc.top;
						OffsetRect(&rc,-rc.left,-rc.top);
						return tmp->DrawBkg(hDC,&rc,&offset);
					}
				}
			}
		}
	}
	else
		tmp = m_pSite;
		
	if(tmp)
	{		
		if(lprc == NULL)
		{
			//NCPAINT
			CopyRect(&rc,&m_rcClient);
			OffsetRect(&rc,-m_rcClient.left,-m_rcClient.top);
			lprc = &rc;
			//offset.cx = m_rcClient.left;
			//offset.cy = m_rcClient.top;
		}
/*		else
		{
			RingBaseWnd::GetClientRect(&rc);
			CopyRect(lprc,&rc);
			//offset.cx = m_winpos.x + m_rcClient.left;
			//offset.cy = m_winpos.y + m_rcClient.top;
		}
*/
		offset.cx = m_rcClient.left;
		offset.cy = m_rcClient.top;

		return tmp->DrawBkg(hDC,lprc,&offset);
	}
	return FALSE;
}

//////////////////////////////////////
//
//���ƾ��ο��ֱ�������Ӵ�����ʾ״̬��m_bVisible��һ�£������ƣ�����FALSE��
//������Ӧ�ø���DockSite����Ϊ�п���Ҫ���й���������
//
/////////////////////////////////////
BOOL RingDockBar::Draw(HDC hDC)
{
	BOOL bUpdate;	
	if(IsVisible(&bUpdate) && !bUpdate)
	{
		//�����Ҳ����²Ż���
		DrawFrame(hDC);
		DrawGripper(hDC);
//		if(m_child)
//			m_child->Repaint();//InvalidateRect(m_child->Handle(),NULL,TRUE);
	}
	return !bUpdate;
}

void RingDockBar::DrawFrame(HDC hDC)
{
	if((m_uStyle & TBS_EX_ALLLINE) == TBS_EX_ALLLINE)
	{
		if(IsVert())
			DrawEdge(hDC,&m_rcDockPos,BDR_RAISEDINNER,BF_RECT);
		else
		{
			//if(m_pSite && m_pSite->)
			RECT rc = {m_rcDockPos.left,m_rcDockPos.top-1,m_rcDockPos.right,m_rcDockPos.top+1};
			DrawEdge(hDC,&rc,BDR_SUNKENOUTER,BF_TOP | BF_BOTTOM);
			OffsetRect(&rc,0,m_rcDockPos.bottom - m_rcDockPos.top);
			DrawEdge(hDC,&rc,BDR_SUNKENOUTER,BF_TOP | BF_BOTTOM);
		}
	}
	else if(LOWORD(m_uStyle) != TBS_EX_NONE)
		DrawEdge(hDC,&m_rcDockPos,BDR_RAISEDINNER,BF_RECT);
}

void RingDockBar::DrawGripper(HDC hDC)
{	
	if(LOWORD(m_uStyle) == TBS_EX_NONE)
		return;

	RECT rc;
	CopyRect(&rc,&m_rcDockPos);
	InflateRect(&rc,-1,-2);
	int x=0,y=0,bx=2,by=2;
	
	if(IsVert())
	{
		rc.bottom = rc.top + 3;
		OffsetRect(&rc,0,1);

		if((m_uStyle & TBS_EX_CANCLOSE))
		{
			rc.right -= 17;
			OffsetRect(&rc,0,3);
			bx = rc.right + 2;
		}
		y = 4;
	}
	else
	{
		rc.right = rc.left + 3;
		OffsetRect(&rc,1,0);

		if((m_uStyle & TBS_EX_CANCLOSE))
		{
			rc.top += 17;
			OffsetRect(&rc,3,0);
			by = rc.top - 15;
		}		
		x = 4;
	}
	DrawEdge(hDC,&rc,BDR_RAISEDINNER,BF_RECT);
	OffsetRect(&rc,x,y);
	DrawEdge(hDC,&rc,BDR_RAISEDINNER,BF_RECT);

	if((m_uStyle & TBS_EX_CANCLOSE))
	{
		rc.left = bx;
		rc.top = by;
		rc.right = rc.left + 14;
		rc.bottom = rc.top + 14;
		DrawFrameControl(hDC,&m_rcClose,DFC_CAPTION,DFCS_CAPTIONCLOSE);
	}
}

//������ǰ��һ��dockbar֮��Ŀռ���룬ͬʱ����ͣ������
//���øú���ǰ���������ͣ��
int RingDockBar::GetPreSpace()
{
	int presize = 0;
	
	if(m_child && m_pSite && m_bVisible)
	{
		if(IsVert())
		{
			if(m_prev)
				presize = m_prev->m_rcDockPos.bottom;
			presize = m_rcDockPos.top - presize;
			if(!IsVisible())
				presize += (m_rcDockPos.bottom - m_rcDockPos.top);
		}
		else
		{
			if(m_prev)
				presize = m_prev->m_rcDockPos.right;
			presize = m_rcDockPos.left - presize;
			if(!IsVisible())
				presize += (m_rcDockPos.right - m_rcDockPos.left);
		}		
	}
	return presize;
}

/***************************************************************************
//
//����ͣ��λ�ã�nEdgeΪ�ұ߽磬nPreSpaceΪǰ����ƶ��ռ䣬������
//�����ұ߽磬Ӧ��ǰ�ƣ����ƶ����벻�ɳ������ƶ��ռ䡣
//�������������й����������ص�
//
***************************************************************************/
HDWP RingDockBar::AdjustDockPosH(HDWP hdwp,int nEdge,int nPreSpace,BOOL bCanWrap)
{
	int nPrevLen,offset = 0;

	if(m_child && m_pSite && m_bVisible)
	{
		RECT rcp;
		CopyRect(&rcp,&m_rcClient);
		if(m_prev)
			nPrevLen = m_rcDockPos.left - m_prev->m_rcDockPos.right;

		if(m_rcDockPos.right > nEdge || m_pSite->IsSizeBarInLine(m_lineInfo))
		{
			offset = -min(m_rcDockPos.right - nEdge,nPreSpace);			
			OffsetDockPos(offset,0);
		}
		else if(nPreSpace < 0)
		{
			offset = -nPreSpace;
			OffsetDockPos(offset,0);
		}			
		UpdateCaption();

		hdwp = m_child->DefDeferWindowPos(hdwp,m_rcClient.left,m_rcClient.top,
			m_rcClient.right - m_rcClient.left,m_rcClient.bottom - m_rcClient.top,
			IsAllLineBar()?0:SWP_NOSIZE);
		if(EqualRect(&rcp,&m_rcClient) && m_State == TBS_FLAG_DOCKBOTTOM)
			InvalidateRect(m_child->Handle(),NULL,FALSE);

		if(m_prev)
		{
			nPreSpace -= nPrevLen;//min(offset,m_winpos.x - m_prev->m_winpos.right);
			hdwp = m_prev->AdjustDockPosH(hdwp,m_rcDockPos.left,nPreSpace,bCanWrap);
		}
	}
	return hdwp;
}

void RingDockBar::SetDockPos(HDWP hdwp)
{
	if(IsMenuBar())
		DeferWindowPos(hdwp,m_child->Handle(),0,m_rcClient.left,m_rcClient.top,
		m_rcClient.right - m_rcClient.left,m_rcClient.bottom - m_rcClient.top,
		SWP_NOZORDER|SWP_NOACTIVATE);//|SWP_NOREDRAW);
	else
		DeferWindowPos(hdwp,m_child->Handle(),0,m_rcClient.left,m_rcClient.top,0,0,
		SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);//|SWP_NOREDRAW);
}

/***************************************************************************
//
//����ͣ��λ�ã�nEdgeΪ�±߽磬nPreSpaceΪ������ƶ��ռ䣬������
//�����±߽磬Ӧ�����ƣ����ƶ����벻�ɳ������ƶ��ռ䡣
//�������������й����������ص�
//
***************************************************************************/
HDWP RingDockBar::AdjustDockPosV(HDWP hdwp,int nEdge,int nPreSpace,BOOL bCanWrap)
{
	int nPrevLen,offset = 0;

	if(m_child && m_pSite && m_bVisible)
	{
		RECT rcp;
		CopyRect(&rcp,&m_rcClient);
		if(m_prev)
			nPrevLen = m_rcDockPos.top - m_prev->m_rcDockPos.bottom;

		if(m_rcDockPos.bottom > nEdge)
		{
			offset = -min(m_rcDockPos.bottom - nEdge,nPreSpace);
			OffsetDockPos(0,offset);
		}
		else if(nPreSpace < 0)
		{
			offset = -nPreSpace;
			OffsetDockPos(0,offset);
		}			
		UpdateCaption();
		//if(m_winpos.x > nEdge && bCanWrap)
		//	m_pSite->WrapBar(m_lineInfo,this,(m_next == NULL));
		//else

		hdwp = m_child->DefDeferWindowPos(hdwp,m_rcClient.left,m_rcClient.top,
			m_rcClient.right - m_rcClient.left,m_rcClient.bottom - m_rcClient.top,
			IsAllLineBar()?0:SWP_NOSIZE);
		if(EqualRect(&rcp,&m_rcClient) && m_State == TBS_FLAG_DOCKRIGHT)
			InvalidateRect(m_child->Handle(),NULL,FALSE);
		/*
		if(IsMenuBar())
			hdwp = DeferWindowPos(hdwp,m_child->Handle(),0,m_rcClient.left,m_rcClient.top,
						m_rcClient.right - m_rcClient.left,m_rcClient.bottom - m_rcClient.top,
						SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);//|SWP_NOCOPYBITS|SWP_NOREDRAW);
		else
			hdwp = DeferWindowPos(hdwp,m_child->Handle(),0,m_rcClient.left,m_rcClient.top,0,0,
							SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);//|SWP_NOCOPYBITS|SWP_NOREDRAW);
		//InvalidateRect(m_child->Handle(),NULL,FALSE);
		*/
		if(m_prev && hdwp)
		{
			//if(offset < 0)
			//	offset = m_prev->m_winpos.right - m_winpos.x;
			nPreSpace -= nPrevLen;//min(offset,m_winpos.x - m_prev->m_winpos.right);
			hdwp = m_prev->AdjustDockPosV(hdwp,m_rcDockPos.top,nPreSpace,bCanWrap);
		}
		//else
		//	m_pSite->WrapBar(m_lineInfo,NULL,FALSE);
	}
	return hdwp;
}

//���ݹ������϶�ʱ�ľ����ж�Ӧ��ͣ������һ�ߣ�lprc,pt��Ϊ��Ļ����,
//����ͣ��λ�ã��������һ򸡶���
int RingDockBar::CheckFloatRect(HWND hWnd,LPRECT lprc,POINT pt)
{
	RECT rcClient,rcCheck;
	
	//�жϴ�������
	GetWindowRect(m_child->Parent()->Handle(),&rcClient);
	
	if(!PtInRect(&rcClient,pt))
		return TBS_FLAG_FLY;	  //��겻�ڴ����ڲ������ظ���

	RingDockSite* pSite;
	RECT rc[4];
	int i,state=TBS_FLAG_FLY,max=0,cmp,hrch,vrcw;
	RECT rcCmp;
	POINT lpt = {pt.x,pt.y};

	ScreenToClient(hWnd,&lpt);
	m_child->Parent()->GetClientRect(&rcClient);
	MapWindowPoints(m_child->Parent()->Handle(),hWnd,(LPPOINT)&rcClient,2);
	
	hrch = m_hrc.bottom - m_hrc.top;
	vrcw = m_vrc.right - m_vrc.left;

	for(i=0;i<4;i++)
	{
		pSite = ((RingWnd*)m_child->Parent())->GetDockSite(i);
		if(CanDock(i))
		{
			if(pSite)
			{
				GetWindowRect(pSite->Handle(),&rcCheck);
				if(PtInRect(&rcCheck,pt))
				{
					//����ڸ��������ȷ���
					if(i != m_currState)
					{
						ScreenToClient(hWnd,&pt);
						AdjustRects(hWnd,pt,i);
					}
					return i;
				}
				MapWindowPoints(HWND_DESKTOP,hWnd,(LPPOINT)&rcCheck,2);
			}
			else
				CopyRect(&rcCheck,&rcClient);

			switch(i)
			{
			case TBS_FLAG_DOCKTOP:
				if(!pSite || rcCheck.top == rcCheck.bottom)
				{
					//�÷�����DockSite��DockSite����ͣ����,�����жϾ���Ϊ
					//��÷���ͣ�����ȿ�/�ߣ���ͻ����ص�1����
					rcCheck.bottom = rcCheck.top + 1;
					rcCheck.top = rcCheck.bottom - (m_hrc.bottom - m_hrc.top);
				}
				//��¼�ص�����
				if(m_currState == TBS_FLAG_FLY)
				{
					rcCmp.left = m_hrc.left;
					rcCmp.right = m_hrc.right;
					if(lpt.y > m_frc.top + hrch)
					{
						rcCmp.bottom = lpt.y + m_BorderSpace;
						rcCmp.top = rcCmp.bottom - hrch;
					}
					else
					{
						rcCmp.top = m_frc.top;
						rcCmp.bottom = rcCmp.top + hrch;
					}					
				}
				else
					CopyRect(&rcCmp,&m_hrc);
				break;
			case TBS_FLAG_DOCKBOTTOM:
				if(!pSite || rcCheck.top == rcCheck.bottom)
				{
					rcCheck.top = rcCheck.bottom - 1;
					rcCheck.bottom = rcCheck.top + (m_hrc.bottom - m_hrc.top);
				}
				if(m_currState == TBS_FLAG_FLY)
				{
					rcCmp.left = m_hrc.left;
					rcCmp.right = m_hrc.right;
					if(lpt.y < m_frc.bottom - hrch)
					{
						rcCmp.top = lpt.y - m_BorderSpace;
						rcCmp.bottom = rcCmp.top + hrch;
					}
					else
					{
						rcCmp.bottom = m_frc.bottom;
						rcCmp.top = rcCmp.bottom - hrch;
					}					
				}
				else
					CopyRect(&rcCmp,&m_hrc);
				break;
			case TBS_FLAG_DOCKLEFT:
				if(!pSite || rcCheck.left == rcCheck.right)
				{
					rcCheck.right = rcCheck.left + 1;
					rcCheck.left = rcCheck.right - (m_vrc.right - m_vrc.left);
				}
				if(m_currState == TBS_FLAG_FLY)
				{
					rcCmp.top = m_vrc.top;
					rcCmp.bottom = m_vrc.bottom;
					if(lpt.x > m_frc.left + vrcw)
					{
						rcCmp.right = lpt.x + m_BorderSpace;
						rcCmp.left = rcCmp.right - vrcw;
					}
					else
					{
						rcCmp.left = m_frc.left;
						rcCmp.right = rcCmp.left + vrcw;
					}
				}
				else
					CopyRect(&rcCmp,&m_vrc);
				break;
			case TBS_FLAG_DOCKRIGHT:
				if(!pSite || rcCheck.left == rcCheck.right)
				{
					rcCheck.left = rcCheck.right - 1;
					rcCheck.right = rcCheck.left + (m_vrc.right - m_vrc.left);
				}
				if(m_currState == TBS_FLAG_FLY)
				{
					rcCmp.top = m_vrc.top;
					rcCmp.bottom = m_vrc.bottom;
					if(lpt.x < m_frc.right - vrcw)
					{
						rcCmp.left = lpt.x - m_BorderSpace;
						rcCmp.right = rcCmp.left + vrcw;
					}
					else
					{
						rcCmp.right = m_frc.right;
						rcCmp.left = rcCmp.right - vrcw;
					}
				}
				else
					CopyRect(&rcCmp,&m_vrc);
				break;
			}
			//���ص������С�ж�Ӧ��ͣ���η�
			IntersectRect(rc+i,&rcCmp,&rcCheck);
			if(!IsRectEmpty(rc+i))
			{
				cmp = (rc[i].right - rc[i].left) * (rc[i].bottom - rc[i].top);
				if(cmp > max)
				{
					max = cmp;
					state = i;
				}
			}
		}
	}
	
	if(state != m_currState)// || state == TBS_FLAG_FLY)
	{
		ScreenToClient(hWnd,&pt);
		AdjustRects(hWnd,pt,state);
	}

	return state;
}

/*
int RingDockBar::CheckFloatRect(HWND hWnd,LPRECT lprc,POINT pt)
{
	RECT rcClient,rcCheck;
	
	//�жϴ�������
	GetWindowRect(m_child->Parent()->Handle(),&rcClient);
	
	if(!PtInRect(&rcClient,pt))
		return TBS_FLAG_FLY;	  //��겻�ڴ����ڲ������ظ���
	
	RingDockSite* pSite;
	RECT rc[4];
	int i,state=TBS_FLAG_FLY,max=0,cmp;
	LPRECT lprcCmp;
	
	m_child->Parent()->GetClientRect(&rcClient);
	MapWindowPoints(m_child->Parent()->Handle(),hWnd,(LPPOINT)&rcClient,2);
	
	for(i=0;i<4;i++)
	{
		pSite = ((RingWnd*)m_child->Parent())->GetDockSite(i);
		if(CanDock(i))
		{
			if(pSite)
			{
				GetWindowRect(pSite->Handle(),&rcCheck);
				if(PtInRect(&rcCheck,pt))
				{
					//����ڸ��������ȷ���
					ScreenToClient(hWnd,&pt);
					AdjustRects(hWnd,pt,i);
					return i;
				}
				MapWindowPoints(HWND_DESKTOP,hWnd,(LPPOINT)&rcCheck,2);
			}
			else
				CopyRect(&rcCheck,&rcClient);
			
			if(!pSite || rcCheck.left == rcCheck.right || rcCheck.top == rcCheck.bottom)
			{
				//�÷�����DockSite��DockSite����ͣ����,�����жϾ���Ϊ
				//��÷���ͣ�����ȿ�/�ߣ���ͻ����ص�1����
				if(i == 0)
				{					
					rcCheck.bottom = rcCheck.top + 1;
					rcCheck.top = rcCheck.bottom - (m_hrc.bottom - m_hrc.top);
				}
				else if(i == 1)
				{
					rcCheck.top = rcCheck.bottom - 1;
					rcCheck.bottom = rcCheck.top + (m_hrc.bottom - m_hrc.top);
				}
				else if(i == 2)
				{
					rcCheck.right = rcCheck.left + 1;
					rcCheck.left = rcCheck.right - (m_vrc.right - m_vrc.left);
				}
				else
				{
					rcCheck.left = rcCheck.right - 1;
					rcCheck.right = rcCheck.left + (m_vrc.right - m_vrc.left);
				}
			}
			if(i < 2)
				lprcCmp = &m_hrc;
			else
				lprcCmp = &m_vrc;
			
			IntersectRect(rc+i,lprcCmp,&rcCheck);
		}
		else
			SetRectEmpty(rc+i);
	}
	//���ص������С�ж�Ӧ��ͣ���η�
	for(i=0;i<4;i++)
	{
		if(!IsRectEmpty(rc+i))
		{
			cmp = (rc[i].right - rc[i].left) * (rc[i].bottom - rc[i].top);
			if(cmp > max)
			{
				max = cmp;
				state = i;
			}
		}
	}
	//if(state != m_currState || state == TBS_FLAG_FLY)
	{
		ScreenToClient(hWnd,&pt);
		AdjustRects(hWnd,pt,state);
	}
	
	return state;
}
*/
//���ݿ�߲������ؿͻ������
int RingDockBar::GetClientWidth(int width)
{
	if(IsHorz())
	{
		if(width == 0)	//==0��ʾ���ø����ڿ��
		{
			if(m_pSite)
				width = ::ClientWidth(m_pSite->Parent()->Handle());
			else
				return 0;
		}
		return width - m_BorderSpace * 2 - m_GripperSpace;
	}
	else if(IsFloat())
	{
		if(width == 0)	//==0��ʾ���ø����ڿ��
		{
			if(m_flyBar)
				width = ::ClientWidth(m_flyBar->Handle());
			else
				return 0;
		}
		else
			width += m_BorderSpace * 2;
		return width;
	}
	else
	{
		if(width == 0)
			width = m_rcDockPos.right - m_rcDockPos.left;
		return width - m_BorderSpace * 2;
	}
}

int RingDockBar::GetClientHeight(int height)
{
	if(IsVert())
	{
		if(height == 0)	//==0��ʾ���ø����ڿ��
		{
			if(m_pSite)
			{
				RECT rc;
				m_pSite->Parent()->GetClientRect(&rc);
				height = rc.bottom - rc.top;
			}
			else
				return 0;
		}
		return height - m_BorderSpace * 2 - m_GripperSpace;
	}
	else
	{
		if(height == 0)
			height = m_rcDockPos.bottom - m_rcDockPos.top;
		return height - m_BorderSpace * 2;
	}
}

//����ͣ���룬�����ͣ������ԭͣ��������
BOOL RingDockBar::SetDockSite(RingDockSite* docksite)
{	
	if(docksite == NULL)
	{		
		//���������ܸı�m_State��ֵ����ֵ��DockFly�õ��Һ�������ʱ����
		if(m_pSite && m_State != TBS_FLAG_FLY && m_State != TBS_FLAG_NONE)
			m_pSite->DelBar(this,FALSE);

		//�����SizeBar��SizeBar�ĸú�����ı䴰�ڳߴ�
		OnDockStateChange(m_State,TBS_FLAG_FLY);
		return TRUE;
	}
	else
	{		
		//docksite��Ч��ȡdocksite��ͣ��λ��
		int state = docksite->GetState();
		
		if(m_child)	//���趨�Ӵ���
		{
			if(m_State == TBS_FLAG_FLY)
			{
				//ԭ���������ظ�����
				if(m_flyBar)
					m_flyBar->Show(SW_HIDE);
			}
			else if(m_pSite && m_pSite != docksite)	//����ͣ�����ڣ�����
				m_pSite->DelBar(this,FALSE);
			
			//�����Ӵ��ڵĸ�����Ϊdocksite
			m_child->SetParent(docksite->Handle());
			
			int nOldState = m_State;
			m_State = state;
			m_pSite = docksite;

			if(!(nOldState == state || (IsVert() && ::IsVert(nOldState)) ||
				(IsHorz() && ::IsHorz(nOldState))))
				if(0 == SendMessage(m_child->Handle(),WM_DOCKSTATECHANGE,
						MAKELONG(nOldState,state),(LPARAM)m_child->Handle()))
				{
					//δ��������Ĭ�ϴ���
					OnDockStateChange(nOldState,state);
				}
			
			UpdateSize();
			return TRUE;		
		}
	}
	return FALSE;
}

//lprc:��Ļ����
BOOL RingDockBar::DockFly(LPRECT lprc/*=NULL*/)
{
	if(m_child == NULL)
		return FALSE;

	if(m_flyBar == NULL)
	{
		m_flyBar = new RingFlyBar(m_child->Parent());
		CopyRect(&m_ldrc,&m_rcDockPos);
		MapWindowPoints(m_pSite->Handle(),m_pSite->Parent()->Handle(),(LPPOINT)&m_ldrc,2);
	}

	if(m_flyBar)
	{
		//winpos��flybar�������ϽǶ��롣
		//����ʱͣ������ͣ�������Ϸ�״̬��ͬ����topλ��Ϊ1����˴�ֱλ��-1
		OffsetDockPos(-m_rcDockPos.left - WINVAR_FRAME_X,//m_BorderSpace,
							- m_rcDockPos.top - 1 - BARTOOLY);
		
		if(m_child->GetParent() == m_flyBar->Handle())
		{
			if(m_State != TBS_FLAG_FLY)
				m_pSite->RegFlyBar(this);

			m_State = TBS_FLAG_FLY;

			if(lprc)
			{
				m_flyBar->SetPos(lprc->left,lprc->top);
				CopyRect(&m_lfrc,lprc);
				if(m_pSite)
				{
					ScreenToClient(m_pSite->Parent()->Handle(),(LPPOINT)&m_lfrc);
					ScreenToClient(m_pSite->Parent()->Handle(),(LPPOINT)&m_lfrc+1);
				}
			}
			else
				CopyRect(&m_lfrc,&m_frc);

			m_flyBar->Show();
			InvalidateRect(m_child->Handle(),NULL,TRUE);			
			return TRUE;
		}
		else if(m_flyBar->Create(m_szTitle))
		{
			int nOldState = m_State;

			if(m_State != TBS_FLAG_FLY)
				m_pSite->RegFlyBar(this);
			m_State = TBS_FLAG_FLY;

			SendMessage(m_child->Handle(),WM_DOCKSTATECHANGE,
							MAKELONG(nOldState,TBS_FLAG_FLY),(LPARAM)m_child->Handle());
			
			if(lprc)
			{
				m_flyBar->SetPos(lprc->left,lprc->top);
				CopyRect(&m_lfrc,lprc);
				if(m_pSite)
				{
					ScreenToClient(m_pSite->Parent()->Handle(),(LPPOINT)&m_lfrc);
					ScreenToClient(m_pSite->Parent()->Handle(),(LPPOINT)&m_lfrc+1);
				}
			}
			else
				CopyRect(&m_lfrc,&m_frc);
			
			m_flyBar->AddChild(this);
			m_flyBar->Show();
			
			return TRUE;
		}
		else
		{
			delete m_flyBar;
			m_flyBar = NULL;
		}
	}
	return FALSE;
}

//////////////////////////////////////
//
//lprc:Ϊ�����ڵ��������
//
//////////////////////////////////////
BOOL RingDockBar::Dockto(int state,LPRECT lprc)
{
	if(m_child == NULL)
		return FALSE;

	//���������������ʾͣ��λ�ñ䶯������DockSite
	if(m_State != state)
		SetDockSite(((RingWnd*)m_child->Parent())->CreateDockSite(state));
	
	if(state != TBS_FLAG_FLY)
	{
		//AddBar���������DockSite�ߴ�
		if(m_pSite)
		{
			if(m_State == TBS_FLAG_FLY)
				m_pSite->UnregFlyBar(this);
			
			m_pSite->AddBar(this,lprc);
		}
	}
	else
	{
		RECT rc;

		if(lprc == NULL)
			CopyRect(&rc,&m_frc);
		else
		{
			CopyRect(&rc,lprc);
			ClientToScreen(m_child->Parent()->Handle(),(LPPOINT)&rc);
			ClientToScreen(m_child->Parent()->Handle(),(LPPOINT)&rc+1);
		}

		//����DockSite�ߴ�
		if(m_State != TBS_FLAG_FLY && m_State != TBS_FLAG_NONE)
			m_pSite->UpdateAllSite();
				
		DockFly(&rc);
	}
	return TRUE;
}

int RingDockBar::DockBack()
{
	if(m_State == TBS_FLAG_FLY)
	{
		if(IsRectEmpty(&m_ldrc))
			Dockto(TBS_FLAG_DOCKTOP);
		else
			Dockto(m_pSite->GetState(),&m_ldrc);
	}
	else
	{
		CopyRect(&m_ldrc,&m_rcDockPos);
		MapWindowPoints(m_pSite->Handle(),m_pSite->Parent()->Handle(),(LPPOINT)&m_ldrc,2);
		//if(IsRectEmpty(&m_hrc))
		{
			POINT pt = {0,0};
			InitDragRects(m_pSite->Handle(),pt,TRUE);
		}
		Dockto(TBS_FLAG_FLY,&m_lfrc);
	}
	return GetState();
}

//��ʼ����������
void RingDockBar::InitDragRects(HWND hWnd,POINT pt,BOOL bInit)
{
	if(bInit)
	{
		CopyRect(&m_hrc,GetDragRects(TBS_FLAG_HORZ));
		CopyRect(&m_vrc,GetDragRects(TBS_FLAG_VERT));
		CopyRect(&m_frc,GetDragRects(TBS_FLAG_FLY));
	}
	else
	{
		//CopyRect(&m_frc,GetDragRects(TBS_FLAG_FLY));
		OffsetRect(&m_hrc,m_rcDockPos.left-m_hrc.left,m_rcDockPos.top-m_hrc.top);
		OffsetRect(&m_vrc,m_rcDockPos.left-m_vrc.left,m_rcDockPos.top-m_vrc.top);
		OffsetRect(&m_frc,m_rcDockPos.left-m_frc.left,m_rcDockPos.top-m_frc.top);
	}
	RECT rc = {0,0,0,0};
	if(m_child)
		GetWindowRect(m_child->Parent()->Handle(),&rc);
	MapWindowPoints(HWND_DESKTOP,hWnd,(LPPOINT)&rc,2);
	InitAdjustRects(pt,&rc);	
}

void RingDockBar::AdjustDragRects(POINT& pt,LPRECT lprcCheck)
{
	//����״̬����hrc�����ƶ���
	if(pt.y < (lprcCheck->top + lprcCheck->bottom)/2)
	{
		//������ϰ벿�֣��϶���
		OffsetRect(&m_hrc,m_frc.left - m_hrc.left,m_frc.top - m_hrc.top);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_hrc,0,pt.y - m_hrc.bottom);
	}
	else
	{
		//������°벿�֣��¶���
		OffsetRect(&m_hrc,m_frc.left - m_hrc.left,m_frc.bottom - m_hrc.bottom);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_hrc,0,pt.y - m_hrc.top);
	}
}

void RingDockBar::InitAdjustRects(POINT& pt,LPRECT lprcCheck)
{
	//��ʼ������
	if(m_State == TBS_FLAG_FLY)
	//if(m_CurrState == TBS_FLAG_FLY)
	{		
		//����״̬����hrc�����ƶ���
		if(pt.y < (lprcCheck->top + lprcCheck->bottom)/2)
		{
			//������ϰ벿�֣��϶���
			OffsetRect(&m_hrc,m_frc.left - m_hrc.left,m_frc.top - m_hrc.top);
			if(!PtInRect(&m_hrc,pt))
				OffsetRect(&m_hrc,0,pt.y - m_hrc.bottom);
		}
		else
		{
			//������°벿�֣��¶���
			OffsetRect(&m_hrc,m_frc.left - m_hrc.left,m_frc.bottom - m_hrc.bottom);
			if(!PtInRect(&m_hrc,pt))
				OffsetRect(&m_hrc,0,pt.y - m_hrc.top);
		}
	}
	else if(IsVert())
	{
		//����
		if(pt.y < (lprcCheck->top + lprcCheck->bottom)/2)
		{
			//������ϰ벿�֣�hrc��vrc���ϽǶ���
			OffsetRect(&m_hrc,m_vrc.left - m_hrc.left,m_vrc.top - m_hrc.top);
			if(!PtInRect(&m_hrc,pt))	//��겻��hrc��Χ��hrc�ױ���������
				OffsetRect(&m_hrc,0,pt.y - m_hrc.bottom + m_BorderSpace);
		}
		else
		{
			//������°벿�֣�hrc��vrc���½Ƕ���
			OffsetRect(&m_hrc,m_vrc.left - m_hrc.left,m_vrc.bottom - m_hrc.bottom);
			if(!PtInRect(&m_hrc,pt))	//��겻��hrc��Χ��hrc�ϱ���������
				OffsetRect(&m_hrc,0,pt.y - m_hrc.left - m_BorderSpace);
		}
		//frc��vrc���ϽǶ���
		OffsetRect(&m_frc,m_vrc.left - m_frc.left,m_vrc.top - m_frc.top);
		if(!PtInRect(&m_frc,pt))
		{
			//��겻��frc��Χ��frc�����������λ��(����)
			OffsetRect(&m_frc,0,pt.y - (m_frc.bottom + m_frc.top)/2);
			if(!PtInRect(&m_frc,pt))
				//�Բ��ڣ�frc�����������λ��(����)
				OffsetRect(&m_frc,pt.x - (m_frc.right + m_frc.left)/2,0);
		}
	}
	else
	{		
		if(pt.x < (lprcCheck->right + lprcCheck->left)/2)
		{
			//�������벿�֣������
			OffsetRect(&m_vrc,m_hrc.left - m_vrc.left,m_hrc.top - m_vrc.top);
			if(!PtInRect(&m_vrc,pt))
				OffsetRect(&m_vrc,pt.x - m_vrc.right,0);
		}
		else
		{
			//������Ұ벿�֣��Ҷ���
			OffsetRect(&m_vrc,m_hrc.right - m_vrc.right,m_hrc.top - m_vrc.top);
			if(!PtInRect(&m_vrc,pt))
				OffsetRect(&m_vrc,pt.x - m_vrc.left,0);
		}
		//frc��hrc���ϽǶ���
		OffsetRect(&m_frc,m_hrc.left - m_frc.left,m_hrc.top - m_frc.top);
		if(!PtInRect(&m_frc,pt))
		{
			//��겻��frc��Χ��frc�����������λ��(����)
			OffsetRect(&m_frc,pt.x - (m_frc.right + m_frc.left)/2,0);
			if(!PtInRect(&m_frc,pt))
				//�Բ��ڣ�frc�����������λ��(����)
				OffsetRect(&m_frc,0,pt.x - (m_frc.bottom + m_frc.top)/2);
		}
	}	
}

/*********************************************************
//
//������������,state��������ɵ�״̬��m_CurrStateΪԭ״̬
//
//��������
//		������ͣ��״̬��Ϊ����״̬���ȵ���m_frc,
//		Ȼ����Ҫ������������
//		����m_frcΪ�����򣨿�߱Ⱦ�����������߱Ƚϴ�
//		���������ڿͻ���λ�ã��ϰ벿�֣�m_hrc��m_frc
//		�϶��룬�°벿���¶��롣��Ƚϴ����ж�����
//
*********************************************************/
void RingDockBar::AdjustRects(HWND hWnd,POINT pt,int state)
{
	switch(state)
	{
	case TBS_FLAG_DOCKTOP:
		OffsetRect(&m_hrc,0,m_frc.top - m_hrc.top);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_hrc,0,pt.y - m_hrc.bottom + m_BorderSpace);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_hrc,m_frc.left - m_hrc.left,0);
		break;
	case TBS_FLAG_DOCKBOTTOM:
		OffsetRect(&m_hrc,0,m_frc.bottom - m_hrc.bottom);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_hrc,0,pt.y - m_hrc.top - m_BorderSpace);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_hrc,m_frc.left - m_hrc.left,0);
		break;
	case TBS_FLAG_DOCKLEFT:
		OffsetRect(&m_vrc,m_frc.left - m_vrc.left,0);
		if(!PtInRect(&m_vrc,pt))
			OffsetRect(&m_vrc,pt.x - m_vrc.right + m_BorderSpace,0);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_vrc,0,m_frc.top - m_vrc.top);
		break;
	case TBS_FLAG_DOCKRIGHT:
		OffsetRect(&m_vrc,m_frc.right - m_vrc.right,0);
		if(!PtInRect(&m_vrc,pt))
			OffsetRect(&m_vrc,pt.x - m_vrc.left - m_BorderSpace,0);
		if(!PtInRect(&m_hrc,pt))
			OffsetRect(&m_vrc,0,m_frc.top - m_vrc.top);
		break;
	case TBS_FLAG_FLY:
		//if((m_frc.bottom - m_frc.top) > (m_frc.right - m_frc.left))
		if(::IsHorz(m_currState))
		{
			if(m_currState == TBS_FLAG_DOCKTOP)
			{
				OffsetRect(&m_frc,0,m_hrc.top - m_frc.top);
				if(!PtInRect(&m_frc,pt))
					OffsetRect(&m_frc,0,pt.y - m_frc.bottom + m_BorderSpace);
			}
			else if(m_currState == TBS_FLAG_DOCKBOTTOM)
			{
				OffsetRect(&m_frc,0,m_hrc.bottom - m_frc.bottom);
				if(!PtInRect(&m_frc,pt))
					OffsetRect(&m_frc,0,pt.y - m_frc.top - m_BorderSpace);
			}
			//�����ƶ�������귶Χ�������ƶ���������������
			if(!PtInRect(&m_frc,pt))
				OffsetRect(&m_frc,pt.x - (m_frc.right - m_frc.left)/2,0);
			
			//frc�������ƶ�������vrc
			OffsetRect(&m_vrc,0,m_frc.top - m_vrc.top);
		}
		else
		{
			if(m_currState == TBS_FLAG_DOCKLEFT)
			{
				OffsetRect(&m_frc,m_vrc.left - m_frc.left,0);
				if(!PtInRect(&m_frc,pt))
					OffsetRect(&m_frc,pt.x - m_frc.right + m_BorderSpace,0);
			}
			else if(m_currState == TBS_FLAG_DOCKRIGHT)
			{
				OffsetRect(&m_frc,m_vrc.right - m_frc.right,0);
				if(!PtInRect(&m_frc,pt))
					OffsetRect(&m_frc,pt.x - m_frc.left - m_BorderSpace,0);
			}
				
			if(!PtInRect(&m_frc,pt))
				OffsetRect(&m_frc,pt.y - (m_frc.bottom - m_frc.top)/2,0);
			//frc�������ƶ�������hrc
			OffsetRect(&m_hrc,m_frc.left - m_hrc.left,0);
		}
	}
	/*ԭ����
	{
	//��Ϊm_frc
	RECT rcCheck;
	m_child->Parent()->GetClientRect(&rcCheck);
	MapWindowPoints(m_child->Parent()->Handle(),hWnd,(LPPOINT)&rcCheck,2);
	
	  if((m_frc.bottom - m_frc.top) > (m_frc.right - m_frc.left))
	  {
	  if(state != m_currState)
	  {
	  if(m_currState == TBS_FLAG_DOCKTOP)
					OffsetRect(&m_frc,0,m_hrc.top - m_frc.top);
					else if(m_currState == TBS_FLAG_DOCKBOTTOM)
					OffsetRect(&m_frc,0,m_hrc.bottom - m_frc.bottom);
					
					  if(!PtInRect(&m_frc,pt))
					  OffsetRect(&m_frc,pt.x - (m_frc.right - m_frc.left)/2,0);
					  //frc�������ƶ�������vrc
					  OffsetRect(&m_vrc,0,m_frc.top - m_vrc.top);
					  }
					  //����hrc
					  if(pt.y > (rcCheck.bottom + rcCheck.top)/2)
					  {
					  //������°벿�֣�����m_hrc;
					  OffsetRect(&m_hrc,0,m_frc.bottom - m_hrc.bottom);
					  if(pt.y <= m_hrc.top)
					  OffsetRect(&m_hrc,0,pt.y - m_hrc.top - m_BorderSpace);
					  }
					  else
					  {
					  //������ϰ벿�֣�����m_hrc;
					  OffsetRect(&m_hrc,0,m_frc.top - m_hrc.top);
					  if(pt.y >= m_hrc.bottom)
					  OffsetRect(&m_hrc,0,pt.y - m_hrc.bottom + m_BorderSpace);
					  }
					  }
					  else
					  {
					  if(state != m_currState)
					  {
					  if(m_currState == TBS_FLAG_DOCKLEFT)
					  OffsetRect(&m_frc,m_vrc.left - m_frc.left,0);
					  else if(m_currState == TBS_FLAG_DOCKRIGHT)
					  OffsetRect(&m_frc,m_vrc.right - m_frc.right,0);
					  
						 if(!PtInRect(&m_frc,pt))
						 OffsetRect(&m_frc,pt.y - (m_frc.bottom - m_frc.top)/2,0);
						 //frc�������ƶ�������hrc
						 OffsetRect(&m_hrc,m_frc.left - m_hrc.left,0);
						 }
						 //����vrc
						 if(pt.x > (rcCheck.right + rcCheck.left)/2)
						 {
						 //������Ұ벿�֣�����m_vrc;
						 OffsetRect(&m_vrc,m_frc.right - m_vrc.right,0);
						 if(pt.x <= m_vrc.left)
						 OffsetRect(&m_vrc,pt.x - m_vrc.left - m_BorderSpace,0);
						 }
						 else
						 {
						 //�������벿�֣�����m_vrc;
						 OffsetRect(&m_vrc,m_frc.left - m_vrc.left,0);
						 if(pt.x >= m_vrc.right)
						 OffsetRect(&m_vrc,pt.x - m_vrc.right + m_BorderSpace,0);
						 }
						 }
						 }
	*/
}

BOOL RingDockBar::AdjustSizeChildH()
{
	int step;
	
	m_rcDockPos.left = 0;	
	m_rcDockPos.right = m_pSite->m_rcPos.right - m_pSite->m_rcPos.left;
	m_rcClient.left = m_BorderSpace + m_GripperSpace;
	m_rcClient.right = m_rcDockPos.right - m_BorderSpace;

	step = m_pSite->m_rcPos.right - m_pSite->m_rcPos.left - m_GripperSpace - m_BorderSpace*2;	
	step = m_child->OnParentNotify(NULL,MYWM_SIZE,step,TBS_FLAG_HORZ);
	m_nSize = step + m_BorderSpace * 2;
	
	if(m_lineInfo)
	{
		//����ߴ�
		m_rcDockPos.top = m_lineInfo->m_pos;
		m_rcDockPos.bottom = m_rcDockPos.top + m_nSize;
		m_rcClient.top = m_rcDockPos.top + m_BorderSpace;
		m_rcClient.bottom = m_rcClient.top + step;

		step = m_nSize - m_lineInfo->m_maxSize;
		if(step != 0)
		{
			m_lineInfo->m_maxSize = m_nSize;
			if(m_pSite && m_State != TBS_FLAG_FLY)
				m_pSite->UpdateLine(m_lineInfo,step,FALSE);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL RingDockBar::AdjustSizeChildV()
{
	int step;
	
	m_rcDockPos.top = 0;	
	m_rcDockPos.bottom = m_pSite->m_rcPos.bottom - m_pSite->m_rcPos.top;
	m_rcClient.top = m_BorderSpace + m_GripperSpace;
	m_rcClient.bottom = m_rcDockPos.bottom - m_BorderSpace;
	
	step = m_pSite->m_rcPos.bottom - m_pSite->m_rcPos.top - m_GripperSpace - m_BorderSpace*2;	
	step = m_child->OnParentNotify(NULL,MYWM_SIZE,step,TBS_FLAG_VERT);
	m_nSize = step + m_BorderSpace * 2;
	
	if(m_lineInfo)
	{
		//����ߴ�
		m_rcDockPos.left = m_lineInfo->m_pos;
		m_rcDockPos.right = m_rcDockPos.left + m_nSize;
		m_rcClient.left = m_rcDockPos.left + m_BorderSpace;
		m_rcClient.right = m_rcClient.left + step;
		
		step = m_nSize - m_lineInfo->m_maxSize;
		if(step != 0)
		{
			m_lineInfo->m_maxSize = m_nSize;
			if(m_pSite && m_State != TBS_FLAG_FLY)
				m_pSite->UpdateLine(m_lineInfo,step,FALSE);
		}
		return TRUE;
	}
	return FALSE;
}


BOOL RingDockBar::AdjustChildH(int)
{
	if(m_child && m_pSite && m_bVisible)
	{
		if(IsAllLineBar())
			return AdjustSizeChildH();

		if(m_lineInfo)
		{
			//����ߴ�
			m_rcDockPos.top = m_lineInfo->m_pos;
			if(m_pSite->IsSizeBarInLine(m_lineInfo))
			{
				//��SizeBar
				if(m_prev == NULL || !m_prev->IsVisible())
					m_rcDockPos.left = 0;
				else
					m_rcDockPos.left = m_prev->m_rcDockPos.right;
			}
			UpdateSize();
		}
		return TRUE;
	}
	return FALSE;
}

BOOL RingDockBar::AdjustChildV(int)
{
	if(m_child && m_pSite && m_bVisible)
	{
		if(IsAllLineBar())
			return AdjustSizeChildV();
		
		if(m_lineInfo)
		{
			//����ߴ�
			m_rcDockPos.left = m_lineInfo->m_pos;
			if(m_pSite->IsSizeBarInLine(m_lineInfo))
			{
				//��SizeBar
				if(m_prev == NULL || !m_prev->IsVisible())
					m_rcDockPos.top = 0;
				else
					m_rcDockPos.top = m_prev->m_rcDockPos.bottom;
			}
			UpdateSize();
		}
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////
//
//	RingAllLineBar
//
///////////////////////////////////////////////////////
RingAllLineBar::RingAllLineBar()
{
	m_bSetH = m_bSetV = FALSE;
}

BOOL RingAllLineBar::ResetDragRect(LPRECT lprc,int state)
{
	RECT rc = {0,0,0,0};
	NMHDR hdr;
	hdr.hwndFrom = m_child->Handle();
	hdr.code = state;
	hdr.idFrom = (UINT)m_child;
	
	if(SendMessage(m_child->Handle(),WM_GETDRAGRECTS,(WPARAM)&rc,(LPARAM)&hdr))
	{
		InflateRect(&rc,m_BorderSpace,m_BorderSpace);
			
		if(::IsVert(state))
			rc.top -= m_GripperSpace + m_BorderSpace;
		else
			rc.left -= m_GripperSpace + m_BorderSpace;
		
		OffsetRect(&rc,lprc->left-rc.left,lprc->top-rc.top);
		CopyRect(lprc,&rc);
		return TRUE;
	}
	return FALSE;
}

//���غ����������϶����Σ����ؾ��ο��
int RingAllLineBar::SetDragRect(HWND hWnd,RingDragger* dragger)
{
	if(m_currState != m_State)
	{
		//�ָ�ԭ����
		if(m_child)
		{
			if(::IsVert(m_State) && !m_bSetV)
			{
				if(!m_bSetV)
					m_bSetV = ResetDragRect(&m_vrc,TBS_FLAG_DOCKRIGHT);
			}
			else
			{
				if(!m_bSetH)
					m_bSetH = ResetDragRect(&m_hrc,TBS_FLAG_DOCKBOTTOM);
			}			
		}
	}
	return RingDockBar::SetDragRect(hWnd,dragger);
}

LRESULT RingAllLineBar::StopDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param)
{
	m_bSetH = m_bSetV = FALSE;
	return RingDockBar::StopDrag(hWnd,dragger,param);
}
/*
void RingAllLineBar::DrawFrame(HDC hDC)
{
	if(LOWORD(m_uStyle) != TBS_EX_NONE)
		if(IsVert())
			DrawEdge(hDC,(LPRECT)&m_winpos,BDR_RAISEDINNER,BF_RECT);
		else
			DrawEdge(hDC,(LPRECT)&m_winpos,BDR_RAISEDINNER,BF_BOTTOM|BF_TOP);
}
*/