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
//ԭ�ļ�����ringmenubar.cpp
//
//���ļ�˵�����˵���ʵ�ִ��롣
//
//�����ߴ�˵����
//		��չ�ߴ� - 	ϵͳͼ�꣬��С/�󻯣��رյ�ϵͳ��ť�ĳߴ磬�óߴ���
//						״̬��Ϊ0��ʵ�ʳߴ�
//		m_BarSize - �˵�����Χ�ߴ磬������չ�ߴ�������к�İ�Χ���γߴ硣
//		m_fSize -	����ʱ�İ�Χ�ߴ硣
//		m_BtnsSize - ��ť����չ�ߴ����г�һ��/��ʱ�Ŀ�/�ߡ�
//		m_WndSize - ʵ�ʴ��ڳߴ�
//		m_SingleSize - ����/��ʱ�ĸ�/��
//
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdows.h"

RingMenuBar::RingMenuBar()
{
   m_hWnd = NULL;
   m_windowType = ITIS_MENUBAR;
	m_rm = NULL;
	m_rmsys = NULL;
	m_sysIcon = NULL;
	m_hbmSys = NULL;
	m_bShowIcon = FALSE;
	m_nCapedExtButton = 0;
	m_nPopupedMenu = m_nHotButton = -1;
	m_ExStyle = TBS_EX_DOCKABLE;
	SetRectEmpty(&m_rcSysBtn);
	m_item = NULL;
	m_nBtnCount = m_BtnAllocCnt = 0;
	m_dockbar = NULL;
	m_fSize.cx = m_fSize.cy = 0;
	m_SingleSize.cx = m_SingleSize.cy = 0;
	m_nMinWidth = 0;
}

RingMenuBar::~RingMenuBar()
{
	delete m_rmsys;
	FreeMenuItems();
	if(m_hbmSys)
		DeleteObject(m_hbmSys);
}

BOOL RingMenuBar::Create(LPCTSTR szTitle,int dockStyle/*=TBS_EX_DOCKABLE*/,
								 int dockState/*=TBS_FLAG_DOCKTOP*/)
{
	if(IsWindow())
		return TRUE;
	
	WNDCLASSEX wndclsex =
   {
		sizeof(WNDCLASSEX),
			CS_PARENTDC,//CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
			InitWndProc,
			0,//sizeof(RingBaseWnd*),
			0,//sizeof(RingBaseWnd*),
			GetInstance(),
			NULL,
			NULL,
			(HBRUSH)NULL,//(COLOR_BTNFACE + 1),
			NULL,
			"RingMenuBar",
			NULL
   };
	
	if(!RegRingdowClass(&wndclsex))
		return NULL;
	
	m_BarSize.cx = m_WndSize.cx = 0;
	m_BarSize.cy = m_WndSize.cy = 0;
	m_BtnsSize.cx = m_BtnsSize.cy = 0;

	CreateEx(szTitle,"RingMenuBar",
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0/*WS_EX_TOOLWINDOW*/,NULL,NULL,0,0,0,0);

	Show(SW_SHOW);
	
	m_dockbar = EnableDock(szTitle,dockStyle|TBS_EX_ALLLINE,dockState);
	return (BOOL)m_hWnd;
}

//ע�⣺hMenu����������ringCoolMenu����ĳ�Ա������
//RingMenuBar::Attach�����������ͬһ��hMenu�����������ȷ��
BOOL RingMenuBar::LoadMenu(HMENU hMenu/*=NULL*/,BOOL bRemove/*=TRUE*/)
{	
	if(hMenu == NULL)
		m_rm = ((RingWnd*)m_parent)->InstallCoolMenu();
	else
	{
		if(m_rm == NULL)
			m_rm = new RingCoolMenu;

		if(m_rm)
			m_rm->Attach(hMenu);
	}
	return LoadMenu(m_rm,bRemove);
}

