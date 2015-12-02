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
//ԭ�ļ�����ringtree.cpp
//
//˵�������Ϳؼ�����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingTree::~RingTree()
{
	if(m_hImageDef)
		ImageList_Destroy(m_hImageDef);		
	
	if(m_hImageExp)
		ImageList_Destroy(m_hImageExp);
}

BOOL RingTree::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height)
{
	InitCommonControls();

	RingControls::Create(dwStyle,dwExStyle,szInitText,uId,x,y,width,height);

	//InitImageList();
	return TRUE;
}

void RingTree::InitImageList()
{
	if(m_hImageDef == NULL)
	{
		m_hImageDef = ImageList_Create(16,16,ILC_COLOR32|ILC_MASK,5,5);
		TreeView_SetImageList(m_hWnd, m_hImageDef,TVSIL_NORMAL);
	}
}

int RingTree::AddIcon(UINT uidIcon,HINSTANCE hInst/*=GetInstance()*/)
{
	HICON hicon = LoadIcon(hInst,MAKEINTRESOURCE(uidIcon));
	int i = AddIcon(hicon);
	DestroyIcon(hicon);
		
	return i;
}
	
int RingTree::AddIcon(HICON hicon)
{
	if(hicon == NULL)
		return -1;
	
	InitImageList();

	if(m_hImageDef)
		return ImageList_AddIcon(m_hImageDef,hicon);
	else
		return -1;
}

HTREEITEM RingTree::AddRoot(LPCTSTR szText,HTREEITEM hAfter,LPARAM lParam,UINT idIcon,UINT idSelIcon)
{
	return AddItem(szText,NULL,hAfter,lParam,idIcon,idSelIcon);
}

HTREEITEM RingTree::AddItem(LPCTSTR szText,HTREEITEM hParent,HTREEITEM hAfter,LPARAM lParam,UINT idIcon,UINT idSelIcon)
{
	TV_ITEM tvItem;
	TV_INSERTSTRUCT tvInsert;
		
	tvItem.mask = TVIF_TEXT | TVIF_PARAM;
	tvItem.pszText = (LPSTR)szText;
	tvItem.cchTextMax = lstrlen(szText);
	tvItem.lParam = lParam;
	
	if(IS_INTRESOURCE(idIcon))
	{
		tvItem.mask |= TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		tvItem.iImage = idIcon;
		tvItem.iSelectedImage = idIcon;
	}
	if(IS_INTRESOURCE(idSelIcon))
		tvItem.iSelectedImage = idSelIcon;
	
	tvInsert.item = tvItem;
	tvInsert.hInsertAfter = hAfter;
	tvInsert.hParent = hParent;
	
	return (HTREEITEM)SendMessage(m_hWnd,TVM_INSERTITEM,0,
											(LPARAM)(LPTV_INSERTSTRUCT)&tvInsert);
}

LPARAM RingTree::GetData(HTREEITEM hItem)
{
	TVITEM tvi;

	memset(&tvi,0,sizeof(TVITEM));
	tvi.mask = TVIF_PARAM | TVIF_HANDLE;
	tvi.hItem = hItem;

	if(TreeView_GetItem(m_hWnd,&tvi))
		return tvi.lParam;
	else
		return 0;
}

BOOL RingTree::SetData(HTREEITEM hItem,LONG data)
{
	TVITEM tvi;
	
	memset(&tvi,0,sizeof(TVITEM));
	tvi.mask = TVIF_PARAM | TVIF_HANDLE;
	tvi.hItem = hItem;
	tvi.lParam = data;

	return TreeView_SetItem(m_hWnd,&tvi);
}

HTREEITEM RingTree::HitTest(LPPOINT lpt,LPDWORD lpFlag/*=NULL*/)
{
	if(lpt)
	{
		TVHITTESTINFO ht = {{lpt->x,lpt->y},0,NULL};
		HTREEITEM hitem = TreeView_HitTest(m_hWnd,&ht);
		if(lpFlag)
			*lpFlag = ht.flags;
		return hitem;
	}
	else
		return NULL;
}

