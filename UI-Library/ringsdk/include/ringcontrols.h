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
#ifndef _INC_RINGCONTROLS
#define _INC_RINGCONTROLS

#define RINGTIMER_BASE		50
#define RINGITEMSTYLE_FLAT		29001
#define RINGITEMSTYLE_NORMAL	29002
//����Ӧ�����ڳߴ�
#define RTV_FITPARENTSIZE	0x1

typedef void (RINGVCLASS::*DRAW_FUNC1)(HDC,LPRECT);

#define CAN_OWNER_DRAW()\
private:\
	void OnMouseOverDraw(HDC,LPRECT);\
	void OnMouseOutDraw(HDC,LPRECT);\
	void OnFocusDraw(HDC,LPRECT);\
	void OnDrawScrollBar(HDC,LPRECT);\
public:\
	void SetOwnerDraw()\
	{\
		m_drawFuncs.mouseoverFunc = (DRAW_FUNC1)OnMouseOverDraw;\
		m_drawFuncs.mouseoutFunc = (DRAW_FUNC1)OnMouseOutDraw;\
	}

#define CR_NORMAL				1
#define CR_FLATSET			2
#define CR_FOCUSSET			4
#define CR_FLATBORDERSET	8
#define CR_FOCUSBORDERSET	16

typedef struct _tagBorderColor
{
	COLORREF crFlat_lefttop;
	COLORREF crFlat_rightbottom;
	COLORREF crFocus_lefttop;
	COLORREF crFocus_rightbottom;
	COLORREF crFlat_border;
	COLORREF crFocus_border;
	COLORREF crBkg;
	DWORD		crFlag;
}BORDERCOLOR,*LPBORDERCOLOR;

enum
{
	RC_LEFT,
	RC_RIGHT,
	RC_TOP,
	RC_BOTTOM,
	RC_LEFTTOP,
	RC_RIGHTBOTTOM,
	RC_RECT
};

class RingOwnerDrawer
{
public:
	RingOwnerDrawer(){};
	virtual ~RingOwnerDrawer(){};
/*
	virtual void OnNcPaint(HDC,LPRECT){}
	virtual void OnMouseHover(HDC,LPRECT){}
	virtual void OnMouseLeave(HDC,LPRECT){}
	virtual void OnSetFocus(HDC,LPRECT){}
	virtual void OnKillFocus(HDC,LPRECT){}
	virtual void OnLButtonDown(RINGPARAMS& param){}
	virtual void OnLButtonUp(RINGPARAMS& param){}
	virtual void OnMouseMove(RINGPARAMS& param){}
*/
	virtual LRESULT CALLBACK DrawProc(RingControls*,RINGPARAMS& param);
};

#define DEF_OWNERDRAWFUNC()	\
	void OnNcPaint(HDC,LPRECT);\
	void OnMouseHover(HDC,LPRECT);\
	void OnMouseLeave(HDC,LPRECT);\
	void OnSetFocus(HDC,LPRECT);\
	void OnKillFocus(HDC,LPRECT);


#define OWNERDRAWER(control) RingOwnerDrawer_##control::
//#define SETOWNERDRAWER

//�ؼ���ֵĬ�ϻ�������С��ÿ���ؼ�����һ�����û�����������Ҫ������
//����û�����ֵ��Ĭ��RBUF_SIZE���ȡ�
#define RBUF_SIZE		80
//��Щ��Ϣ�Ĳ���ֵ������-1����INDEX_CURSELһ�£��������޸ĸ�ֵ��Ҫ
//����õ�INDEX_CURSEL�ĺ�������RingCombo::Select(int index)��index
//Ϊ-1��ȡ��ѡ������޸�INDEX_CURSELֵ����Ҫ�޸ĸ�Select������
#define INDEX_CURSEL (-1)

class RingControls:public RingCtrlWnd
{
public:
	RingControls();
   virtual ~RingControls() = 0;

