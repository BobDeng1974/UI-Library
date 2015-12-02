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
//ԭ�ļ�����ringstatus.cpp
//
//���ļ�˵����״̬��ʵ�ִ���
//
**********************************************************************/
#define MAKE_SELF_LIB
//#ifndef __BORLANDC__
#include "ringdows.h"
#include <malloc.h>

#define MIN_PARTWIDTH	6

//RingMsg mg;

RingStatusBar::RingStatusBar()
{
	InitCommonControls();
	m_windowType = ITIS_STATUSBAR;
	m_pane = NULL;
	m_nPartCount = 0;
//mg.SetMsgFile((ringStr(RSTR_CURRPATH,TRUE) + "\\msg.txt").string());
}

RingStatusBar::~RingStatusBar()
{
}

BOOL RingStatusBar::Create(DWORD dwStyle/*=SBARS_SIZEGRIP*/)
{
	if(IsWindow())
		return TRUE;

	//�����ڲ����ǿؼ�
	if(m_parent->IsControls())
		return FALSE;
	
	RingDockSite* pSite;
	
	if(m_parent->GetWindowType() != ITIS_DOCKSITE)
		pSite = ((RingWnd*)m_parent)->CreateDockSite(TBS_FLAG_DOCKBOTTOM);
	else
		pSite = (RingDockSite*)m_parent;
	
	if(pSite == NULL)
		return FALSE;
	
	//ȥ�߿�RingControls::Create�Զ���WS_CHILD | WS_VISIBLE
	dwStyle &= ~(WS_BORDER | CCS_NODIVIDER);
	
	RingControls::Create(dwStyle,0,"",(UINT)m_windowType,0,0,10,10);
	
	m_dockbar = EnableDock(NULL,TBS_EX_NONE|TBS_EX_ALLLINE,TBS_FLAG_DOCKBOTTOM);
	return (BOOL)m_hWnd;
}

LRESULT RingStatusBar::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	switch(param.uMsg)
	{
		case SB_SIMPLE:
			//for MenuHelp
			if(m_pane && (m_pane->m_uStyle & SBPS_STRETCH))
			{
				//�����һ���ǲ���������Ϊ��ʾ�˵���������ʱ��Ҫ����Ƿ����θ���Ϣ
				//�ô�����Ϊ��������VC״̬����Ч������ǰ����ʾ������������ʾ״̬��
				if(param.wParam && param.lParam == 0)
					return 0;//ϵͳ���͵���Ϣ������
			}
			return CallWindowProc(m_DefProc,m_hWnd,SB_SIMPLE,param.wParam,0);
		case SB_SETTEXTA:
		case SB_SETTEXTW:
			//Win2000�ڲ���UNICODE�汾����˴˴�������Ҫ�ж�
			if((param.wParam & 0xFF) == 0xFF)
			{
				//MenuHelp,ϵͳ���͵���Ϣ
				//�����һ���ǲ���������Ϊ��ʾ�˵���������ʱ��Ҫ�ڵ�һ����ʾ�˵�������
				//���ֺ����״̬������������VC״̬����Ч��
				if(m_pane && (m_pane->m_uStyle & SBPS_STRETCH) && !IsSimple())
				{
					if(CallWindowProc(m_DefProc,m_hWnd,param.uMsg,SBT_NOBORDERS,param.lParam))
					{
						InvalidateRect(m_hWnd,NULL,TRUE);
						return TRUE;
					}
					else
						return FALSE;
				}
			}
			return CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
		case SB_SETPARTS:
			if(CallWindowProc(m_DefProc,m_hWnd,SB_SETPARTS,param.wParam,param.lParam))
				m_nPartCount = param.wParam;
			return (BOOL)m_nPartCount;
		case WM_COMMAND:
			return SendMessage(m_parent->Handle(),param.uMsg,param.wParam,param.lParam);		
		case WM_SIZE:
			UpdateAllParts();
			InvalidateRect(m_hWnd,NULL,TRUE);
			return 0;
		case WM_NCDESTROY:
			CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
			FreeAllPart();
			OnNcDestroy();
			return 0;			
	}
	return DefaultProc(param);
}

