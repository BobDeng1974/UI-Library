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
//���ļ�˵����������ʵ�ִ��룬������RingToolBar��RingCoolBar
//				  RingToolBar:��ͨ�������������ƶ�����϶�
//				  RingCoolBar:����IE�Ĺ������������϶����������ɶ��
//								  ��������ؼ���֧�����䷶Χ�ڵ���λ�á�
//
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdows.h"

RingToolBar::RingToolBar()
{
	InitCommonControls();

	m_windowType = ITIS_TOOLBAR;
	m_btnSize.cx = m_btnSize.cy = 0;
	m_BarSize.cx = m_BarSize.cy = 0;
	m_fSize.cx = m_fSize.cy = 0;
	m_nMaxRow = 0;
	m_nfRows = 1;
	m_dockbar = NULL;
	m_img = m_imgHot = NULL;
}

RingToolBar::~RingToolBar()
{
	if(m_img)
		ImageList_Destroy(m_img);
	if(m_imgHot)
		ImageList_Destroy(m_imgHot);
}

void RingToolBar::ReplaceStyle(DWORD dwsRemove,DWORD dwsReplace)
{
	DWORD style = GetStyle();
   style &= ~dwsRemove;
   style |= dwsReplace;
   SetStyle(style);
}

RingDockBar* RingToolBar::EnableDock(LPCTSTR szTitle,UINT uDockStyle,UINT uDockFlag,LPRECT lprcPos)
{
	DWORD style = CCS_NORESIZE;
	if(!MASK_MATCH(uDockStyle,TBS_EX_NONE))
		style |= TBSTYLE_FLAT;
	ReplaceStyle(0,style);
	//����Ƿ�FLAT���ͣ����˻����пռ��Ա���ƺ��ߣ�
	//�������һ��SetButtonSize�Ż�ȡ���ÿռ�
	DWORD dw = GetButtonSize();
	SetButtonSize(LOWORD(dw),HIWORD(dw));
	return RingBaseWnd::EnableDock(szTitle,uDockStyle,uDockFlag,lprcPos);
}

