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
//							RingSDK���������� ringdows.lib
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
//ԭ�ļ�����ringdowbase.h
//
//˵�������ڻ��ඨ�壬���д��������������Щ�ࡣ
//
**********************************************************************/
#ifndef _INC_RINGDOWBASE
#define _INC_RINGDOWBASE
#include "ringlib.h"
#include "ringmenu.h"

//��������
enum{
ITIS_PROPSHEET = 12298,		//����ҳ�Ի���
ITIS_PROPPAGE = 12299,			//����ҳ
ITIS_DIALOG	= 12300,				//��������DialogBox
//��������������밴���д���������У�RingBaseWnd::stdProc��
ITIS_WINDOW	= 12301,				//��������Window
ITIS_MDIFRAME = 12302,
ITIS_MDICHILD = 12303,
ITIS_TOOLBAR = 12305,
ITIS_COOLBAR = 12306,
ITIS_MENUBAR = 12307,
ITIS_FLYBAR	= 12308,
ITIS_STATUSBAR = 12310,
ITIS_DOCKSITE = 12313,
ITIS_EDIT =	12320,
ITIS_COMBO =	12321,
ITIS_LISTBOX =	12322,
ITIS_BUTTON	= 12323,
ITIS_STATIC = 12324,
ITIS_TAB = 12325,
ITIS_SCROLLBAR = 12326,
ITIS_LISTVIEW = 12327,

ITIS_DLGITEM = 12400,
ITIS_MAINWINDOW =	20000
};

//ͣ��������״̬/���Ͷ��壨���棺���¶���ֵ�����޸ģ��������޸Ĵ��룩
#define TBS_EX_DOCKABLE			0x0000000F		//����ı�λ�ã������϶���
#define TBS_EX_NONE				0x00000020     //�������϶�(SizeBar�ɵ����ߴ�)
#define TBS_EX_DOCKTOP			0x00000001
#define TBS_EX_DOCKBOTTOM		0x00000002
#define TBS_EX_DOCKHORZ			0x00000003
#define TBS_EX_DOCKLEFT			0x00000004
#define TBS_EX_DOCKRIGHT		0x00000008
#define TBS_EX_DOCKVERT			0x0000000C
#define TBS_EX_CANCLOSE			0x00010000
#define TBS_EX_ALLLINE			0x00100000		//��ռһ��

#define TBS_FLAG_NONE			22000
#define TBS_FLAG_FLYING			22001
#define TBS_FLAG_FLOATTING		22002

#define TBS_FLAG_FLY				(0xFFFF)
#define TBS_FLAG_DOCKTOP		0
#define TBS_FLAG_DOCKBOTTOM	1
#define TBS_FLAG_DOCKLEFT		2
#define TBS_FLAG_DOCKRIGHT		3

#define TBS_FLAG_MASK			2
#define TBS_FLAG_HORZ			0
#define TBS_FLAG_VERT			2
//�������

class RingOptionItem;

//�Զ��崰����Ϣ
#define MYWM_BASEEX				0x3000
#define MYWM_BASE					0x2000
#define MYWM_NOTIFYBASE			0x4000
#define MYWM_SIZE					(WM_SIZE + MYWM_BASEEX)
#define MYWM_ACTIVATE			(WM_ACTIVATE + MYWM_BASEEX)
#define MYWM_ENABLE				(WM_ENABLE + MYWM_BASEEX)
#define WM_RINGTOOLTIP			(WM_NOTIFY + MYWM_NOTIFYBASE)
#define WM_RINGTOOLHELP			(WM_NOTIFY + MYWM_NOTIFYBASE + 1)
//����Ϊ���ӵ��Զ��崰����Ϣ
//�õ����ڶ���ָ��
#define MYWM_GETCLASS			(MYWM_BASE + 1)
//MDI��MDICLIENT���ڹر���Ϣ
#define MYWM_MDICLIENTCLOSE	(MYWM_BASE + 2)
//����������ƶ���ȥ���뿪����Ϣ
#define WM_LINKHOVER				(MYWM_BASE + 3)
#define WM_LINKLEAVE				(MYWM_BASE + 4)
//������Ϣ�����¼���̧���͸���Ϣ������ͬWM_KEYUP
#define WM_KEYPRESS				(MYWM_BASE + 5)
//ͣ��������Ϣ
//��ѯ�õ������϶�����,������wParam:ָ���������ε�ָ�룬
//����Ϊ�������򣬸�����RECT��lParam��������
#define WM_GETDRAGRECTS			(MYWM_BASE + 6)
//����ȷ��ͣ��λ�ú���ʾǰ���͸���Ϣ��
//wParamΪͣ��λ�ñ�־(�������һ򸡶�)
//�������ͣ��λ�ñ�־���������ʾ����������
//lParamΪ�Ӵ���(ͣ������)�����
//����Ϣ�ȷ��͵������ڣ�ͣ�����ڵĸ����ڣ�����������δ��������0����
//���͵�Ҫͣ���Ĵ��ڣ���ô���δ������Ĭ��Ϊ���ڲ���ͣ��
//��ʲôλ�ã������øô��ڴ���ʱ�ĳߴ�
#define WM_DOCKSTATECHANGE		(MYWM_BASE + 7)
//DockSite�ߴ�䶯
#define WM_DOCKSITESIZECHANGE	(MYWM_BASE + 8)
//DockSite��Ϣ��
#define RD_ADDBAR					(MYWM_BASE + 9)
#define RD_DELETEBAR				(MYWM_BASE + 10)
#define RD_MOVEBAR				(MYWM_BASE + 11)
//header��Ϣ
#define HDM_ENABLECOLUMNHIDE	(MYWM_BASE + 12)
#define HDM_SHOWCOLUMN			(MYWM_BASE + 13)
//֪ͨ��Ϣ
#define HDN_SHOWCOLUMN			(MYWM_BASE + 13)
#define LVN_SHOWCOLUMN			HDN_SHOWCOLUMN
//RingComm�����¼�֪ͨ��Ϣ(����Ϣ��Ringcomm.h����ͬ������)
#ifndef WM_COMMEVENT
#define WM_COMMEVENT				(MYWM_BASE + 101)
#endif

#define RDERR_OK					(1)
#define RDERR_FAIL				(2)
//��Ǵ�����Ϣδ����
#define RM_NOTPROCESS			(-9999)

