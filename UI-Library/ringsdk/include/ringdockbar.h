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
//ԭ�ļ�����ringctrlbar.h
//
//���ļ�˵����RingDockBar������
//
//��⹦�ܣ����������ڷ�װ
//
**********************************************************************/
#ifndef _INC_RINGCTRLBAR
#define _INC_RINGCTRLBAR

#define TB_NEEDCHANGECTRLBAR	50001				//��Ҫ����RingDockBar
#define TB_NOCHANGECTRLBAR		0					//����Ҫ����RingDockBar

#define RINGCTRL_IDBASE			0xC100			//������ID

//����RingDockSite����
#define RBIMG_HBITMAP			1					//��HBITMAP
#define RBIMG_IDBMP				2					//��BMP��ԴID
#define RBIMG_BKCOLOR			3					//��COLORREF

//���λ�ã��رհ�ť�����/С����ť������λ�ã�
#define RCUR_INCLOSE			(1)
#define RCUR_INMINMAX		(2)
#define RCUR_NOWHERE			(3)

//������ID
extern DWORD gdwDockBarId;

class RingDockBar;

//�������϶��ĸ�����
class RingDragger
{
public:
	RingDragger():m_bDragging(FALSE){}
	~RingDragger(){}

	void StartDrag(HWND hWnd,int xMouse,int yMouse,LPRECT lprc,int width);
	//�����ɾ��Σ�������λ���¾��Σ����������¾��Σ��ṩ�����
	//�������ж���λ����¾��λ���д���
	//ע�ⷵ�ص�RECT����Ļ���ꡣlprc����hWnd���������
	LPRECT EraseRect(int xMouse,int yMouse,HWND hWnd=NULL,LPRECT lprc=NULL);
	//���Ƶ�ǰ����
	//ע�⣺������ƺ�m_DragRcλ��û�б䶯�����ٴε���Ϊ��������
	void DrawRect(int width);
	//��ָ����Ȼ���ָ���ľ���
	void DrawRect(LPRECT lprc,int width);
	void Drag(int xMouse,int yMouse);
	//����hWnd���������
	LPRECT StopDrag(HWND hWnd,int xMouse,int yMouse);

	inline BOOL IsDragging(){return m_bDragging;}
	LPRECT GetCurrRect(HWND hWnd = NULL,LPRECT lprc = NULL);
	//�����������λ�ã�����ʵ�ʸı����λ�ã�
	//������������Ϊ�ı��϶�ʱ��ƫ����
	void SetPointX(int x){m_pt.x = x;}
	void SetPointY(int y){m_pt.y = y;}

	//�����϶����Σ����hWnd�ǿգ�lprc��Ϊ�����hWnd�����꣬
	//ת��lprc����Ļ����
	void SetDragRect(LPRECT lprc,HWND hWnd/*=NULL*/);

private:
	BOOL m_bDragging;
	int m_width;
	HDC m_DC;
	POINT m_pt;
	RECT m_DragRc;
};

class RingFlyBar:public RingWnd
{
public:
	RingFlyBar(RingBaseWnd* parent);
   ~RingFlyBar();

   BOOL Create(LPCTSTR lpszTitle);
   
   BOOL OnParentNotify(HWND,UINT,UINT,LONG);
	void AddChild(RingDockBar* child);

	void Update();
	void UpdateDockSite();	//�麯������
	RingDockBar* GetDockBar(){return m_child;}

protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);
	BOOL DrawBkg(HDC hDC,LPRECT lprc = NULL);

private:
	//ǿ��ֻ�ɵ��ø��ƹ��캯����������
	RingFlyBar(){}

private:
	RingDockBar* m_child;
	RingDragger* m_dragger;
   RECT m_parentRc,m_DragRc;
   BOOL m_bDrag;
   POINT m_CurPos;
   int m_CurrState;
   int m_cy;
};

typedef struct tagRingBarLineInfo
{
  	RingDockBar* m_first;
	int m_maxSize,m_pos;//�и�/����ʼλ��	
	RECT m_SplitRc;	//SizeBar�ķָ���
	WORD isVisible;
	short m_nSizeBarCnt;
   tagRingBarLineInfo *m_nextline;   
}RINGBARLINEINFO,*LPRINGBARLINEINFO;

