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
//							RingSDK������������ʾ���� toolbar
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
//ԭ�ļ�����main.cpp
//
//˵����	��������ʾRingToolBar��ʹ�ã�
//			1���˵�����ʹ��
//			2������ԭʼ״̬����ť��������
//			3������Դ�����ƽ������
//			4���������+����Ч��������
//			5���ڹ������ϲ��뾲̬�ı����ؼ�
//
**********************************************************************/
#include "global.h"

#define CM_CONNECT			12001
#define CM_DISCONNECT		12002
#define IDC_HOST				201
#define IDC_PORT				202

BEGIN_REGMAINMSG
	REGCMD(CM_NEW,OnFileNew)
	REGCMD(CM_OPEN,OnFileOpen)
	REGCMD(CM_ABOUT,OnAbout)
	REGCMD(CM_EXIT,OnExit)
	REGCMD(CM_CASCADE,OnCascadeChild)
	REGCMD(CM_TILEH,OnTileChildH)
	REGCMD(CM_TILEV,OnTileChildV)
	REGCMD(CM_CLOSEALL,MaxChild)
	REGCMD(CM_CONNECT,OnConnect)
	REGCMD(CM_DISCONNECT,OnDisConnect)

	REGMSG(WM_CREATE)
	REGMSG(WM_DESTROY)
END_REGMSG

BEGIN_REGMSG(tooldlg)
	REGMSG(WM_INITDIALOG)
	REGMSG(WM_GETDRAGRECTS)
	REGMSG(WM_DOCKSTATECHANGE)
END_REGMSG

int RingMain()
{
	APP_CREATEWINDOW("��������ʾ����","RINGSDK_TOOLBAR_DEMO",WS_OVERLAPPEDWINDOW,
		MAKEINTRESOURCE(IDM_MAIN));
	return RingApp.Run();
}