/**********************************************************
//
//�ָ�������
//������
//		nParts:�ָ�������
//		lpArr:���飬ÿ��Ԫ�ظ�4λָ���ַ�����ԴID����3λָ�����
//				0Ϊ��Ӧ���֣�0xFFFΪ���������ȣ���4λ���4BITָ��
//				������ʾ��ʽ��������Ԫ��ֵ����MAKESBPINT������
//
//********************************************************/
BOOL RingStatusBar::SetParts(int nParts,LPINT lpArr/*=NULL*/)
{
	if(nParts > 0 && nParts < 256)
	{
		int i;
		UINT uId;
		LPINT lpi = lpArr;
		LPRINGSTATUSPANE pane = NULL;
		int nStep = Width() / nParts,nEdge = 0;

		if(lpi == NULL)
		{
			lpi = (LPINT)_alloca(nParts * sizeof(int));
			memset(lpi,0,nParts * sizeof(int));
			*lpi = MAKESBPINT(0,SBTS_NOBORDER,SBPW_STRETCH);
		}		
		
		CreatePaneInfo(nParts);

		for(i=0;i<nParts;i++)
		{
			pane = GetPanePtr(i);
			if(pane)
			{
				uId = lpi[i] >> 16;
				if(uId)
				{
					//������Դ�ַ���
					if(pane->m_str == NULL)
						pane->m_str = new ringStr;
					if(pane->m_str)
						pane->m_str->Load(uId);					
				}
				//�������
				pane->m_nWidth = lpi[i] & 0x0FFF;
				if(pane->m_nWidth > 0 && pane->m_nWidth < SBPW_FITTEXT)					
				{
					//�̶����
					pane->m_uStyle = SBPF_UPDATE | SBPS_STATICW | SBPWSTYLE(lpi[i]);					
				}
				else if(pane->m_nWidth == SBPW_STRETCH)// || lpi[i] == -1)
				{
					//������
					pane->m_uStyle = SBPF_UPDATE | SBPS_STRETCH | SBPWSTYLE(lpi[i]);
				}
				else if(pane->m_nWidth == SBPW_FITTEXT)
				{
					//��Ӧ����
					pane->m_uStyle = SBPF_UPDATE | SBPS_FITTEXT | SBPWSTYLE(lpi[i]);
				}
				else
				{
					//Ĭ�Ϲ̶����
					pane->m_nWidth = nStep;
					pane->m_uStyle = SBPF_UPDATE | SBPS_STATICW;
				}
			}
			else
				return FALSE;
		}		
		UpdateAllParts();
		return TRUE;
	}
	else
		return FALSE;
}

//�������֣�valΪ�ַ�����ԴID
BOOL RingStatusBar::SetValue(int val,int index/*=INDEX_CURSEL*/)
{
	//״̬������ÿ���ַ����127
	char s[160];
	memset(s,0,160);
	if(!LoadString(GetInstance(),val,s,160))
		wsprintf(s,"%d",val);

	return SetValue(s,index);	
}

BOOL RingStatusBar::SetValue(LPCTSTR lpszVal,int index/*=INDEX_CURSEL*/)
{	
	LPRINGSTATUSPANE pane = GetPanePtr(LOWORD(index));
	if(pane)
		return SetPaneText(pane,lpszVal,HIWORD(index));
	else
		return ::SendMessage(m_hWnd,SB_SETTEXT,index,(LPARAM)lpszVal);
}

int RingStatusBar::GetItemText(int index,LPTSTR lpBuf,int size)
{
	//���������뷵���ַ����ȣ������LOWORDȥ��HIWORD������
	return (int)LOWORD(SendMessage(m_hWnd,SB_GETTEXT,index,(LPARAM)lpBuf));
}

BOOL RingStatusBar::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	switch(uMsg)
	{
		case MYWM_SIZE:
			if(IsVert(lParam))
				return WinWidth(m_hWnd);
			else
				return WinHeight(m_hWnd);
/*		case WM_SIZE:
			UpdateAllParts();
			break;
*/
	}
	return FALSE;
}