typedef struct tagmBmpInfo
{
  	int cx;
   int cy;
	BOOL bSetOrg;			//�Ƿ���Ҫ�����洰���ƶ�
   BOOL bNeedDelete;		//����ʱ�Ƿ���Ҫ����hBrush
	HBRUSH hBrush;
	HWND hStandWnd;		//�������������ô��ڼ���
}BARBMPINFO,*LPBARBMPINFO;

#define RING_SPLITERSIZE 5
	
class RingDockSite:public RingWnd
{
public:
	RingDockSite(RingBaseWnd* parent,DWORD state/*=TBS_FLAG_DOCKTOP*/);
   ~RingDockSite();

	HWND Create();
	   	
   void SetBgBmp(HBITMAP hbm,BOOL bSetOrg = FALSE);
   void SetBgBmp(LPCTSTR lpszBmp,BOOL bSetOrg = FALSE,HINSTANCE hInst=GetInstance());
	void SetBgColor(COLORREF crColor);
	//���ñ������λ�ñ�׼�Ĵ��ڣ���������Ըô��ڶ���
	void SetStandWindow(HWND hWnd){m_Background.hStandWnd = hWnd;}
	
	//���Ʊ���ͼ����lprcΪ���Ʒ�Χ,lpoffsetΪ��ˢԭ��ƫ������
	//����б���ͼ��������TRUE������FALSE
	BOOL DrawBkg(HDC hDC,LPRECT lprc=NULL,LPSIZE lpoffset=NULL);
	
	LPBARBMPINFO GetBgInfo(){return &m_Background;}

	RingDockBar* CreateDockBar(RingBaseWnd* wnd,BOOL bCanSize);
	BOOL ReleaseDockBar(RingBaseWnd* wnd);
	BOOL AddBar(RingDockBar* dockbar,LPRECT lprc);
	RingDockBar* AddBar(RingBaseWnd* wnd,LPCTSTR szTitle,UINT uDockStyle=TBS_EX_DOCKABLE,
								UINT uDockFlag=TBS_FLAG_DOCKTOP,LPRECT lprcPos=NULL,
								BOOL bCanSize = FALSE);
	BOOL DelBar(RingDockBar* dockbar,BOOL bUpdate=TRUE);
	BOOL HideBar(RingDockBar* dockbar);

	BOOL IsVert(){return (m_State > TBS_FLAG_DOCKBOTTOM);}
	BOOL IsHorz(){return (m_State < TBS_FLAG_DOCKLEFT);}

	RingDockBar* GetDockBar(RingBaseWnd* wnd);
	RingDockBar* GetDockBar(UINT uDockBarId);
	//������DockSite��Ѱ��DockBar
	RingDockBar* FindDockBar(UINT uDockBarId);
	RingCoolMenu* GetDockSiteMenu();