RINGMAINMSG(WM_CREATE)
{
	//���ó���ͼ��
	SetIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_MAIN)));

	//��ʼ���˵�
	rm = InstallXPMenu();
	//���ò˵�ͼ��
	rm->SubMenu(0)->SetItemIcon(CM_NEW,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_NEW)));
	rm->SubMenu(0)->SetItemIcon(CM_OPEN,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_OPEN)));
	rm->SubMenu(0)->SetItemIcon(CM_SAVE,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_SAVE)));
	rm->SubMenu(0)->SetItemIcon(CM_SAVEALL,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_SAVEALL)));
	rm->SubMenu(0)->SetItemIcon(CM_PRINT,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_PRINT)));
	rm->SubMenu(0)->SetItemIcon(CM_EXIT,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_EXIT)));
	rm->SubMenu(1)->SetItemIcon(CM_UNDO,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_UNDO)));
	rm->SubMenu(1)->SetItemIcon(CM_REDO,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_REDO)));
	rm->SubMenu(1)->SetItemIcon(CM_COPY,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_COPY)));
	rm->SubMenu(1)->SetItemIcon(CM_PASTE,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_PASTE)));
	rm->SubMenu(1)->SetItemIcon(CM_CUT,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_CUT)));
	rm->SubMenu(1)->SetItemIcon(CM_DELETE,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_DEL)));
	rm->SubMenu(3)->SetItemIcon(CM_CLOSEWINDOW,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_CLOSE)));
	rm->SubMenu(3)->SetItemIcon(CM_NEXT,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_NEXT)));
	rm->SubMenu(3)->SetItemIcon(CM_PREV,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_PREV)));
	rm->SubMenu(3)->SetItemIcon(CM_CASCADE,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_CASCADE)));
	rm->SubMenu(3)->SetItemIcon(CM_TILEH,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_TILEH)));
	rm->SubMenu(3)->SetItemIcon(CM_TILEV,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_TILEV)));
	rm->SubMenu(4)->SetItemIcon(CM_ABOUT,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_HELP)));

	menubar = NewRingObj(itemof(RingMenuBar));
	menubar->LoadMenu(rm);

	SetDockBackGround(MAKEINTRESOURCE(IDB_BKG),TRUE);

	toolbar = NewRingObj(toolbar);
	toolbar->Create(IDB_TOOL);//,TBSTYLE_FLAT);
	
	toolbar->InsertButton(1,0,CM_NEW);
	toolbar->InsertButton(2,1,CM_OPEN);
	toolbar->InsertButton(3,2,3);
	toolbar->InsertButton(4,3,4);
	toolbar->InsertSep(5);
	toolbar->InsertButton(7,4,5);
	toolbar->InsertButton(8,5,6);

	toolbar = NewRingObj(itemof(RingToolBar));
	toolbar->Create(IDC_DEFTOOLBAR,TBSTYLE_FLAT);
	toolbar->LoadToolBar(IDC_DEFTOOLBAR);

	toolbar = NewRingObj(toolbar);
	toolbar->Create(IDB_COOLGRAY,TBSTYLE_FLAT);
	toolbar->InsertButton(1,0,1);
	toolbar->InsertButton(2,1,2);
	toolbar->InsertButton(3,2,3);
	toolbar->InsertButton(4,3,4);
	toolbar->InsertSep(5);
	toolbar->InsertButton(6,4,5);
	toolbar->InsertButton(7,5,6);

	toolbar->InsertSep(8);
	toolbar->InsertButton(9,12,7,0,TBSTYLE_BUTTON | TBSTYLE_CHECK);
	toolbar->InsertButton(10,13,8);
	toolbar->InsertSep(11);
	toolbar->InsertButton(12,6,9);
	toolbar->InsertButton(13,7,10);
	toolbar->InsertButton(14,8,11);
	toolbar->InsertButton(15,9,12);
	toolbar->InsertButton(16,10,13);
	toolbar->InsertButton(17,14,14);
	toolbar->InsertSep(18);
	toolbar->InsertButton(19,11,15);
	//��������ͼ��
	toolbar->AddHotImage(IDB_COOL,20,20,RGB(192,192,192));
	toolbar->EnableDock("��ʹ�����");

	//�����ؼ�
	m_ipAddr = NewRingObj(itemof(RingIPAddr));
	m_ipAddr->Create(0,0,NULL,IDC_HOST,0,0,120,20);
	m_edPort = NewRingObj(itemof(RingEdit));
	m_edPort->Create(ES_NUMBER,WS_EX_CLIENTEDGE,NULL,IDC_PORT,0,0,40,20);
	
	toolbar = NewRingObj(toolbar);
	toolbar->Create(IDB_NET,TBSTYLE_FLAT);
	//�����ı����Ѵ����Ŀؼ�
	toolbar->InsertText(0,"��������ַ:");
	toolbar->InsertItem(1,m_ipAddr);
	toolbar->InsertText(2,"�˿�:");
	toolbar->InsertItem(3,m_edPort);
	toolbar->InsertButton(4,1,CM_CONNECT);
	toolbar->InsertButton(5,0,CM_DISCONNECT);
	toolbar->InsertButton(6,2,1);
	toolbar->InsertButton(7,3,2);
	toolbar->InsertButton(8,5,3,NULL,TBSTYLE_CHECK);
	toolbar->EnableDock("SOCKET������");
	toolbar->EnableButton(CM_DISCONNECT,FALSE);
	
	return DefaultProc(param);
}

RINGMAINMSG(WM_DESTROY)
{
	return Quit();
}

RINGMAINMSG(WM_RINGTOOLHELP)
{
	char s[80];
	wsprintf(s,"%d\0",param.wParam);
	SetTitle(s);
	return 0;
}

RINGMAINCMD(OnFileNew)
{	
	child = NewRingObj(child);
	CreateChild("new document",child,FALSE,WS_OVERLAPPEDWINDOW);
}

RINGMAINCMD(OnFileOpen)
{
}

RINGMAINCMD(OnAbout)
{
	ShowHelpInfo("RingSDK ��������ʾ���� v1.0\n(C)CopyRight 2005\n�ٷ� ringphone@njwx.com","��������ʾ����",MAKEINTRESOURCE(IDI_MAIN));
}

RINGMAINCMD(OnExit)
{
	SendExit();
}

