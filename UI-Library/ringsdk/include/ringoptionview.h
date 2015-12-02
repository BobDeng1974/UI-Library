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
//ԭ�ļ�����ringoptionview.h
//
//���ļ�˵����RingOptionView������
//
//��⹦�ܣ�OptionView�ؼ���װ��OptionView�ǽ�������Ƶ�
//				�ؼ���������DELPHI�Ŀؼ��������ã���һ������
//				��ListBox���������У���һ�����������ƣ��ڶ�
//				��Ϊ����ֵ����������Ա༭��
//
**********************************************************************/
#ifndef _INC_RINGOPTIONVIEW
#define _INC_RINGOPTIONVIEW

//ITEM����
#define OPTYPE_EDIT			0x0
#define OPTYPE_LIST			0x1
#define OPTYPE_BROWSE		0x2
#define OPTYPE_PACK			0x4
#define OPTYPE_NONE			0x8
//��չ����
#define OPTYPE_READONLY		0x10
//��EDIT������Ч���Ƿ�����ֻ��������
#define OPPROP_NUMBERONLY	1
#define OPPROP_PASSWORD		4
//��BROWSE������Ч���Ƿ��ѡ���ļ�
#define OPPROP_BROWSEFILE	2
//���ͼ������
#define OPTYPE_MASK		0x0F
//ITEM�Ƿ����ⲿ���ݣ��ɵ��ó���������ڴ���ͷ�
#define OPTYPE_EXTERN	0x1000
#define ROPV_LEFTMARGIN	16
//ɾ��ѡ���־�����ݰ�չ��/������ɾ������ѡ�
#define DELITEM_HIDE			0xABCDEF00
#define DELITEM_CLEAR		0xABCDEF01

class RingOptionItemPack;

typedef BOOL (RINGVCLASS::*ROPV_CHECKVALFUNC)(RingOptionItem*);

typedef struct tagRPDataPackItem
{
	RingOptionItem* m_item;
	tagRPDataPackItem* m_next;
}RPDATAPACKITEM,*LPRPDATAPACKITEM;

//�¼�֪ͨ��Ϣ��WM_NOTIFY��
#define ROPVN_VALUECHANGE	0x0180
#define ROPVN_BUTTONCLICK	0x0181
#define ROPVN_SELCHANGE		0x0182

class RingOptionView:public RingListBox
{
public:
	RingOptionView();
	virtual ~RingOptionView();
	
	BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);
	BOOL Create(UINT uId,int x=0,int y=0,int width=-999,int height=-999);

	//��������Attach(HWND),�������ظú��������������أ�Ҫ��RingControls::���ܵ���
	BOOL Attach(UINT uId,RingBaseWnd* parent=NULL){return RingControls::Attach(uId,parent);}
	BOOL Attach(HWND);
	
	//�����Ի棬����ΪTRUE��ʾ�ɸ����ڴ����Ի�
	void SetOwnerDraw(BOOL bParentDraw=FALSE)
		{RingControls::SetOwnerDraw(bParentDraw,LBS_OWNERDRAWFIXED);}
	void SetLeftMargin(int nSize){m_LeftPos=(nSize>=0?nSize:m_LeftPos);}
	int GetLeftMargin(){return m_LeftPos;}

	//������������
	int AddOption(LPCTSTR szOption,LPCTSTR szDefValue=NULL,LPARAM lParam=0,
						UINT uEditType=OPTYPE_EDIT,LONG lTypeCfg=0);
	int AddOptionPack(LPCTSTR szOption,LPARAM lParam=0);
	int AddOption(RingOptionItem* item);
	int InsertOption(int index,LPCTSTR szOption,LPCTSTR szDefValue=NULL,LPARAM lParam=0,
							UINT uEditType=OPTYPE_EDIT,LONG lTypeCfg=0);
	int InsertOption(RingOptionItem* item,int index);
	int InsertOption(RingOptionItem* item,LPCTSTR lpString);
	int AddSubOption(LPCTSTR szPackName,LPCTSTR szOption,LPCTSTR szDefValue=NULL,
							LPARAM lParam=0,UINT uEditType=OPTYPE_EDIT,LONG lTypeCfg=0);
	int AddSubOption(int index,LPCTSTR szOption,LPCTSTR szDefValue=NULL,
							LPARAM lParam=0,UINT uEditType=OPTYPE_EDIT,LONG lTypeCfg=0);
	//ɾ����Ҳ�ɵ���RingListBox::DelString
	int DelOption(RingOptionItem* item);
	//��հ�����
	int ClearPack(int index);
	int ClearPack(LPCTSTR lpName);

	//��ȡ������,���ݰ�����ֻ����չ��״̬�²��ܵõ������豣֤�ɹ��õ���
	//������GetPackItem��ȡ���ݰ���Ȼ�������GetSubItem�õ���
	RingOptionItem* GetOptionItem(int index = INDEX_CURSEL);
	RingOptionItem* GetOptionItem(LPCTSTR lpName);
	RingOptionItemPack* GetPackItem(LPCTSTR lpName);
	RingOptionItemPack* GetPackItem(int index);
	
	//����GetIntΪ�������ֵ,GetText�������أ��������GetItemText,
	//�ʸı�����Ϊ������GetItemText��������������ù�����ֵ������������ֵ��
	//��LIST������б�ѡ���ֵ������ʾ�ַ�����Browser����ͬ��
	int GetInt(int index=INDEX_CURSEL);
	virtual int GetInt(LPCTSTR lpName);
	
	//��ȡ�������ƣ�ԭGetText����Ϊ��
	LPCTSTR GetName(int index);
	//��ȡ��������
	int GetType(int nIndex);

	//��ȡѡ����ţ���List������Ч��
	int GetSelectIndex(int index);
	int GetSelectIndex(LPCTSTR lpName);
	//��ȡѡ��DATA
	int GetSelectData(int index);
	int GetSelectData(LPCTSTR lpName);

	//��ȡ���ݰ��������ݣ����ݰ��п������𣬵���GetText��GetInt��ʧ�ܻ�ò���
	//��ȷֵ��GetPackItemText��GetPackItemInt�������ݰ��Ƿ��������ܻ����ȷֵ
	LPCTSTR GetPackItemText(int index,LPCTSTR szSubItem);
	LPCTSTR GetPackItemText(LPCTSTR szPackName,LPCTSTR szSubItem);
	LPCTSTR GetPackItemText(RingOptionItemPack* pack,LPCTSTR szSubItem);
	int GetPackItemInt(int index,LPCTSTR szSubItem);
	int GetPackItemInt(LPCTSTR szPackName,LPCTSTR szSubItem);	
	int GetPackItemInt(RingOptionItemPack* pack,LPCTSTR szSubItem);
	
	//������ݣ����������
	void Reset();
	//��������
	BOOL SetValue(int val,int index=INDEX_CURSEL);
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);
	virtual BOOL SetValue(int index,LPARAM lVal,LPCTSTR szShowVal=NULL);
	virtual BOOL SetValue(LPCTSTR szItemName,LPARAM lVal,LPCTSTR szShowVal=NULL);
	//�������ݰ���������
	virtual BOOL SetPackItemValue(int index,LPCTSTR szSubItem,LPARAM lVal,LPCTSTR szShowVal=NULL);
	virtual BOOL SetPackItemValue(LPCTSTR szPackName,LPCTSTR szSubItem,LPARAM lVal,LPCTSTR szShowVal=NULL);
	BOOL SetPackItemValue(RingOptionItemPack* pack,LPCTSTR szSubItem,LPARAM lVal,LPCTSTR szShowVal=NULL);
	//����ѡ����ţ���List������Ч��
	BOOL SetSelectIndex(int index,int val);
	BOOL SetSelectIndex(LPCTSTR lpName,int val);
	//��������ֵ����ѡ�����
	BOOL SetSelectIndexByData(int index,LONG lData,BOOL IsString=FALSE);
	BOOL SetSelectIndexByData(LPCTSTR lpName,LONG lData,BOOL IsString=FALSE);
	
	RingEdit* GetEdit(){return m_edit;}
	RingCombo* GetCombo(BOOL bReadOnly){return bReadOnly?m_comboSel:m_combo;}
	RingButton* GetButton(){return m_button;}
	
	//�Ƿ��ǽ�ʣ��һ�����ݰ���������Ҫ������ߵĿ�λ��
	BOOL IsLastPack(RingOptionItem* item);
	//�趨��ֵ�ı䣬��ť������¼�������
	BOOL SetItemValChangeFunc(int index,RINGVCLASS* obj,ROPV_CHECKVALFUNC func);
	BOOL SetItemValChangeFunc(LPCTSTR lpName,RINGVCLASS* obj,ROPV_CHECKVALFUNC func);
	BOOL SetItemValSelectFunc(int index,RINGVCLASS* obj,ROPV_CHECKVALFUNC func);
	BOOL SetItemValSelectFunc(LPCTSTR lpName,RINGVCLASS* obj,ROPV_CHECKVALFUNC func);
	BOOL SetItemBrowserFunc(int index,RINGVCLASS* obj,ROPV_CHECKVALFUNC func);
	BOOL SetItemBrowserFunc(LPCTSTR lpName,RINGVCLASS* obj,ROPV_CHECKVALFUNC func);

protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);
	BOOL OnDrawItem(LPDRAWITEMSTRUCT lpds);
	BOOL OnFitSize(HWND,WPARAM,LPARAM);
	void OnCreate(HWND);
	LRESULT OnDestroy(RINGPARAMS& param);
	//���ظú���ʹGetText��������ֵ��������Item Text
	//��GetName�õ�.��������������ֵ�ַ������ȣ�����
	//ʵ��COPY��lpBuf�����ֽڡ�
	virtual int GetItemText(int index,LPTSTR lpBuf,int size);

private:
	RingOptionItem* OnAddString(DWORD type,LPCTSTR lpString);
	void OnDelString(int index,DWORD dwFlag);
	void SendNotify(int type);	
	friend class RingOptionItem;

protected:
	RingEdit* m_edit;
	RingCombo* m_combo,*m_comboSel;	//�б�����룬���б�
	RingButton* m_button;
	HPEN m_pen;
	HCURSOR m_cursor,m_cursorSize;
	int m_SplitPos;
	int m_LeftPos;
	//���ݰ�������ʾ�Ӵ�����
	ringFont m_fontPack,m_fontSubItem;

private:
	BOOL m_bCapture,m_bUpdate;
};

class RingOptionItem
{
public:
	RingOptionItem():m_edit(NULL),m_parent(NULL),
		m_bAutoDelete(FALSE),m_data(0),m_obj(NULL),
		m_OnCheckValFunc(NULL),m_parentItem(NULL),m_bReadOnly(FALSE){}
	virtual ~RingOptionItem()=0{}

	void SetName(LPCTSTR lpszName){m_strName = lpszName;}
	LPCTSTR GetName(){return m_strName.string();}
	//�õ���OptionView�е��������(���ݰ������LB_ERR)
	int GetIndex();
	//�õ������ݰ��е��������
	int GetSubIndex(RingOptionItemPack** parentItem=NULL);

	virtual int FindSubItem(LPCTSTR lpszName){return -1;}

	virtual LPCTSTR GetText();
	virtual int GetInt();
	virtual void Reset(){SetValue("");}
	//���ڵ�ǰ���ڱ༭״̬�������ԭ����ֵ������ͬGetText;
	virtual LPCTSTR GetPrevText(){return m_strValue.string();}

	virtual void UpdateValue();
	
	BOOL IsReadOnly(){return m_bReadOnly;}
	void SetReadOnly(BOOL bReadOnly){m_bReadOnly = bReadOnly;}
	void SetData(LPARAM data){m_data = data;}
	LONG GetData(){return m_data;}
	//��ȡ�����趨�����ݣ�List,Sel���ͣ�
	virtual LONG GetItemData(int index=INDEX_CURSEL){return 0;}
	virtual void SetItemData(LPARAM lParam,int index=INDEX_CURSEL){}
	//��ȡѡ����ţ���List������Ч��
	virtual int GetSelectIndex(){return LB_ERR;}
	virtual BOOL SetSelectIndex(int index){return FALSE;}
	virtual BOOL SetSelectIndexByData(LONG lData,BOOL IsString){return FALSE;}
	//virtual LPARAM GetValue(){return 0}
	virtual BOOL SetValue(int nVal);
	virtual BOOL SetValue(LPCTSTR lpszVal);
	virtual BOOL SetValue(LPARAM lVal,LPCTSTR lpszShowVal);
	//������Ϊ���ֻ��ڲ�����(LIST:ѡ���Ӧ��ʾֵ���ڲ�ֵ��EDIT���Ƿ��������֣�
	//SelFile:ѡ���ļ�����չ��
	virtual BOOL SetOption(LONG lConfig){return TRUE;}

	virtual int GetType(){return OPTYPE_NONE;}
	BOOL IsSubItem(){return (BOOL)m_parentItem;}
	virtual BOOL IsPackItem(){return FALSE;}
	//չ��
	//virtual void Expand();
	//����
	//virtual void Collapse();