/********************************************************
//
//�˵�����ID��������״̬����ʾ������
//������Դ�ַ���Ӧ�������¸�ʽ��
//MH_BASE + �˵���ID "�˵���˵��"
//MH_POPUPBASE + n "�Ӳ˵�˵����nΪ��0��ʼ��������"
//�Ӳ˵�˵����ָ��չ���˵��ϵ�һ���Ӳ˵�������Ƴ��˵�ʱ�İ�����Ϣ
//nָ�����ǵ�n���Ӳ˵�
//MH_POPUPBASE��ֵ�����޸ģ���ֵΪ�����ڲ˵��ĵ�һ���Ӳ˵��ľ��ֵ
//
********************************************************/
#define MH_BASE					0
#define MH_POPUPBASE				0x1100

//����֪ͨ��Ϣ�ṹ(OptionView�ؼ�)
typedef struct _PROPVNOTIFY
{ 
	NMHDR hdr;
	LPCTSTR lpszName,lpszText,lpszOrgText;
	LONG lValue,lData;
	UINT uType;
	RingOptionItem* item; 
}ROPVNOTIFY,*LPROPVNOTIFY;

//����ͣ�����ڽṹ
typedef struct _RINGDOCKBARINFO
{ 
	UINT cbSize;
	UINT uDockStyle;
	UINT uDockFlag;
	HWND hChildWnd;	
}RINGDOCKBARINFO,*LPRINGDOCKBARINFO;

//������Ϣ
typedef struct tagRindowInfo
{
   HCURSOR	hCursor;			//��꣬���������Ҫ���ù�꣬�򴴽�������ʱ�����ϵͳ�����ΪNULL��
   HICON		hIconBig;		//������һ�ƶ���ϵͳ��ָ�ԭ�й��
   HICON		hIconSmall;
   LPCTSTR	lpszMenu;
}RINDOWINFO,*LPRINDOWINFO;

typedef struct tagRingParams
{	
	UINT uMsg;				//������Ϣ
	union
	{
		struct 
		{
			WORD low;		//wParam
			WORD high;		//wParam
		}shortval;			//wParam
		HDC hdc;				//wParam
		HRGN hrgn;			//WM_NCPAINT,etc.
		WPARAM wParam;
	};
	union
	{
		LPCREATESTRUCT lpcreate;				//WM_CREATE(lParam)
		LPNMHDR lpnmhdr;							//WM_NOTIFY(lParam)
		LPROPVNOTIFY lpropvnm;					//for OptionView NOTIFY(lParam)
		LPWINDOWPOS lpwindowpos;				//WM_WINDOWPOSXXX(lParam)
		LPDRAWITEMSTRUCT lpdrawitem;			//WM_DRAWITEM(lParam)
		LPMEASUREITEMSTRUCT lpmeasureitem;	//WM_MEASUREITEM(lParam)
		LPNCCALCSIZE_PARAMS lpncsp;			//WM_NCCALCSIZE(lParam)
		LPDELETEITEMSTRUCT lpdeleteitem;		//WM_DELETEITEM(lParam)
		LPNMTREEVIEW lpnmtreeview;				//WM_NOTIFY(lParam-treeview)
		LPNMLISTVIEW lpnmlistview;				//WM_NOTIFY(lParam-listview)
		POINTS mousept;							//�����Ϣ(lParam)		
		HANDLE handle;								//(lParam)
		DWORD dwval;								//(lParam)
		int intval;									//(lParam)
		LPARAM lParam;
	};
}RINGPARAMS,*LPRINGPARAMS;
/*
template <class T>
inline void (T::*v_Sig(void (T::*pmf)()))()
{
	return pmf;
}
*/
template <class T>
struct RingdowMsg
{
	typedef LRESULT (T::*MSG_FUNC1)(HWND,RINGPARAMS&);
	typedef void (T::*CMD_FUNC1)(UINT);
   UINT uMsg;
	UINT uId;
   MSG_FUNC1 msgFunc;
	CMD_FUNC1 cmdFunc;
};

//RINGVCLASS������Ϊת������� RingdowMsg �ͳ�Ա������ָ�����ͣ�
//���������������κζ����ʵ�֣���BC����������VC������ʵ��
/**********************************************************
//�öδ���������ringlib.h,RingThread��Ҫ�õ�
#ifndef __BORLANDC__
class RINGVCLASS
{
public:
	RINGVCLASS(){}
	~RINGVCLASS(){}
};
#else
class RINGVCLASS;
#endif
**********************************************************/
typedef RingdowMsg<RINGVCLASS> RingMsgs;
typedef LRESULT (RINGVCLASS::*MSG_FUNC1)(HWND,RINGPARAMS&);
typedef void (RINGVCLASS::*CMD_FUNC1)(UINT);

typedef void (*IDLEPROC)(void);

class RingBaseWnd;
class RingControls;
class RingDialog;
class RingOFNHookDialog;
class RingWnd;
class RingMDIClient;
class RingMDIChildWnd;
class RingDockBar;
class RingSizeBar;
class RingFlyBar;
class RingDockSite;
class RingToolBar;
class RingCoolBar;
class RingMenuBar;
class RingStatusBar;
class RingdowApp;
class RingCtrlWnd;
class RingCoolMenu;
class RingEdit;
class RingCombo;
class RingListBox;
class RingPropSheet;
class RingPropPage;
class RingTab;

//�ؼ��Ի渨����
class RingOwnerDrawer;

extern "C"
{
   int RingMain(void);
   void InitRingControls(HWND hWnd);
   //ATOM WINAPI RegRingdowClass(LPWNDCLASSEX lpwc);
	BOOL WINAPI RegRingdowClass(LPWNDCLASSEX lpwc);
   void WINAPI DrawDragRect(HDC hDC,LPRECT pRect,int nWidth);
	void WINAPI DrawLineRect(HDC hDC,LPRECT pRect);	
	RingBaseWnd* GetWindowPtr(HWND);
	BOOL IsWindowPtr(RingBaseWnd* obj);
	char * DecodeMessage(UINT msg);
	//for dockbar only
	BOOL IsVert(int state);
	BOOL IsHorz(int state);
	BOOL IsFloat(int state);
	//��ʼ����չ�ؼ�
	void InitIPControl();
	void InitBarControl();
	void InitCommonCtl32(DWORD dwICC);
}