///////////////////////////////////////////////////////////////////////
//
//����RingControls��Create��szInitText����Ϊͼ����Դ������ԴͼƬ
//�����ڱ�ģ����Դ�л���BMP�ļ������Ұ�ťͼƬ������һ�½�������
//��һ�л���16*15��׼��TOOLBAR��ťͼƬ��ͬ��Դ�༭���������ɵ�ͼƬ����
//��������16ɫ����ͼƬ0,0λ�����ر�����͸��ɫ
//
///////////////////////////////////////////////////////////////////////
BOOL RingToolBar::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,
								 int x,int y,int width,int height)
{
	if(IsWindow())
		return TRUE;

	//�����ڲ����ǿؼ�
	if(m_parent->IsControls())
		return FALSE;

	RingDockSite* pSite;
		
	if(m_parent->GetWindowType() != ITIS_DOCKSITE)
		pSite = ((RingWnd*)m_parent)->CreateDockSite(TBS_FLAG_DOCKTOP);
	else
		pSite = (RingDockSite*)m_parent;

	if(pSite == NULL)
		return FALSE;

	if(dwStyle & TBSTYLE_FLAT)
		dwStyle |= CCS_NORESIZE;

	dwStyle |= WS_CHILD | WS_VISIBLE | TBSTYLE_BUTTON | WS_CLIPCHILDREN | 
		WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | CCS_NODIVIDER | CCS_NOMOVEY;

	RingControls::Create(dwStyle,0,"",uId,x,y,width,height);
	//����WM_NOTIFY��Ϣ���մ���ΪpSite
	SendMessage(m_hWnd,TB_SETPARENT,(WPARAM)pSite->Handle(),0);
	SendMessage(m_hWnd,TB_BUTTONSTRUCTSIZE,sizeof(TBBUTTON),0);
	//������IMAGELIST
	SendMessage(m_hWnd,CCM_SETVERSION,5,0);
	SetExStyle(dwExStyle);

	if(szInitText)
	{
		HBITMAP hbm;
		if(IS_INTRESOURCE(szInitText))
			hbm = (HBITMAP)LoadBitmap(GetInstance(),szInitText);
		else
			hbm = (HBITMAP)LoadImage(GetInstance(),szInitText,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		if(hbm)
		{
			SIZE size;
			int ncnt;

			GetBmpSize(hbm,&size);
			ncnt = size.cx;

			if(size.cy != 15)
				size.cx = size.cy;
			else
				size.cx = 16;
			
			m_img = ImageList_Create(size.cx,size.cy,ILC_COLOR32|ILC_MASK,ncnt/size.cx,4);
			ImageList_AddMasked(m_img,hbm,CLR_DEFAULT);

			DeleteObject(hbm);
			SetImageList(m_img);
			SetButtonSize(size.cx,size.cy);			
		}		
	}
	m_dockbar = RingBaseWnd::EnableDock(NULL,TBS_EX_NONE|TBS_EX_ALLLINE);
	//����ʱΪ���й�����������Ƿ�FLAT���ͣ���Ҫ��ȡ��CCS_NORESIZE���ͣ�
	//EnableDockʱdockbar��ȡ����ȷ�߶Ⱥ��������Ա��ܿ��ƹ�����λ��
	ReplaceStyle(0,CCS_NORESIZE);
	return (BOOL)m_hWnd;
}

//idBitmap����ͼ��ֻ���ڱ�ģ����Դ��
BOOL RingToolBar::Create(int idBitmap,DWORD dwStyle/*=0*/,DWORD dwExStyle/*=0*/)
{
   if(idBitmap > 0)
		return Create(dwStyle,dwExStyle,MAKEINTRESOURCE(idBitmap),gdwDockBarId++,0,0,0,0);
	else
		return Create(dwStyle,dwExStyle,NULL,gdwDockBarId++,0,0,0,0);
}

void RingToolBar::SetExStyle(DWORD dwExStyle)
{
	if(dwExStyle != 0 && m_hWnd)
	{
		//Toolbarר����չ���Ա��������·������ã�
		DWORD dwStyle = SendMessage(m_hWnd,TB_GETEXTENDEDSTYLE,0,0);
		dwStyle |= dwExStyle;
		SendMessage(m_hWnd,TB_SETEXTENDEDSTYLE,0,dwStyle);
	}
}

/*******************************************************
//
//LoadToolbar:	���빤������Դ����Ϊ��Ҫ֪����Դ��ʽ��
//					���´��벿��ժ��MFC
//
*******************************************************/
struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	//WORD aItems[wItemCount]
	
	WORD* items()
	{ return (WORD*)(this+1); }
};

BOOL RingToolBar::LoadToolBar(UINT uid,DWORD dwStyle,DWORD dwExStyle,HINSTANCE hInst)
{
	return LoadToolBar(MAKEINTRESOURCE(uid),dwStyle,dwExStyle,hInst);
}

BOOL RingToolBar::LoadToolBar(LPCTSTR lpszResourceName,DWORD dwStyle,DWORD dwExStyle,HINSTANCE hInst)
{
	if(lpszResourceName == NULL)
		return FALSE;
	
	if(m_hWnd == NULL)
		if(!Create(0,dwStyle,dwExStyle))
			return FALSE;
		
	HRSRC hRsrc = FindResource(hInst,lpszResourceName,RT_TOOLBAR);
	if(hRsrc == NULL)
		return FALSE;
	
	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if(hGlobal == NULL)
		return FALSE;
	
	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if(pData == NULL || pData->wVersion != 1)
		return FALSE;
	
	BOOL bResult = FALSE;
	HBITMAP hbm = LoadBitmap(hInst,lpszResourceName);
	if(hbm)
	{
		bResult = TRUE;
		AddImage(hbm,pData->wWidth,pData->wHeight);
		DeleteObject(hbm);
	}

	DeleteAllButtons();

	TBBUTTON tb;
	memset(&tb,0,sizeof(TBBUTTON));
	tb.iString = -1;
	int iImg = 0;

	for(int i = 0; i < pData->wItemCount; i++)
	{
		tb.fsState = TBSTATE_ENABLED;
		if((tb.idCommand = pData->items()[i]) == 0)
		{
			// separator
			tb.fsStyle = TBSTYLE_SEP;
			// width of separator includes 8 pixel overlap
			
			if ((GetStyle() & TBSTYLE_FLAT))
				tb.iBitmap = 6;
			else
				tb.iBitmap = 8;
		}
		else
		{
			// a command button with image
			tb.fsStyle = TBSTYLE_BUTTON;
			tb.iBitmap = iImg ++;
		}
		if(!SendMessage(m_hWnd,TB_ADDBUTTONS,1,(LPARAM)&tb))
			return FALSE;
	}
	
	SetButtonSize(pData->wWidth + 7, pData->wHeight + 7);
	
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	Update(pData->wItemCount);

	return bResult;
}