UINT RingStatusBar::GetPartStyle(int nPart)
{
	LPRINGSTATUSPANE pane = GetPanePtr(nPart);
	if(pane)
		return pane->m_uStyle;
	else
		return 0;
}

int RingStatusBar::GetPartWidth(int nPart)
{
	RECT rc;
	if(GetRect(nPart,&rc))
		return rc.right - rc.left;
	else
		return 0;
}

/*****************************************************
//
//���÷ָ�������
//
//������
//	nPart:�ָ�����ţ���0��ʼ��
//	lpszVal:Ҫ��ʾ������
//	uStyle:���ͣ������-1���򲻸ı�ԭ����
//	nWidth:��ȣ��������ָ��Ϊ��Ӧ���֣�����Ըò���
//
******************************************************/
BOOL RingStatusBar::SetValue(int nPart,LPCTSTR lpszVal,int uStyle/*=-1*/,int nWidth/*=0*/)
{
	LPRINGSTATUSPANE pane = GetPanePtr(nPart);
	if(pane)
	{
		if(uStyle != -1)
			pane->m_uStyle = uStyle;

		//�ж��Ƿ�ı���
		if(pane->m_uStyle == SBPS_STATICW && nWidth > 0)
			pane->m_nWidth = nWidth;
		
		if(pane->m_str == NULL)
			pane->m_str = new ringStr(lpszVal);
		else
			*(pane->m_str) = lpszVal;
		
		pane->m_uStyle |= SBPF_UPDATE;
		UpdateAllParts();
		return TRUE;
	}
	return FALSE;
}

BOOL RingStatusBar::SetPaneText(LPRINGSTATUSPANE lpPart,LPCTSTR lpszVal,UINT uStyle)
{
	if(lpPart)
	{
		//DestroyTimerPart(lpPart);
		//lpPart->m_uStyle |= SPS_TEXT;
		if(lpPart->m_str == NULL)
			lpPart->m_str = new ringStr(lpszVal);
		else
			*(lpPart->m_str) = lpszVal;
		
		lpPart->m_uStyle &= 0xFFFF00FF;
		lpPart->m_uStyle |= uStyle|SBPF_UPDATE;
		if(lpPart->m_uStyle & SBPS_FITTEXT)
			UpdateAllParts();
		else
			return ::SendMessage(m_hWnd,SB_SETTEXT,MAKELONG(lpPart->m_nIndex,uStyle),
										(LPARAM)lpPart->m_str->string());
		return TRUE;
	}
	else
		return FALSE;
}

