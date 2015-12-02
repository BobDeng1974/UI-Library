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
//ԭ�ļ�����ringlistview.cpp
//
//˵����listview�ؼ�����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingListView::RingListView()
{
	m_hImageBig = m_hImageSmall = m_hImageState = NULL;
	m_dwImgFreeMask = 0;
	m_header = NULL;
	m_windowType = ITIS_LISTVIEW;
}

RingListView::~RingListView()
{
	if(m_hImageBig)
		ImageList_Destroy(m_hImageBig);		
	if(m_hImageSmall)
		ImageList_Destroy(m_hImageSmall);
	if(m_hImageState)
		ImageList_Destroy(m_hImageState);
}

BOOL RingListView::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height)
{   
	InitCommonControls();
	
	RingControls::Create(dwStyle,dwExStyle,szInitText,uId,x,y,width,height);
		
	//InitImageList();
   return (BOOL)m_hWnd;
}

void RingListView::SetExStyle(DWORD dwExStyle)
{
	if(dwExStyle != 0 && m_hWnd)
	{
		//ListViewר����չ���Ա��������·������ã�
		DWORD dwStyle = SendMessage(m_hWnd,LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
		dwStyle |= dwExStyle;
		SendMessage(m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,0,dwStyle);
	}
}

LRESULT RingListView::DefaultProc(RINGPARAMS& param)
{
	if(param.uMsg == WM_NCDESTROY)
	{
		//BoundCheck�����BUG�����ڹ���������IMAGELIST,����ʱ����
		//ImageList_Destroy����BoundCheck����Դй©��
		//Ϊ�������Ϣ����������������õľͰ�ImageList����Ϊ��
		if(ListView_GetImageList(m_hWnd,LVSIL_NORMAL) == m_hImageBig)
			ListView_SetImageList(m_hWnd,NULL,LVSIL_NORMAL);
		if(ListView_GetImageList(m_hWnd,LVSIL_SMALL) == m_hImageSmall)
			ListView_SetImageList(m_hWnd,NULL,LVSIL_SMALL);
		if(ListView_GetImageList(m_hWnd,LVSIL_STATE) == m_hImageState)
			ListView_SetImageList(m_hWnd,NULL,LVSIL_STATE);
		CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
		OnNcDestroy();
		return 0;
	}	
	return CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
}

//����IMAGELIST���粻��Ҫ����Ϊ0
void RingListView::InitImageList(int nBigX,int nBigY,
											int nSmallX,int nSmallY,
											int nStateX,int nStateY)
{
	if(m_hImageBig == NULL && (nBigX != 0 && nBigY != 0))
	{
		m_hImageBig = ImageList_Create(nBigX,nBigY,ILC_COLOR32|ILC_MASK,5,5);
		if(m_hWnd && m_hImageBig && ListView_GetImageList(m_hWnd,LVSIL_NORMAL) == NULL)
			ListView_SetImageList(m_hWnd,m_hImageBig,LVSIL_NORMAL);
	}	
	if(m_hImageSmall == NULL && (nSmallX != 0 && nSmallY != 0))
	{
		m_hImageSmall = ImageList_Create(nSmallX,nSmallY,ILC_COLOR32|ILC_MASK,5,5);
		if(m_hWnd && m_hImageSmall && ListView_GetImageList(m_hWnd,LVSIL_SMALL) == NULL)
			ListView_SetImageList(m_hWnd,m_hImageSmall,LVSIL_SMALL);
	}
	if(m_hImageState == NULL && (nStateX != 0 && nStateY != 0))
	{
		m_hImageState = ImageList_Create(nStateX,nStateY,ILC_COLOR32|ILC_MASK,5,5);
		if(m_hWnd && m_hImageState && ListView_GetImageList(m_hWnd,LVSIL_STATE) == NULL)
			ListView_SetImageList(m_hWnd,m_hImageState,LVSIL_STATE);
	}
}

////////////////////////////////////////////////////
//
//���ͼ�꣬���������ù�IMAGELIST����ù�InitImageList(),
//���ͼ�������ͬ�ߴ磬�ֱ����ƥ��ߴ�ͼ����ӵ���Ӧ��
//IMAGELIST�С�
//
//���أ�����ͼ�����
//
////////////////////////////////////////////////////
int RingListView::AddIcon(LPCTSTR lpszIcon,HINSTANCE hInst/*=GetInstance()*/)
{
	//��Ϊ�п��ܵ��ù�SetImageList����˲���ʹ�����õ�m_hImageXXX,
	//����ListView_GetImageList
	HIMAGELIST himl = ListView_GetImageList(m_hWnd,LVSIL_NORMAL);
	HICON hicon;
	int x,y;
	int nIndex = -1;

	if(himl)
	{
		if(!ImageList_GetIconSize(himl,&x,&y))
			x = y = 32;
			
		hicon = (HICON)LoadImage(hInst,lpszIcon,IMAGE_ICON,x,y,LR_DEFAULTCOLOR);
		if(hicon)
			nIndex = ImageList_AddIcon(himl,hicon);
	}
	
	himl = ListView_GetImageList(m_hWnd,LVSIL_SMALL);
	if(himl)
	{
		if(!ImageList_GetIconSize(himl,&x,&y))
			x = y = 16;
		
		hicon = (HICON)LoadImage(hInst,lpszIcon,IMAGE_ICON,x,y,LR_DEFAULTCOLOR);
		if(hicon)
			nIndex = max(nIndex,ImageList_AddIcon(himl,hicon));
	}
	return nIndex;
}

int RingListView::AddIcon(UINT uidIcon,HINSTANCE hInst/*=GetInstance()*/)
{
	return AddIcon(MAKEINTRESOURCE(uidIcon),hInst);
}

//���ͼ�꣬ͬʱ��ӵ���Сͼ���б��У���������ͼ�����
int RingListView::AddIcon(HICON hicon)
{
	if(hicon == NULL)
		return -1;

	int nIndex = -1;
	HIMAGELIST himl = ListView_GetImageList(m_hWnd,LVSIL_NORMAL);
	if(himl)
		nIndex = ImageList_AddIcon(himl,hicon);
	
	himl = ListView_GetImageList(m_hWnd,LVSIL_SMALL);
	if(himl)
		nIndex = max(nIndex,ImageList_AddIcon(himl,hicon));

	return nIndex;
}

////////////////////////////////////////////////////
//
//���ͼ�꣬���������ù�IMAGELIST����ù�InitImageList(),
//���ͼ�������ͬ�ߴ磬����ƥ��ߴ�ͼ����ӵ���Ӧ��
//IMAGELIST�С�
//
//���أ�ͼ�����
//
////////////////////////////////////////////////////
int RingListView::AddIcon(UINT uidIcon,UINT uFlag,HINSTANCE hInst/*=GetInstance()*/)
{
	HICON hicon;
	HIMAGELIST himl = ListView_GetImageList(m_hWnd,uFlag);
	int x,y;
	
	if(himl)
	{
		if(ImageList_GetIconSize(himl,&x,&y))
			hicon = (HICON)LoadImage(hInst,MAKEINTRESOURCE(uidIcon),IMAGE_ICON,x,y,LR_DEFAULTCOLOR);
		else
			hicon = LoadIcon(hInst,MAKEINTRESOURCE(uidIcon));
		
		if(hicon)
			return ImageList_AddIcon(himl,hicon);
	}
	return -1;	
}

//uFlag:LVSIL_NORMAL,LVSIL_SMALL,LVSIL_STATE
int RingListView::AddIcon(HICON hicon,UINT uFlag)
{
	HIMAGELIST himl = ListView_GetImageList(m_hWnd,uFlag);
	
	if(hicon == NULL || himl == NULL)
		return -1;

	return ImageList_AddIcon(himl,hicon);	
}

LONG RingListView::GetData(int index)
{
	LVITEM lvt;
	lvt.mask = LVIF_PARAM;
	lvt.iItem = index;
	lvt.iSubItem = 0;
	lvt.lParam = 0;

	if(ListView_GetItem(m_hWnd,&lvt))
		return lvt.lParam;
	else
		return 0;
}

BOOL RingListView::SetData(int index,LONG lParam)
{
	LVITEM lvt;
	lvt.mask = LVIF_PARAM;
	lvt.iItem = index;
	lvt.iSubItem = 0;
	lvt.lParam = lParam;
	
	return ListView_SetItem(m_hWnd,&lvt);
}

int RingListView::SetColumns(LPCTSTR szColumn)
{
	if(m_hWnd == NULL || szColumn == NULL)
		return 0;

	ClearColumn();

	LV_COLUMN lvc; 
	int i,c = 0;
	ringStr str;
	LPTSTR s;
	int iCol,iWidth;

	str = szColumn;
	iCol = str.count("|") + 1;
	iWidth = (Width() - GetSystemMetrics(SM_CXVSCROLL))/iCol;

		// Initialize the LV_COLUMN structure. 
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT; 
		
	// Add the columns. 
	for(i=0;i<iCol;i++) 
	{ 
		lvc.iSubItem = i;
		s = (LPTSTR)str.string() + c;
		c = str.indexOf("|",c);
		str[c] = '\0';
		
		lvc.cx = iWidth;
		lvc.pszText = s;
			
		if(ListView_InsertColumn(m_hWnd,i,&lvc) == -1) 
		{
			str[c] = '|';
			return i;
		}
		str[c++] = '|';
	}
	return iCol;
}

BOOL RingListView::SetColumnsWidth(LPCTSTR szWidth)
{
	if(m_hWnd == NULL || szWidth == NULL)
		return FALSE;
	
	int i,c = 0;
	ringStr str;
	LPTSTR s;
	int iCol,iWidth;
	
	str = szWidth;
	iCol = str.count("|") + 1;
	
	for(i=0;i<iCol;i++) 
	{ 
		s = (LPTSTR)str.string() + c;
		c = str.indexOf("|",c);
		if(str[c-1] == '\\')
		{
			c++;
			continue;
		}
		str[c] = '\0';
		iWidth = atoi(s);
		
		if(!ListView_SetColumnWidth(m_hWnd,i,iWidth))
		{
			str[c] = '|';
			return FALSE;
		}
		str[c++] = '|';
	}	
	return TRUE; 
}

BOOL RingListView::InsertColumn(LPCTSTR szColumn,int subindex,int width,UINT uAlign/*=LVCFMT_LEFT*/,UINT uidIcon/*=0*/)
{
	if(m_hWnd == NULL || szColumn == NULL)
		return FALSE;
	
	LV_COLUMN lvc; 
	int iconId = -1;

	if(uidIcon != 0)
	{
		iconId = AddIcon(uidIcon);
		lvc.mask = LVCF_IMAGE;
	}
	else
		lvc.mask = 0;

	// Initialize the LV_COLUMN structure. 
	lvc.mask |= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = uAlign;
	lvc.cx = width;
	lvc.iSubItem = subindex;
	lvc.iImage = iconId;
	lvc.pszText = (LPSTR)szColumn;
			
	return (ListView_InsertColumn(m_hWnd,subindex,&lvc) != -1);
}

int RingListView::InsertRow(int index)
{
	LV_ITEM lvi;
	
	lvi.mask = LVIF_TEXT | LVIF_STATE;
	lvi.state = LVIS_FOCUSED | LVIS_SELECTED;
	lvi.stateMask = 0;
	lvi.pszText = "";
	lvi.cchTextMax = 300;
	
	lvi.iItem = index;
	lvi.iSubItem = 0; 
	
	return ListView_InsertItem(m_hWnd,&lvi);
}

int RingListView::AddRow()
{
	return InsertRow(GetCount());
}

int RingListView::AddItem(int index,LPCTSTR szText,LPARAM lParam,int idIcon)
{
	LV_ITEM lvi; 
	lvi.mask = 0;
	
	if(idIcon >= 0)
	{
		lvi.mask = LVIF_IMAGE;
		lvi.iImage = idIcon;
	}
	
	lvi.state = 0;
	lvi.stateMask = 0;
	
	if(szText)
	{
		lvi.mask |= TVIF_TEXT;
		lvi.pszText = (LPSTR)szText;
		lvi.cchTextMax = strlen(szText);
	}
	
	lvi.iItem = index; 
	lvi.iSubItem = 0;
	
	lvi.mask |= LVIF_PARAM;
	lvi.lParam = lParam;
	return ListView_InsertItem(m_hWnd,&lvi);	
}

int RingListView::SetItem(int index,int subindex,LPCTSTR szText,LPARAM lParam,int idIcon)
{
	LV_ITEM lvi; 
	lvi.mask = 0;

	if(idIcon >= 0)
	{
		lvi.mask = LVIF_IMAGE;
		lvi.iImage = idIcon;
	}
	
	lvi.state = 0;
	lvi.stateMask = 0;
	
	if(szText)
	{
		lvi.mask |= TVIF_TEXT;
		lvi.pszText = (LPSTR)szText;
		lvi.cchTextMax = strlen(szText);
	}
	
	lvi.iItem = index; 
	lvi.iSubItem = subindex;
	
	return ListView_SetItem(m_hWnd,&lvi)?index:-1;
} 

int RingListView::AddItem(int index,int nVal,LPARAM lParam,int uiconId)
{
	char s[16];
	wsprintf(s,"%d\0",nVal);
	return AddItem(index,s,lParam,uiconId);
}

int RingListView::SetItem(int index,int subindex,int nVal,LPARAM lParam,int uiconId)
{
	char s[16];
	wsprintf(s,"%d\0",nVal);
	return SetItem(index,subindex,s,lParam,uiconId);
}

BOOL RingListView::SetViewOf(UINT uType)
{
	LONG dNotView = ~(LVS_ICON|LVS_SMALLICON|LVS_LIST|LVS_REPORT);
	return SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)&dNotView|uType);
}

