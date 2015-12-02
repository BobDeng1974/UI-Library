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
//							RingSDK ��ʾ����(ISeeͼ�������)
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
//˵��������ͷ�ļ�
//
//����������һ����ʾ�����������ʾ��
//		����XP��ʽ�Ĳ˵�
//		���������
//		�����ļ������
//		ListView�ؼ�ʹ����ʾ
//		������Ϣ�Ի�����ʾ
//
**********************************************************************/
#ifndef _INC_GLOBAL_H
#define _INC_GLOBAL_H
#include "ringmain.h"
#include "ringdib.h"
#include "resource.h"

#define IDC_FOLDERTREE			12001
#define IDC_FILEVIEW				12002
#define IDC_NAVBAR				12003
#define IDC_NAVPANE				12004
#define IDC_PROGRESS				12005
#define CM_UPPATH					13000

//�鿴����
#define CM_ZOOMIN					12010
#define CM_ZOOMOUT				12011
#define CM_NEXTPIC				12012
#define CM_PREVPIC				12013
//�ļ���������
#define CM_FILTER					12020
#define CM_SORTER					12021
#define CM_MODE					12022
//�ļ����˱�־
#define FILTER_FOLDER         FILE_ATTRIBUTE_DIRECTORY
#define FILTER_ALBUM          0x10000000
#define FILTER_PIC            0x20000000
#define FILTER_MEDIA          0x40000000
#define FILTER_OTHER          0x80000000
#define FILTER_HIDDEN			FILE_ATTRIBUTE_HIDDEN

#define THUMB_MASK				0x10000000
#define SIZE_MASK					0x0FFFFFFF

//�Զ���ؼ�
class RingTabEx:public RingTab
{
public:
	RingTabEx(){}
	~RingTabEx(){}

protected:
	//�����麯����ʹ������������һ��
	BOOL OnDrawBkg(HDC);
};

//�������趨,��ͨ����
BEGIN_SETUPMAINWINDOW(IS_WINDOW)
	BEGIN_DEF_VAR(NO_INIT)
		RingStatusBar* m_statbar;
		RingDirTree* m_dirtree;
		RingListView* m_fileview;
		RingTab* m_navbar;
		RingTabEx *m_tab;
		RingProgress* m_progress;
		int m_nMarkCol,m_nOrder,m_nMarkUp,m_nMarkDown;
		DWORD m_dwFilter;
		HICON m_iconFile;
	END_DEF_VAR
	
	void ListFile();
	void SetStatusFilePart(LPCTSTR lpszFile);

	BEGIN_DEFMSG
		DEFMSG(WM_CREATE)
		DEFMSG(WM_DESTROY)
		DEFMSG(WM_DRAWITEM)
		DEFMSG(WM_MENUSELECT)
		DEFMSG(WM_ACTIVATEAPP)
		DEFNOTIFY(TVN_SELCHANGED)
		DEFNOTIFY(TCN_SELCHANGE)		
		DEFCMD(OnOpen)
		DEFCMD(OnExit)
		DEFCMD(OnHelp)
		DEFCMD(OnSetViewMode)
		DEFCMD(OnSetFileFilter)
		DEFCMD(OnSetColumn)
		DEFCMD(OnSortColumn)
	END_DEFMAINMSG
END_SETUPMAINWINDOW

#define VIEW_FITSIZE		9999	//ͼ����Ӧ������ʾ