//�궨�壬������
#define BEGIN_SETUPWINDOW(id,baseclass)	\
class RingWnd_##id:public baseclass\
{\
public:\
	RingWnd_##id(){m_msgs = (RingMsgs*)RingSDK_winmsg;InitRingSDKWindow();}

//	RingWnd_##id(RingBaseWnd* parent){SetParentObj(parent);m_msgs = (RingMsgs*)RingSDK_winmsg;InitRingSDKWindow();}\
	
#define BEGIN_DEFMSG
#define END_DEFMSG(id)	\
private:\
   typedef RingdowMsg<RingWnd_##id>::MSG_FUNC1   MyRingMsgFunc;\
	typedef RingdowMsg<RingWnd_##id>::CMD_FUNC1   MyRingCmdFunc;\
   typedef RingWnd_##id		                      RingSDKClass;\
	static RingdowMsg<RingWnd_##id> RingSDK_winmsg[];

#define END_SETUPWINDOW(id)	};\
extern RingWnd_##id* id;

#define BEGIN_REGMSG(id) \
RingWnd_##id* id = NULL;\
RingdowMsg<RingWnd_##id> RingWnd_##id::RingSDK_winmsg[] = {

#define REGMSG(wmsg)	{wmsg,0,(MyRingMsgFunc)&RingSDKClass::On_##wmsg,0},
#define REGMSGEX(wmsg,func)	{wmsg,0,(MyRingMsgFunc)&RingSDKClass::func,0},
#define REGCMD(id,func)	{0,id,(MyRingMsgFunc)&RingWnd::DispCommand,(MyRingCmdFunc)&RingSDKClass::func},
#define REGCMDRANGE(idlow,idhigh,func)	{idhigh,idlow,(MyRingMsgFunc)&RingWnd::DispCommand,(MyRingCmdFunc)&RingSDKClass::func},
#define REGNOTIFY(id)	{0,id,(MyRingMsgFunc)&RingSDKClass::On_##id,0},
#define END_REGMSG	{0,0,0,0}};

#define DEFMSG(msgname) 	LRESULT On_##msgname(HWND,RINGPARAMS&);
#define DEFMSGEX(msgname) 	LRESULT msgname(HWND,RINGPARAMS&);
#define DEFCMD(funcname) 	void funcname(UINT);
#define DEFNOTIFY(notify)		LRESULT On_##notify(HWND,RINGPARAMS&);
#define RINGMSG(id,msgname)	LRESULT RingWnd_##id::On_##msgname(HWND hWnd,RINGPARAMS& param)
#define RINGCMD(id,cmdfunc)	void RingWnd_##id::cmdfunc(UINT event)
#define RINGMSGEX(id,msgname)	LRESULT RingWnd_##id::msgname(HWND hWnd,RINGPARAMS& param)
#define RINGNOTIFY(id,notify)		LRESULT RingWnd_##id::On_##notify(HWND hWnd,RINGPARAMS& param)
#define RINGCLASS(id)	RingWnd_##id
#define RINGCLASSPTR(id,ptr)	((RingWnd_##id*)ptr)
#define RINGMETHOD(id,func) RingWnd_##id::##func

#define NEED_INIT		void InitRingSDKWindow();
#define NO_INIT      void InitRingSDKWindow(){}

#define BEGIN_DEF_VAR(setup)	setup\
	
#define END_DEF_VAR

#define INITWINDOW(id) void RingWnd_##id::InitRingSDKWindow

#define BEGIN_SETUPDIALOG(id)	BEGIN_SETUPWINDOW(id,IS_DIALOG)

#define END_SETUPDIALOG(id)	END_SETUPWINDOW(id)

#define BINDID(id)	void SetId(UINT uId=0){m_id = id;}
#define RINGDOWOBJ(id) RingWnd_##id()

#define itemof(objtype) ((objtype*)0)
#define windowof(obj) ((RingWnd_##obj*)0)

/**********************************************************
//
//	���д�����Ļ���
//
**********************************************************/
class RingBaseWnd
{
public:
	RingBaseWnd();
   virtual ~RingBaseWnd();
	
   //�ڲ���������
   RingBaseWnd* Parent() const{return m_parent;}
   HWND Handle() const{return this==NULL?NULL:m_hWnd;}
	operator HWND() const{return Handle();}

	HMENU GetMenu(){return ::GetMenu(m_hWnd);}
	int GetWindowType(){return m_windowType;}
	HWND GetParent(){return ::GetParent(m_hWnd);}

	int Width(){return WinWidth(m_hWnd);}
	int Height(){return WinHeight(m_hWnd);}
	int ClientWidth();
	int ClientHeight();
	//�õ��������꣨����ڸ����ڣ��޸�������Ϊ��Ļ���꣩
	BOOL GetRect(LPRECT lprc);

	HWND GetChild(){return GetWindow(m_hWnd,GW_CHILD);}
	HWND GetChild(HWND hwnd,UINT uFlag){return GetWindow(hwnd,uFlag);}
	HWND GetNextChild(HWND hwnd){return GetWindow(hwnd,GW_HWNDNEXT);}
	
	virtual DWORD GetStyle(){return GetWindowLong(m_hWnd,GWL_STYLE);}
	virtual DWORD GetExStyle(){return GetWindowLong(m_hWnd,GWL_EXSTYLE);}

	virtual BOOL GetClientRect(LPRECT lprc){return ::GetClientRect(m_hWnd,lprc);}
	COLORREF GetBkColor(HDC hDC = NULL);

	//��������
	void SetTitle(LPCTSTR szTitle);
	HWND SetParent(HWND hParentWnd);
   void SetParentObj(RingBaseWnd* parent);
	BOOL SetParentObj(HWND hParentWnd);
	virtual BOOL SetPos(int x,int y,int width=-999,int height=-999,HWND hPos = NULL,UINT uFlags = SWP_NOZORDER);
   virtual void SetCursor(HCURSOR hCur = NULL);
	//���ñ���ɫ�����ڴ����һֱ��Ч��
	virtual HBRUSH SetBkgColor(COLORREF crColor);
	virtual HBRUSH SetBkgBitmap(HBITMAP hbm);
	virtual HBRUSH SetBkgBitmap(UINT uId,HINSTANCE hInst = GetInstance());
	//�����˳�ʱ�Ƿ������������
	void SetAutoDelete(BOOL bFlag = TRUE){m_bKillOnExit = bFlag;}

