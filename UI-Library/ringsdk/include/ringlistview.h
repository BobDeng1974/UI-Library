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
//ԭ�ļ�����ringlistview.h
//
//���ļ�˵����RingListView������
//
//��⹦�ܣ�ListView�ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGLISTVIEW
#define _INC_RINGLISTVIEW
#include <commctrl.h>

class RingListView:public RingControls
{
public:
	RingListView();
	virtual ~RingListView();
	
   BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);

	LPCTSTR WndClassName(){return WC_LISTVIEW;}

	void SetExStyle(DWORD dwExStyle);
	void SetOwnerDraw(BOOL bParentDraw=FALSE)
		{RingControls::SetOwnerDraw(bParentDraw,LVS_OWNERDRAWFIXED);}

	//������ͷ�������ַ�����ʽ��"Column1|Column2|..."
	int SetColumns(LPCTSTR);
	//���ø���ͷ�������ַ�����ʽ��"width1|width2|..."
	BOOL SetColumnsWidth(LPCTSTR);
	BOOL InsertColumn(LPCTSTR szColumn,int subindex,int width,UINT uAlign=LVCFMT_LEFT,UINT uidIcon=0);
	void ClearColumn()
	{
		while(ListView_DeleteColumn(m_hWnd,0));
	}
	//������ͼ��ʽ(LVC_ICON,LVC_SMALLICON,LVC_LIST,LVC_REPORT)
	BOOL SetViewOf(UINT uType);

	//�粻��Ҫ����Ϊ0
	void InitImageList(int nBigX=32,int nBigY=32,
							 int nSmallX=16,int nSmallY=16,
							 int nStateX=0,int nStateY=0);

	//���ͼ�꣬ͬʱ��ӵ���Сͼ���б���(ͼ���������ͬ��Сͼ�꣬�Զ��������)
	int AddIcon(UINT uidIcon,HINSTANCE hInst=GetInstance());
	int AddIcon(LPCTSTR lpszIcon,HINSTANCE hInst=GetInstance());
	//���ͼ�꣬ͬʱ��ӵ���Сͼ���б���
	int AddIcon(HICON hicon);
	//ָ�����ͼ��
	int AddIcon(UINT uidIcon,UINT uFlag,HINSTANCE hInst=GetInstance());
	int AddIcon(HICON hicon,UINT uFlag);

	//�в���
	int AddRow();
	int InsertRow(int index);
	void Select(int index,BOOL bSelect=TRUE){ListView_SetItemState(m_hWnd,index,bSelect?(LVIS_SELECTED|LVIS_FOCUSED):0,LVIS_SELECTED|LVIS_FOCUSED);}
	LONG GetData(int index);
	BOOL SetData(int index,LONG lParam);

	//����������޵�һ�����������SetValue,SetData��SetItem
	int AddItem(int index,LPCTSTR szText,LPARAM lParam=0,int idIcon=-1);
	int AddItem(int index,int nVal,LPARAM lParam=0,int idIcon=-1);
	//�������������
	int SetItem(int index,int subindex,LPCTSTR szText,LPARAM lParam=0,int idIcon=-1);
	int SetItem(int index,int subindex,int nVal,LPARAM lParam=0,int idIcon=-1);
	
	//ע��ú���������RingControl::GetText���麯���������ָ��ִ��
	//RingControl::GetText������GetItemText�������Ȼ��ȷ
	LPCTSTR GetText(int index,int subIndex,int size=RBUF_SIZE,LPSTR lpBuf=NULL);
	
	//��ͷ����
	RingHeader* GetHeaderObj();
	//������ͷ��IMAGELIST�����ΪNULL�����cx,cy�ߴ紴��
	HIMAGELIST SetColumnImageList(HIMAGELIST hImage=NULL,int cx=16,int cy=16);
	HIMAGELIST GetColumnImageList();
	BOOL SetColumnImage(int nIndex,int nIconId,BOOL bOnRight=FALSE);
	int AddColumnIcon(int nIconId,HINSTANCE hInst=GetInstance());
	int AddColumnIcon(HICON hIcon);
	//��ʾ/�����У�iColΪȫ������ʾ״̬�µ����
	BOOL ShowColumn(int iCol,BOOL bShow);
	//��������ͷ�һ������˵�ѡ����ʾ/������
	RingCoolMenu* EnableColumnMenu(BOOL bEnable=TRUE);

	//�麯������
	COLORREF GetTextColor(){return ListView_GetTextColor(m_hWnd);}
	void SetTextColor(COLORREF crColor){ListView_SetTextColor(m_hWnd,crColor);}
	BOOL SetBkColor(COLORREF crColor){return ListView_SetBkColor(m_hWnd,crColor);}
	BOOL SetValue(int val,int index=INDEX_CURSEL);
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);
	
	int GetCurSel(int nIndex=0);
	//��Ϣ/���װ
	/*δ���
	ListView_CreateDragImage
	ListView_EditLabel
	ListView_GetBkImage
	ListView_GetCallbackMask
	ListView_GetEditControl
	ListView_GetISearchString
	ListView_GetToolTips
	ListView_SetBkImage
	ListView_SetCallbackMask
	ListView_SetToolTips
	*/
	DWORD ApproximateViewRect(int iCount,int cx=-1,int cy=-1)
	{
		return ListView_ApproximateViewRect(m_hWnd,cx,cy,iCount);
	}
	BOOL Arrange(UINT code=LVA_DEFAULT){return ListView_Arrange(m_hWnd,code);}
	BOOL Clear(){return ListView_DeleteAllItems(m_hWnd);}
	BOOL DeleteColumn(int iCol){return ListView_DeleteColumn(m_hWnd,iCol);}
	BOOL DeleteItem(int index){return ListView_DeleteItem(m_hWnd,index);}
	BOOL EnsureVisible(int index,BOOL fPartialOK=TRUE)
	{
		return ListView_EnsureVisible(m_hWnd,index,fPartialOK);
	}
	int FindString(LPCTSTR szFind,int nStart=-1);
	int FindMatchString(LPCTSTR szFind,int nStart=-1);
	int Find(LVFINDINFO* lvf,int nStart=-1){return ListView_FindItem(m_hWnd,nStart,lvf);}
	COLORREF GetBkColor(){return ListView_GetBkColor(m_hWnd);}
	BOOL GetCheckState(int index)	{return ListView_GetCheckState(m_hWnd,index);}
	BOOL GetColumn(int iCol,LPLVCOLUMN pcol)
	{
		return ListView_GetColumn(m_hWnd,iCol,pcol);
	}
	BOOL GetColumnOrderArray(int iCount,int *lpiArray)
	{
		return ListView_GetColumnOrderArray(m_hWnd,iCount,lpiArray);
	}
	int GetColumnWidth(int index){return ListView_GetColumnWidth(m_hWnd,index);}
	int GetCountPerPage(){return ListView_GetCountPerPage(m_hWnd);}
	DWORD GetExtendedListViewStyle()
	{
		return ListView_GetExtendedListViewStyle(m_hWnd);
	}
	HWND GetHeader(){return ListView_GetHeader(m_hWnd);}
	HCURSOR GetHotCursor(){return ListView_GetHotCursor(m_hWnd);}
	int GetHotItem(){return ListView_GetHotItem(m_hWnd);}
	DWORD GetHoverTime(){return ListView_GetHoverTime(m_hWnd);}
	HIMAGELIST GetImageList(int nFlag){return ListView_GetImageList(m_hWnd,nFlag);}
	//Ϊ������RingBaseWnd::GetItem���ĳ�GetItemInfo
	BOOL GetItemInfo(LPLVITEM pitem){return ListView_GetItem(m_hWnd,pitem);}
	int GetCount(){return ListView_GetItemCount(m_hWnd);}
	
	BOOL GetItemPosition(int i,LPPOINT lpt)
	{
		return ListView_GetItemPosition(m_hWnd,i,lpt);
	}
	BOOL GetItemRect(int i,LPRECT lprc,int code)
	{
		return ListView_GetItemRect(m_hWnd,i,lprc,code);
	}
	DWORD GetItemSpacing(BOOL bSmall){return ListView_GetItemSpacing(m_hWnd,bSmall);}
	UINT GetItemState(int index,UINT mask){return ListView_GetItemState(m_hWnd,index,mask);}
	int GetNextItem(int iStart,UINT flags){return ListView_GetNextItem(m_hWnd,iStart,flags);}
	BOOL GetNumberOfWorkAreas(LPUINT lpuWorkAreas)
	{
		return ListView_GetNumberOfWorkAreas(m_hWnd,lpuWorkAreas);
	}
	BOOL GetOrigin(LPPOINT lpptOrg){return ListView_GetOrigin(m_hWnd,lpptOrg);}
	int GetSelectedCount(){return ListView_GetSelectedCount(m_hWnd);}
	int GetSelectionMark(){return ListView_GetSelectionMark(m_hWnd);}
	int GetStringWidth(LPCSTR psz){return ListView_GetStringWidth(m_hWnd,psz);}
	BOOL GetSubItemRect(int iItem,int iSubItem,int code,LPRECT lpRect)
	{
		return ListView_GetSubItemRect(m_hWnd,iItem,iSubItem,code,lpRect);
	}
	COLORREF GetTextBkColor(){return ListView_GetTextBkColor(m_hWnd);}
	
	int GetTopIndex(){return ListView_GetTopIndex(m_hWnd);}
	BOOL GetUnicodeFormat(){return ListView_GetUnicodeFormat(m_hWnd);}
	BOOL GetViewRect(LPRECT lprc){return ListView_GetViewRect(m_hWnd,lprc);}
	void GetWorkAreas(int nWorkAreas,LPRECT lprc)
	{
		ListView_GetWorkAreas(m_hWnd,nWorkAreas,lprc);
	}
	int HitTest(int x,int y,LPLVHITTESTINFO lpvInfo=NULL);
	int InsertColumn(int iCol,LPLVCOLUMN pcol)
	{
		return ListView_InsertColumn(m_hWnd,iCol,pcol);		
	}
	int InsertItem(LPLVITEM pitem){return ListView_InsertItem(m_hWnd,pitem);}
	BOOL IsSelected(int index)
	{
		return (ListView_GetItemState(m_hWnd,index,LVIS_SELECTED) == LVIS_SELECTED);
	}
	BOOL RedrawItems(int iFirst,int iLast){return ListView_RedrawItems(m_hWnd,iFirst,iLast);}
	BOOL Scroll(int dx,int dy){return ListView_Scroll(m_hWnd,dx,dy);}
	void SetCheckState(UINT iIndex,BOOL fCheck){ListView_SetCheckState(m_hWnd,iIndex,fCheck);}
	BOOL SetColumn(int iCol,LPLVCOLUMN pcol){return ListView_SetColumn(m_hWnd,iCol,pcol);}
	BOOL SetColumnOrderArray(int iCount,int *lpiArray)
	{
		return ListView_SetColumnOrderArray(m_hWnd,iCount,lpiArray);
	}
	BOOL SetColumnWidth(int iCol,int cx){return ListView_SetColumnWidth(m_hWnd,iCol,cx);}
	HCURSOR SetHotCursor(HCURSOR hCursor){return ListView_SetHotCursor(m_hWnd,hCursor);}
	int SetHotItem(int iIndex){return ListView_SetHotItem(m_hWnd,iIndex);}
	void SetHoverTime(DWORD dwHoverTime){ListView_SetHoverTime(m_hWnd,dwHoverTime);}
	DWORD SetIconSpacing(int cx,int cy){return ListView_SetIconSpacing(m_hWnd,cx,cy);}
	HIMAGELIST SetImageList(HIMAGELIST hImg,int nFlag)
	{
		return ListView_SetImageList(m_hWnd,hImg,nFlag);
	}
	BOOL SetItem(LPLVITEM pitem){return ListView_SetItem(m_hWnd,pitem);}
	void SetRows(int nRowCount){ListView_SetItemCount(m_hWnd,nRowCount);}
	void SetItemCountEx(int cItems,DWORD dwFlags)
	{
		ListView_SetItemCountEx(m_hWnd,cItems,dwFlags);
	}
	BOOL SetItemPosition(int i,int x,int y){return ListView_SetItemPosition(m_hWnd,i,x,y);}
	void SetItemPosition32(int i,int x,int y){ListView_SetItemPosition32(m_hWnd,i,x,y);}
	void SetItemState(int i,UINT state,UINT mask)
	{
		ListView_SetItemState(m_hWnd,i,state,mask);
	}
	void SetItemText(int i,int iSubItem,LPCSTR pszText)
	{
		ListView_SetItemText(m_hWnd,i,iSubItem,(LPSTR)pszText);
	}
	int SetSelectionMark(int iIndex){return ListView_SetSelectionMark(m_hWnd,iIndex);}
	BOOL SetTextBkColor(COLORREF clrText){return ListView_SetTextBkColor(m_hWnd,clrText);}
	BOOL SetUnicodeFormat(BOOL fUnicode){return ListView_SetUnicodeFormat(m_hWnd,fUnicode);}
	void SetWorkAreas(int nWorkAreas,LPRECT lprc)
	{
		ListView_SetWorkAreas(m_hWnd,nWorkAreas,lprc);
	}
	BOOL SortItems(PFNLVCOMPARE pfnCompare,LPARAM lParamSort)
	{
		return ListView_SortItems(m_hWnd,pfnCompare,lParamSort);
	}
	BOOL SortItemsEx(PFNLVCOMPARE pfnCompare,LPARAM lParamSort)
	{
		return ListView_SortItemsEx(m_hWnd,pfnCompare,lParamSort);
	}
	int SubItemHitTest(LPLVHITTESTINFO pInfo){return ListView_SubItemHitTest(m_hWnd,pInfo);}
	BOOL Update(int iItem){return ListView_Update(m_hWnd,iItem);}

protected:
	HIMAGELIST m_hImageBig,m_hImageSmall,m_hImageState;
	DWORD m_dwImgFreeMask;
	RingHeader* m_header;

protected:
	//LRESULT RingdowProc(HWND,RINGPARAMS);	
	//void OnParentNotify(HWND,UINT,UINT,LONG);
	//���ظú���Ϊ��ȡ��һ�е��ı�
	int GetItemText(int index,LPTSTR lpBuf,int size);
	LRESULT DefaultProc(RINGPARAMS&);

};

#endif