//Ԥ���Ӵ����趨
BEGIN_SETUPWINDOW(child,IS_DIALOG)
	BINDID(IDD_ABOUT)					//�󶨶Ի�����ԴID
	BEGIN_DEF_VAR(NO_INIT)
		RingDIB m_dib;
		BOOL m_bFullScreen;
		RECT m_rcFloat;
		int m_nRadio;		//�Ŵ�/��С����
		POINT m_ptMouse;
		HCURSOR m_hCursor,m_hCurHand,m_hCurGrab,m_hCurZoom,m_hCurArr;
	END_DEF_VAR

	void SetCursor(HCURSOR hCur = NULL);
	BOOL View(LPCTSTR lpszPic);
	void SetFullScreen(BOOL bFullScreen);
	void UpdatePicShow();
	int CalcRadio();
	void SetImgDest();
	BOOL MoveImg(int nDir);
	void MoveImg(int nOffX,int nOffY);
	void ZoomIn();
	void ZoomOut();
	void ViewNextImg(BOOL bNext=TRUE);

	BEGIN_DEFMSG
		DEFMSG(WM_INITDIALOG)
		DEFMSG(WM_PAINT)
		DEFMSG(WM_ERASEBKGND)
		DEFMSG(WM_SIZE)
		DEFMSG(WM_LBUTTONDOWN)
		DEFMSG(WM_MOUSEMOVE)
		DEFMSG(WM_LBUTTONUP)
		DEFMSG(WM_KEYDOWN)
		DEFMSG(WM_LBUTTONDBLCLK)
	END_DEFMSG(child)
END_SETUPWINDOW(child)

//�����Ӵ����趨
BEGIN_SETUPWINDOW(NavPane,IS_DIALOG)
	BINDID(IDD_ABOUT)					//�󶨶Ի�����ԴID
	BEGIN_DEF_VAR(NO_INIT)
		RingToolBar* m_toolbar;
		RingToolBar* m_navtool;
		RingListView* m_fileview;
		RingTab* m_tab;
		int m_nViewMode,m_nFileMode;
		int m_nMarkCol,m_nOrder,m_nMarkUp,m_nMarkDown;
		RECT m_RectViewer;
		int m_stateViewer,m_nBarBorder;		
		RingImageList* m_img;
		HIMAGELIST m_hSysImgL,m_hSysImgS,m_hHeaderImg;
		RingThread* m_thread;
		RingDataGuard m_guard;
		//HBRUSH m_br1,m_br2,m_brline,m_brsel;
		//COLORREF m_crSel;
	END_DEF_VAR

	void SetViewMode(int iFlag);
	void SetFullScreen(BOOL bFullScreen);
	BOOL OnParentNotify(HWND,UINT,UINT,LONG);
	static int CALLBACK SortFileProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	DWORD ViewThumbProc(LONG);
	void UpdateFileList();

	BEGIN_DEFMSG
		DEFMSG(WM_INITDIALOG)
		DEFMSG(WM_SIZE)
		DEFMSG(WM_DESTROY)
		DEFNOTIFY(NM_DBLCLK)
		DEFNOTIFY(LVN_COLUMNCLICK)
		DEFNOTIFY(LVN_ITEMCHANGED)
		DEFNOTIFY(TBN_DROPDOWN)
		DEFNOTIFY(NM_RETURN)
		DEFNOTIFY(LVN_SHOWCOLUMN)
		DEFCMD(OnUpParentPath)
		DEFCMD(OnZoomIn)
		DEFCMD(OnZoomOut)
		DEFCMD(OnNextImg)
		DEFCMD(OnPrevImg)
		DEFCMD(OnSetFileFilter)
		DEFCMD(OnSortColumn)
		DEFCMD(OnSetViewMode)
	END_DEFMSG(NavPane)
END_SETUPWINDOW(NavPane)

//��Ȩ��Ϣ�Ի����趨
BEGIN_SETUPWINDOW(DlgAbout,IS_DIALOG)
	BINDID(IDD_ABOUT)					//�󶨶Ի�����ԴID
	BEGIN_DEF_VAR(NO_INIT)		
		RingDIB* m_dib,*m_ripple;
		short *m_buf1,*m_buf2;
		RingThread* m_thread;
	END_DEF_VAR

	DWORD RippleThread(LONG);
	void RippleSpread();
	void RenderRipple();
	void DropStone(int x,int y,int stonesize,int stoneweight);

	BEGIN_DEFMSG
		DEFMSG(WM_INITDIALOG)
		DEFMSG(WM_LBUTTONDOWN)
		DEFMSG(WM_MOUSEMOVE)
		DEFMSG(WM_CLOSE)
	END_DEFMSG(DlgAbout)
END_SETUPWINDOW(DlgAbout)

#endif