	int GetState(){return m_State;}
	//��������DockSite
	void UpdateAllSite();

protected:
	BOOL OnParentNotify(HWND,UINT,UINT,LONG);
	void NotifyChild(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	LRESULT RingdowProc(HWND,RINGPARAMS);
	
	//�Ӽ�nAddSize���ڳߴ�
	void UpdatePos(int nAddSize,BOOL bUpdate = TRUE);
	//����λ��lpline����ı��߿�λ��,nStepΪ�����ĳߴ�
	void UpdateLine(LPRINGBARLINEINFO lpline,int nStep,BOOL bUpdate = TRUE);
	
	LRESULT OnLButtonDown(RINGPARAMS& param);
	LRESULT OnLButtonUp(RINGPARAMS& param);
	LRESULT OnMouseMove(RINGPARAMS& param);
	LRESULT OnRButtonDown(RINGPARAMS& param);
	LRESULT OnLButtonDbClk(RINGPARAMS& param);
	
	BOOL CheckMouseInSpliter(RINGPARAMS& param);
	RingDockBar* CheckMouseInDockBar(RINGPARAMS& param);
	
	BOOL ParentReSize(int width,int height);
	//void AdjustChildren(int width,int height);
	void AdjustChildrenH(int nEdge);
	void AdjustChildrenV(int nEdge);
	
private:
	//ǿ��ֻ��ͨ�����ƹ��캯����������
	RingDockSite(){}
	void Free();
	
	//��������DockSite���������ٻ�ˢʱ����
	void UpdateAllSiteBkg(HBRUSH hBrush);
	//ˢ�±�����ˢ������ͣ������
	void UpdateBkg();
	//����ϵͳ�˵����������б�˵���
	BOOL UpdateSysMenu(RingDockBar* dockbar,LPCTSTR szText);
	void RemoveSysMenu(RingDockBar* dockbar);
	//��Ӧ�˵���Ϣ������/��ʾ������
	BOOL OnDockBarHide(UINT uId);
	//��DockBar֪ͨ�����²˵�
	void OnDockBarHide(RingDockBar* dockbar);
	//�϶��ָ���
	void OnDragSpliter(RINGPARAMS& param);

	//�õ�dockbar�����е�ǰһ�С�
	LPRINGBARLINEINFO GetPrevLine(LPRINGBARLINEINFO lpline);
	//�õ�dockbarͣ���У������϶�������ʱ�ж�ʹ�á�
	//bLast:�Ƿ�ӵ����
	LPRINGBARLINEINFO GetDockLine(int nPos);//,BOOL &bLast);

	//dockbar���뵽��
	int AddBarInLine(RingDockBar* dockbar,LPRINGBARLINEINFO lpline,LPRECT lprc=NULL);
	//����insbar��dockbarǰ�棬�����룬�����κ�λ�õ���
	void InsertDockBar(LPRINGBARLINEINFO lpline,RingDockBar* insbar,RingDockBar* dockbar);
	
	//ɾ��������
	BOOL DelLine(LPRINGBARLINEINFO lpline);
	
	BOOL AddBarInLineH(RingDockBar* dockbar,LPRINGBARLINEINFO lpline,LPRECT lprc,int checkpos);
	BOOL AddBarInLineV(RingDockBar* dockbar,LPRINGBARLINEINFO lpline,LPRECT lprc,int checkpos);

	LPRINGBARLINEINFO AddLine(LPRINGBARLINEINFO prev);
	void LinkLine(LPRINGBARLINEINFO prev,LPRINGBARLINEINFO next);

	//int AdjustBarLineV(RingDockBar* bar,int nSpace,RingDockBar* dockbar,int& nSize);
	//int AdjustBarLineH(HDWP hdwp,int nSize,LPRINGBARLINEINFO lpLine,RingDockBar* dockbar,BOOL bCanWrap=FALSE);
	HDWP BarLineResizeH(HDWP hdwp,LPRINGBARLINEINFO lpLine,int nSize);
	HDWP BarLineResizeV(HDWP hdwp,LPRINGBARLINEINFO lpLine,int nSize);
	int WrapLine(RingDockBar* bar);
	
	int BarOutLine(RingDockBar* dockbar,BOOL bCalcSize,LPRINGBARLINEINFO& line);
	BOOL IsSizeBarInLine(LPRINGBARLINEINFO line){return line?(line->m_SplitRc.right>0):FALSE;}
	BOOL SetSizeSplitRect(LPRINGBARLINEINFO line);

	LPRINGBARLINEINFO GetEmptyLine();
	void SetLineEmpty(LPRINGBARLINEINFO line);
	void RegFlyBar(RingDockBar* dockbar);
	void UnregFlyBar(RingDockBar* dockbar);

	//����ˢ��
	void DrawSite(HDC hDC,RINGPARAMS& param);

public:
	int m_SplitterSize;

protected:
	RingDragger m_dragger;

private:
	static LPRINGBARLINEINFO m_EmptyLine;
	static RingDockBar* m_flybar;
	LPRINGBARLINEINFO m_First,m_Curr;
	
	int m_State;
	int m_nChild;
	RECT m_rcPos;
	BARBMPINFO m_Background;
	BOOL m_bSPLimited;	//�����������Ƿ��ѵ�����λ���޷��ƶ�
	HCURSOR m_cursor,m_cursorLine;
	POINT m_ptLimit;		//����������ʱ�����˼���λ��
	RingDockBar* m_CaptureBar;
	
	friend RingFlyBar;
	friend RingDockBar;
	friend RingSizeBar;
};

class RingDockBar
{
public:
	RingDockBar();
	virtual ~RingDockBar();

	UINT GetId(){return m_id;}
	RingBaseWnd* GetChild(){return m_child;}
	RingBaseWnd* GetDockSite(){return m_pSite;}
	//��������ڸ���������
	BOOL GetDockRect(LPRECT lprc);
	void SetDockStyle(UINT style);
	void SetBarBorder(int nBorder){m_BorderSpace = nBorder;}
	BOOL SetDockTitle(LPCTSTR lpszTitle);
	LPCTSTR GetDockTitle(){return m_szTitle;}