LPCTSTR RingListView::GetText(int index,int subIndex,int size/*=RBUF_SIZE*/,LPSTR lpBuf/*=NULL*/)
{
   if(lpBuf == NULL)
	{
		if(GetBuffer(size))
		{
			memset(m_buf,0,m_bufsize);
			ListView_GetItemText(m_hWnd,index,subIndex,m_buf,m_bufsize);
			return m_buf;
		}
		else
			return "";
	}
	else
	{
		memset(lpBuf,0,size);
		ListView_GetItemText(m_hWnd,index,subIndex,lpBuf,size);
		return lpBuf;
	}
}

//���ص��麯������ȡ��һ�е��ı�
int RingListView::GetItemText(int index,LPTSTR lpBuf,int size)
{
	if(index == INDEX_CURSEL)
	{
		index = GetCurSel();
		if(index == -1)
			return 0;
	}
	
	LV_ITEM lv;
	lv.pszText = lpBuf;
	lv.cchTextMax = size;
	lv.iSubItem = 0;
	return SendMessage(m_hWnd,LVM_GETITEMTEXT,index,(LPARAM)&lv);
}

int RingListView::HitTest(int x,int y,LPLVHITTESTINFO lpvInfo/*=NULL*/)
{
	LVHITTESTINFO lpv;
	if(lpvInfo == NULL)
		lpvInfo = &lpv;

	lpvInfo->pt.x = x;
	lpvInfo->pt.y = y;

	return ListView_HitTest(m_hWnd,lpvInfo);
}

