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
//ԭ�ļ�����ringtree.h
//
//���ļ�˵�������Ϳؼ��ඨ��
//
**********************************************************************/
#ifndef _INC_RINGTREE
#define _INC_RINGTREE
#include <commctrl.h>

class RingTree:public RingControls
{
public:
	RingTree():m_hImageDef(NULL),m_hImageExp(NULL){}
   ~RingTree();

	LPCTSTR WndClassName(){return WC_TREEVIEW;}	//"SysTreeView32"

	int AddIcon(UINT uidIcon,HINSTANCE hInst=GetInstance());
	int AddIcon(HICON hicon);

   BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x=0,int y=0,int width=0,int height=0);
		
	HTREEITEM AddItem(LPCTSTR szText,HTREEITEM hParent,HTREEITEM hAfter,LPARAM lParam=0,UINT idIcon=-1,UINT idSelIcon=-1);
	HTREEITEM AddRoot(LPCTSTR szText,HTREEITEM hAfter,LPARAM lParam=0,UINT idIcon=-1,UINT idSelIcon=-1);
	
	//�麯������
	BOOL SetValue(int val,int index=INDEX_CURSEL){return FALSE;}
	//�����������֣����Ĳ���ΪHTREEITEM�������ΪINDEX_CURSEL���ǵ�ǰѡ����
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);

	HTREEITEM GetSelection(){return TreeView_GetSelection(m_hWnd);}
	//��ȡ��굱ǰλ���µĽڵ�(һ������NM_CLICK֮�����Ӧ������)
	HTREEITEM GetMouseSelection();
	HTREEITEM GetRoot(){return TreeView_GetRoot(m_hWnd);}
	HTREEITEM GetChildItem(HTREEITEM hItem){return TreeView_GetChild(m_hWnd,hItem);}
	HTREEITEM GetNext(HTREEITEM hItem){return TreeView_GetNextSibling(m_hWnd,hItem);}
	HTREEITEM GetPrev(HTREEITEM hItem){return TreeView_GetPrevSibling(m_hWnd,hItem);}
	HTREEITEM GetParentItem(HTREEITEM hItem){return TreeView_GetParent(m_hWnd,hItem);}
	//��ȡhItem����ΪlpszName���ӽڵ㣬������һ�㡣hItemΪNULL���������Ŀ¼
	HTREEITEM SearchItem(LPCTSTR lpszName,HTREEITEM hItem = NULL);
	LPCTSTR GetItemString(HTREEITEM hItem,int size=RBUF_SIZE,LPSTR lpBuf=NULL);

	LPARAM GetData(HTREEITEM hItem);
	BOOL SetData(HTREEITEM hItem,LONG data);
	HTREEITEM HitTest(LPPOINT lpt,LPDWORD lpFlag=NULL);
	BOOL Select(HTREEITEM hItem){return TreeView_SelectItem(m_hWnd,hItem);}
	BOOL Expand(HTREEITEM hItem,UINT uFlag = TVE_EXPAND){return TreeView_Expand(m_hWnd,hItem,uFlag);}
	BOOL Clear(){return TreeView_DeleteAllItems(m_hWnd);}
	BOOL SortChildren(HTREEITEM hItem){return TreeView_SortChildren(m_hWnd,hItem,FALSE);}
	BOOL SortChildren(HTREEITEM hParent,PFNTVCOMPARE lpfn,LPARAM lParam=0);

protected:
	virtual void InitImageList();
	//���ص��麯��,index:��֧��TVGN_CARET,TVGN_CHILD,TVGN_NEXT,TVGN_PARENT,TVGN_PREVIOUS,TVGN_ROOT
	int GetItemText(int index,LPTSTR lpBuf,int size);
	
protected:
	HIMAGELIST m_hImageDef,m_hImageExp;

private:
   
};

//�ļ����б���
class RingDirTree:public RingTree
{
public:
	RingDirTree():m_hImage(NULL){memset(m_szFolder,0,MAX_PATH);}
	~RingDirTree(){}

	BOOL Create(UINT uId,int x=0,int y=0,int width=0,int height=0);
	BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x=0,int y=0,int width=0,int height=0);

	BOOL Attach(HWND);

	BOOL ListDir();
	LRESULT OnDefNotify(RingBaseWnd* w,RINGPARAMS& param);
	virtual LRESULT OnSelectChanged(RINGPARAMS& param);
	virtual LRESULT OnItemExpanding(RINGPARAMS& param);
	//��ȡѡ�е�Ŀ¼����bRootNoSlash�����ѡ����Ǹ�Ŀ¼(����C:)���򷵻ص���"C:\\"��
	//����ͨĿ¼���������û��"\"�ģ����Ϊͳһ�����øò���ΪTRUE����ȥ������"\"��
	//������"C:",Ĭ�ϲ����򷵻�"C:\\"
	LPCTSTR GetSelFolderName(BOOL bRootNoSlash=FALSE);

	static int CALLBACK SortCBProc(LPARAM lparam1,LPARAM lparam2,LPARAM lparamSort);
	
protected:
	void InitImageList();
	void FillTreeView(LPSHELLFOLDER lpsf,LPITEMIDLIST lpifq,HTREEITEM hParent);

private:
	LPITEMIDLIST ConcatPidls(LPCITEMIDLIST pidl1,LPCITEMIDLIST pidl2);
	LPITEMIDLIST CopyITEMID(LPMALLOC lpMalloc,LPITEMIDLIST lpi);
	BOOL GetName(LPSHELLFOLDER lpsf,LPITEMIDLIST lpi,DWORD dwFlags,LPSTR lpFriendlyName);
	LPITEMIDLIST GetFullyQualPidl(LPSHELLFOLDER lpsf,LPITEMIDLIST lpi);
	int GetIcon(LPITEMIDLIST lpi,UINT uFlags);
	HICON GetFileIcon(LPSTR lpi);
	void GetNormalAndSelectedIcons(LPITEMIDLIST lpifq,LPTV_ITEM lptvitem);
	UINT GetSize(LPCITEMIDLIST pidl);
	LPITEMIDLIST Next(LPCITEMIDLIST pidl);
	LPITEMIDLIST CreateIDList(UINT cbSize);
	
private:	
	HIMAGELIST m_hImage;
	char m_szFolder[MAX_PATH];
};

#endif