RINGMAINCMD(OnConnect)
{
	int val = m_ipAddr->GetInt();
	MsgBox("��������","���ӵ�%d.%d.%d.%d:%d\n\n�������ʾ���棬����ʵ�����ӡ�",
				FIRST_IPADDRESS(val),SECOND_IPADDRESS(val),THIRD_IPADDRESS(val),
				FOURTH_IPADDRESS(val),m_edPort->GetInt());
	toolbar->EnableButton(CM_CONNECT,FALSE);
	toolbar->EnableButton(CM_DISCONNECT);
}

RINGMAINCMD(OnDisConnect)
{
	toolbar->EnableButton(CM_CONNECT);
	toolbar->EnableButton(CM_DISCONNECT,FALSE);
}

INITWINDOW(tooldlg)()
{
	SetRectEmpty(&m_vrc);
	SetRectEmpty(&m_hrc);
}

RINGMSG(tooldlg,WM_INITDIALOG)
{
	return FALSE;
}

RINGMSG(tooldlg,WM_GETDRAGRECTS)
{	
	RECT rc,rcS;
	
	if(param.wParam == TBS_FLAG_HORZ)
	{
		if(IsRectEmpty(&m_hrc))
		{
			GetWindowRect(GetDlgItem(m_hWnd,IDC_LIST1),&rc);
			GetWindowRect(GetDlgItem(m_hWnd,IDC_TEXT),&rcS);
			ScreenToClient(m_hWnd,(LPPOINT)&rcS);
			ScreenToClient(m_hWnd,(LPPOINT)&rcS+1);
			m_hrc.right = rcS.right + 20 + rc.right - rc.left;
			
			GetWindowRect(GetDlgItem(m_hWnd,IDC_ICON1),&rcS);
			ScreenToClient(m_hWnd,(LPPOINT)&rcS);
			ScreenToClient(m_hWnd,(LPPOINT)&rcS+1);

			m_hrc.bottom = max((rcS.bottom + rcS.top),(rc.bottom - rc.top + rcS.top * 2));
		}
		if(param.lParam)
			CopyRect((LPRECT)param.lParam,&m_hrc);
		return 1;
	}
	else
	{
		if(IsRectEmpty(&m_vrc))
		{
			GetWindowRect(GetDlgItem(m_hWnd,IDC_LIST1),&rc);
			
			m_vrc.right = rc.right - rc.left + 4;
			
			GetWindowRect(GetDlgItem(m_hWnd,IDC_ICON1),&rcS);
			ScreenToClient(m_hWnd,(LPPOINT)&rcS);
			ScreenToClient(m_hWnd,(LPPOINT)&rcS+1);
			
			m_vrc.bottom = rcS.bottom + rc.bottom - rc.top + rcS.top * 2;
		}
		if(param.lParam)
			CopyRect((LPRECT)param.lParam,&m_vrc);
	}
	return 1;
}

RINGMSG(tooldlg,WM_DOCKSTATECHANGE)
{
	if(param.shortval.high == param.shortval.low)
		return 0;

	RingListBox* list = GetItem(list,IDC_LIST1);
	RECT rc;
	
	if(m_param.wParam > TBS_FLAG_DOCKBOTTOM || m_param.wParam == TBS_FLAG_FLY)
	{
		GetWindowRect(GetDlgItem(m_hWnd,IDC_ICON1),&rc);
		ScreenToClient(m_hWnd,(LPPOINT)&rc);
		ScreenToClient(m_hWnd,(LPPOINT)&rc+1);

		SetPos(0,0,m_vrc.right,m_vrc.bottom);
		list->SetPos(2,rc.bottom + 10,0,0,0,SWP_NOSIZE);
	}
	else
	{
		GetWindowRect(GetDlgItem(m_hWnd,IDC_TEXT),&rc);
		ScreenToClient(m_hWnd,(LPPOINT)&rc);
		ScreenToClient(m_hWnd,(LPPOINT)&rc+1);

		SetPos(0,0,m_hrc.right,m_hrc.bottom);
		list->SetPos(rc.right + 10,rc.top,0,0,0,SWP_NOSIZE);
	}

	return 1;
}