int RingListView::FindString(LPCTSTR szFind,int nStart/*=-1*/)
{
	LVFINDINFO lvf;

	lvf.flags = LVFI_PARTIAL;
	lvf.psz = szFind;

	return ListView_FindItem(m_hWnd,nStart,&lvf);
}

int RingListView::FindMatchString(LPCTSTR szFind,int nStart/*=-1*/)
{
	LVFINDINFO lvf;
	
	lvf.flags = LVFI_STRING;
	lvf.psz = szFind;
	
	return ListView_FindItem(m_hWnd,nStart,&lvf);
}

BOOL RingListView::SetValue(int val,int index/*=INDEX_CURSEL*/)
{
	char szText[16];
	wsprintf(szText,"%d\0",val);
	return SetValue(szText,index);
}

BOOL RingListView::SetValue(LPCTSTR lpszVal,int index/*=INDEX_CURSEL*/)
{
	if(lpszVal == NULL)
		return FALSE;

	if(index == INDEX_CURSEL)
	{
		index = GetCurSel();
		if(index == -1)
			return FALSE;
	}
	
	LV_ITEM lv;
	lv.pszText = (LPSTR)lpszVal;
	lv.iSubItem = 0;
	return SendMessage(m_hWnd,LVM_SETITEMTEXT,index,(LPARAM)&lv);
}