	virtual void SetExStyle(DWORD dwExStyle);
   virtual void SetStyle(DWORD dwStyle);
	virtual void ReplaceStyle(DWORD dwsRemove,DWORD dwsReplace);
	
	//RingOwnerDrawer* SetItemOwnerDraw(RingOwnerDrawer* drawer);

	HWND SetFocus(){return ::SetFocus(m_hWnd);}

	//��������
	LRESULT SendMessage(HWND hWnd,RINGPARAMS& param)
	{return ::SendMessage(hWnd,param.uMsg,param.wParam,param.lParam);}
	//��Ϊ����ĺ�����ϵ������ȫ��ͬ�������ᱨ�Ҳ�������
	//�Ĵ���������øú���
	LRESULT SendMessage(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{return ::SendMessage(hWnd,uMsg,wParam,lParam);}
	
	BOOL CenterWindow(BOOL bWithParent = TRUE);
   virtual BOOL Show(int nCmdShow = SW_SHOW){return ::ShowWindow(m_hWnd,nCmdShow);}
	BOOL Enable(){return EnableWindow(m_hWnd,TRUE);}
	BOOL Disable(){return EnableWindow(m_hWnd,FALSE);}
	int Quit(){PostQuitMessage(0);return 0;}
	virtual int Close(int nResult=0){return DestroyWindow(m_hWnd);}

	virtual BOOL Attach(HWND);
	BOOL Detach();
	virtual WNDPROC SubClassWindow(HWND hWnd);

	virtual RingControls* InsertItem(RingControls* item,DWORD dwStyle=0,int left=0,int top=0,int width=0,int height=0);
		
	//���� new ��Ŀ�ģ������ new �������󣬴��ڹر�ʱ����Ҫ�Զ����١�
	//�ؼ���Ĭ���Զ����٣�Ӧ�ó��򴰿��ࣨ���ǿؼ��ࣩ���ڹر�ʱ���Զ��ͷţ�
	//����Ϊ��ͨ��SetAutoDelete����
	void* operator new(size_t size);

	//ͣ��������LPRECT����Ϊ��Ը�����
	virtual RingDockBar* EnableDock(LPCTSTR szTitle,UINT uDockStyle = TBS_EX_DOCKABLE,
		UINT uDockFlag = TBS_FLAG_DOCKTOP,LPRECT lprcPos = NULL);
	virtual RingDockBar* EnableDockSize(LPCTSTR szTitle,int nWidth,int nHeight,
			UINT uDockFlag = TBS_FLAG_DOCKLEFT,UINT uDockStyle = TBS_EX_DOCKABLE);
	virtual BOOL DockTo(UINT uDockFlag,LPRECT lprcPos = NULL);
	virtual void DisableDock();
	//������ͣ��״̬�л�
	int DockBack();
	RingDockBar* GetDockBar();
	int GetDockState();

	//�����Ӵ���
	BOOL Popup(RingBaseWnd* obj,LPCTSTR lpDlgOrTitle = NULL,LPARAM lParamOrClass = 0,DWORD dwStyle = WS_POPUP,DWORD dwExStyle = 0);
	//�����Ӵ���
	//int CreateChild(RingBaseWnd* child,LPCTSTR lpTemplate=NULL,LPARAM dwInitParam = 0);
	
	//δ�����WM_NOTIFY��Ϣ��Ĭ�ϴ���������Ӧ���ɿؼ���̳У�δ������Ϣ���뷵��-1
	virtual LRESULT OnDefNotify(RingBaseWnd* w,RINGPARAMS& param){return -1;}
	//����ͣ��ʱ���ƶ�λ����Ӧ����ͬ�ؼ�������Ҫ��ͬ�����Ų���˸,
	//uAppendFlag��Ĭ�ϲ����ĸ��Ӳ���
	virtual HDWP DefDeferWindowPos(HDWP,int,int,int,int,UINT uAppendFlag=0);

	//ˢ�£�������Ϊͣ�����裬�����ڸı�ߴ�ʱDockBar����ã�������²�ͬ�ؼ��򴰿���Ҫ�в�ͬ���֣�
	//����ֹ�������Ū�������ؼ��򴰿ڵľ������
	//virtual void Repaint(){InvalidateRect(m_hWnd,NULL,TRUE);}

	template<typename tname>
	tname* NewRingObj(tname* obj,BOOL bAutoDelete=FALSE)
	{
		try
		{
			tname* tmp = new tname;
			if(tmp)
			{
				tmp->SetParentObj(this);
				if(bAutoDelete)
					tmp->SetAutoDelete();
			}
			return tmp;
		}
		catch(...)
		{
			return NULL;
		}		
	}

	template<typename tname>
	tname* GetItem(tname* obj,UINT id)
	{
		try
		{
			HWND hwnd = GetDlgItem(m_hWnd,id);
			if(hwnd)
			{
				RingBaseWnd* temp = GetWindowPtr(hwnd);
				tname* tmp = NULL;
				if(temp)	//���п��ƶ���
				{
					//bug:tmp = (tname*)temp;ǿ������ת���п���
					//�᷵������ӳ���ָ�룬
					//�����dynamic_cast<tname*>(temp)����Ҫ��
					//��������Ϊ������RTTI����VCĬ�Ϲرգ��Ҵ�
					//��ѡ��Ӱ��Ч�ʣ���˵��øú�������ע�ⲻҪ
					//��������ӳ�䡣
					if(((RingControls*)temp)->TypeMatched(hwnd))
						tmp = (tname*)temp;

					return tmp;
				}
				else
				{
					tmp = new tname;
				
					if(tmp->TypeMatched(hwnd))
					{
						tmp->SetParentObj(this);
						//���Ͷ�
						tmp->Attach(id);
						return tmp;
					}
					else
					{
						delete tmp;
						return NULL;
					}
				}
			}
		}
		catch(...)
		{
		}
		return NULL;
	}

	template<typename tname>
	BOOL AssignItem(tname& obj,UINT id)
	{
		try
		{
			HWND hwnd = GetDlgItem(m_hWnd,id);
			if(hwnd)
			{
				RingBaseWnd* temp = GetWindowPtr(hwnd);
				
				if(temp == NULL)	//��δAttach
					if(obj.TypeMatched(hwnd))
					{
						obj.Detach();
						obj.SetParentObj(this);
						return obj.Attach(hwnd);
					}				
			}
		}
		catch(...)
		{
		}
		return FALSE;
	}

	//�ж�����	
	virtual BOOL IsControls(){return FALSE;}
	virtual BOOL IsFocused(){return m_hWnd == GetFocus();}

	//���ں�����WINAPI�е�һ������ΪHWND�ĺ�����
	//˵�������²�δ�������е�WINAPI����,�Ժ�������	
	BOOL IsVisible(){return IsWindowVisible(m_hWnd);}
	BOOL IsWindow(){return ::IsWindow(m_hWnd);}
	BOOL IsEnabled(){return IsWindowEnabled(m_hWnd);}
	BOOL UpdateWindow(){return ::UpdateWindow(m_hWnd);}

protected:
	//������Ϣ����
	static LRESULT CALLBACK WINAPI InitWndProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);
	