void RingStatusBar::UpdateAllParts()
{
	HDC hDC;
	int *lpEdge = (int*)_alloca(m_nPartCount * sizeof(int));
	int nRight = Width();
	int i,nEdge=0,nStretchCnt = 0;
	SIZE size;
	LPRINGSTATUSPANE pane;
	int nBorder[3],nMin0 = 0;
	int nBordSize = GetSystemMetrics(SM_CXBORDER);

	GetBorders(nBorder);
	if((GetStyle() & SBARS_SIZEGRIP) && !::IsZoomed(m_parent->Handle()))
	{
		// size grip uses a border + size of scrollbar + cx border
		nRight -= nBorder[0] + GetSystemMetrics(SM_CXVSCROLL) + nBordSize * 2;
	}

	hDC = GetDC(m_hWnd);
	HFONT hf;
	
	hf = (HFONT)SendMessage(m_hWnd,WM_GETFONT,0,0);
	SelectObject(hDC,hf);

	for(i=0;i<m_nPartCount;i++)
	{
		pane = GetPanePtr(i);
		if(pane)
		{
			if(pane->m_child)
			{
				//����Ӵ�����Ч�ԣ������û����ߵ�δ֪ͨ������
				if(!IsWindowPtr(pane->m_child) || pane->m_child->GetParent() != m_hWnd)
				{
					pane->m_child = NULL;
					pane->m_nWidth = MIN_PARTWIDTH;
				}
			}

			if(MASK_MATCH(pane->m_uStyle,SBPS_STRETCH))
				nStretchCnt ++;
			else 
			{
				if(MASK_MATCH(pane->m_uStyle,SBPS_FITTEXT))
				{
					if(MASK_MATCH(pane->m_uStyle,SBPF_UPDATE))
					{						
						if(pane->m_str && !pane->m_str->isClear())
						{							
							GetTextExtentPoint32(hDC,pane->m_str->string(),
														pane->m_str->len(),&size);
							pane->m_nWidth = size.cx + 12 + nBordSize * 2;
							if(GetIcon(i))
								pane->m_nWidth += size.cy + nBordSize * 2;
						}
						else if(pane->m_child)
							pane->m_nWidth = pane->m_child->Width() + nBordSize * 2;
						else
							pane->m_nWidth = MIN_PARTWIDTH;
					}
				}
				nRight -= pane->m_nWidth;
			}
		}
		else
			nRight -= (GetPartWidth(i) + nBorder[2]);
	}	
	ReleaseDC(m_hWnd,hDC);

	int nPrevEdge=nBordSize,x = 0;
	RECT rc;
	GetRect(0,&rc);
	for(i=0;i<m_nPartCount;i++)
	{
		pane = GetPanePtr(i);
		if(pane)
		{
			if(pane->m_uStyle & SBPS_STRETCH)
			{
				if(i == 0)
				{
					//��һ��䳤��Ϊ��ʾ�˵������ã�����1/4��
					nEdge = max(GetSystemMetrics(SM_CXSCREEN)/4,nRight / nStretchCnt);
					x = 1;
					nMin0 = nEdge;					
				}
				else					
					nEdge += max(MIN_PARTWIDTH,(nRight-nMin0) / (nStretchCnt-x));
			}
			else
				nEdge += pane->m_nWidth;
			
			if(pane->m_child)
			{
				if(MASK_MATCH(pane->m_child->GetExStyle(),RTV_FITPARENTSIZE))
					pane->m_child->SetPos(nPrevEdge,rc.top,nEdge-nBordSize-nPrevEdge,rc.bottom-rc.top);
				else
					pane->m_child->SetPos(nPrevEdge,rc.top);
			}
			nPrevEdge = nEdge + nBordSize;

			if(pane->m_uStyle & SBPF_UPDATE)
			{
				pane->m_uStyle &= ~SBPF_UPDATE;
				if(pane->m_str)
					::SendMessage(m_hWnd,SB_SETTEXT,i|(pane->m_uStyle&0xFF00),
										(LPARAM)pane->m_str->string());
			}			
		}
		else
			nEdge += GetPartWidth(i) + nBorder[2];

		lpEdge[i] = nEdge;
	}
	::SendMessage(m_hWnd,SB_SETPARTS,m_nPartCount,(LPARAM)lpEdge);	
}

LPRINGSTATUSPANE RingStatusBar::GetPanePtr(int index)
{
	if(index >=0 && index < m_nPartCount)
		return m_pane?(m_pane + index):NULL;
	else
		return NULL;
}

void RingStatusBar::FreeAllPart()
{
	LPRINGSTATUSPANE pane = m_pane;
	
	for(int i=0;i<m_nPartCount;i++,pane++)
	{
		delete pane->m_str;		
	}
	m_pane = (LPRINGSTATUSPANE)Del(m_pane);
}

BOOL RingStatusBar::CreatePaneInfo(int nParts)
{
	if(nParts > 0 && nParts < 256)
	{
		if(m_pane == NULL)
		{
			m_pane = (LPRINGSTATUSPANE)New(nParts * sizeof(RINGSTATUSPANE));
			m_nPartCount = nParts;
		}
		else if(m_nPartCount != nParts)
		{
			LPRINGSTATUSPANE pane = (LPRINGSTATUSPANE)New(nParts * sizeof(RINGSTATUSPANE));
			if(pane)
			{
				memcpy(pane,m_pane,m_nPartCount * sizeof(RINGSTATUSPANE));
				Del(m_pane);
				m_pane = pane;
				m_nPartCount = nParts;
			}
		}
		else
			return TRUE;

		if(m_pane)
		{
			for(int i=0;i<m_nPartCount;i++)
				m_pane[i].m_nIndex = i;
			return TRUE;
		}
	}
	return FALSE;
}