//ɾ�����а�ť
void RingToolBar::DeleteAllButtons()
{
	int nCount = SendMessage(m_hWnd,TB_BUTTONCOUNT,0,0);
	while(nCount--)
		SendMessage(m_hWnd,TB_DELETEBUTTON,0,0);
}

///////////////////////////////////////////////////
//
//��������ð�ťͼƬ��δ���ã�����m_img�����룬�����ã�
//���뵽m_img
//
///////////////////////////////////////////////////
int RingToolBar::AddImage(UINT id,int cx/*=16*/,int cy/*=15*/,
									COLORREF crMask/*=CLR_DEFAULT*/,
									HINSTANCE hInst/*=GetInstance()*/)
{
	HBITMAP hbm = LoadBitmap(hInst,MAKEINTRESOURCE(id));
	int nIdx = AddImage(hbm,cx,cy,crMask);
	DeleteObject(hbm);
	return nIdx;
}

int RingToolBar::AddImage(HBITMAP hbm,int cx/*=15*/,int cy/*=16*/,
									COLORREF crMask/*=CLR_DEFAULT*/)
{
	return AddInterImage(hbm,cx,cy,crMask,m_img,TB_GETIMAGELIST,TB_SETIMAGELIST,TRUE);
}

int RingToolBar::AddHotImage(UINT id,int cx/*=16*/,int cy/*=15*/,
									  COLORREF crMask/*=CLR_DEFAULT*/,
									  HINSTANCE hInst/*=GetInstance()*/)
{
	HBITMAP hbm = LoadBitmap(hInst,MAKEINTRESOURCE(id));
	int nIdx = AddHotImage(hbm,cx,cy,crMask);
	DeleteObject(hbm);
	return nIdx;
}

int RingToolBar::AddHotImage(HBITMAP hbm,int cx/*=15*/,int cy/*=16*/,
								  COLORREF crMask/*=CLR_DEFAULT*/)
{
	return AddInterImage(hbm,cx,cy,crMask,m_imgHot,TB_GETHOTIMAGELIST,TB_SETHOTIMAGELIST,FALSE);
}

int RingToolBar::AddInterImage(HBITMAP hbm,int cx,int cy,COLORREF crMask,
										 HIMAGELIST& imgInter,UINT uMsgGet,UINT uMsgSet,
										 BOOL bSetButtonSize)
{
	int nIdx = -1;
	if(hbm)
	{
		HIMAGELIST himl = (HIMAGELIST)::SendMessage(m_hWnd,uMsgGet,0,0);
		
		if(himl)	  //�Ѵ��ڲ�������
			nIdx = ImageList_AddMasked(himl,hbm,crMask);
		else
		{
			SIZE size;
			int ncnt;
			GetBmpSize(hbm,&size);
			ncnt = size.cx/cx;
			
			if(bSetButtonSize)
				SetButtonSize(cx,cy);

			if(imgInter == NULL)
				imgInter = ImageList_Create(cx,cy,ILC_COLOR32|ILC_MASK,ncnt,4);
			//imgInter(m_img��m_imgHot�п����Ѵ�����δ���ý�TOOLBAR��������ﲻ����else
			if(imgInter)
			{
				nIdx = ImageList_AddMasked(imgInter,hbm,crMask);
				himl = imgInter;
			}
		}
		::SendMessage(m_hWnd,uMsgSet,0,(LPARAM)himl);
	}
	return nIdx;
}

BOOL RingToolBar::AddButtons(int num,LPCTBBUTTON lptb)
{
	if(lptb == NULL)
		return FALSE;

	int cnt = GetButtonCount();
	
	TBBUTTON tb = *lptb;

	if(m_dockbar && m_dockbar->IsVert() && lptb)
		tb.fsState |= TBSTATE_WRAP;
	
	BOOL bOK = SendMessage(m_hWnd,TB_ADDBUTTONS,num,(LPARAM)&tb);
   Update(cnt);

   return bOK;
}