RingHeader* RingListView::GetHeaderObj()
{
	if(m_header == NULL)
	{
		HWND hWnd = GetHeader();
		if(hWnd)
		{
			m_header = (RingHeader*)GetWindowPtr(hWnd);

			if(m_header == NULL)
			{
				m_header = NewRingObj(itemof(RingHeader));
				m_header->Attach(hWnd);
			}
		}
	}
	return m_header;
}

HIMAGELIST RingListView::GetColumnImageList()
{
	if(GetHeaderObj())
		return m_header->GetImageList();
	else
		return NULL;
}

HIMAGELIST RingListView::SetColumnImageList(HIMAGELIST hImage,int cx,int cy)
{
	if(GetHeaderObj())
	{
		if(hImage == NULL)
		{
			//ListView���������IMAGELIST������ͷ��IMAGELIST�����ù�
			//���listview�����ļ��б����ϵͳ�ļ�IMAGELIST������ͷ��Ҫ
			//�����Լ���IMAGELIST������ֻ��ʹ��ϵͳ�ļ�IMAGELIST���ͼ��
			//�Ҳ����Լ����ͼ�꣬�������ӻ�Ӱ��Listview��������ͼ�꣩
			hImage = m_header->GetImageList();
			//��ͷ����Ƴ������m_hImageΪ�գ��򴴽�������IMAGELISTΪm_hImage
			m_header->InitImageList(cx,cy);
			return hImage;
		}
		else
			return m_header->SetImageList(hImage);
	}
	return NULL;
}