	int GetState(){return m_State;}
	DWORD GetBarStyle(){return m_uStyle;}
	int GetBarBorder(){return m_BorderSpace;}
	BOOL IsVert(){return ::IsVert(m_State);}
	BOOL IsHorz(){return ::IsHorz(m_State);}
	BOOL IsFloat(){return ::IsFloat(m_State);}

	virtual BOOL Dockto(int state,LPRECT lprc=NULL);
	BOOL DockFly(LPRECT lprc=NULL);
	int DockBack();
	virtual void Show(int nState);

	void UpdateSize();
	void UpdateClientSize();
	void UpdateSize(int nWidth,int nHeight,BOOL bUpdate);
	virtual void UpdateFlySize(int nWidth,int nHeight);
	void UpdateFlyBkg();
	//��������DockBar
	void UpdateAllSite();
	//BOOL GetMaxClientRect(LPRECT lprcClient);

	virtual BOOL IsMenuBar(){return FALSE;}
	virtual BOOL IsSizeBar(){return FALSE;}
	BOOL IsStatusBar(){return m_child?(m_child->GetWindowType()==ITIS_STATUSBAR):FALSE;}
	BOOL IsVisible(BOOL *lpbUpdate=NULL);
	//�Ƿ��ռһ��
	BOOL IsAllLineBar(){return (m_uStyle & TBS_EX_ALLLINE);}
	BOOL CanDock(int nState){return (m_uStyle & (1 << nState));}

	//���ݿ�߲������ؿͻ������
	int GetClientWidth(int width);
	int GetClientHeight(int height);

	virtual void InitDockSize(){}
	virtual UINT HitTest(POINT* lpt);

	BOOL DrawBkg(HDC hDC,LPRECT lprc=NULL);

protected:
	BOOL SetChild(RingBaseWnd* child);

	virtual LRESULT OnClose();
	virtual LRESULT OnSizing(RINGPARAMS& param);
	//virtual LRESULT OnWinPosChanged(RINGPARAMS& param);
	
	void AdjustSizingRect(int flag,LPRECT lprc,RECT& rc);
	
	virtual LRESULT StartDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param);
	virtual LRESULT OnDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param);
	virtual LRESULT StopDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param);
	virtual void StartPushFrameControl(HWND hWnd,RingDragger* dragger,RINGPARAMS& param);
	virtual void OnPushHoverFrameControl(HWND hWnd,RingDragger* dragger,LPPOINT lpt);
	virtual void StopPushFrameControl(HWND hWnd,RingDragger* dragger,LPPOINT lpt);

	virtual BOOL SetDockSite(RingDockSite* docksite);
	//ͣ��״̬�ı��¼���Ĭ�ϴ�������-1��δ�ı䣬TRUE���Ѵ���0��δ����
	virtual void OnDockStateChange(int stateOld,int stateNew){}

	//DockBar�϶�ʱ�ж��϶��ľ����Ƿ�Ҫ�ı���״
	//lprc,pt��Ϊ��Ļ����,
	//w,h:����������״̬�Ŀ�����״̬�ĸߣ�������״̬�¿�ߵ���С�ߴ磩
	int CheckFloatRect(HWND hWnd,LPRECT lprc,POINT pt);//,int w,int h);
	//CheckFloatRect���ã�����draggerʹ�þ��Σ����ؾ��α߿���
	virtual int SetDragRect(HWND hWnd,RingDragger* dragger);

	virtual void DrawGripper(HDC hDC);
	virtual void DrawFrame(HDC hDC);
	virtual void UpdateCaption();
	BOOL Draw(HDC hDC);
	
	//�õ��϶�����
	virtual LPRECT GetDragRects(int state);
	virtual BOOL GetClientRect(int state,LPRECT lprc);
	virtual void InitDragRects(HWND hWnd,POINT pt,BOOL bInit);
	void InitAdjustRects(POINT& pt,LPRECT lprcCheck);
	void AdjustDragRects(POINT& pt,LPRECT lprcCheck);
	void AdjustRects(HWND hWnd,POINT pt,int state);
	
	BOOL SetDockPos(int x,int y,int width=-999,int height=-999);
	void SetDockPos(HDWP hdwp);
	void OffsetDockPos(int x,int y);

	virtual int GetPreSpace();
	virtual HDWP AdjustDockPosH(HDWP hdwp,int nEdge,int nPreSpace,BOOL bCanWrap);
	virtual HDWP AdjustDockPosV(HDWP hdwp,int nEdge,int nPreSpace,BOOL bCanWrap);

	virtual BOOL AdjustChildH(int);
	virtual BOOL AdjustChildV(int);