BOOL RingToolBar::InsertButton(int index,LPCTBBUTTON lptb)
{
	if(lptb == NULL)
		return FALSE;

	int cnt = GetButtonCount();

	TBBUTTON tb = *lptb;

	if(m_dockbar && m_dockbar->IsVert() && lptb)
		tb.fsState |= TBSTATE_WRAP;

	BOOL bOk = SendMessage(m_hWnd,TB_INSERTBUTTON,index,(LPARAM)&tb);
   Update(cnt);
   return bOk;
}

BOOL RingToolBar::InsertButton(int index,int iBitmap,int idCmd,
										 LPCTSTR szText/*=NULL*/,BYTE btStyle,
										 BYTE btState,DWORD dwData)
{
   int iString = -1;
	if(szText != NULL)
		iString = SendMessage(m_hWnd,TB_ADDSTRING,0,(LPARAM)szText);

	TBBUTTON tbButton;
   tbButton.iBitmap = iBitmap;
   tbButton.idCommand = idCmd;
   tbButton.fsState = btState;
   tbButton.fsStyle = (BYTE)(TBSTYLE_BUTTON | btStyle);
   tbButton.dwData = dwData;
   tbButton.iString = iString;

	if(m_dockbar && m_dockbar->IsVert())
		tbButton.fsState |= TBSTATE_WRAP;

	int cnt = GetButtonCount();
	BOOL bOk = SendMessage(m_hWnd,TB_INSERTBUTTON,index,(LPARAM)&tbButton);

	Update(cnt);
   return bOk;
}

BOOL RingToolBar::InsertSep(int index,int width/*=0*/,int id/*=0*/)
{
   int cnt = GetButtonCount();
	TBBUTTON tbButtons = {width,id,TBSTATE_ENABLED,TBSTYLE_SEP,{0,0},0,0};
	BOOL bOk;

	if(m_dockbar && m_dockbar->IsVert())
		tbButtons.fsState |= TBSTATE_WRAP;
	bOk = SendMessage(m_hWnd,TB_INSERTBUTTON,index,(LPARAM)&tbButtons);
   Update(cnt);
   return bOk;
}

RingControls* RingToolBar::InsertItem(int index,RingControls* item,DWORD dwStyle,UINT uId,int width,int height)
{
   if(item)
   {
      RECT rt;

		item->SetParentObj(this);
		
		if(item->Handle() == NULL)
			item->Create(dwStyle,0,"",uId,0,0,width,height);
		
		if(uId == 0)
			uId = item->GetId();
		
		GetWindowRect(item->Handle(),&rt);
		width = max((rt.right-rt.left),width);
		height = max((rt.bottom-rt.top),height);

		InsertButton(index,-1,uId,NULL,TBSTYLE_BUTTON,TBSTATE_INDETERMINATE);
		TBBUTTONINFO tb;
		tb.cbSize = sizeof(TBBUTTONINFO);
		tb.dwMask = TBIF_SIZE;
		tb.cx = width + 4;
		SendMessage(m_hWnd,TB_SETBUTTONINFO,uId,(LPARAM)&tb);
		//InsertSep(index,width + 4,uId);
		GetButtonRect(index,&rt);

		item->SetParent(m_hWnd);
		item->SetPos(rt.left + 2,rt.top,width,height);		
   }
   return item;
}

BOOL RingToolBar::InsertText(int index,LPCTSTR szText)
{
   if(szText == NULL)
		return FALSE;

	RingStatic *item = NewRingObj(itemof(RingStatic));
	UINT uId = gdwDockBarId++;

	item->Create(SS_CENTERIMAGE,WS_EX_TRANSPARENT,0,uId,0,0,0,0);

	if(item && szText)
   {
      item->SetValue(szText);
		item->SizeFitText();
		item->SetBkColor(CLR_INVALID);
		
		return (BOOL)InsertItem(index,item,0,0,0,HIWORD(GetButtonSize()));
	}
	return FALSE;	
}

RingControls* RingToolBar::GetContrlolItem(UINT uId)
{
	HWND hwnd = GetDlgItem(m_hWnd,uId);
	if(hwnd)
	{
		RingBaseWnd* obj = GetWindowPtr(hwnd);
		if(obj && obj->IsControls())
			return (RingControls*)obj;
	}
	return NULL;
}