	virtual LRESULT RingdowProc(HWND,RINGPARAMS);
   virtual LRESULT DefaultProc(RINGPARAMS&) = 0;
	LRESULT CtlColorProc(HWND hWnd,RINGPARAMS& param);
	virtual LRESULT OnBkColor(UINT uMsg,HDC hDC){return RM_NOTPROCESS;}

	//ת����Ϣ���Ӵ���
	virtual void NotifyChild(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	BOOL NotifyChildDrawItem(HWND hParentWnd,HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	virtual BOOL OnParentNotify(HWND,UINT,UINT,LONG){return FALSE;}
	void OnNcDestroy();
	
	WNDPROC GetThunk() const{return m_thunk;}
	WNDPROC InitThunk();

   virtual HWND CreateEx(LPCTSTR szTitle,
   						 LPCTSTR szClass,
                      DWORD dwStyle,
                      DWORD dwExStyle = 0,
							 HWND hParentWnd = NULL,
                      HMENU hMenu = NULL,
   						 int x = CW_USEDEFAULT,
                      int y = CW_USEDEFAULT,
                      int width = CW_USEDEFAULT,
                      int height = CW_USEDEFAULT,
							 LPARAM lParam=0);
	
   friend RingCoolBar;
	friend RingFlyBar;
	friend RingDockBar;
	friend RingSizeBar;
	friend RingWnd;
	friend RingTab;
	
protected:	
   HWND m_hWnd;
	WNDPROC m_DefProc;
   RingBaseWnd* m_parent;
	RINGPARAMS m_param;
   int m_windowType;	//��������
	HBRUSH m_brush;

   friend RingdowApp;
	friend RingDialog;
	friend RingOFNHookDialog;

private:
	static LRESULT CALLBACK WINAPI stdProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);

private:
	WNDPROC m_thunk;
	DWORD bNeedDestroy;
	BOOL m_bKillOnExit;
	//WM_KEYPRESS��Ϣ��
	//BOOL m_bKeyDown;

	friend RingCtrlWnd;
};

typedef struct tagRingWndParam
{
	RingWnd* WndObj;
   LPARAM dwInitParam;
}RINGWNDPARAM,*LPRINGWNDPARAM;

#define CLIRECT_WITHSCROLL	TRUE
#define CLIRECT_NOSCROLL	FALSE

class RingWnd:public RingBaseWnd
{
public:
	RingWnd();
   virtual ~RingWnd()
	{
		UnInstallCoolMenu();
		Del(m_dockSite);
	}

	//�ڲ���������
	BOOL GetClientRect(LPRECT lprc);//,BOOL bWithScrollSize=CLIRECT_WITHSCROLL);
	
	//��������	
	virtual HICON SetIcon(HICON hIconBig,HICON hIconSmall = NULL);
   BOOL SetMenu(LPCTSTR lpszMenu,HINSTANCE hInst=GetInstance());
   BOOL SetMenu(HMENU hMenu);
	void SetCursor(HCURSOR hCur = NULL);

	//��������
	virtual HWND Create(LPCTSTR szTitle,LPCTSTR szClass,
								DWORD dwStyle,DWORD dwExStyle = 0,
								int x=CW_USEDEFAULT,int y=CW_USEDEFAULT,
								int width=CW_USEDEFAULT,int height=CW_USEDEFAULT,
								HWND hParentWnd=NULL,HMENU hMenu=NULL,LPARAM lParam=0);
	//virtual int Exit(int nResult=0){return DestroyWindow(m_hWnd);}
	virtual void SendExit(){SendMessage(m_hWnd,WM_CLOSE,0,0);}
	
	//����������
	int GetScrollPos(int nBar){return ::GetScrollPos(m_hWnd,nBar);}
	int SetScrollPos(int nBar,int nPos,BOOL bRedraw=TRUE)
	{
		return ::SetScrollPos(m_hWnd,nBar,nPos,bRedraw);
	}
	int SetScrollPosHV(int nPosX=-1,int nPosY=-1,BOOL bRedraw=TRUE);
	BOOL GetScrollRange(int nBar,LPSIZE lpSize);
	BOOL SetScrollRange(int nBar,int nPosMin,int nPosMax,BOOL bRedraw=TRUE)
	{
		return ::SetScrollRange(m_hWnd,nBar,nPosMin,nPosMax,bRedraw);
	}
	int SetScrollRangePage(int nBar,int nMin,int nMax,int nPage,BOOL bRedraw=TRUE);
	BOOL GetScrollInfo(int nBar,LPSCROLLINFO lpsi);
	int SetScrollInfo(int nBar,int nMask,int nMin,int nMax,int nPage,int nPos,
							int nTrackPos,BOOL bRedraw=TRUE);
	BOOL SetScrollInfo(int nBar,LPSCROLLINFO lpsi,BOOL bRedraw=TRUE);	
	int GetScrollLimit(int nBar);
	BOOL EnableScrollBar(int nBar,UINT wArrows)
	{
		return ::EnableScrollBar(m_hWnd,nBar,wArrows);
	}	