int RingStatusBar::OnMenuHelp(RINGPARAMS &param,HMENU hMenu)
{
	UINT uIds[] = {MH_BASE,MH_POPUPBASE,0,0};
	MenuHelp(WM_MENUSELECT,param.wParam,param.lParam,hMenu,GetInstance(),m_hWnd,uIds);
	return 0;
}

BOOL RingStatusBar::InsertItem(RingBaseWnd* wnd,int iPart)
{
	if(IsWindowPtr(wnd))
	{
		LPRINGSTATUSPANE pane = GetPanePtr(iPart);
		if(pane && pane->m_child == NULL)
		{
			pane->m_child = wnd;
			pane->m_uStyle |= SBPF_UPDATE;
			wnd->SetParent(m_hWnd);
			UpdateAllParts();
			return TRUE;
		}
	}
	return FALSE;
}

/*
BOOL RingStatusBar::SetTimerFormat(int index,LPCTSTR lpszFormat)
{
	if(lpszFormat == NULL || *lpszFormat == '\0')
		return FALSE;

	LPRINGSTATUSPANE pane = GetPanePtr(index,TRUE);
	if(pane)
	{
		if((pane->m_uStyle & SPS_TIMER) == 0)
		{
			pane->m_uStyle |= SPS_TIMER;
			pane->m_Timer = NULL;
			return SetTimerFormat(pane,lpszFormat);
		}
	}
	return TRUE;
}

BOOL RingStatusBar::SetTimerFormat(LPRINGSTATUSPANE lpPart,LPCTSTR lpszFormat)
{
	if((lpPart->m_uStyle & SPS_TIMER) && lpszFormat)
	{
		if(lpPart->m_str == NULL)
		{
			lpPart->m_str = new ringStr;
			if(lpPart->m_str == NULL)
				return FALSE;
		}
		
		if(lpPart->m_Timer == NULL)
			lpPart->m_Timer = new RingThread;

		if(lpPart->m_Timer)
		{
			lpPart->m_Timer->Wait(TRUE);
			lpPart->m_Timer->Start(this,(THREADFUNC)m_TimerProc,(LONG)lpPart);
			Sleep(200);
			ringStr str1 = lpszFormat;
			ringStr str2;
			str1.replace(str2,"YYYY","%4d",FALSE);
			str2.replace(str1,"MM","%02d",FALSE);
			str1.replace(str2,"DD","%02d",FALSE);
			str2.replace(str1,"hh","%02d",FALSE);
			str1.replace(str2,"mm","%02d",FALSE);
			str2.replace(str1,"ss","%02d",FALSE);
			str1.replace(str2,"M","%d",FALSE);
			str2.replace(str1,"D","%d",FALSE);
			str1.replace(str2,"h","%d",FALSE);
			str2.replace(str1,"m","%d",FALSE);
			str1.replace(str2,"s","%d",FALSE);
			*(lpPart->m_str) = str1;
			lpPart->m_Timer->Continue();
			return TRUE;
		}		
	}
	return FALSE;
}

BOOL RingStatusBar::SetProgress(int index,int nStep)
{
	return TRUE;
}

void RingStatusBar::DestroyTimerPart(LPRINGSTATUSPANE lpPart)
{
	if(lpPart && (lpPart->m_uStyle & SPS_TIMER))
	{
		try
		{
			delete lpPart->m_Timer;
		}
		catch(...)
		{
		}
		lpPart->m_Timer = NULL;
		lpPart->m_uStyle &= ~SPS_TIMER;
	}
}

DWORD RingStatusBar::m_TimerProc(LPRINGSTATUSPANE lpPart)
{
	return 0x57a7;
}
*/