LRESULT RingToolBar::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	LRESULT res;
	switch(param.uMsg)
	{
		case WM_ERASEBKGND:
			if(GetStyle() & TBSTYLE_CUSTOMERASE)	//CUSTOMERASE��Ĭ�ϴ��������б�������
				break;
			if(m_dockbar && m_dockbar->DrawBkg(param.hdc))
				return TRUE;
			break;
		case WM_SIZING:
			if(m_dockbar)
				return OnSizing(param);
		case WM_PAINT:
			res = DefaultProc(param);
			{
				//���������Ӵ��ڡ�InsertText�����Static�ؼ���͸���ģ�
				//��������£���Static�ؼ����ֲ�����������Ϣ�������Ḳ�Ǹÿؼ�
				//��Static�ؼ���WM_PAINT��Ϣȴ����ȫ�����£�����������ʧ��
				HWND hwnd = GetChild();
				while(hwnd)
				{
					InvalidateRect(hwnd,NULL,TRUE);
					hwnd = GetNextChild(hwnd);
				}
			}
			return res;
		/*case TB_SETBUTTONSIZE:
		{
			LRESULT res = DefaultProc(param);
			Update(GetButtonCount());
			if(m_dockbar)
				m_dockbar->UpdateAllSite();
			return res;
		}*/
		default:
		{
			LRESULT res = CtlColorProc(m_hWnd,param);
			if(res != RM_NOTPROCESS)
				return res;
			break;
		}
/*
		case WM_GETDRAGRECTS:
			return OnGETDRAGRECTS(hWnd,param);
//*/
	}
	return RingControls::RingdowProc(hWnd,param);
}

//���㹤�������ź����ŵĳߴ�
void RingToolBar::CalcSize(LPSIZE lpsize/*=NULL*/)
{
	RECT rc;
	int bw;
	int n = GetButtonCount();
	TBBUTTON tb;
	//��������ť�б䶯�����������0����WM_SIZING�����¼���
	m_nMaxRow = 0;

	m_btnSize.cx = 0;
	m_BarSize.cx = m_BarSize.cy = 0;
	
	m_btnSize.cy = HIWORD(GetButtonSize());

	for(int i=0;i<n;i++)
	{
		GetButton(i,&tb);

		if(tb.fsState & TBSTATE_HIDDEN)
			continue;

		if(GetButtonRect(i,&rc))
		{
			bw = rc.right - rc.left;
			if(bw > m_btnSize.cx)
				m_btnSize.cx = bw;
			m_BarSize.cx += bw;			
		}

		if((tb.fsState & TBSTATE_INDETERMINATE) || (tb.fsStyle & TBSTYLE_SEP))
		{
			if(tb.idCommand !=0)
			{
				HWND hwnd = GetDlgItem(m_hWnd,tb.idCommand);
				if(hwnd && hwnd != m_hWnd)
					SetWindowPos(hwnd,0,rc.left,rc.top,0,0,SWP_NOSIZE);
				m_BarSize.cy += m_btnSize.cy;
			}
			else
				m_BarSize.cy += bw;
		}
		else
			m_BarSize.cy += m_btnSize.cy;
	}	

	if(lpsize)
	{
		lpsize->cx = m_BarSize.cx;
		lpsize->cy = m_BarSize.cy;
	}
}

void RingToolBar::UpdateItems()
{
	RECT rc;
	int n = GetButtonCount();
	TBBUTTON tb;
	
	for(int i=0;i<n;i++)
	{
		GetButton(i,&tb);
		
		if(tb.fsState & TBSTATE_HIDDEN)
			continue;
		
		if(tb.fsState & TBSTATE_INDETERMINATE)
		{
			if(tb.idCommand !=0)
			{
				GetButtonRect(i,&rc);
				HWND hwnd = GetDlgItem(m_hWnd,tb.idCommand);
				if(hwnd && hwnd != m_hWnd)
					SetWindowPos(hwnd,0,rc.left,rc.top,0,0,SWP_NOSIZE);
			}
		}
	}	
}