   RingCoolMenu* LoadCoolMenu(LPCTSTR lpszMenu,BOOL bPopup=TRUE,HINSTANCE hInst=GetInstance());
	RingCoolMenu* InstallCoolMenu(LPCTSTR lpszMenu=NULL,BOOL bPopup=TRUE,HINSTANCE hInst=GetInstance());
	RingCoolMenu* InstallXPMenu(LPCTSTR lpszMenu=NULL,BOOL bPopup=TRUE,HINSTANCE hInst=GetInstance());
	RingCoolMenu* GetCoolMenu(){return m_pInterMenu;}
	RingCoolMenu* GetDockMenu();
   void UnInstallCoolMenu()
   {
		delete m_pInterMenu;
      m_pInterMenu = NULL;
   }
	   
	virtual void InitRingSDKWindow(){}		//��ʼ����Ա���ݣ���������ʵ��
	virtual void UpdateToolBar(){}			//���¹���������������ʵ��

	BOOL IsClipBoradFormat(UINT format);
	BOOL IsClipBoradFormatText();

	//����ͣ��
	RingDockSite* GetDockSite(int index);
	RingDockSite* CreateDockSite(int index);

	BOOL SetDockBackGround(LPCTSTR lpszBmp,BOOL bSetOrg = FALSE,int index = -1,
							HWND hStandWnd=(HWND)0xFFFFFFFF,HINSTANCE hInst=GetInstance());
	BOOL SetDockBackGround(HBITMAP hbmp,BOOL bSetOrg = FALSE,int index = -1,HWND hStandWnd=(HWND)0xFFFFFFFF);
	BOOL SetDockBackGround(COLORREF crColor,int index = -1,HWND hStandWnd=(HWND)0xFFFFFFFF);
	//����ͣ����
	virtual void UpdateDockSite();

	LRESULT OnMenuSelect(HWND hStatusWnd,RINGPARAMS& param,HINSTANCE hInst=GetInstance());

	virtual DEFMSG(WM_RINGTOOLTIP)
	virtual DEFMSG(WM_MENUSELECT)

protected:
	//������Ϣ����
	//static LRESULT CALLBACK WINAPI InitWndProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);
	LRESULT DispMessage(RINGVCLASS*,RingMsgs*,HWND,RINGPARAMS&);
	LRESULT DispCommand(RINGVCLASS*,RingMsgs*&,HWND,RINGPARAMS&);
	LRESULT DispNotify(RINGVCLASS* obj,RingMsgs*& msgs,HWND hWnd,RINGPARAMS& param);

	void NotifyChild(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT NotifyChildProc(HWND,UINT,UINT,LPARAM);	
	virtual LRESULT RingdowProc(HWND,RINGPARAMS);
	virtual LRESULT DefaultProc(RINGPARAMS& param)
	{
		//��Ϊ�����û����HOOK�ػ񴴽������¼�����˰Ѵ��ڹ�������ΪRingdowProc��ʱ������ͺ�
		//����г���HOOK����ⴴ���ĳ��򣬲��ҰѴ��ڹ�������Ϊ�Լ��Ĵ��ڹ��̣����п����䴰��
		//�����ֱ������Ĵ��ڹ����滻�����Ϊ���ݸ������Ĭ����Ϣ��������ΪCallWindowProc��
		//����һ������RingSDK����HOOK�ĳ���һ��д���þ��п��ܵ��·Ƿ�������������в�������
		//��֪�����ǽ������ŵ㻹��ȱ����:)
		LRESULT res = 0;

		if(m_hWnd)
			res = CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
			//DefWindowProc(m_hWnd,param.uMsg,param.wParam,param.lParam);
		
		if(param.uMsg == WM_NCDESTROY)
			OnNcDestroy();
		
		return res;
	}
	
   virtual void OnCreate(HWND);
	void OnMeasureCoolMenu(LPMEASUREITEMSTRUCT lParam);
   void OnDrawCoolMenu(LPDRAWITEMSTRUCT lParam);
	BOOL Register(LPCTSTR szClass,
                 HBRUSH hbkg,
                 LPCTSTR lpszMenu = NULL,
                 HICON hBigIcon = NULL,
                 HCURSOR hCur = LoadCursor(NULL,IDC_ARROW),
                 HICON hSmallIcon = NULL,
					  int windowType = 0);

private:
	RingDockSite* GetFirstDockSite();
		
protected:
	RingMsgs* m_msgs;
	RingDockSite** m_dockSite;
	RINDOWINFO m_windowInfo;

	friend RingFlyBar;
	friend RingCoolBar;
	friend RingDockSite;
	friend RingDockBar;
	friend RingStatusBar;
	friend RingToolBar;

private:
	RingCoolMenu* m_pInterMenu;
};

//#define ACTIVECHILDWND(id,szTitle,szClass,ringvar)	\
//ActiveChild(&id,&id##_rdinfo,szTitle,szClass,ringvar)

#define DUMPRINGDOW(classid,id)	\
typedef RingWnd_##classid	RingWnd_##id;\
RingWnd_##id* id = NULL

/*****************************************************
//
//	MDI������
//
*****************************************************/
class RingMDIClient:public RingBaseWnd
{
public:
	RingMDIClient():m_extWnd(NULL),m_nFirstChildid(0),m_bChildMax(FALSE){}
	~RingMDIClient(){}
	
	HWND Create(CLIENTCREATESTRUCT* css);
	
	//�����ⲿ������MDICLIENT��HWND��WinMain�д���
	//��Ϣѭ����ҪTranslateMDISysAccel(hwndMDIClient,&msg)
	//��MDICLIENT���ڹر�ʱ����ⲿ��hwndMDIClient�޷�֪��
	//�Լ�����Ч����Ҫ���ý�����MDICLIENT�ر�ʱ�ø�
	//hwndMDIClientΪNULL��
	void SetMonExtClientWnd(HWND * extWnd)
	{m_extWnd = extWnd;}

	//���¸����ڲ˵���
	void RefreshMenu(RingCoolMenu* rm,HWND hChildWnd);
	void UpdateMenuBar(HWND hChildWnd);

protected:
	virtual LRESULT RingdowProc(HWND,RINGPARAMS);
	virtual LRESULT DefaultProc(RINGPARAMS& param)
	{
		return CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
	}

private:
	HWND* m_extWnd;
	HMENU m_parentMenu;
	int m_nFirstChildid;
	BOOL m_bChildMax;
};

class RingMDIFrameWnd:public RingWnd
{
public:
	RingMDIFrameWnd();
   virtual ~RingMDIFrameWnd();

