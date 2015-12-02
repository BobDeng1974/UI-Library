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
//ԭ�ļ�����ringtab.h
//
//���ļ�˵����RingTab������
//
//��⹦�ܣ�Tab�ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGTAB
#define _INC_RINGTAB

typedef struct _tagRingTabItemData
{
	UINT uType;
	RingBaseWnd* obj;
}RINGTABITEMDATA,*LPRINGTABITEMDATA;

class RingTab:public RingControls
{
public:
	RingTab();
	virtual ~RingTab();

	//����/��������
   BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);

	LPCTSTR WndClassName(){return WC_TABCONTROL;}
	void SetExStyle(DWORD dwExStyle);

	BOOL InsertTab(LPCTSTR szText,int subindex,UINT uidIcon=-1);
	//������tab,szTabs��ʽ��"firstname|secondname|..."
	BOOL InsertTabs(LPCTSTR szTabs);
	BOOL DeleteTab(int iCol);
	
	int AddIcon(UINT uidIcon,HINSTANCE hInst=GetInstance());
	int AddIcon(HICON hicon);
	BOOL SetIcon(int indexTab,int indexIcon);
	
	//��TAB��ָ������
	BOOL SetTabToWnd(int iCol,RingBaseWnd* wnd);

	int GetItemCount(){return TabCtrl_GetItemCount(m_hWnd);}
	//�õ�ָ��TAB�󶨵�����
	LONG GetItemData(int iCol);
	int GetCurSel(){return TabCtrl_GetCurSel(m_hWnd);}
	int Select(int iCol);

	BOOL GetClientRect(LPRECT lprc);

	virtual LRESULT OnTabSelect(RINGPARAMS& param);

	//�麯������
	HDWP DefDeferWindowPos(HDWP,int,int,int,int,UINT uAppendFlag=0);	
	BOOL Attach(UINT uId,RingBaseWnd* parent=NULL){return RingControls::Attach(uId,parent);}
	BOOL Attach(HWND);
	RingDockBar* EnableDock(LPCTSTR szTitle,UINT uDockStyle = TBS_EX_DOCKABLE,
		UINT uDockFlag = TBS_FLAG_DOCKTOP,LPRECT lprcPos = NULL);
	RingDockBar* EnableDockSize(LPCTSTR szTitle,int nWidth,int nHeight,
		UINT uDockFlag = TBS_FLAG_DOCKLEFT,UINT uDockStyle = TBS_EX_DOCKABLE);

	//��Ϣ��װ
	int GetRowCount(){return TabCtrl_GetRowCount(m_hWnd);}
	int GetCount(){return TabCtrl_GetItemCount(m_hWnd);}
	BOOL GetItemRect(int nIndex,LPRECT lprc);

protected:
	//LRESULT DefaultProc(RINGPARAMS&);
	LRESULT RingdowProc(HWND,RINGPARAMS);
	LRESULT OnDestroy(RINGPARAMS& param);
	virtual LRESULT OnPaint(RINGPARAMS& param);
	LRESULT OnDefNotify(RingBaseWnd* w,RINGPARAMS& param);
	virtual BOOL OnDrawBkg(HDC hDC);
	//�ͷ��ڲ��󶨵�itemdata
	void DelBindWndData();

	int GetItemText(int index,LPTSTR lpBuf,int size);
	
	void CheckReposChild();

protected:
	HIMAGELIST m_hImage;
	//��ǰ��ʾ�Ĵ��ڣ�BindWndToTab����Ч��
	RingBaseWnd* m_CurrTabWindow;
	RingDockBar* m_dockbar;
	int m_nCheckPos;
};
#endif