void RingToolBar::Update(int nOrgButtonCnt,LPRECT lprc/*=NULL*/,
								 BOOL bUpdateFloatRect/*=TRUE*/,BOOL bSetPos/*=TRUE*/)
{
	RECT rc;
	
	CalcSize();
	
	if(lprc)
		CopyRect(&rc,lprc);
	else
	{		
		SendMessage(m_hWnd,TB_AUTOSIZE,0,0);
		GetWindowRect(m_hWnd,&rc);
		OffsetRect(&rc,-rc.left,-rc.top);
	}
		
	if(m_dockbar && m_dockbar->IsVert())
	{
		WrapToolBar(nOrgButtonCnt+2,&rc);		
		m_BarSize.cy = rc.bottom - rc.top;
		if(m_dockbar->IsAllLineBar())
			SetPos(0,0,rc.right - rc.left,m_dockbar->GetClientHeight(0),NULL,SWP_NOMOVE);
	}
	else
	{
		SendMessage(m_hWnd,TB_SETROWS,MAKELONG(1,TRUE),(LPARAM)&rc);
		UpdateItems();
		
		//bSetPos:��ʼ����ʱΪĬ��ʽ������߶���Ҫ���ƺã��Ժ󸸴����϶�ʱ�Ĵ���������Ҫ
		if(bSetPos)
		{
			if(m_dockbar && m_dockbar->IsAllLineBar())//GetStyle() & TBSTYLE_FLAT)
				SetPos(0,0,m_BarSize.cx,m_btnSize.cy,NULL,SWP_NOMOVE|SWP_FRAMECHANGED);
			else
				SetPos(0,0,m_BarSize.cx,rc.bottom - rc.top,NULL,SWP_NOMOVE|SWP_FRAMECHANGED);
		}
	}
	if(bUpdateFloatRect)
	{
		m_fSize.cx = m_BarSize.cx;
		m_fSize.cy = m_btnSize.cy;
	}
//	UpdateClientSize(w,h);
}

//����lprc��������ϸþ��εĴ��ڳߴ�
LRESULT RingToolBar::OnSizing(RINGPARAMS& param)
{
	int n,h = (int)HIWORD(SendMessage(m_hWnd,TB_GETBUTTONSIZE,0,0));

	LPRECT lprc = (LPRECT)param.lParam;
	n = (lprc->bottom - lprc->top)/h;
	
	WrapToolBar(n,lprc);
	m_fSize.cx = lprc->right - lprc->left;
	m_fSize.cy = lprc->bottom - lprc->top;
	m_nfRows = n;
	return TRUE;
}

BOOL RingToolBar::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{	
	if(uMsg == MYWM_SIZE)
	{
		//����Ҫ���ƣ�����Ϣ��DockBar֪ͨ���Ժ�DockBar��֪ͨ����
		Update(GetButtonCount(),NULL,TRUE,FALSE);
		//CalcSize();
		
		RECT rc;
		GetWindowRect(m_hWnd,&rc);

		if(IsVert(lParam))
		{
			//SendMessage(m_hWnd,TB_SETROWS,MAKELONG(GetButtonCount(),TRUE),(LPARAM)&rc);
			//UpdateItems();
			//GetWindowRect(m_hWnd,&rc);
			return rc.right - rc.left;
			//return m_BarSize.cx;//CalcReSizeV(wParam);
		}
		else
		{
			//SendMessage(m_hWnd,TB_SETROWS,MAKELONG(1,TRUE),(LPARAM)&rc);
			//UpdateItems();
			//GetWindowRect(m_hWnd,&rc);
			return rc.bottom - rc.top;
		}
	}
	return FALSE;
}

/*
//	HWND�������ھ��
LRESULT RingToolBar::OnNotify(RINGPARAMS param)
{
	switch(param.lpnmhdr->code)
   {
   	case TTN_NEEDTEXT:
      {
		   ((LPTOOLTIPTEXT)param.lParam)->hinst = m_hInst;
         return SendMessage(m_parent->Handle(),WM_RINGTOOLTIP,((LPTOOLTIPTEXT)param.lParam)->hdr.idFrom,param.lParam);
      }		
		case TBN_DROPDOWN:
		{
			 //LPNMTOOLBAR nm = (LPNMTOOLBAR)param.lParam;
			Errmsg("Asd");
			return 0;//TBDDRET_NODEFAULT;
		}
	}
	return DefaultProc(param);
}
*/

