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
//							RingSDK ��ʾ����
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
//˵����
//
//��������ʾ��
//		����MDI Frame����
//		����MDI�Ӵ���
//		��ͼ���ļ���ʾ
//		��ӹ�������״̬��
//		��״̬����ʾ�˵�������Ϣ
//
**********************************************************************/
#include "ringmain.h"
#include "ringdib.h"
#include "resource.h"

//MDIFrame�������趨
BEGIN_SETUPMAINWINDOW(IS_MDIFRAME)
	BEGIN_DEF_VAR(NO_INIT)
		RingStatusBar* m_statbar;
	END_DEF_VAR
	BEGIN_DEFMSG
		DEFMSG(WM_CREATE)
		DEFMSG(WM_MENUSELECT)
		DEFCMD(OnOpen)
		DEFCMD(OnExit)
		DEFCMD(OnHelp)
	END_DEFMAINMSG
END_SETUPMAINWINDOW

//MDI�Ӵ����趨
BEGIN_SETUPWINDOW(child,IS_MDICHILD)
	BEGIN_DEF_VAR(NO_INIT)
		RingDIB m_dib;
	END_DEF_VAR

	BEGIN_DEFMSG
		DEFMSG(WM_CREATE)
		DEFMSG(WM_PAINT)
		DEFMSG(WM_ERASEBKGND)
		DEFMSG(WM_SIZE)		
	END_DEFMSG(child)
END_SETUPWINDOW(child)

//��Ϣ��
BEGIN_REGMAINMSG
	REGCMD(CM_OPEN,OnOpen)
	REGCMD(CM_EXIT,OnExit)
	REGCMD(CM_ABOUT,OnHelp)
	//OnCascadeChild,OnTileChildH,OnTileChildV��Ϊ����RingMDIFrameWnd��Ա����
	REGCMD(CM_CASCADE,OnCascadeChild)	
	REGCMD(CM_TILEH,OnTileChildH)
	REGCMD(CM_TILEV,OnTileChildV)
	REGMSG(WM_MENUSELECT)
	REGMSG(WM_CREATE)	
END_REGMSG

BEGIN_REGMSG(child)
	REGMSG(WM_PAINT)
	REGMSG(WM_ERASEBKGND)
	REGMSG(WM_SIZE)	
	REGMSG(WM_CREATE)
END_REGMSG

int RingMain()
{
	RingApp.SetAccelTable(MAKEINTRESOURCE(IDR_ACCEL));
	APP_CREATEWINDOW("RingSDK���ĵ���ʾ����","RingSDKApp3",WS_OVERLAPPEDWINDOW,MAKEINTRESOURCE(IDR_MAINMENU));
	return RingApp.Run();
}

RINGMAINMSG(WM_CREATE)
{
	//�趨֧�ֵ�ͼ���ʽ��Ŀǰ֧���������ָ�ʽ��BMPΪĬ�ϣ�����Ҫ�趨��
	DIBREGFORMAT(GIF);
	DIBREGFORMAT(JPG);
	DIBREGFORMAT(PNG);
	DIBREGFORMAT(PCX);

	SetIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_APP)));
	
	//��������������NewRingObj�����Ŀؼ���MDI�Ӵ��ڶ���ֻҪ�������ڣ�����Create��Attach�����Զ��ͷţ�
	//�������Ժ���ʹ�ã������ָ����Բ�����ʱ������
	//NewRingObj�Ĳ�����ʹ��itemof(�ؼ�����)
	RingToolBar *toolbar = NewRingObj(itemof(RingToolBar));
	toolbar->Create(IDB_TOOLBAR);
	//������ť����������ť��ţ���ťͼ���ڹ�����ͼ������ţ������
	toolbar->InsertSep(0);
	toolbar->InsertButton(1,1,CM_OPEN);
	toolbar->InsertSep(2);
	toolbar->InsertButton(3,11,CM_ABOUT);
	//toolbar->EnableDock("",TBS_EX_NONE|TBS_EX_ALLLINE,TBS_FLAG_DOCKLEFT);
	//����״̬��
	m_statbar = NewRingObj(itemof(RingStatusBar));
	m_statbar->Create();
	
	return DefaultProc(param);
}

RINGMAINMSG(WM_MENUSELECT)
{
	//��״̬����ʾ�˵�������Ϣ����Դ�ַ���ID���򣺲˵�����Դ�ַ���ID����=�ò˵�������ID��
	//�Ӳ˵���Դ�ַ���ID����=0x1100+�Ӳ˵���ţ���0��ʼ���㣩
	UINT uIds[] = {MH_BASE,MH_POPUPBASE,0,0};
	MenuHelp(WM_MENUSELECT,param.wParam,param.lParam,GetMenu(),GetInstance(),m_statbar->Handle(),uIds);
	return 0;
}

RINGMAINCMD(OnOpen)
{
	ringFile rf;
	if(rf.Select("ͼ���ļ�|*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.pcx||"))
	{
		HWND hChild = FindWindowEx(GetClient(),NULL,GetChildClassName(),rf.Fullname());
		if(hChild)
			ActiveChild(hChild);
		else
			CreateChild(rf.Fullname(),NewRingObj(child),TRUE,WS_OVERLAPPEDWINDOW);
	}
}

RINGMAINCMD(OnExit)
{
	SendExit();
}

RINGMAINCMD(OnHelp)
{
	ShellAbout(m_hWnd,"RingSDK���ĵ���ʾ����#RingSDK���ĵ���ʾ����","",
					LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_APP)));
}

////////////MDI�Ӵ��ڴ���////////////////////////
RINGMSG(child,WM_CREATE)
{
	char szPic[MAX_PATH];
	GetWindowText(m_hWnd,szPic,MAX_PATH);
	m_dib.Load(szPic);
	m_dib.SetTarget(m_hWnd);
	
	SetIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_APP)));
	return DefaultProc(param);
}

RINGMSG(child,WM_SIZE)
{
	InvalidateRect(m_hWnd,NULL,FALSE);
	return DefaultProc(param);
}

RINGMSG(child,WM_ERASEBKGND)
{
	return TRUE;
}

RINGMSG(child,WM_PAINT)
{
	int w,h;
	RECT rc,rcImage;
	PAINTSTRUCT ps;
	
	GetClientRect(&rc);
	//����ͼ����ʾ���
	if(rc.right < m_dib.Width())
	{
		//������ʾ
		h = rc.right*m_dib.Height()/m_dib.Width();
		if(h > rc.bottom)
		{
			h = rc.bottom;
			w = h*m_dib.Width()/m_dib.Height();
		}
		else
			w = rc.right;
	}
	else if(rc.bottom < m_dib.Height())
	{
		//������ʾ
		w = rc.bottom*m_dib.Width()/m_dib.Height();
		if(w > rc.right)
		{
			w = rc.right;
			h = w*m_dib.Height()/m_dib.Width();
		}
		else
			h = rc.bottom;
	}
	else
	{
		w = m_dib.Width();
		h = m_dib.Height();
	}
	rcImage.left = (rc.right - w)/2;
	rcImage.right = rcImage.left + w;
	rcImage.top = (rc.bottom - h)/2;
	rcImage.bottom = rcImage.top + h;

	//����ͼ��
	BeginPaint(m_hWnd,&ps);
	m_dib.Draw(ps.hdc,rcImage.left,rcImage.top,0,0,w,h);
	//�ų�ͼ��������Ʊ������������������˸
	ExcludeClipRect(ps.hdc,rcImage.left,rcImage.top,rcImage.right,rcImage.bottom);
	FillRect(ps.hdc,&rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
	EndPaint(m_hWnd,&ps);
	return 0;
}