BOOL RingListView::SetColumnImage(int nIndex,int nIconId,BOOL bOnRight)
{
	LVCOLUMN lvc;
	
	lvc.mask = LVCF_FMT|LVCF_IMAGE|LVCFMT_COL_HAS_IMAGES;
	GetColumn(nIndex,&lvc);

	if(nIconId < 0)
	{
		//����ʾͼ��
		lvc.mask = LVCF_FMT|LVCF_IMAGE;
		lvc.fmt &= ~(LVCFMT_BITMAP_ON_RIGHT|LVCFMT_IMAGE);
	}
	else
	{
		if(bOnRight)
			lvc.fmt |= LVCFMT_BITMAP_ON_RIGHT|LVCFMT_IMAGE;
		else
			lvc.fmt |= LVCFMT_IMAGE;
	}
	lvc.iImage = nIconId;
	return SetColumn(nIndex,&lvc);
}

int RingListView::AddColumnIcon(int nIconId,HINSTANCE hInst)
{
	if(GetHeaderObj())
		return m_header->AddIcon(nIconId,hInst);
	else
		return -1;
}

int RingListView::AddColumnIcon(HICON hIcon)
{
	if(GetHeaderObj())
		return m_header->AddIcon(hIcon);
	else
		return -1;
}

//��ʾ/�����У�iColΪȫ������ʾ״̬�µ����
BOOL RingListView::ShowColumn(int iCol,BOOL bShow)
{
	if(GetHeaderObj())
	{
		int nWidth = m_header->ShowColumn(iCol,bShow);
		if(!bShow)
			nWidth = 0;
		BOOL bRes = SetColumnWidth(iCol,nWidth);
		InvalidateRect(m_hWnd,NULL,TRUE);
	}
	return FALSE;
}

//��������ͷ�һ������˵�ѡ����ʾ/������
RingCoolMenu* RingListView::EnableColumnMenu(BOOL bEnable)
{
	if(GetHeaderObj())
		return m_header->EnableColumnMenu(bEnable); 
	else
		return NULL;
}

int RingListView::GetCurSel(int nIndex)
{
	int nCnt = GetCount();
	for(int i=nIndex;i<nCnt;i++)
	{
		if(IsSelected(i))
			return i;
	}
	return -1;
}