private:
	BOOL AdjustSizeChildH();
	BOOL AdjustSizeChildV();

protected:	
	UINT m_uStyle,m_id;
	int m_CurWhere;	//���λ�ںδ���
	int m_State,m_currState;
	RingBaseWnd* m_child;
   RingDockSite* m_pSite;
	RingFlyBar* m_flyBar;
	RingDockBar* m_next,*m_prev;
	int m_nSize;	//խ�߳��ȣ���ͣ��ʱ����ĸ߶ȣ�����Ŀ��
	LPRINGBARLINEINFO m_lineInfo;
	//�������򣬸���״̬�ľ��Σ��϶�ʱ�ã������ĸ������Σ�ͣ�����Σ�DockBack�ã�
	RECT m_hrc,m_vrc,m_frc,m_lfrc,m_ldrc;	
	int m_BorderSpace;
	int m_GripperSpace;
	//����Ƿ��ڹرհ�ť����С����󻯰�ť��������������
	int m_bNCBtDown;
	//���꣺�رհ�ť���ͻ���
	RECT m_rcClose,m_rcClient;
	RECT m_rcDockPos,m_rcCurr;
	LPSTR m_szTitle;	//���ڱ���
	int m_nTitleSize;	//���ڱ��⻺�����ߴ�
	BOOL m_bVisible;


	friend class RingDockSite;
	friend class RingFlyBar;
	friend class RingMenuBar;
	friend class RingSizeBar;	
};

typedef DWORD (RingDockBar::*CHECKPOSFUNC)(LPRECT lprc);

class RingSizeBar:public RingDockBar
{
public:
	RingSizeBar();
	virtual ~RingSizeBar();
	
	BOOL IsSizeBar(){return TRUE;}
	void InitDockSize();
	void Show(int nState);
	UINT HitTest(POINT* lpt);

	void UpdateFlySize(int nWidth,int nHeight);

protected:
	void DrawGripper(HDC hDC);
	void UpdateCaption();
	
	BOOL GetClientRect(int state,LPRECT lprc);

	void StartPushFrameControl(HWND hWnd,RingDragger* dragger,RINGPARAMS& param);
	void OnPushHoverFrameControl(HWND hWnd,RingDragger* dragger,LPPOINT lpt);
	void StopPushFrameControl(HWND hWnd,RingDragger* dragger,LPPOINT lpt);
	void OnMinMax();

	//LRESULT OnSizing(RINGPARAMS& param);
	int GetPreSpace();
	HDWP AdjustDockPosH(HDWP hdwp,int nEdge,int nPreSpace,BOOL bCanWrap);
	HDWP AdjustDockPosV(HDWP hdwp,int nEdge,int nPreSpace,BOOL bCanWrap);

	BOOL AdjustChildH(int nAddSpace);
	BOOL AdjustChildV(int nAddSpace);

	//ͣ��״̬�ı��¼���Ĭ�ϴ�������-1��δ�ı䣬TRUE���Ѵ���0��δ����
	void OnDockStateChange(int stateOld,int stateNew);
	//���غ����������϶����Σ����ؾ��ο��
	int SetDragRect(HWND hWnd,RingDragger* dragger);

protected:	
	HFONT m_font,m_vfont;
	//���꣺�رհ�ť����С����󻯰�ť�����������϶�ʱ����ͣ������
	RECT m_rcMinMax,m_rcCaption,m_rcAllLine,m_rcSpliter[2];	
	//���/С����־,����״̬�¿�/��
	int m_nZoom,m_nRestoreSize;
};

//����ͣ�����������˵�����
class RingAllLineBar:public RingDockBar
{
public:
	RingAllLineBar();
	virtual ~RingAllLineBar(){}
	
	BOOL IsMenuBar(){return TRUE;}
	
protected:
	//���غ����������϶����Σ����ؾ��ο��
	int SetDragRect(HWND hWnd,RingDragger* dragger);
	LRESULT StopDrag(HWND hWnd,RingDragger* dragger,RINGPARAMS& param);
	//void DrawFrame(HDC hDC);

private:
	BOOL ResetDragRect(LPRECT lprc,int state);

protected:	
	BOOL m_bSetH,m_bSetV;
};

#endif