//�����������֣����Ĳ���ΪHTREEITEM�������ΪINDEX_CURSEL���ǵ�ǰѡ����
BOOL RingTree::SetValue(LPCTSTR lpszVal,int index/*=INDEX_CURSEL*/)
{
	if(lpszVal && *lpszVal != '\0')
	{
		HTREEITEM ht;
		if(index == INDEX_CURSEL)
			ht = GetSelection();
		else
			ht = (HTREEITEM)index;

		if(ht)
		{
			TVITEM tvi;
			tvi.cchTextMax = strlen(lpszVal);
			tvi.hItem = ht;
			tvi.pszText = (LPTSTR)lpszVal;
			tvi.mask = TVIF_TEXT;
			
			return TreeView_SetItem(m_hWnd,&tvi);
		}
	}
	return FALSE;
}

LPCTSTR RingTree::GetItemString(HTREEITEM hItem,int size/*=RBUF_SIZE*/,LPSTR lpBuf/*=NULL*/)
{
	if(lpBuf == NULL && GetBuffer(size))
		lpBuf = m_buf;

	if(lpBuf)
	{
		memset(lpBuf,0,size);
		
		TVITEM tvi;
		tvi.cchTextMax = size;
		tvi.hItem = hItem;
		tvi.pszText = lpBuf;
		tvi.mask = TVIF_TEXT;

		if(TreeView_GetItem(m_hWnd,&tvi))
		{
			if(tvi.pszText != m_buf)	//tvi.pszText��ֵ�п��ܸı�
				strncpy(lpBuf,tvi.pszText,size);
			return lpBuf;
		}
	}
	return "";
}

//index:��֧��TVGN_CARET,TVGN_CHILD,TVGN_NEXT,TVGN_PARENT,TVGN_PREVIOUS,TVGN_ROOT
int RingTree::GetItemText(int index,LPTSTR lpBuf,int size)
{
	HTREEITEM hitem = NULL;

	if(index == TVGN_ROOT)
		hitem = TreeView_GetRoot(m_hWnd);
	else
	{		
		hitem = TreeView_GetSelection(m_hWnd);
		if(hitem && index != INDEX_CURSEL && index != TVGN_CARET)
			hitem = TreeView_GetNextItem(m_hWnd,hitem,index);
	
		if(hitem)
			return lstrlen(GetItemString(hitem,size,lpBuf));
	}
	return 0;
}

//��ȡhItem����ΪlpszName���ӽڵ㣬������һ�㡣hItemΪNULL���������Ŀ¼
HTREEITEM RingTree::SearchItem(LPCTSTR lpszName,HTREEITEM hItem/*=NULL*/)
{
	if(hItem == NULL)
		hItem = TreeView_GetRoot(m_hWnd);
	else
		hItem = TreeView_GetChild(m_hWnd,hItem);

	if(hItem)
	{
		if(lpszName == NULL)
			return hItem;

		while(hItem)
		{
			if(strcmp(lpszName,GetItemString(hItem)) == 0)
				return hItem;
			else
				hItem = TreeView_GetNextSibling(m_hWnd,hItem);
		}
	}
	return NULL;
}

//��ȡ��굱ǰλ���µĽڵ�(һ������NM_CLICK֮�����Ӧ������)
HTREEITEM RingTree::GetMouseSelection()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd,&pt);
	return HitTest(&pt);
}

BOOL RingTree::SortChildren(HTREEITEM hParent,PFNTVCOMPARE lpfn,LPARAM lParam)
{
	TV_SORTCB tvs;
	tvs.hParent = hParent;
	tvs.lParam = lParam;
	tvs.lpfnCompare = lpfn;
	return TreeView_SortChildrenCB(m_hWnd,&tvs,FALSE);
}