   //void RegChild(LPRINGDOWMSGINFO lprmi){m_lpChildMsgInfo = lprmi;}
   BOOL CreateChild(LPCTSTR lpsztitle,RingWnd* child,BOOL bOpen=FALSE,
							DWORD dwStyle=0,LPARAM lParam=0,LPSTR szClass=NULL);

   virtual HWND Create(LPCTSTR szTitle,LPCTSTR szClass,
								DWORD dwStyle,DWORD dwExStyle = 0,
								int x=CW_USEDEFAULT,int y=CW_USEDEFAULT,
								int width=CW_USEDEFAULT,int height=CW_USEDEFAULT,
								HWND hParentWnd=NULL,HMENU hMenu=NULL,LPARAM lParam=0);
	void NotifyChild(HWND,UINT,UINT,LPARAM);

   inline HWND GetClient(){return m_client.Handle();}
	inline RingMDIClient* GetClientObj(){return &m_client;}
	RingMDIChildWnd* GetCurrChild(BOOL* bMaxMin=NULL);
	RingMDIChildWnd* GetMDIChild();
	RingMDIChildWnd* GetNextMDIChild(RingMDIChildWnd* child);
	HWND GetCurrChildWnd(LPBOOL bMax=NULL);
	//��ȡĬ�ϴ�����MDI�Ӵ����������Ӵ��ڴ���ʱ���ָ���˴���������������Ч
	LPCTSTR GetChildClassName(){return m_szChildClass;}

	//���º����������ã���Ϊ�����봰�ڹ�����CMDFUNC����һ�£�
	//��ʹ��Щ��������ΪCMDFUNC���뵽��Ϣӳ���б�
	virtual void OnCascadeChild(UINT){CascadeChild();}
	virtual void OnTileChildH(UINT){TileChild(MDITILE_HORIZONTAL);}
	virtual void OnTileChildV(UINT){TileChild(MDITILE_VERTICAL);}
	virtual void OnTileChild(UINT){TileChild();}
	virtual void OnCloseChild(UINT){CloseChild(GetCurrChildWnd());}
	virtual void OnArrangeIcon(UINT){ArrangeIcon();}
	virtual void OnNextChild(UINT){ActiveNextChild();}
	virtual void OnPrevChild(UINT){ActivePrevChild();}
	
	void CascadeChild(UINT flag=0/*MDITILE_SKIPDISABLED*/);
	void TileChild(UINT flag = MDITILE_VERTICAL);
	void CloseChild(HWND hWnd = NULL);
	void ActiveChild(HWND);
	//���º����������ã���Ϊ�����봰�ڹ�����CMDFUNC����һ�£�
	//��ʹ��Щ��������ΪCMDFUNC���뵽��Ϣӳ���б�
	void ArrangeIcon(UINT event=0);
	void MaxChild(UINT event=0);
	void MinChild(UINT event=0);
	void RestoreChild(UINT event=0);
	void ActiveNextChild(UINT event=0){SendMessage(m_client.Handle(),WM_MDINEXT,NULL,0);}
	void ActivePrevChild(UINT event=0){SendMessage(m_client.Handle(),WM_MDINEXT,NULL,1);}
	//ֻҪ�����Ӵ��ڣ���֤�л����
	BOOL KeepChildActive();

	//�����ⲿ������MDICLIENT��HWND��WinMain�д���
	//��Ϣѭ����ҪTranslateMDISysAccel(hwndMDIClient,&msg)
	//��MDICLIENT���ڹر�ʱ����ⲿ��hwndMDIClient�޷�֪��
	//�Լ�����Ч����Ҫ���ý�����MDICLIENT�ر�ʱ�ø�
	//hwndMDIClientΪNULL��
	void SetMonExtClientWnd(HWND * extWnd)
	{m_client.SetMonExtClientWnd(extWnd);}

	BOOL IsChildMax();

protected:
	void OnCreate(HWND);
	virtual LRESULT DefaultProc(RINGPARAMS& param)
	{		
		if(param.uMsg != WM_SIZE)
		{
			LRESULT res = DefFrameProc(m_hWnd,m_client.Handle(),param.uMsg,param.wParam,param.lParam);
			if(param.uMsg == WM_NCDESTROY)
				OnNcDestroy();
			return res;
		}
		else
			return CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
		//DefWindowProc(m_hWnd,param.uMsg,param.wParam,param.lParam);
	}
	
private:
	void FindWindowMenu(LPCLIENTCREATESTRUCT lpccs);
	//�˵�������
	void SetMenuBar(RingMenuBar* menubar){m_menuBar = menubar;}
	RingMenuBar* GetMenuBar();

private:	
	RingMDIClient m_client;
	int m_nChildid;
   char m_szChildClass[80];
	RingMenuBar* m_menuBar;
	friend class RingMDIClient;	
	friend class RingMenuBar;
};

class RingMDIChildWnd:public RingWnd
{
public:
	RingMDIChildWnd(){m_windowType = ITIS_MDICHILD;SetAutoDelete();}
	~RingMDIChildWnd(){}
	
	virtual HWND Create(LPCTSTR szTitle,LPCTSTR szClass,
								DWORD dwStyle,DWORD dwExStyle = 0,								
								int x=CW_USEDEFAULT,int y=CW_USEDEFAULT,
								int width=CW_USEDEFAULT,int height=CW_USEDEFAULT,
								HWND hParentWnd=NULL,HMENU hMenu=NULL,LPARAM lParam=0);

	//virtual int Exit(int nResult=0){return DestroyWindow(m_hWnd);}
	virtual int Close(int nResult=0){return SendMessage(m_hWnd,WM_CLOSE,0,0);}

protected:
	virtual LRESULT RingdowProc(HWND,RINGPARAMS);
	friend class RingWnd;

private:
	static LRESULT CALLBACK InitChildProc(HWND,UINT,UINT,LPARAM);
};

class RingDialog:public RingWnd
{
public:
	RingDialog();
	RingDialog(UINT id){m_windowType = ITIS_DIALOG;m_id=id;}
   virtual ~RingDialog();

	WNDPROC SubClassWindow(HWND hWnd);
	   