	//ֻ�д��ڱ༭״̬�ŷ��ؿؼ�ָ��
	virtual RingControls* GetCtrl(){return m_edit;}
	//�༭���ݿؼ��Ƿ�ɼ�����OptionView�ͻ����ɼ����ǿؼ��ɼ����ԣ�
	BOOL IsEditVisible();

	RingOptionView* GetParent(){return m_parent;}
		
	void SetCheckValueFunc(RINGVCLASS* obj,ROPV_CHECKVALFUNC func)
	{		
		m_obj = obj;
		m_OnCheckValFunc = func;		
	}
	virtual BOOL SetSelChangeFunc(RINGVCLASS* obj,ROPV_CHECKVALFUNC func){return FALSE;}
	virtual BOOL SetBrowserFunc(RINGVCLASS* obj,ROPV_CHECKVALFUNC func){return FALSE;}
	
protected:
	virtual void EnterEdit(int,int,int,int);
	virtual BOOL ExitEdit();
	virtual void DrawValue(HDC hDC,LPRECT lprc);
	//����ؼ���Ϊ����
	virtual void ActiveOption(RingControls& edit);
	//ֵ�ı�ȷ���֪ͨ��Ϣ֮ǰ��Ԥ����
	virtual BOOL OnBeforeNotify(int type){return FALSE;}
	//void SetSubItem(){m_bIsSubItem = TRUE;}

private:
	void SetAutoDelete(BOOL bAuto){m_bAutoDelete = bAuto;}
	void SetParent(RingOptionView* parent){m_parent = parent;}
	virtual BOOL InterSetValue(LPARAM lVal,LPCTSTR lpszShowVal){return SetValue(lVal,lpszShowVal);}
	virtual RingControls* GetEditCtrl(){return NULL;}

	friend class RingOptionView;
	friend class RingOptionItemPack;

protected:
	ringStr m_strName,m_strValue;
	LPARAM m_data;		//�Զ�������
	RingControls* m_edit;
	RINGVCLASS* m_obj;
	RingOptionView* m_parent;
	
	ROPV_CHECKVALFUNC m_OnCheckValFunc;

private:
	BOOL m_bAutoDelete;
	BOOL m_bReadOnly;
	RingOptionItemPack* m_parentItem;	
};

class RingOptionItemEdit:public RingOptionItem
{
public:
	RingOptionItemEdit():m_uType(0),m_strPsw(NULL){}
	virtual ~RingOptionItemEdit(){delete m_strPsw;}

	int GetType(){return OPTYPE_EDIT;}
	BOOL SetOption(LONG lConfig){m_uType = lConfig;return TRUE;}
	//���ص��麯��
	BOOL SetValue(LPCTSTR lpszVal);
	BOOL SetValue(int nVal);
	
protected:
	//����ؼ���Ϊ����
	void ActiveOption(RingControls& edit);
	void DrawValue(HDC hDC,LPRECT lprc);
	BOOL ExitEdit();
	BOOL OnBeforeNotify(int type);

private:
	UINT m_uType;
	ringStr* m_strPsw;

private:
	RingControls* GetEditCtrl(){return GetParent()?GetParent()->GetEdit():NULL;}
};

//////////////////////////////////////////////////
//
//�б�ѡ������ݱ����ʽ��
//		m_strList�����б�ѡ��ԡ�|���ָ�
//		m_InterData���鱣����Ӧѡ���DATA��ע��DATA������ָ�룬
//		��Ϊɾ��ʱ����Ϊ��ѡ���WM_DELETEITEM��Ϣ
//
//////////////////////////////////////////////////
class RingOptionItemList:public RingOptionItem
{
public:
	RingOptionItemList():m_objSel(NULL),m_SelectFunc(NULL),m_selData(NULL),
								m_nItemCnt(0),m_InterData(0),m_nDataSize(0){}
	virtual ~RingOptionItemList(){}
	
	int GetType(){return IsReadOnly()?(OPTYPE_LIST|OPTYPE_READONLY):OPTYPE_LIST;}
	
	//��ȡ��ǰѡ�����
	int GetSelectIndex();
	BOOL SetSelectIndex(int index);
	//����DATAѡ����Ӧѡ�IsStringָ��lData�Ƿ�Ϊ�ַ���
	BOOL SetSelectIndexByData(LONG lData);