	//����/��������
   virtual BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);
	virtual BOOL Attach(UINT uId,RingBaseWnd* parent=NULL);
	virtual BOOL Attach(HWND);

	//��ȡ,��������
	virtual int GetInt(int index=INDEX_CURSEL);
	//�������������أ����ڲ�����GetItemText����������������GetItemText�ı�ú�����Ϊ��
	LPCTSTR GetText(int index=INDEX_CURSEL,int size=RBUF_SIZE,LPTSTR lpBuf=NULL);
	virtual BOOL SetValue(int val,int index=INDEX_CURSEL);
	virtual BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);
	virtual BOOL Clear(){return TRUE;}

	DWORD GetExStyle(){return m_ExStyle;}
	void SetAutoSize(BOOL bAuto=TRUE);
	virtual BOOL SetReadOnly(BOOL bReadOnly){return FALSE;}
	//virtual BOOL IsReadOnly(){return FALSE;}
	//������Ϣ���͸�������
	void SetKeyEventToParent(BOOL bYes=TRUE){m_bNotifiParentKeyEvent = bYes;}
	void SetSizeFitParent(BOOL bFit = TRUE){m_ExStyle |= (bFit?1:0);}

	virtual void SetTextColor(COLORREF crColor);
	virtual COLORREF GetTextColor(){return m_crTextColor;}
	
	virtual BOOL SetBkColor(COLORREF crColor);
	
	void SetFont(LPCTSTR szFontName,int size,int nStyle=0,COLORREF crText=0);
	ringFont* GetFont(){return &m_font;}

	void SetMouseTrack(RINGPARAMS& param);

	//����Ƿ���ؼ�����һ��
	BOOL TypeMatched(UINT id);
	BOOL TypeMatched(HWND hWnd);

	virtual LPCTSTR WndClassName() = 0;
				
	//�����Ի棬����ΪTRUE��ʾ�ɸ����ڴ����Ի�
	//uItemStyleΪ����ؼ����Ի�����
	void SetOwnerDraw(BOOL bParentDraw,UINT uItemStyle);
	
	RingOwnerDrawer* SetDrawer(RingOwnerDrawer* drawer)
	{
		RingOwnerDrawer* prev = m_drawer;
		if(drawer)
			m_drawer = drawer;
		return prev;
	}
	
/*
	template<typename tname>
	tname* SetOwnerDraw(tname* drawer)
	{
		if(m_drawer)
		{
			tname* tmp = dynamic_cast<tname*>(m_drawer);
			if(tmp)	//�����ͬ����Ϊ������
				return (tname*)m_drawer;
			else
				delete m_drawer;
		}
		m_drawer = new tname;
		return (tname*)m_drawer;
	}
*/	
protected:
	//���пؼ�����ʱ��ʼ��
	//virtual void InitControl(RingBaseWnd* parent,UINT uId);
	LRESULT RingdowProc(HWND hWnd,RINGPARAMS);
	BOOL OnParentNotify(HWND,UINT,UINT,LONG);
	virtual BOOL OnFitSize(HWND,WPARAM,LPARAM);
	virtual BOOL OnDrawItem(LPDRAWITEMSTRUCT lpds){return FALSE;}
	//ͣ��״̬����Ӧͣ��״̬�ı��¼�������-1��δ�ı䣬TRUE���Ѵ���0��δ����
	virtual int OnDockStateChange(int stateOld,int stateNew);
	virtual BOOL OnGetDragRects(int state,LPRECT lprc);
	//LRESULT DefaultProc(RINGPARAMS&);
	int GetBuffer(int size);	
	LRESULT SetColors(HDC);
	virtual LRESULT OnDestroy(RINGPARAMS& param){return DefaultProc(param);}
	virtual int GetItemText(int index,LPTSTR lpBuf,int size);

protected:
	ringFont m_font;
	COLORREF m_crTextColor,m_crBkgColor;
	BOOL m_bNotifiParentKeyEvent;
	RingOwnerDrawer* m_drawer;

	BOOL m_bOwnerDraw;
//private:
	LPSTR m_buf;
   int m_bufsize;	
};

#endif