	void InitRingControls();
   //static BOOL CALLBACK CheckControlProc(HWND hWnd,LPARAM lParam);
   //static BOOL CALLBACK AddControlProc(HWND hWnd,LPARAM lParam);
   //RingControls* GetItem(UINT uId,DWORD dwStyle = 0);

	virtual void SetId(UINT uId=0){if(uId!=0)m_id=uId;}
   virtual int DoModal(LPCTSTR lpTemplate=NULL,LPARAM dwInitParam = 0,
							HWND hParentWnd = NULL,HINSTANCE hInst=GetInstance());
	HWND Create(LPCTSTR szTitle,LPCTSTR szClass,
					DWORD dwStyle,DWORD dwExStyle = 0,
					int x=CW_USEDEFAULT,int y=CW_USEDEFAULT,
					int width=CW_USEDEFAULT,int height=CW_USEDEFAULT,
					HWND hParentWnd=NULL,HMENU hMenu=NULL,LPARAM lParam=0);
	virtual HWND Create(LPCTSTR lpTemplate=NULL,LPARAM dwInitParam = 0,
								HWND hParentWnd = NULL,HINSTANCE hInst=GetInstance());
   virtual HWND CreateIndirect(LPCTSTR szCaption,DWORD dwStyle,
											int x,int y,int width,int height,
											HWND hParentWnd=NULL,LPARAM lParam=0);
	virtual int DialogIndirect(LPCTSTR szCaption,DWORD dwStyle,
											int x,int y,int width,int height,
											HWND hParentWnd=NULL,LPARAM lParam=0);
	int Close(int nResult=0);

	//HBRUSH SetBkgColor(COLORREF crColor);
	//HBRUSH SetBkgBitmap(HBITMAP hbm);
	HBRUSH SetBkgBitmap(UINT uId,HINSTANCE hInst = GetInstance())
	{return RingBaseWnd::SetBkgBitmap(uId,hInst);}
	//���öԻ��򷵻�ֵ
	LONG SetResult(LONG lRes){return SetWindowLong(m_hWnd,DWL_MSGRESULT,lRes);}
//	void SendExit(){PostMessage(m_hWnd,WM_CLOSE,0,0);}
	
	virtual LRESULT OnCtlColorDlg(HWND,RINGPARAMS&);
	virtual void OnClose(UINT event){Close();}

protected:	
	virtual LRESULT RingdowProc(HWND hWnd,RINGPARAMS);
	static LRESULT CALLBACK DefaultDlgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		return FALSE;
	}
	
protected:
	BOOL m_modal;	//ģ̬/��ģ̬���
	UINT m_id;
	//BOOL CALLBACK WINAPI OnClose(WPARAM,LPARAM);

private:
	static LRESULT CALLBACK WINAPI InitDlgProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);
	static void CreateTemplate(LPDLGTEMPLATE lpdt,LPCTSTR szCaption,DWORD dwStyle,int x,int y,int width,int height);
	friend class RingDockDlg;
};

//��/�����ļ��Ի���HOOK�Ի����װ
class RingOFNHookDialog:public RingDialog
{
public:
	RingOFNHookDialog(){m_thunkParent = m_OFNProc = NULL;}
	RingOFNHookDialog(UINT id){m_windowType=ITIS_DIALOG;m_id=id;m_thunkParent=m_OFNProc=NULL;}
   virtual ~RingOFNHookDialog(){m_OFNProc=m_thunkParent=(WNDPROC)Del((LPSTR)m_thunkParent);}

	LPOFNHOOKPROC GetHookProc(){return (LPOFNHOOKPROC)InitDlgProc;}

	//��Ϣ��װ
	LPCTSTR GetFilePath(LPTSTR szFile,int nSize)
	{
		if(szFile && SendMessage(m_hWndParent,CDM_GETFILEPATH,nSize,(LPARAM)szFile) > 0)
			return szFile;
		else
			return NULL;
	}
	LPCTSTR GetFolderPath(LPTSTR szFolder,int nSize)
	{
		if(szFolder && SendMessage(m_hWndParent,CDM_GETFOLDERPATH,nSize,(LPARAM)szFolder) > 0)
			return szFolder;
		else
			return NULL;
	}
	LPCTSTR GetSpec(LPTSTR szFile,int nSize)
	{
		if(szFile && SendMessage(m_hWndParent,CDM_GETSPEC,nSize,(LPARAM)szFile) > 0)
			return szFile;
		else
			return NULL;
	}
	void SetControlText(UINT uId,LPSTR szText)
	{
		SendMessage(m_hWndParent,CDM_SETCONTROLTEXT,uId,(LPARAM)szText);
	}

protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);

private:
	static LRESULT CALLBACK WINAPI InitDlgProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);
	static LRESULT CALLBACK WINAPI OFNDlgProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);

private:
	WNDPROC m_thunkParent,m_OFNProc;
	HWND m_hWndParent;
};

class RingCtrlWnd:public RingBaseWnd
{
public:
	RingCtrlWnd():m_ID(0),m_ExStyle(0){SetAutoDelete(TRUE);}
   virtual ~RingCtrlWnd() = 0;
	
	UINT GetId(){return m_ID;}
	UINT ReScanId(){return m_ID = GetWindowLong(m_hWnd,GWL_ID);}
	
	BOOL IsControls(){return TRUE;}

protected:
	//void OnParentNotify(HWND,UINT,UINT,LONG){}
	LRESULT DefaultProc(RINGPARAMS&);
	
	HWND Create(LPCTSTR szTitle,
		LPCTSTR szClass,
		DWORD dwStyle,
		HWND hParentWnd = NULL,
		DWORD dwExStyle = 0,		
		HMENU hMenu = NULL,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = CW_USEDEFAULT,
		int height = CW_USEDEFAULT);

protected:
   UINT m_ID;
   DWORD m_ExStyle;

	friend RingCoolBar;
	
private:   
//   static RingdowList m_childList;
//	friend RingAppWnd;
//	friend RingdowList;
};



/******************************************************************
//
//���ٴ��������
//	ע�⣺obj��������ԭʼ���ͣ�����Ϊ��������ʱ������ǿ������ת��
//
******************************************************************/
template<typename tname>
void DelRingObj(tname*& obj)
{
	try
	{
		delete obj;
	}
	catch(...)
	{
	}
	obj = NULL;
}

#endif
