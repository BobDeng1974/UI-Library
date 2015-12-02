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
//ԭ�ļ�����ringheader.h
//
//���ļ�˵����RingHeader������
//
//��⹦�ܣ�Header��װ
//
**********************************************************************/
#ifndef _INC_RINGHEADER
#define _INC_RINGHEADER

#define HDCOL_HIDEMASK						0x10000000
#define HDCOL_EXTRACTORDER(lParam)		(((lParam) >> 16) & 0x0FFF)
#define HDCOL_ISHIDDEN(lParam)			MASK_MATCH((lParam),HDCOL_HIDEMASK)
#define HDMENU_BASEID						0x9000

class RingHeader:public RingControls
{
public:
	RingHeader();
	virtual ~RingHeader();

	LPCTSTR WndClassName(){return WC_HEADER;}

	void InitImageList(int cx=16,int cy=16);

	//���ͼ��
	int AddIcon(UINT uidIcon,HINSTANCE hInst=GetInstance());
	int AddIcon(LPCTSTR lpszIcon,HINSTANCE hInst=GetInstance());
	int AddIcon(HICON hicon);
	
	LONG GetData(int index);
	BOOL SetData(int index,LONG lParam);
	
	//�������
	int AddItem(int index,LPCTSTR szText,LPARAM lParam=0,int idIcon=-1);
	int AddItem(int index,int nVal,LPARAM lParam=0,int idIcon=-1);
	//��ʾͼ�꣬-1Ϊ����ʾ
	void Select(int index,int idIcon);

	//����������ͷ
	void EnableHideColumn(BOOL bEnable=TRUE){SendMessage(m_hWnd,HDM_ENABLECOLUMNHIDE,bEnable,0);}
	BOOL IsColumnHide(int iCol);
	//�����Ҽ��˵�ѡ����ʾ/������ͷ
	RingCoolMenu* EnableColumnMenu(BOOL bEnable=TRUE);

	//��ʾ/������ͷ��iColΪȫ����ͷ��ʾ״̬�µ����,��ʾ/���ز���������Ч��
	//����������ÿ�Ȳſ��ԣ�����������ΪListView�����п��ͬʱ���ñ�HEADER���п�
	//��������ԭ��ʾ״̬���п�
	int ShowColumn(int iCol,BOOL bShow);

	//��ȡ��ͷ��ȣ�nMode:�����ͷδ���أ�����ʵ�ʿ�ȣ�
	//�����ͷ��������nMode=SW_HIDE������δ����ǰ�Ŀ��
	int GetItemWidth(int iCol,int nMode=SW_SHOW);

	//�麯������
	BOOL SetValue(int val,int index);
	BOOL SetValue(LPCTSTR lpszVal,int index);

	//��Ϣ/���װ
	/*δ���
	Header_CreateDragImage
	*/
	int ClearAllFilters(){return Header_ClearAllFilters(m_hWnd);}
	int ClearFilter(int i){return Header_ClearFilter(m_hWnd,i);}
	BOOL Delete(int index){return Header_DeleteItem(m_hWnd,index);}
	int EditFilter(int i,BOOL fDiscardChanges){return Header_EditFilter(m_hWnd,i,fDiscardChanges);}
	int GetBitmapMargin(){return Header_GetBitmapMargin(m_hWnd);}
	HIMAGELIST GetImageList(){return Header_GetImageList(m_hWnd);}
	//Ϊ������RingBaseWnd::GetItem���ĳ�GetItemInfo
	BOOL GetItemInfo(int index,LPHDITEM phdi){return Header_GetItem(m_hWnd,index,phdi);}
	int GetCount(){return Header_GetItemCount(m_hWnd);}
	BOOL GetItemRect(int iIndex,LPRECT lpItemRect)
	{
		return Header_GetItemRect(m_hWnd,iIndex,lpItemRect);
	}
	BOOL GetOrderArray(int iSize,int *lpiArray)
	{
		return Header_GetOrderArray(m_hWnd,iSize,lpiArray);
	}
	int GetOrder(int index);
	BOOL GetUnicodeFormat(){return Header_GetUnicodeFormat(m_hWnd);}
	int InsertItem(int index,LPHDITEM phdi){return Header_InsertItem(m_hWnd,index,phdi);}
	BOOL Layout(LPHDLAYOUT playout){return Header_Layout(m_hWnd,playout);}
	int OrderToIndex(int iOrder){return Header_OrderToIndex(m_hWnd,iOrder);}
	int SetBitmapMargin(int iWidth){return Header_SetBitmapMargin(m_hWnd,iWidth);}
	int SetFilterChangeTimeout(int i){return Header_SetFilterChangeTimeout(m_hWnd,i);}
	int SetHotDivider(BOOL flag,DWORD dwInputValue)
	{
		return Header_SetHotDivider(m_hWnd,flag,dwInputValue);
	}
	HIMAGELIST SetImageList(HIMAGELIST himl){return Header_SetImageList(m_hWnd,himl);}
	BOOL SetItem(int iIndex,LPHDITEM phdItem){return Header_SetItem(m_hWnd,iIndex,phdItem);}
	BOOL SetItemWidth(int iCol,int nWidth);
	BOOL SetOrderArray(int iSize,int *lpiArray)
	{
		return Header_SetOrderArray(m_hWnd,iSize,lpiArray);
	}
	BOOL SetUnicodeFormat(BOOL fUnicode){return Header_SetUnicodeFormat(m_hWnd,fUnicode);}

protected:
	int GetItemText(int index,LPTSTR lpBuf,int size);
	//ֱ�ӻ�ȡ/�������ݣ�
	LPLONG GetRealData(int index);
	BOOL SetRealData(int index,LONG lParam);
	LPLONG GetHDInfo(int index);
	LRESULT RingdowProc(HWND,RINGPARAMS);

	BOOL IsMouseOnHiddenCol(int x,int y);

protected:
	HIMAGELIST m_hImage;
	RingCoolMenu *m_CoolMenu;

private:
	BOOL m_bCanHideColumn;
	int m_nHideColCnt;
};

#endif
