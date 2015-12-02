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
//ԭ�ļ�����ringchecklist.cpp
//
//˵������ѡ����ListBox�ؼ�����
//
**********************************************************************/
#define MAKE_SELF_LIB
#define OEMRESOURCE
#include "ringdows.h"

RingCheckList::RingCheckList()
{
	m_hbmp = NULL;
}

RingCheckList::~RingCheckList()
{
	if(m_hbmp)
		DeleteObject(m_hbmp);
}

BOOL RingCheckList::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height)
{
	dwStyle |= (LBS_HASSTRINGS|LBS_OWNERDRAWFIXED|LBS_NOTIFY|WS_VSCROLL);
	
	if(RingControls::Create(dwStyle,dwExStyle,szInitText,uId,x,y,width,height))
	{
		RingControls::SetOwnerDraw(FALSE,(UINT)0xFFFFFFFF);
		if(m_hbmp == NULL)
			CreateCheckBmp(NULL,GetItemHeight());
		return TRUE;
	}
	return FALSE;
}
	
BOOL RingCheckList::Create(UINT uId,int x/*=0*/,int y/*=0*/,int width/*=-999*/,int height/*=-999*/)
{
	DWORD dwExStyle = 0;
	if(width == -999 || height == -999)
		dwExStyle = RTV_FITPARENTSIZE;
	
	return Create(LBS_HASSTRINGS|LBS_OWNERDRAWFIXED|LBS_NOTIFY|WS_VSCROLL,
						dwExStyle,"",uId,x,y,width,height);
}

void RingCheckList::CreateCheckBmp(HDC hDC,int nSize)
{
	if(m_hbmp)
		DeleteObject(m_hbmp);

	BOOL bDelDC = FALSE;
	if(hDC == NULL)
	{
		hDC = GetDC(m_hWnd);
		bDelDC = TRUE;
	}
	m_nCheckSize = nSize;
	m_hbmp = CreateCompatibleBitmap(hDC,nSize*2,nSize);
	
	HBITMAP hbmCheck = LoadBitmap(NULL,(LPTSTR)OBM_CHECK);
	BITMAP bm;	
	HDC hDstDC = CreateCompatibleDC(hDC);
	HDC hSrcDC = CreateCompatibleDC(hDC);
	
	GetObject(hbmCheck,sizeof(BITMAP),&bm);
	
	SelectObject(hSrcDC,hbmCheck);
	SelectObject(hDstDC,m_hbmp);
	
	BitBlt(hDstDC,0,0,nSize*2,nSize,hSrcDC,0,0,WHITENESS);	
	Rectangle(hDstDC,2,2,nSize - 2,nSize - 2);
	Rectangle(hDstDC,nSize + 2,2,nSize*2 - 2,nSize - 2);

	BitBlt(hDstDC,nSize + 3,2,nSize,nSize,hSrcDC,0,0,SRCAND);
	
	DeleteDC(hSrcDC);
	DeleteDC(hDstDC);
	DeleteObject(hbmCheck);
	if(bDelDC)
		ReleaseDC(m_hWnd,hDC);
}

BOOL RingCheckList::Attach(HWND hWnd)
{
	//��ΪListBoxһ��������Style�Ͳ��ɸı䣬�����������Style
	//��������Ի�����Ҫ��DestroyȻ����Create	
	DWORD style = GetWindowLong(hWnd,GWL_STYLE);
	style &= ~(LBS_SORT|LBS_MULTIPLESEL|LBS_MULTICOLUMN);
	DWORD st = LBS_HASSTRINGS|LBS_OWNERDRAWFIXED|LBS_NOTIFY;
	if((style & st) != st)
	{
		RECT rc;
		HWND hwnd;
		
		style |=	(st|WS_VSCROLL);
		
		DWORD exstyle = GetWindowLong(hWnd,GWL_EXSTYLE);
		
		GetWindowRect(hWnd,&rc);
		hwnd = ::GetParent(hWnd);
		
		ScreenToClient(hwnd,(LPPOINT)&rc);
		ScreenToClient(hwnd,(LPPOINT)&rc+1);
		
		m_ID = GetWindowLong(hWnd,GWL_ID);
		
		DestroyWindow(hWnd);
		
		if(RingControls::Create(style,exstyle,"",m_ID,
			rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top))
		{
			RingControls::SetOwnerDraw(FALSE,(UINT)0xFFFFFFFF);
			if(m_hbmp == NULL)
				CreateCheckBmp(NULL,GetItemHeight());
			return TRUE;
		}
	}
	else
		if(RingBaseWnd::Attach(hWnd))
		{
			RingControls::SetOwnerDraw(FALSE,(UINT)0xFFFFFFFF);
			if(m_hbmp == NULL)
				CreateCheckBmp(NULL,GetItemHeight());
			return TRUE;
		}
		
	return FALSE;
}

