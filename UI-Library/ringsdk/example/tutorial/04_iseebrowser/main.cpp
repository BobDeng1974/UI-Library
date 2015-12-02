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
//˵������֪����Ƿ񻹼ǵ��������и���Դ��ĿISeeͼ���������������Ŀ��
//		  ��������ԽACDSee�������������������������ԭ�򣬱��ֻ�ṩͼ��
//		  ��������Ŀ����������Ŀ��ֹ�ˡ��ٷ統ʱ�������Ŀ��ͼ����
//		  ��ĸ����ˣ�ISee��Ŀ��ز���Ǻܿ�ϧ�ģ���������������ͼ�����
//		  ����Ϊ��ʾ��������������Ȼ�Ȳ���ACDSee��ͼ������Ĳ���Ҳû��
//		  ʹ�������Ŀ�ĳɹ����룬��Ҳ���Ǳ��һ�¶���Ŀ���Ա�ľ��⣬��
//		  ��һ��δ�˵���Ը�ɡ�
//
//����������һ����ʾ�����������ʾ��
//		����XP��ʽ�Ĳ˵�
//		ͣ��������
//		���������
//		�����ļ����б���
//		ListView�ؼ�ʹ����ʾ
//		������Ϣ�Ի�����ʾ
//
**********************************************************************/
#include "global.h"

//��Ϣ��
BEGIN_REGMAINMSG
	REGCMD(CM_OPEN,OnOpen)
	REGCMD(CM_EXIT,OnExit)
	REGCMD(CM_ABOUT,OnHelp)
	//OnCascadeChild,OnTileChildH,OnTileChildV��Ϊ����RingMDIFrameWnd��Ա����
	REGCMD(CM_CASCADE,OnCascadeChild)	
	REGCMD(CM_TILEH,OnTileChildH)
	REGCMD(CM_TILEV,OnTileChildV)
	REGNOTIFY(TVN_SELCHANGED)
	REGNOTIFY(NM_DBLCLK)
	REGMSG(WM_MENUSELECT)
	REGMSG(WM_DESTROY)	
	REGMSG(WM_CREATE)	
END_REGMSG

int RingMain()
{
	RingApp.SetAccelTable(MAKEINTRESOURCE(IDR_ACCEL));
	APP_CREATEWINDOW("Isee�����[��ʾ�汾]","RingSDKApp4",WS_OVERLAPPEDWINDOW,MAKEINTRESOURCE(IDR_MAINMENU));
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
	
	//����XP��ʽ�˵�
	InstallXPMenu();

	//��������������NewRingObj�����Ŀؼ���MDI�Ӵ��ڶ���ֻҪ�������ڣ�����Create��Attach�����Զ��ͷţ�
	//�������Ժ���ʹ�ã������ָ����Բ�����ʱ������
	//NewRingObj�Ĳ�����ʹ��itemof(�ؼ�����)
	RingToolBar *toolbar = NewRingObj(itemof(RingToolBar));
	toolbar->Create(IDB_TOOLBAR,TBSTYLE_FLAT);
	//������ť����������ť��ţ���ťͼ���ڹ�����ͼ������ţ������
	toolbar->InsertButton(0,1,CM_OPEN);
	toolbar->InsertSep(1);
	toolbar->InsertButton(2,11,CM_ABOUT);
	toolbar->EnableDock("������");
	
	//�����ļ����б���
	m_dirtree = NewRingObj(itemof(RingDirTree));
	m_dirtree->Create(IDC_FOLDERTREE);
	m_dirtree->ListDir();
	
	//���������
	m_dirtree->EnableDockSize("�ļ���",200,400,TBS_FLAG_DOCKLEFT,TBS_EX_NONE);	

	//�����ļ��б���
	m_fileview = NewRingObj(itemof(RingListView));
	m_fileview->Create(LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS,WS_EX_CLIENTEDGE,NULL,IDC_FILEVIEW,0,0,300,300);
	m_fileview->SetExStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_fileview->SetColumns("����|��С|�޸�ʱ��");
	m_fileview->SetColumnsWidth("300|180|230");
	m_fileview->EnableDockSize("�ļ��б�",300,200,TBS_FLAG_DOCKBOTTOM,TBS_EX_NONE);
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

RINGMAINNOTIFY(TVN_SELCHANGED)
{
	//�û�ѡ��Ŀ¼�Ĵ����Ƚ��ɿؼ���Ӧ
	m_dirtree->OnSelectChanged(param);

	//��ȡѡ���Ŀ¼���г�ͼ���ļ�
	ringDir dir(m_dirtree->GetSelFolderName());
	ringFile rf;
	ringStr str;
	char szFile[MAX_PATH];
	int index;
	HICON hicon;

	if(IsWindowPtr(m_fileview))		//���m_fileview�Ƿ���Ч���Ѵ���
		m_fileview->Clear();

	if(dir.BeginFindFile())
	{
		while(dir.FindNext(szFile))
		{
			rf.SetFile("%s\\%s",dir.Getdir(),szFile);
			str = rf.Extname();
			str.toLower();
			if(str == "bmp" || str == "jpg" || str == "jpeg" || str == "gif" || 
				str == "pcx" || str == "png")
			{				
				index = m_fileview->GetCount();
				hicon = ExtractIcon(GetInstance(),rf.Fullname(),0);
				m_fileview->AddItem(index,rf.Filename(),(LPARAM)hicon,m_fileview->AddIcon(hicon));
				m_fileview->SetItem(index,1,rf.Size());
			}
		}
	}

	return 0;
}

RINGMAINNOTIFY(NM_DBLCLK)
{
	if(param.lpnmhdr->hwndFrom == m_fileview->Handle())
	{
		char szFile[MAX_PATH];
		LPNMITEMACTIVATE lpv = (LPNMITEMACTIVATE)param.lParam;
		wsprintf(szFile,"%s\\%s\0",m_dirtree->GetSelFolderName(),m_fileview->GetText(lpv->iItem,0));
		
		HWND hChild = FindWindowEx(GetClient(),NULL,GetChildClassName(),szFile);
		if(hChild)
			ActiveChild(hChild);
		else
			CreateChild(szFile,NewRingObj(child),TRUE,WS_OVERLAPPEDWINDOW);
	}
	return 0;
}

RINGMAINMSG(WM_DESTROY)
{
	//�����ڹر�ʱ��������룬��������Ҫ�������˳����ɡ�
	//�����Ӧ����Ϣ��������������Quit()����PostQuitMessage,
	//���򴰿����٣��������������ڴ档
	return Quit();	
}

RINGMAINCMD(OnOpen)
{
	ringFile rf;
	if(rf.Select("ͼ���ļ�|*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.pcx||"))
		CreateChild(rf.Fullname(),NewRingObj(child),TRUE,WS_OVERLAPPEDWINDOW);	
}

RINGMAINCMD(OnExit)
{
	SendExit();
}

RINGMAINCMD(OnHelp)
{
	//�Ի��������÷�������Ի���ֻ����ʾ��Ȩ��Ϣ������Ҫ���û�������
	//�Ϳ�������ʹ�ã��öԻ�����ԴID��Ϊ�������ø��ƹ��캯�����ɡ�
	RingDialog(IDD_ABOUT).DoModal();
}