	int GetCount(){return m_nItemCnt;}
	LPCTSTR GetText();
	int GetInt();
	LONG GetItemData(int index=INDEX_CURSEL);
	void SetItemData(LPARAM lParam,int index=INDEX_CURSEL);
	int FindString(LPCTSTR lpString);

	BOOL SetValue(int nVal);
	BOOL SetValue(LPCTSTR lpszVal);

	//����ѡ�����Ϊ�ַ�������ʽΪ"ѡ��1|ѡ��2|...||DATA1|DATA2|..."
	BOOL SetOption(LONG lConfig);

	BOOL SetSelection(LPCTSTR lpString);
	BOOL AddSelection(LPCTSTR lpString,LONG lData=0);
	BOOL DelSelection(LPCTSTR lpString);
	void Clear();

	BOOL SetSelChangeFunc(RINGVCLASS* obj,ROPV_CHECKVALFUNC func)
	{
		m_objSel = obj;
		m_SelectFunc = func;
		return TRUE;
	}

protected:
	void EnterEdit(int,int,int,int);
	BOOL OnBeforeNotify(int type);

private:
	RingControls* GetEditCtrl(){return GetParent()?GetParent()->GetCombo(IsReadOnly()):NULL;}
	BOOL InterSetValue(LPARAM lVal,LPCTSTR lpszShowVal);

protected:
	ringStr m_strList;
	LPLONG m_selData;
	LONG m_InterData;
	int m_nItemCnt,m_nDataSize;
	RINGVCLASS* m_objSel;
	ROPV_CHECKVALFUNC m_SelectFunc;
};

class RingOptionItemSel:public RingOptionItem
{
public:
	RingOptionItemSel():m_button(NULL),m_bIsSelectFile(FALSE),m_ItemData(0),
								m_objBrowser(NULL),m_BrowserFunc(NULL){}
	virtual ~RingOptionItemSel(){}
	
	int GetType(){return OPTYPE_BROWSE;}
	LONG GetItemData(int index=INDEX_CURSEL){return m_ItemData;}
	void SetItemData(LPARAM lParam,int index=INDEX_CURSEL){m_ItemData = lParam;}

	//BOOL SetValue(int nVal){return RingOptionItem::SetValue(nVal);}
	//BOOL SetValue(LPCTSTR lpszVal){return RingOptionItem::SetValue(lpszVal);}
	//���ǻ��ຯ������������������������趨��������������
	//BOOL SetValue(LPARAM lVal,LPCTSTR lpszShowVal);
	BOOL SetOption(LONG lConfig);

	BOOL SetBrowserFunc(RINGVCLASS* obj,ROPV_CHECKVALFUNC func)
	{
		m_objBrowser = obj;
		m_BrowserFunc = func;
		return TRUE;
	}

protected:
	void EnterEdit(int x,int y,int width,int height);
	BOOL ExitEdit();
	BOOL OnBeforeNotify(int type);

protected:
	RingButton* m_button;
	LPARAM m_ItemData;
	BOOL m_bIsSelectFile;
	RINGVCLASS* m_objBrowser;
	ROPV_CHECKVALFUNC m_BrowserFunc;
	TCHAR m_szFileExt[80];

private:
	RingControls* GetEditCtrl(){return GetParent()?GetParent()->GetEdit():NULL;}
};

class RingOptionItemPack:public RingOptionItem
{
public:
	RingOptionItemPack();
	virtual ~RingOptionItemPack();
	
	void Clear();
	void Reset();

	int GetType(){return OPTYPE_PACK;}
	BOOL IsPackItem(){return TRUE;}
	int GetSubItemCount();

	int AddSubItem(RingOptionItem* item);
	BOOL DelSubItem(RingOptionItem* item);
	RingOptionItem* GetSubItem(LPCTSTR lpszName);
	RingOptionItem* GetSubItem(int nIndex);
	int GetSubItemIndex(LPCTSTR lpszName);

	//չ��
	void Expand();
	//����
	void Collapse();
	//�Զ�չ��������
	void AutoExpand();

protected:
	void EnterEdit(int x,int y,int width,int height){};
	BOOL ExitEdit(){return FALSE;}
	void DrawValue(HDC hDC,LPRECT lprc);
	//void InsertItems();
	//void DeleteItems();

protected:
	BOOL m_bExpand;
	LPRPDATAPACKITEM m_SubItem;
		
private:
	RingControls* GetEditCtrl(){return NULL;}
};

#endif