void RingToolBar::ChangeState(int state,RingDockSite* docksite/*=NULL*/)
{
	if(docksite && m_dockbar && 
		(m_dockbar->GetState() == TBS_FLAG_FLY || 
		((m_dockbar->GetState() & 0x2) != (state & 0x2))))
	{
		//ԭʼ״̬�ı䣬������������
		int cnt = GetButtonCount();	
		//TBBUTTON tb;
		//tb.

		//for(int i=0;i<cnt;i++)
		//{
		//	if(GetButton(i,&tb))
				if(state > TBS_FLAG_DOCKBOTTOM)
				{
					//tb.fsState |= TBSTATE_WRAP;
					RECT rc;
					SendMessage(m_hWnd,TB_SETROWS,MAKEWPARAM(cnt,TRUE),(LPARAM)&rc);
				}
				else
				{
					RECT rc;
					SendMessage(m_hWnd,TB_SETROWS,MAKEWPARAM(1,FALSE),(LPARAM)&rc);
					//tb.fsState &= ~TBSTATE_WRAP;
					//SendMessage(m_hWnd,TB_SETBUTTONINFO,i,(LPARAM)&tb);
				}
		//}
		//m_State = state;
		Update(cnt);
	}
	else
	{
/*		//m_State = state;
		if(m_dockbar && m_dockbar->GetState() == TBS_FLAG_FLY)
		{
			RECT rc;
			GetButtonRect(0,&rc);
			int row = (m_frc.bottom - m_frc.top - BARTOOLY)/((rc.bottom - rc.top)+m_BorderSpace*2);
			SendMessage(m_hWnd,TB_SETROWS,MAKEWPARAM(row,TRUE),(LPARAM)&rc);
			m_winpos.cx = rc.right - rc.left;
			m_winpos.cy = rc.bottom - rc.top;
		}
//*/
	}
}

//���й�����
void RingToolBar::WrapToolBar(int nRows,LPRECT lprc)
{
	if(lprc == NULL)
		return;

	if(m_nMaxRow)
		nRows = min(nRows,m_nMaxRow);

	SendMessage(m_hWnd,TB_SETROWS,MAKEWPARAM(nRows,TRUE),(LPARAM)lprc);
	while(lprc->right - lprc->left < m_btnSize.cx)
	{
		nRows --;
		m_nMaxRow = nRows;
		SendMessage(m_hWnd,TB_SETROWS,MAKEWPARAM(nRows,TRUE),(LPARAM)lprc);
	}
	UpdateItems();
}

BOOL RingToolBar::OnGetDragRects(int state,LPRECT lprc)
{
	if(lprc)
	{
		switch(state)
		{
		case TBS_FLAG_FLY:
			lprc->left = lprc->top = 0;
			lprc->right = m_fSize.cx;
			lprc->bottom = m_fSize.cy;
			break;
		case TBS_FLAG_HORZ:
			lprc->left = lprc->top = 0;
			lprc->right = m_BarSize.cx;
			lprc->bottom = m_btnSize.cy;
			break;
		case TBS_FLAG_VERT:
			lprc->left = lprc->top = 0;
			lprc->right = m_btnSize.cx;
			lprc->bottom = m_BarSize.cy;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

//ͣ��״̬����Ӧͣ��״̬�ı��¼�������-1��δ�ı䣬TRUE���Ѵ���0��δ����
BOOL RingToolBar::OnDockStateChange(int stateOld,int stateNew)
{
	if(stateNew == stateOld)
		return -1;
	
//	m_State = param.wParam;
	if(stateNew != TBS_FLAG_FLY)
	{
		//ԭʼ״̬�ı䣬������������
		int cnt = GetButtonCount();	
		RECT rc;
		int w,h;
		if(IsVert(stateNew))
		{
			WrapToolBar(cnt,&rc);
			CalcSize();
			w = m_btnSize.cx;
			h = m_BarSize.cy;
		}
		else
		{
			WrapToolBar(1,&rc);
			//SendMessage(m_hWnd,TB_SETROWS,MAKEWPARAM(1,FALSE),(LPARAM)&rc);
			//UpdateItems();
			CalcSize();
			w = m_BarSize.cx;
			h = m_btnSize.cy;
		}
		
//		m_State = param.wParam;
		//����ҪUpdate����״̬
		
		SetPos(0,0,w,h,NULL,SWP_NOMOVE);
	}
	else
	{
		RECT rc;
		//GetButtonRect(0,&rc);
		//int row = (m_fSize.cy)/(rc.bottom - rc.top);
		
		WrapToolBar(m_nfRows,&rc);		
	}
	return TRUE;
}


