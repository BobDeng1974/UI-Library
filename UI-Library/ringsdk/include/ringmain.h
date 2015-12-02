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
//							RingSDK��� ringmain.lib
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
//ԭ�ļ�����ringmain.h
//
//˵������WinMain�ķ�װ���������⽫���ڱ���⣬�������ļ�����ʵ��
//      WinMain��������ΪRingMain�������Լ�ʵ�֡�
//
**********************************************************************/
#ifndef _INC_RINGMAIN
#define _INC_RINGMAIN
#include "ringdows.h"

class RingdowApp
{
public:
	RingdowApp():m_bIsService(FALSE){}
	~RingdowApp();

   static BOOL SetAccelTable(LPCTSTR lpTableName);
   static int Run(BOOL bShow=TRUE);

   static void SetIdleProc(IDLEPROC IdleProc);
   static LRESULT OnIdle(void);

	BOOL IsService(){return m_bIsService;}
	int GetShowCmd(){return m_CmdShow;}
	void SetShowCmd(int nCmd){m_CmdShow = nCmd;}
	LPSTR GetCmdLine(){return m_szCommand;}

private:
   static IDLEPROC OnIdleProc;
	BOOL m_bIsService;
	static int m_CmdShow;
	friend class RingService;

public:
   static LPSTR m_szCommand;				//������յ�����������
	static HACCEL m_hAccel;
   static RingBaseWnd* m_window;
   static int m_Result;
};

#define IS_WINDOW		RingWnd
#define IS_MDIFRAME	RingMDIFrameWnd
#define IS_MDICHILD	RingMDIChildWnd
#define IS_DIALOG		RingDialog
#define IS_PROPPAGE	RingPropPage
#define IS_PROPSHEET RingPropSheet

//m_windowType+=ITIS_MAINWINDOW:���Ϊ������,OnCreate����Ⲣ��ԭԭʼ��������
#define BEGIN_SETUPMAINWINDOW(baseclass)	\
class RingWnd_window:public baseclass\
{\
public:\
	RingWnd_window(RingBaseWnd* parent)\
   {\
   	SetParentObj(parent);\
      m_msgs = (RingMsgs*)RingSDK_winmsg;\
      m_windowType += ITIS_MAINWINDOW;\
		InitRingSDKWindow();\
   }\

#define BEGIN_DEFMAINMSG
#define END_DEFMAINMSG	END_DEFMSG(window)

#define BEGIN_REGMAINMSG	\
RingWnd_window* window = NULL;\
RingdowMsg<RingWnd_window> RingWnd_window::RingSDK_winmsg[] = {

//���ﲻ��defineΪRINGXXX(window,notify)����Ϊ���
//RINGMAINNOTIFY(NM_DBLCLK),NM_DBLCLK�Ѷ��壬ת��
//RINGNOTIFY(window,notify)ʱNM_DBLCLK��ת������
#define RINGMAINMSG(msgname) LRESULT RingWnd_window::On_##msgname(HWND hWnd,RINGPARAMS& param)
#define RINGMAINMSGEX(msgname) LRESULT RingWnd_window::msgname(HWND hWnd,RINGPARAMS& param)
#define RINGMAINCMD(funcname) void RingWnd_window::funcname(UINT event)
#define RINGMAINNOTIFY(notify)	LRESULT RingWnd_window::On_##notify(HWND hWnd,RINGPARAMS& param)

#define RINGMAINCLASS			RINGCLASS(window)
#define RINGMAINMETHOD(func)	RINGMETHOD(window,func)
#define RINGMAINVAR				RINGVAR(window)
#define END_SETUPMAINWINDOW	END_SETUPWINDOW(window)

#define INITMAINWINDOW  INITWINDOW(window)

#define NEW_MAINWINDOW()	\
try\
{\
	if(window == NULL)\
		window = new RingWnd_window(NULL);\
	RingdowApp::m_window = window;\
}\
catch(...)\
{\
	abort();\
}

#define APP_CREATEWINDOW(szTitle,szClass,dwStyle,lpszMenu)	\
NEW_MAINWINDOW()\
window->SetMenu(lpszMenu);\
InitRingLib(window->Create(szTitle,szClass,dwStyle),GetInstance())

#define APP_CREATEWINDOWEX(szTitle,szClass,dwStyle,dwExStyle,lpszMenu)	\
NEW_MAINWINDOW()\
window->SetMenu(lpszMenu);\
InitRingLib(window->Create(szTitle,szClass,dwStyle,dwExStyle),GetInstance())

#define APP_CREATEDIALOG(szTemplate)	\
NEW_MAINWINDOW()\
RingdowApp::m_Result = window->DoModal(szTemplate);

//#define REG_CHILDWND(szClass,dwStyle,dwExStyle)	RingWnd::RegClass(szClass,dwStyle,dwExStyle)

extern RingdowApp RingApp;

#ifndef MAKE_SELF_LIB
#ifdef _DEBUG
#pragma comment(lib, "ringmaind.lib")
#else
#pragma comment(lib, "ringmain.lib")
#endif
#endif

#endif