BOOL RingCheckList::OnDrawItem(LPDRAWITEMSTRUCT lpds)
{
	if(lpds->itemAction & ODA_FOCUS)
		return TRUE;

	RECT rc;	
	LPRINGCHECKLISTDATA lpData = (LPRINGCHECKLISTDATA)lpds->itemData;
	

	CopyRect(&rc,&lpds->rcItem);
	rc.left += m_nCheckSize;
	
	int ndc = SaveDC(lpds->hDC);
	
	SelectObject(lpds->hDC,m_brush);
	HDC hDC = CreateCompatibleDC(lpds->hDC);

	SelectObject(hDC,m_hbmp);

	if(lpData && lpds->itemData != LB_ERR)
		if(lpData->m_bChecked)
			BitBlt(lpds->hDC,0,rc.top,m_nCheckSize,m_nCheckSize,hDC,m_nCheckSize,0,SRCCOPY);
		else
			BitBlt(lpds->hDC,0,rc.top,m_nCheckSize,m_nCheckSize,hDC,0,0,SRCCOPY);

	if((lpds->itemState & ODS_SELECTED))
	{		
		::SetBkColor(lpds->hDC,GetSysColor(COLOR_HIGHLIGHT));
		::SetTextColor(lpds->hDC,GetSysColor(COLOR_HIGHLIGHTTEXT));
		ExtTextOut(lpds->hDC,0,0,ETO_OPAQUE,&rc,NULL,0,NULL);
	}
	else
		FillRect(lpds->hDC,&rc,m_brush);
	
	DrawText(lpds->hDC,GetText(lpds->itemID),GetTextLen(lpds->itemID),&rc,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DeleteDC(hDC);
	RestoreDC(lpds->hDC,ndc);
	
	return TRUE;
}

//RingdowProc�Ѷ�LB_GETITEMDATA��LB_SETITEMDATA���д���ʹ�û�����ֻ�ܼӵ�
//LPRINGCHECKLISTDATA��m_data�ֶΣ�������ṩԭGetData��SetData����Ϊ������
//ʹLPRINGCHECKLISTDATA�ܰ󶨵��б�����
LPRINGCHECKLISTDATA RingCheckList::GetPriData(int nIndex)
{
	LONG lpData = CallWindowProc(m_DefProc,m_hWnd,LB_GETITEMDATA,nIndex,0);
	if(lpData != LB_ERR)
		return (LPRINGCHECKLISTDATA)lpData;
	else
		return NULL;
}

BOOL RingCheckList::SetPriData(int nIndex,LPRINGCHECKLISTDATA lpData)
{
	if(lpData)
		return (BOOL)CallWindowProc(m_DefProc,m_hWnd,LB_SETITEMDATA,nIndex,(LPARAM)lpData);
	else
		return FALSE;
}

//���ѡ��״̬
BOOL RingCheckList::IsItemChecked(int nIndex)
{
	if(nIndex == -1)
		nIndex = GetCurSel();

	LPRINGCHECKLISTDATA lpData = GetPriData(nIndex);
	if(lpData)
	{
		try
		{
			return lpData->m_bChecked;
		}
		catch(...)
		{
		}		
	}
	return FALSE;
}

//�л�ѡ��/��ѡ��״̬��ToggleItemCheck������״̬��SetItemCheck����ԭ״̬
BOOL RingCheckList::ToggleItemCheck(int nIndex)
{
	BOOL bRet = FALSE;

	if(nIndex == -1)
		nIndex = GetCurSel();
	
	LPRINGCHECKLISTDATA lpData = GetPriData(nIndex);
	if(lpData)
	{
		try
		{
			bRet = !lpData->m_bChecked;
			lpData->m_bChecked = bRet;
			SetPriData(nIndex,lpData);
		}
		catch(...)
		{
		}
	}
	return bRet;
}

BOOL RingCheckList::SetItemCheck(int nIndex,BOOL bCheck)
{
	BOOL bRet = FALSE;
	
	if(nIndex == -1)
		nIndex = GetCurSel();
	
	LPRINGCHECKLISTDATA lpData = GetPriData(nIndex);
	if(lpData)
	{
		try
		{
			bRet = lpData->m_bChecked;
			lpData->m_bChecked = bCheck;
			SetPriData(nIndex,lpData);
		}
		catch(...)
		{
		}
	}
	return bRet;
}

LRESULT RingCheckList::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	switch(param.uMsg)
	{
		case WM_LBUTTONDOWN:
			if(param.mousept.y < GetCount() * GetItemHeight() && 
				param.mousept.x > 0 && param.mousept.x < m_nCheckSize)
			{
				int index = SendMessage(m_hWnd,LB_ITEMFROMPOINT,0,param.lParam);				
				RECT rc;
				ToggleItemCheck(index);				
				GetItemRect(index,&rc);
				InvalidateRect(m_hWnd,&rc,TRUE);
				return 0;
			}
			break;
		case WM_VSCROLL:
		case (WM_MOUSELAST+1)://=WM_MOUSEWHEEL
		{
			LRESULT res = RingControls::RingdowProc(hWnd,param);
			if(param.uMsg == (WM_MOUSELAST+1) || 
				(param.uMsg == WM_VSCROLL && param.wParam == SB_ENDSCROLL))
				InvalidateRect(m_hWnd,NULL,TRUE);
			return res;
		}
		case LB_ADDSTRING:
		case LB_INSERTSTRING:
		{
			LRESULT res = RingControls::RingdowProc(hWnd,param);
			
			if(res != LB_ERR && res != LB_ERRSPACE)
			{
				LPRINGCHECKLISTDATA lpData = (LPRINGCHECKLISTDATA)New(sizeof(RINGCHECKLISTDATA));
				if(lpData)
				{
					lpData->m_bChecked =	param.wParam;
					SetPriData(res,lpData);					
				}				
			}
			return res;
		}
		case LB_DELETESTRING:
		{			
			OnDelString(param.wParam);
			break;
		}
		case LB_RESETCONTENT:
		{
			int n = GetCount();
			for(int i=0;i<n;i++)
				OnDelString(i);
			break;
		}
		case LB_SETITEMDATA:
		{	
			LONG lpData = CallWindowProc(m_DefProc,m_hWnd,LB_GETITEMDATA,param.wParam,0);
			if(lpData != LB_ERR)
				if(lpData == 0)
					break;
				else
				{
					try
					{
						((LPRINGCHECKLISTDATA)lpData)->m_data = param.lParam;
						return TRUE;
					}
					catch(...)
					{						
					}
				}
			return LB_ERR;
		}
		case LB_GETITEMDATA:
		{	
			LONG lpData = CallWindowProc(m_DefProc,m_hWnd,LB_GETITEMDATA,param.wParam,0);
			if(lpData != LB_ERR)
			{
				if(lpData == 0)
					return 0;
				else
				{
					try
					{
						return ((LPRINGCHECKLISTDATA)lpData)->m_data;
					}
					catch(...)
					{
					}
				}
			}
			return LB_ERR;
		}
		//case WM_ERASEBKGND:
		case WM_WINDOWPOSCHANGED:
			return 0;
	}
	return RingControls::RingdowProc(hWnd,param);
}

//ɾ������������RINGCHECKLISTDATA�������û����õ����ݣ����򽫸�����
//�滻RINGCHECKLISTDATA���ͷ�RINGCHECKLISTDATA���������غ�ִ��Ĭ�ϵ�ɾ��
//�������
void RingCheckList::OnDelString(int index)
{
	LONG res = CallWindowProc(m_DefProc,m_hWnd,LB_GETITEMDATA,index,0);
	if(res != LB_ERR)
		if(res != 0)
		{
			LPRINGCHECKLISTDATA lpData = (LPRINGCHECKLISTDATA)res;
			if(lpData)
			try
			{
				if(lpData->m_data)
				{
					CallWindowProc(m_DefProc,m_hWnd,LB_SETITEMDATA,index,lpData->m_data);
					Del(lpData);
				}
			}
			catch(...)
			{
			}
		}
}

LRESULT RingCheckList::OnDestroy(RINGPARAMS& param)
{
	Clear();
	return DefaultProc(param);
}