BOOL RingMenuBar::LoadMenu(RingCoolMenu* rm,BOOL bRemove/*=TRUE*/)
{	
	if(m_rm == NULL)
		if(rm)
			m_rm = rm;
		else
			return FALSE;

	if(Create(LANSTR_MENUBARNAME))
	{
		if(m_font.GetFont() == NULL)
			m_font.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
			
		if(m_vfont.GetFont() == NULL)
		{
			LOGFONT lf;
			if(m_font.GetLogFont(&lf))
			{
				lf.lfEscapement = 2700;
				m_vfont.SetFont(&lf);
			}
		}
		if(m_dockbar && m_dockbar->IsVert())
			m_vfont.SetWindowFont(m_hWnd);
		else
			m_font.SetWindowFont(m_hWnd);

		if(!MakeMenuButtons())
			return FALSE;
			
		if(bRemove)
			::SetMenu(m_parent->Handle(),NULL);

		if(m_parent->GetWindowType() == ITIS_MDIFRAME && m_hbmSys == NULL)
		{
			//MDI�˵�������Ҫ��ʼ����С����ԭ���رհ�ť
			ringFont rf("Marlett",9);
			HDC hMDC = GetDC(GetDesktopWindow());
			HDC hDC = CreateCompatibleDC(hMDC);
			//�����С12����С����ԭ���رհ�ť�������й���36��
			//һ����ʾ���˵�����Ϊһ��ͼ���ͼ���50���رհ�ť��
			//ǰһ��ť��2���أ�ÿһ��ť16*14��36+50=86��
			m_hbmSys = CreateCompatibleBitmap(hMDC,86,14);
			SelectObject(hDC,m_hbmSys);
				
			::SetBkColor(hDC,0x00FFFFFF);
			::SetTextColor(hDC,0L);
			RECT rc = {0,0,36,12};
			SelectObject(hDC,rf.GetFont());
			DrawText(hDC,"02r",-1,&rc,DT_LEFT);
			DeleteDC(hDC);
			ReleaseDC(GetDesktopWindow(),hMDC);
			SetRect(&m_rcSysBtn,0,3,50,17);
			if(m_parent->GetWindowType() == ITIS_MDIFRAME)
				((RingMDIFrameWnd*)m_parent)->SetMenuBar(this);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL RingMenuBar::MakeMenuButtons()
{
	if(m_rm == NULL)
		return FALSE;

	int i,cnt;//,nStr;
	char menutext[80];
		
	cnt = GetMenuItemCount(m_rm->GetMenu());
	if(!AllocMenuItems(cnt))		
		return FALSE;

	MENUITEMINFO mi;
	DWORD data;
	UINT state;
	int index = 0;

	for(i=0;i<cnt;i++)
	{
		memset(&mi,0,sizeof(MENUITEMINFO));
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_ID | MIIM_STATE | MIIM_SUBMENU;

		memset(menutext,0,80);
		GetMenuString(m_rm->GetMenu(),i,menutext,80,MF_BYPOSITION);
	
		GetMenuItemInfo(m_rm->GetMenu(),i,TRUE,&mi);
		
		state = (mi.fState == MFS_DISABLED)?TBSTATE_INDETERMINATE:TBSTATE_ENABLED;
		if(mi.hSubMenu)
		{
			data = index | 0xFFFFFF00;
			index ++;
		}
		else
			data = mi.wID;

		if(!AddButton(menutext,state,data))
			return FALSE;
	}
	return TRUE;
}

BOOL RingMenuBar::AllocMenuItems(int cnt)
{
	cnt ++;
	if(cnt > 0)
	{
		LPRINGMENUBARITEM tmp = m_item;
		if(m_item && cnt > m_BtnAllocCnt)
		{
			m_item = (LPRINGMENUBARITEM)GlobalReAlloc(m_item,sizeof(RINGMENUBARITEM)*cnt,GMEM_MOVEABLE);
			if(m_item == NULL)
			{
				m_item = tmp;
				return FALSE;
			}
			else
				m_BtnAllocCnt = cnt;
		}
		else if(m_item == NULL)
		{
			m_item = (LPRINGMENUBARITEM)GlobalAlloc(GPTR,sizeof(RINGMENUBARITEM)*cnt);
			m_BtnAllocCnt = cnt;
		}
		return (BOOL)m_item;
	}
	else
		return FALSE;
}

void RingMenuBar::FreeMenuItems()
{
	LPRINGMENUBARITEM tmp = m_item;	
	try
	{
		if(tmp)
			for(int i=0;i<m_nBtnCount;i++)
			{
				Del((LPSTR)tmp->m_text);
				tmp ++;
			}
	}
	catch(...)
	{
	}
	GlobalFree(m_item);
	m_item = NULL;
}

BOOL RingMenuBar::AddButton(LPCTSTR szText,int state,DWORD dwData)
{
	if(m_item == NULL)
		return FALSE;

	int index;
	
	for(index=0;index<m_nBtnCount;index++)
	{
		if(m_item[index].m_state == 0)
			break;	
	}
	if(index >= m_BtnAllocCnt && !AllocMenuItems(index + 2))
		return FALSE;

	if(szText)
	{
		m_item[index].m_text = (LPTSTR)New(strlen(szText)+2);
		if(m_item[index].m_text)
			wsprintf(m_item[index].m_text,szText);
	}
	m_item[index].m_state = state;
	m_item[index].m_data = dwData;
	m_item[index].m_id = index;

	m_nBtnCount ++;
	return TRUE;	
}

BOOL RingMenuBar::InsertButton(int index,LPCTSTR szText,int state,DWORD dwData)
{
	index ++;
	if(index >= 0)
	{
		if(index >= m_nBtnCount && m_nBtnCount < m_BtnAllocCnt - 1)
			index = m_nBtnCount + 1;

		if(!AllocMenuItems(index))
			return FALSE;
				
		if(index <= m_nBtnCount)
			memmove(m_item + index + 1,m_item + index,
						sizeof(RINGMENUBARITEM)*(m_nBtnCount - (index - 1)));
		
		if(szText)
		{
			m_item[index].m_text = (LPTSTR)New(strlen(szText)+2);
			if(m_item[index].m_text)
				wsprintf(m_item[index].m_text,szText);
		}
		m_item[index].m_state = state;
		m_item[index].m_data = dwData;
		m_item[index].m_id = index;
		m_nBtnCount ++;
		return TRUE;
	}
	return FALSE;
}

//���¸���Ťλ�ã����㵱ǰ״̬��BarSize,WndSize,BtnsSize
//����ʱ��Ҫ���ݴ��ڿ��ж��Ƿ����У�����ʱ������
void RingMenuBar::Update(int state)
{
	if(m_dockbar && m_item)
	{
		int index = 1,ext = EndSize();
		int nIconSize = PreSize();
		HDC hDC = GetDC(m_hWnd);	
		SIZE s;
		RECT rc = {0,0,0,0};

		if(state == -1)
			state = m_dockbar->GetState();

		if(IsVert(state))
		{
			SelectObject(hDC,m_vfont.GetFont());

			if(m_item->m_text)
			{
				s.cy = DrawText(hDC,m_item->m_text,
						strlen(m_item->m_text),&rc,
						DT_CALCRECT);
					//GetTextExtentPoint32(hDC,m_item->m_text,
					//	strlen(m_item->m_text),&s);
				s.cx = rc.right + 12;

				SetRect(&m_item->m_rect,0,nIconSize,s.cy + 8,nIconSize + s.cx);
			}
			m_BtnsSize.cy = m_BarSize.cy = s.cx + nIconSize;
			m_BarSize.cx = s.cy + 8;
			
			m_WndSize.cy = m_dockbar->GetClientHeight(0);

			for(;index<m_nBtnCount;index ++)
			{
				if(m_item[index].m_text)
				{
					s.cy = DrawText(hDC,m_item[index].m_text,
							strlen(m_item[index].m_text),&rc,
							DT_CALCRECT/*|DT_CENTER|DT_NOCLIP|DT_SINGLELINE*/);
					s.cx = rc.right + 12;
						
					SetRect(&m_item[index].m_rect,
							m_item[index-1].m_rect.left,
							m_item[index-1].m_rect.bottom,
							m_item[index-1].m_rect.left + s.cy + 8,
							m_item[index-1].m_rect.bottom + s.cx);
					m_BarSize.cy = max(m_BarSize.cy,m_item[index].m_rect.bottom);
					m_BtnsSize.cy += s.cx;
				}
			}
			m_WndSize.cx = m_BarSize.cx;
		}
		else
		{
			SelectObject(hDC,m_font.GetFont());
			
			m_nMinWidth = EndSize();

			if(m_item->m_text)
			{
				s.cy = DrawText(hDC,m_item->m_text,strlen(m_item->m_text),&rc,DT_CALCRECT);
				s.cx = rc.right + 12;
				SetRect(&m_item->m_rect,nIconSize,0,nIconSize + s.cx,s.cy + 8);
			}
			m_BarSize.cx = s.cx + nIconSize;
			m_BarSize.cy = s.cy + 8;
			m_nMinWidth = max(s.cx + nIconSize,m_nMinWidth);
		
			if(state == TBS_FLAG_FLY)
			{
				if(m_fSize.cy > GetHorzLineHeight())
					m_WndSize.cx = m_dockbar->GetClientWidth(m_fSize.cx + nIconSize);
				else
					m_WndSize.cx = m_dockbar->GetClientWidth(m_fSize.cx + nIconSize + ext);
			}
			else
				m_WndSize.cx = m_dockbar->GetClientWidth(0);

			for(;index<m_nBtnCount;index ++)
			{
				if(m_item[index].m_text)
				{
					SetRectEmpty(&rc);
					s.cy = DrawText(hDC,m_item[index].m_text,
										strlen(m_item[index].m_text),
										&rc,DT_CALCRECT);
					s.cx = rc.right + 12;

					SetRect(&m_item[index].m_rect,
								m_item[index-1].m_rect.right,
								m_item[index-1].m_rect.top,
								m_item[index-1].m_rect.right + s.cx,
								m_item[index-1].m_rect.top + s.cy + 8);
					m_nMinWidth = max(s.cx,m_nMinWidth);
				}

				if((m_item[index].m_rect.right) >= m_WndSize.cx)
				{
					//wrap
					OffsetRect(&m_item[index].m_rect,
							- m_item[index].m_rect.left,
							m_item[index].m_rect.bottom - m_item[index].m_rect.top);
					m_BarSize.cy += s.cy + 8;
				}
				m_BarSize.cx = max(m_BarSize.cx,m_item[index].m_rect.right);
			}
			if(m_item[m_nBtnCount-1].m_rect.right + ext >= m_WndSize.cx)
				m_BarSize.cy += s.cy + 8;
			else
				m_BarSize.cx += ext;
			
			m_BtnsSize.cx = min(m_BarSize.cx + ext,m_WndSize.cx);
			m_WndSize.cy = m_BarSize.cy;
		}
		ReleaseDC(m_hWnd,hDC);
	}
}

//���㰴ť�������гߴ�
int RingMenuBar::GetBtnsWidth()
{
	HDC hDC = GetDC(m_hWnd);	
	RECT rc = {0,0,0,0};
	
	SelectObject(hDC,m_font.GetFont());
			
	if(m_item->m_text)
	{
		DrawText(hDC,m_item->m_text,strlen(m_item->m_text),&rc,DT_CALCRECT);
		m_BtnsSize.cx = rc.right + 12 + PreSize();
	}
	for(int i=1;i<m_nBtnCount;i++)
	{
		if(m_item[i].m_text)
		{
			DrawText(hDC,m_item[i].m_text,strlen(m_item[i].m_text),&rc,DT_CALCRECT);
			m_BtnsSize.cx += rc.right + 12;
		}
	}
	m_BtnsSize.cx += EndSize();
	ReleaseDC(m_hWnd,hDC);
	return m_BtnsSize.cx;
}

//���㰴ť�������гߴ�
int RingMenuBar::GetBtnsHeight()
{
	int ext = EndSize();
	int nIconSize = PreSize();
	HDC hDC = GetDC(m_hWnd);	
	RECT rc = {0,0,0,0};
	
	SelectObject(hDC,m_vfont.GetFont());
			
	if(m_item->m_text)
	{
		DrawText(hDC,m_item->m_text,strlen(m_item->m_text),&rc,DT_CALCRECT);
		m_BtnsSize.cy = rc.right + 12 + PreSize();
	}
	for(int i=1;i<m_nBtnCount;i++)
	{
		if(m_item[i].m_text)
		{
			DrawText(hDC,m_item[i].m_text,strlen(m_item[i].m_text),	&rc,DT_CALCRECT);
			m_BtnsSize.cy += rc.right + 12;
		}		
	}
	ReleaseDC(m_hWnd,hDC);
	m_BtnsSize.cy += EndSize();
	return m_BtnsSize.cy;
}

//��ȡ���в˵������
int RingMenuBar::GetVertLineWidth()
{
	if(m_SingleSize.cx == 0)
	{
		HDC hDC = GetDC(m_hWnd);	
		RECT rc = {0,0,0,0};
			
		SelectObject(hDC,m_vfont.GetFont());
			
		if(m_item && m_item->m_text)
		{
			DrawText(hDC,m_item->m_text,strlen(m_item->m_text),&rc,DT_CALCRECT);
			m_SingleSize.cx = rc.bottom + 8;
		}
		ReleaseDC(m_hWnd,hDC);
	}
	return m_SingleSize.cx;
}

//��ȡ���в˵����߶�
int RingMenuBar::GetHorzLineHeight()
{
	if(m_SingleSize.cy == 0)
	{
		HDC hDC = GetDC(m_hWnd);	
		RECT rc = {0,0,0,0};
			
		SelectObject(hDC,m_font.GetFont());
			
		if(m_item && m_item->m_text)
		{
			DrawText(hDC,m_item->m_text,strlen(m_item->m_text),&rc,DT_CALCRECT);
			m_SingleSize.cy = rc.bottom + 8;
		}
		ReleaseDC(m_hWnd,hDC);
	}
	return m_SingleSize.cy;
}

BOOL RingMenuBar::OnGetDragRects(int state,LPRECT lprc)
{
	if(lprc && m_dockbar)
	{
		lprc->left = lprc->top = 0;

		switch(state)
		{
		case TBS_FLAG_FLY:
			if(m_fSize.cx == 0 || m_fSize.cy == 0)
			{
				//��ʼ���������Σ�Ϊ���о���
				m_fSize.cx = GetBtnsWidth();
				m_fSize.cy = GetHorzLineHeight();
			}
			lprc->right = m_fSize.cx;
			lprc->bottom = m_fSize.cy;
			break;
		case TBS_FLAG_HORZ:
			if(m_dockbar->IsVert())
			{
				lprc->right = GetBtnsWidth();
				lprc->bottom = m_BarSize.cx;
			}
			else
			{
				lprc->right = m_WndSize.cx;
				lprc->bottom = m_BarSize.cy;
			}
			break;
		case TBS_FLAG_VERT:
			if(m_dockbar->IsVert())
			{
				lprc->right = m_BarSize.cx;
				lprc->bottom = m_WndSize.cy;
			}
			else
			{
				lprc->bottom = GetBtnsHeight();
				//if(m_item)
				//	lprc->right = m_item->m_rect.bottom - m_item->m_rect.top;
				//else
					lprc->right = m_BarSize.cy;
			}
			break;
		case TBS_FLAG_DOCKBOTTOM:
			//��ȡ���и�
			lprc->right = GetBtnsWidth();
			lprc->bottom = GetHorzLineHeight();
			break;
		case TBS_FLAG_DOCKRIGHT:
			//��ȡ���п�
			lprc->right = GetVertLineWidth();
			lprc->bottom = GetBtnsHeight();
			break;
		}
		return TRUE;
	}
	return FALSE;
}

int RingMenuBar::OnDockStateChange(int stateOld,int stateNew)
{
	Update();

	if(IsFloat(stateNew))
		SetPos(0,0,m_BarSize.cx,m_BarSize.cy,NULL,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOREDRAW);
	else
		SetPos(0,0,m_WndSize.cx,m_WndSize.cy,NULL,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOREDRAW);
	return TRUE;
}

LRESULT RingMenuBar::RingdowProc(HWND hWnd,RINGPARAMS param)
{	
	switch(param.uMsg)
	{
		case WM_LBUTTONDOWN:
			return OnLButtonDown(param);
		case WM_MOUSEMOVE:
			return OnMouseMove(param);			
		case WM_LBUTTONUP:
			return OnLButtonUp(param);
		case WM_RBUTTONDOWN:
			return OnRButtonDown(param);
		case WM_SIZING:
			if(m_dockbar)
				return OnSizing(param);
		case WM_GETDRAGRECTS:
			if(param.lpnmhdr->hwndFrom == m_hWnd)
				return OnGetDragRects(param.lpnmhdr->code,(LPRECT)param.wParam);
		case WM_DOCKSTATECHANGE:
			return OnDockStateChange(param.shortval.low,param.shortval.high);
		//case WM_ERASEBKGND:
		case WM_PAINT:
			return OnPaint(param);
		case TB_HITTEST:
			return OnHitTest(param);
		case WM_MOUSELEAVE:
			if(m_nPopupedMenu == -1)
				PressButton(m_nHotButton,FALSE);
			m_nHotButton = -1;
			return 0;
		case WM_COMMAND:
			m_nPopupedMenu = -1;
			//don't break;
		case WM_MENUSELECT:
			return SendMessage(m_parent->Handle(),param);
		case WM_TIMER:
			return OnTimer(param);
		case WM_MEASUREITEM:      	
			if(m_rm)
   			m_rm->MeasureItem(param.lpmeasureitem);
         break;
      case WM_DRAWITEM:
			if(m_rm)
   			m_rm->Draw(param.lpdrawitem);
         break;
		case WM_ENTERMENULOOP:
			//�����˵�����Ҫѹ�°�ť��������ʱ��������궯��
			PressButton(m_nPopupedMenu,TRUE);
			SetTimer(m_hWnd,1,10,NULL);
			break;
		case WM_EXITMENULOOP:
			//�˵��رգ���Ҫ����ť���رն�ʱ��
			PressButton(m_nPopupedMenu,FALSE);
			KillTimer(m_hWnd,1);
			m_nPopupedMenu = -1;
			//��ť���º�ѡ��˵����Ϊ�ڲ˵����������޷��õ�
			//WM_LBUTTONUP��Ϣ�����¹��������˵��ڸǵİ�ť�޷���ʾ��
			//�����Ҫ�ò˵����ػ���
			InvalidateRect(m_hWnd,NULL,FALSE);

			//δ���BUG�������BUTTON�����޲˵��������򲻻��յ�
			//����Ϣ���������������޷�����ť
			//���˼·����⵽��BUTTON�����º�SetCapture��OnTimer��
			//����Ƶ��в˵��İ�ťʱReleaseCapture��Capture��
			//��갴����ϢReleaseCapture��Ȼ����б���Ϣ��ɹ���
			//�Ĵ�����Ҫ��֤SetCapture��ReleaseCapture��ʱ����ȷ��
			//�����Ժ������Ͼ���BUG���ڸ��˵����зǵ����˵�����
			//ѡ�иò˵���ڲ˵����ⰴ�������Żᷢ����������
			//ע��༭��Դʱȷ�����˵����޷ǵ����˵�����ɱ����BUG��			
			break;
		case MYWM_ICON:
			//��ʱ����⵽�˵�������Ȼ������ƶ�����İ�ť��
			//�����¼�������Ϣ
			////���͵�����Ϣ��m_nPopupedMenu��Ϊ�Ϸ�ֵ
			if(m_rm && m_dockbar)
			{				
				PressButton(m_nPopupedMenu,FALSE);
				m_nPopupedMenu = param.wParam;

				RECT rc = {0,0,16,16};
				ClientToScreen(m_hWnd,(LPPOINT)&rc);
				ClientToScreen(m_hWnd,(LPPOINT)&rc+1);

				if(m_nPopupedMenu == MBAR_SYSICONSEL)
				{
					if(m_rmsys)
						m_rmsys->PopupEx(m_hWnd,rc.left,rc.top,m_dockbar->IsVert(),&rc);
					return 0;
				}
								
				GetButtonRect(param.wParam,&rc);
				ClientToScreen(m_hWnd,(LPPOINT)&rc);
				ClientToScreen(m_hWnd,(LPPOINT)&rc+1);				
				
				if(m_item[m_nPopupedMenu].m_data >= 0xFFFFFF00)	//�в˵�
					m_rm->SubMenu(m_item[m_nPopupedMenu].m_data & 0xFF)->PopupEx(m_hWnd,rc.left,rc.top,m_dockbar->IsVert(),&rc);
				else
				{
					//�ǲ˵���޵����˵�����˲��ᴥ��WM_ENTERMENULOOP
					//��Ϣ����WM_ENTERMENULOOP����ɵĹ���
					PressButton(m_nPopupedMenu,TRUE);
					SetTimer(m_hWnd,1,10,NULL);
				}
			}
			return 0;		
	}
	return DefaultProc(param);
}

LRESULT RingMenuBar::OnHitTest(RINGPARAMS& param)
{
	LPPOINT pt = (LPPOINT)param.lParam;
	if(m_item)
		for(int i=0;i<m_nBtnCount;i++)
		{
			if(PtInRect(&m_item[i].m_rect,*pt))
				return i;
		}
	return -1;
}

LRESULT RingMenuBar::OnLButtonDown(RINGPARAMS& param)
{	
	if(m_nPopupedMenu != -1)
	{
		//����˵����зǵ����˵������ƶ���ʹ�ð�ť�ʰ���״̬��
		//Ȼ������Ƴ��˵���������ð�ť��Ϊ����״̬�����´���
		//��Ϊ���ȴ�ʩ����������
		PressButton(m_nPopupedMenu,FALSE);
		m_nPopupedMenu = -1;
		KillTimer(m_hWnd,1);
	}

	POINT pt = {param.mousept.x,param.mousept.y};
		
	int n = SendMessage(m_hWnd,TB_HITTEST,0,(LPARAM)&pt);

	if(m_nPopupedMenu == n && n != -1)
	{
		m_nPopupedMenu = -1;
		return 0;
	}

	if(n < 0 || n >= GetButtonCount())
	{
		if(m_hbmSys && m_bShowIcon)
		{
			n = CheckInExtButton(&pt,TRUE);
			if(n == MBAR_SYSICONSEL)
			{
				//����ͼ�����С��/��ԭ/�رհ�ť
				m_nPopupedMenu = MBAR_SYSICONSEL;
				PostMessage(m_hWnd,MYWM_ICON,m_nPopupedMenu,0);
				return 0;
			}
			else if(n != 0)
			{
				SetCapture(m_hWnd);
				m_nCapedExtButton = n | 0x10000;
				PressExtButton(n,TRUE);
				return 0;
			}
		}
		else
		{
			if(m_dockbar && m_dockbar->m_pSite)
			{
				MapWindowPoints(m_hWnd,m_dockbar->m_pSite->Handle(),&pt,1);
				param.mousept.x = (short)pt.x;
				param.mousept.y = (short)pt.y;
				PostMessage(m_dockbar->m_pSite->Handle(),WM_LBUTTONDOWN,param.wParam,param.lParam);
				return 0;
			}
		}
	}

	if(m_rm && m_dockbar)
	{
		RECT rc;
		if(GetButtonRect(n,&rc))
		{
			ClientToScreen(m_hWnd,(LPPOINT)&rc);
			ClientToScreen(m_hWnd,(LPPOINT)&rc+1);
			m_nPopupedMenu = n;			
			
			if(m_item[n].m_data >= 0xFFFFFF00)	//�в˵�
				m_rm->SubMenu(m_item[n].m_data & 0xFF)->PopupEx(m_hWnd,rc.left,rc.top,m_dockbar->IsVert(),&rc);
			else
				PressButton(n);
		}
	}	
	return 0;
}

LRESULT RingMenuBar::OnMouseMove(RINGPARAMS& param)
{
	if(m_nCapedExtButton != 0)
	{
		POINT pt = {param.mousept.x,param.mousept.y};
		int n = CheckInExtButton(&pt,TRUE);
		
		if(m_nCapedExtButton < 0x10000 && n == (int)m_nCapedExtButton)
		{
			//���ӵİ�ťδ���£�����Ƶ��ð�ť
			m_nCapedExtButton |= 0x10000;
			PressExtButton(n,TRUE);
		}
		else if(m_nCapedExtButton > 0x10000 && n != LOWORD(m_nCapedExtButton))
		{
			//���ӵİ�ť�Ѱ��£�����Ƴ��ð�ť
			m_nCapedExtButton &= 0xFFFF;
			PressExtButton(m_nCapedExtButton,FALSE);
		}
	}	
	//if(!m_dragger.IsDragging())
	{
		POINT pt = {param.mousept.x,param.mousept.y};
		param.lParam = (LONG)&pt;
		if(SetHotButton(OnHitTest(param)))
		{
			TRACKMOUSEEVENT tme;
			
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			TrackMouseEvent(&tme);
		}
		return 0;		
	}

	return DefaultProc(param);
	//return RingDockBar::OnMouseMove(param);
}

LRESULT RingMenuBar::OnLButtonUp(RINGPARAMS& param)
{	
//	if(m_dragger.IsDragging())
//		return RingDockBar::OnLButtonUp(param);

	if(m_nCapedExtButton)
	{
		ReleaseCapture();
		POINT pt = {param.mousept.x,param.mousept.y};
		int n = CheckInExtButton(&pt,TRUE);
		if(n == (int)(m_nCapedExtButton & 0xFFFF))
		{
			PressExtButton(n,FALSE);
			switch(n)
			{
				case MBAR_SYSMINSEL:
					((RingMDIFrameWnd*)m_parent)->MinChild();
					break;
				case MBAR_SYSRESTORESEL:
					((RingMDIFrameWnd*)m_parent)->RestoreChild();
					break;
				case MBAR_SYSCLOSESEL:
					((RingMDIFrameWnd*)m_parent)->CloseChild();
					break;
			}			
		}
		m_nCapedExtButton = 0;
		return 0;
	}
	//else
	//	return RingDockBar::OnLButtonUp(param);

	if(m_nPopupedMenu >= 0 && m_nPopupedMenu < m_nBtnCount
		&& m_item)
	{
		if(m_item[m_nPopupedMenu].m_data >= 0xFFFFFF00)
			m_nPopupedMenu = -1;
		else
		{
			//��ť�޵����˵�����Ҫ��������ȷ�İ�ť����ID������������
			//(DefaultProc����)�����ûذ�ťID����ȷ�������İ��°�ť
			//������ڰ�ť���ƶ�����ȷ��⵽�ð�ť��
			SendMessage(m_parent->Handle(),WM_COMMAND,m_item[m_nPopupedMenu].m_data,(LPARAM)m_hWnd);
			PressButton(m_nPopupedMenu,FALSE);
			m_nPopupedMenu = -1;			
		}
	}

	return 0;
}

LRESULT RingMenuBar::OnRButtonDown(RINGPARAMS& param)
{	
	if(m_nPopupedMenu != -1)
	{
		//����˵����зǵ����˵������ƶ���ʹ�ð�ť�ʰ���״̬��
		//Ȼ������Ƴ��˵���������ð�ť��Ϊ����״̬�����´���
		//��Ϊ���ȴ�ʩ����������
		PressButton(m_nPopupedMenu,FALSE);
		m_nPopupedMenu = -1;
		KillTimer(m_hWnd,1);
	}
	
	POINT pt = {param.mousept.x,param.mousept.y};
	
	if(m_dockbar && m_dockbar->m_pSite)
	{
		MapWindowPoints(m_hWnd,m_dockbar->m_pSite->Handle(),&pt,1);
		param.mousept.x = (short)pt.x;
		param.mousept.y = (short)pt.y;
		return SendMessage(m_dockbar->m_pSite->Handle(),param);
	}
	return 0;
}

LRESULT RingMenuBar::OnTimer(RINGPARAMS& param)
{
	POINT pt;
	int n;
	GetCursorPos(&pt);
	
	//�˵������У��˵�������ס������ťʱ��Ҫ�ж��Ƿ��ڲ˵���
	if(m_item && m_nPopupedMenu >= 0 && m_nPopupedMenu < GetButtonCount())
	{
		if(m_item[m_nPopupedMenu].m_data >= 0xFFFFFF00)	//�в˵�
		{
			POINT ppt = {pt.x + 3,pt.y + 3};	//���ϲ˵��߿�
			n = MenuItemFromPoint(m_hWnd,
					m_rm->SubMenu(m_item[m_nPopupedMenu].m_data & 0xFF)->GetMenu(),
					ppt);
			if(n != -1)
				return 0;
		}
	}

	ScreenToClient(m_hWnd,&pt);
	n = SendMessage(m_hWnd,TB_HITTEST,0,(LPARAM)&pt);

	if(m_nPopupedMenu == -1 || n < 0 || n >= GetButtonCount())
		if(m_bShowIcon && CheckInExtButton(&pt) == MBAR_SYSICONSEL)
			n = MBAR_SYSICONSEL;
		else		
			return 0;

	if(n != m_nPopupedMenu)
	{	
		PostMessage(m_hWnd,WM_CANCELMODE,0,0);
		PostMessage(m_hWnd,MYWM_ICON,n,0);
	}
	
	return 0;
}

LRESULT RingMenuBar::OnSizing(RINGPARAMS& param)
{
	RECT rc;
	LPRECT lprc = (LPRECT)param.lParam;
	//param.lParam = (LPARAM)&rc;
	CopyRect(&rc,lprc);
	
	OffsetRect(lprc,-lprc->left,-lprc->top);
	lprc->right = max(m_nMinWidth,lprc->right);
	m_fSize.cx = lprc->right;
	m_fSize.cy = lprc->bottom;
	Update();
	
	lprc->right = m_BarSize.cx;
	lprc->bottom = m_BarSize.cy;
	m_fSize.cx = lprc->right;
	m_fSize.cy = lprc->bottom;
	
	MoveWindow(m_hWnd,0,0,m_fSize.cx,m_fSize.cy,FALSE);
	InvalidateRect(m_hWnd,NULL,TRUE);
	return TRUE;
}

BOOL RingMenuBar::GetButtonRect(int index,LPRECT lprect)
{
	if(lprect && index >= 0 && index < m_nBtnCount && m_item)
		return CopyRect(lprect,&m_item[index].m_rect);
	else
		return FALSE;
}

BOOL RingMenuBar::SetHotButton(int index)
{
	if(m_nHotButton == index && m_nHotButton != -1)
		return TRUE;

	PressButton(m_nHotButton,FALSE);
	m_nHotButton = -1;

	if(index >= 0 && index < m_nBtnCount && m_item)
	{
		HDC hDC = GetDC(m_hWnd);
		DrawEdge(hDC,&m_item[index].m_rect,BDR_RAISEDINNER,BF_RECT);//TOPLEFT);
		ReleaseDC(m_hWnd,hDC);

		m_nHotButton = index;
		return TRUE;
	}
	return FALSE;
}

void RingMenuBar::PressButton(int index,BOOL bPress/*=TRUE*/)
{
	if(index >= 0 && index < m_nBtnCount && m_item)
	{
		if(bPress)
			m_item[index].m_state |= TBSTATE_PRESSED;
		else
			m_item[index].m_state &= ~TBSTATE_PRESSED;
		InvalidateRect(m_hWnd,&m_item[index].m_rect,TRUE);		
	}
}

//�ж����λ��λ��ϵͳͼ�꣬��С������ԭ���رհ�ť�ĸ�λ��
//���ô˺���ǰӦ�жϲ˵����Ƿ���ʾ����Щ��ť
//�������������Ϊ����ڱ����ڵ�����
//bCheckAll���Ƿ���ж�ϵͳͼ��
int RingMenuBar::CheckInExtButton(LPPOINT lpMousePt,BOOL bCheckAll/*=FALSE*/)
{
	RECT rc = {0,0,16,16};
	POINT pt = {lpMousePt->x,lpMousePt->y};
	int res = 0;
	
	if(PtInRect(&rc,pt))
		return MBAR_SYSICONSEL;
		
	if(bCheckAll)// && PtInRect(&m_rcSysBtn,pt))
	{
		int ch = pt.x - m_rcSysBtn.left;
		CopyRect(&rc,&m_rcSysBtn);

		if(m_dockbar && m_dockbar->IsVert())
		{			
			rc.left = m_rcSysBtn.right - 16;
			rc.top = m_rcSysBtn.bottom - 50;
			ch = pt.y - rc.top;
		}
		
		if(PtInRect(&rc,pt) && (ch < 32 || ch > 34))	//���ڻ�ԭ��رհ�ť��Ŀյ�
			return (ch / 16) + MBAR_SYSMINSEL;
	}

	return -1;
}

void RingMenuBar::PressExtButton(UINT uID,BOOL bPress)
{
	if(uID < MBAR_SYSMINSEL || uID > MBAR_SYSCLOSESEL)
		;//PressButton(uID,bPress);
	else
	{
		//���£�������С������ԭ���رհ�ť
		HDC hdc = GetDC(m_hWnd);
		HDC hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC,m_hbmSys);

		RECT rc = {0,3,16,17};
		int offsetx,offsety;
		int xpos = (uID - MBAR_SYSMINSEL) * 16 + 36;

		if(m_dockbar && m_dockbar->IsVert())
		{
			offsetx = m_rcSysBtn.right - rc.right;
			offsety = (uID - MBAR_SYSMINSEL)*16 + m_rcSysBtn.bottom - 48 - rc.top;
			if(uID == MBAR_SYSCLOSESEL)
			{
				offsety += 2;
				xpos += 2;
			}			
		}
		else
		{
			offsetx = (uID - MBAR_SYSMINSEL)*16 + m_rcSysBtn.left;
			offsety = m_rcSysBtn.top - rc.top;
			if(uID == MBAR_SYSCLOSESEL)
			{
				offsetx += 2;
				xpos += 2;
			}
		}
		OffsetRect(&rc,offsetx,offsety);

		BitBlt(hdc,rc.left,rc.top,16,14,hMemDC,xpos,0,SRCCOPY);

		if(bPress)
		{
			BitBlt(hdc,rc.left+3,rc.top+2,12,12,hMemDC,(uID - MBAR_SYSMINSEL) * 12,0,SRCAND);
			DrawEdge(hdc,&rc,EDGE_SUNKEN,BF_TOPLEFT);
			DrawEdge(hdc,&rc,BDR_SUNKENINNER,BF_BOTTOMRIGHT);			
		}
		else
		{
			BitBlt(hdc,rc.left+2,rc.top+1,12,12,hMemDC,(uID - MBAR_SYSMINSEL) * 12,0,SRCAND);
			DrawEdge(hdc,&rc,BDR_RAISEDINNER,BF_TOPLEFT);
			DrawEdge(hdc,&rc,EDGE_RAISED,BF_BOTTOMRIGHT);
		}
		DeleteDC(hMemDC);
		ReleaseDC(m_hWnd,hdc);
	}

}

LRESULT RingMenuBar::OnPaint(RINGPARAMS& param)
{
	//OnEraseBkg(param);
		
	PAINTSTRUCT ps;
	RECT rc;
	BeginPaint(m_hWnd,&ps);
	if(m_dockbar)
		m_dockbar->DrawBkg(ps.hdc);
	//ps.hdc = param.hdc;

	SetBkMode(ps.hdc,TRANSPARENT);
		
	if(m_dockbar && m_dockbar->IsVert())
		SelectObject(ps.hdc,m_vfont.GetFont());
	else
		SelectObject(ps.hdc,m_font.GetFont());

	for(int i=0;i<=m_nBtnCount;i++)
	{
		if(m_item[i].m_text)
		{
			CopyRect(&rc,&m_item[i].m_rect);
			if((m_item[i].m_state & TBSTATE_PRESSED) == TBSTATE_PRESSED)
			{
				DrawEdge(ps.hdc,&rc,BDR_SUNKENOUTER,BF_RECT);
				OffsetRect(&rc,1,1);
			}
			if(m_dockbar && m_dockbar->IsVert())
				TextOut(ps.hdc,rc.right-4,rc.top+6,m_item[i].m_text,strlen(m_item[i].m_text));
			else
				DrawText(ps.hdc,m_item[i].m_text,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}			
	}
	
	DrawExtButton(ps.hdc);	
	EndPaint(m_hWnd,&ps);	

	return 0;
}

//����ͼ�꣬��С������ԭ���رհ�ť
void RingMenuBar::DrawExtButton(HDC hdc)
{
	if(m_hbmSys && m_bShowIcon)
	{
		HDC hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC,m_hbmSys);
		
		if(m_parent->GetWindowType() == ITIS_MDIFRAME)
		{
			HWND hwnd = ((RingMDIFrameWnd*)m_parent)->GetCurrChildWnd();
			m_sysIcon = (HICON)SendMessage(hwnd,WM_GETICON,ICON_SMALL,0);
		}
		//��ͼ�꣬����Ĭ��ͼ��
		if(m_sysIcon == NULL)
			m_sysIcon = LoadIcon(NULL,IDI_WINLOGO);
		DrawIconEx(hdc,0,0,m_sysIcon,16,16,0,NULL,DI_NORMAL);
		
		RECT rc;

		::GetClientRect(m_hWnd,&rc);
		//����ť�����Ƶ��봰�������Ҷ���
		OffsetRect(&m_rcSysBtn,rc.right - m_rcSysBtn.right,rc.bottom - 3 - m_rcSysBtn.bottom);
		BitBlt(hMemDC,36,0,50,14,hdc,m_rcSysBtn.left,m_rcSysBtn.top,SRCCOPY);

		rc.right = rc.right - rc.left - 34;
		rc.left = rc.right - 16;
		rc.top = m_rcSysBtn.top;
		rc.bottom = m_rcSysBtn.bottom;

		int offx1,offy1,offx2,offy2;

		if(m_dockbar && m_dockbar->IsVert())
		{
			OffsetRect(&rc,34,-34);
			//��С����ť��+2
			BitBlt(hdc,rc.left+2,rc.top+1,12,12,hMemDC,0,0,SRCAND);
			//��ԭ��ť��+16+2
			BitBlt(hdc,rc.left+2,rc.top+17,12,12,hMemDC,12,0,SRCAND);
			//�رհ�ť��+32+2���ð�ť�뻹ԭ��ť���ռ䣩+2
			BitBlt(hdc,rc.left+2,rc.top+35,12,12,hMemDC,24,0,SRCAND);
			
			offx1 = offx2 = 0;
			offy1 = 16;
			offy2 = 18;
		}
		else
		{
			//��С����ť��+2
			BitBlt(hdc,m_rcSysBtn.left+2,m_rcSysBtn.top+1,12,12,hMemDC,0,0,SRCAND);
			//��ԭ��ť��+16+2
			BitBlt(hdc,m_rcSysBtn.left+18,m_rcSysBtn.top+1,12,12,hMemDC,12,0,SRCAND);
			//�رհ�ť��+32+2���ð�ť�뻹ԭ��ť���ռ䣩+2
			BitBlt(hdc,m_rcSysBtn.left+36,m_rcSysBtn.top+1,12,12,hMemDC,24,0,SRCAND);
			offy1 = offy2 = 0;
			offx1 = 16;
			offx2 = 18;
		}
		
		DeleteDC(hMemDC);

		DrawEdge(hdc,&rc,BDR_RAISEDINNER,BF_TOPLEFT);
		DrawEdge(hdc,&rc,EDGE_RAISED,BF_BOTTOMRIGHT);
		OffsetRect(&rc,offx1,offy1);
		DrawEdge(hdc,&rc,BDR_RAISEDINNER,BF_TOPLEFT);
		DrawEdge(hdc,&rc,EDGE_RAISED,BF_BOTTOMRIGHT);
		OffsetRect(&rc,offx2,offy2);
		DrawEdge(hdc,&rc,BDR_RAISEDINNER,BF_TOPLEFT);
		DrawEdge(hdc,&rc,EDGE_RAISED,BF_BOTTOMRIGHT);
	}
}

LRESULT RingMenuBar::OnNotify(RINGPARAMS& param)
{
	//����ҪTOOLTIP
	return DefaultProc(param);
}

//hWnd�Ǹ����ڵ�HWND
BOOL RingMenuBar::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{	
   if(uMsg == MYWM_SIZE)
	{
		Update();
		InvalidateRect(m_hWnd,NULL,TRUE);
		if(IsVert(lParam))
			return m_BarSize.cx;//CalcReSizeV(wParam);
		else
			return m_BarSize.cy;//CalcReSizeH(wParam);
	}
	return FALSE;
}

void RingMenuBar::SetSysControlInfo(BOOL m_bChildMax)
{
	m_bShowIcon = m_bChildMax;

	if(m_rmsys == NULL)
		m_rmsys = new RingMenu;		
	
	if(m_rmsys)
	{
		HWND hwnd = ((RingMDIFrameWnd*)m_parent)->GetCurrChildWnd();
		if(hwnd)
		{
			m_rmsys->Detach();
			m_rmsys->Attach(GetSystemMenu(hwnd,FALSE));
		}
	}
	
	if(m_dockbar)
		m_dockbar->UpdateAllSite();
}


