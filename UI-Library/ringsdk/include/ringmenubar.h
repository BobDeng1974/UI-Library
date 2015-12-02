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
//ԭ�ļ�����ringmenubar.h
//
//���ļ�˵����RingMenuBar������
//
//��⹦�ܣ��˵������ڷ�װ
//
**********************************************************************/
#ifndef _INC_RINGMENUBAR
#define _INC_RINGMENUBAR

//�Ƿ�ѡ�в˵����ϵ�ϵͳͼ�꣬��С������ԭ���رհ�ť
//���棺���¶���ֵ���ɸ���
#define MBAR_SYSICONSEL		996
#define MBAR_SYSMINSEL		997
#define MBAR_SYSRESTORESEL	998
#define MBAR_SYSCLOSESEL	999
//�������

typedef struct tagRingMenuBarItem
{
	RECT m_rect;
	int m_state;
	LPTSTR m_text;
	int m_id;
	DWORD m_data;
}RINGMENUBARITEM,*LPRINGMENUBARITEM;

class RingMenuBar:public RingCtrlWnd
{
public:
	RingMenuBar();
	~RingMenuBar();
	
	BOOL Create(LPCTSTR szTitle,int dockStyle=TBS_EX_DOCKABLE,
					int dockState=TBS_FLAG_DOCKTOP);
	BOOL LoadMenu(HMENU hMenu=NULL,BOOL bRemove=TRUE);
	BOOL LoadMenu(RingCoolMenu* rm,BOOL bRemove=TRUE);
	//void PressButton(UINT uID,BOOL bPress,BOOL bCheckSysBtn);
	
	void PressButton(int index,BOOL bPress=TRUE);
	int GetButtonCount(){return m_nBtnCount;}
	BOOL GetButtonRect(int index,LPRECT lprect);

	void SetSysControlInfo(BOOL m_bChildMax);
	RingCoolMenu* GetCoolMenu(){return m_rm;}

	//HDWP DefDeferWindowPos(HDWP,int,int,int,int,UINT uAppendFlag=0);

protected:
	BOOL AddButton(LPCTSTR szText,int state,DWORD dwData);
	BOOL InsertButton(int index,LPCTSTR szText,int state,DWORD dwData);

	BOOL AllocMenuItems(int cnt);
	void FreeMenuItems();
	
	void Update(int state=-1);
	BOOL SetHotButton(int index);

	LRESULT RingdowProc(HWND,RINGPARAMS);
	LRESULT OnLButtonDown(RINGPARAMS& param);
	LRESULT OnMouseMove(RINGPARAMS& param);
	LRESULT OnLButtonUp(RINGPARAMS& param);
	LRESULT OnRButtonDown(RINGPARAMS& param);
	LRESULT OnTimer(RINGPARAMS& param);
	LRESULT OnPaint(RINGPARAMS& param);
	LRESULT OnNotify(RINGPARAMS& param);
	LRESULT OnHitTest(RINGPARAMS& param);
	//LRESULT OnWinPosChanged(RINGPARAMS& param);
	LRESULT OnSizing(RINGPARAMS& param);

	BOOL OnParentNotify(HWND,UINT,UINT,LONG);
	
	BOOL OnGetDragRects(int state,LPRECT lprc);
	//ͣ��״̬����Ӧͣ��״̬�ı��¼�������-1��δ�ı䣬TRUE���Ѵ���0��δ����
	int OnDockStateChange(int stateOld,int stateNew);
	//void ChangeState(int state,RingDockSite* docksite=NULL);

	BOOL MakeMenuButtons();
		
	void DrawExtButton(HDC hdc);
	//bCheckAll���Ƿ���ж�ϵͳͼ��,�ò���Ĭ��ֵ�������׸���
	int CheckInExtButton(LPPOINT lpMousePt,BOOL bCheckAll = FALSE);
	void PressExtButton(UINT uID,BOOL bPress);
	
	int GetBtnsWidth();
	int GetBtnsHeight();
	
	//��ť���ǰ����չ�ߴ磨��ͼ�꣬ϵͳ��ť�Ƿ���ʾ�йأ�
	int EndSize(){return m_bShowIcon?50:0;}
	int PreSize(){return m_bShowIcon?18:0;}
		
private:
	BOOL InterLoadMenu(BOOL bRemove);
	//��ȡ���в˵������
	int GetVertLineWidth();
	//��ȡ���в˵����߶�
	int GetHorzLineHeight();

private:
	RingCoolMenu* m_rm;
	RingMenu* m_rmsys;
	ringFont m_font,m_vfont;
	HICON m_sysIcon;
	HBITMAP m_hbmSys;
	int m_nPopupedMenu,m_nHotButton;
	BOOL m_bShowIcon;
	LPRINGMENUBARITEM m_item;
	int m_nBtnCount,m_BtnAllocCnt;
	//���꣺�رհ�ť����С����󻯰�ť��������
	RECT m_rcSysBtn,m_rcRestore,m_rcIcon;
	//ͣ��/����״̬�µĿ��,��ť������ʱ�������,ʵ�ʴ��ڿ�ߣ�����/�п�
	SIZE m_BarSize,m_fSize,m_BtnsSize,m_WndSize,m_SingleSize;
	int m_nMinWidth;	//��С���
	//����Ƿ��ڹرհ�ť����С����󻯰�ť��������������
	DWORD m_nCapedExtButton;
	RingDockBar* m_dockbar;
};


#endif
