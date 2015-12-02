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
//		�����˵���
//		ͣ������/�Ի���
//		չ��RingDIB���������ڰ�����Ϣ�Ի�������ʾˮ��Ч��
//		�߳���ĵ���
//		ListView�ؼ�ʹ��
//		�ļ����б���ʹ��
//		ģ��ACDSee4.0���棬��ACDSee����̫�࣬��˽�ʵ�ֲ��ֹ��ܼ���������
//		���棬����Ȥ�Ŀ�����Ӵ��������Щ���ܣ��ǵõ�ʱ��һ�ݴ������
//
**********************************************************************/
#include "global.h"

//��������Ϣ��
BEGIN_REGMAINMSG
	REGCMD(CM_OPEN,OnOpen)
	REGCMD(CM_EXIT,OnExit)
	REGCMD(CM_ABOUT,OnHelp)
	REGNOTIFY(TVN_SELCHANGED)
	REGNOTIFY(TCN_SELCHANGE)	
	REGMSG(WM_MENUSELECT)
	REGMSG(WM_DRAWITEM)
	REGMSG(WM_ACTIVATEAPP)
	REGCMDRANGE(CM_VIEWTD,CM_VIEWDETAIL,OnSetViewMode)
	REGCMDRANGE(CM_SHOWFOLDER,CM_SHOWHIDDEN,OnSetFileFilter)
	REGCMDRANGE(CM_COLSIZE,CM_COLTIME,OnSetColumn)
	REGCMDRANGE(CM_SORTEXT,CM_SORTDESC,OnSortColumn)
	REGMSG(WM_DESTROY)	
	REGMSG(WM_CREATE)	
END_REGMSG

int RingMain()
{
	//��ʼ��OLE�⣬��Ϊ����Ҫ����ϵͳSHELL
	CoInitialize(NULL);
	//���ÿ�ݼ�
	RingApp.SetAccelTable(MAKEINTRESOURCE(IDR_ACCEL));
	APP_CREATEWINDOW("Isee�����[��ʾ�汾]","ISeeBrowser",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
							MAKEINTRESOURCE(IDR_MAINMENU));
	return RingApp.Run();
}

RINGMAINMSG(WM_CREATE)
{	
	//�趨֧�ֵ�ͼ���ʽ��Ŀǰ֧���������ָ�ʽ��BMPΪĬ�ϣ�����Ҫ�趨��
	DIBREGFORMAT(GIF);
	DIBREGFORMAT(JPG);
	DIBREGFORMATS("jpeg",JPG);	//�趨���ʽ֧�ֽ���
	DIBREGFORMAT(PNG);
	DIBREGFORMAT(PCX);

	//���ô���ͼ��
	SetIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_APP)));
	
	//����XP��ʽ�˵�
	RingCoolMenu* rm = InstallXPMenu();

	//�����˵���
	RingMenuBar *menubar = NewRingObj(itemof(RingMenuBar));
	menubar->LoadMenu(rm);
	
	//���ó��򱳾�
	SetDockBackGround(MAKEINTRESOURCE(IDB_BKG),TRUE);

	//���ò˵���ͼ��
	rm->SubMenu(2)->SubMenu(3)->SetItemIcon(CM_SORTASEC,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_COLMARKUP)));
	rm->SubMenu(2)->SubMenu(3)->SetItemIcon(CM_SORTDESC,LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_COLMARKDOWN)));
	//CHECKһ�£�����ͼ����ѡ�б�־ͼ����ص�����
	rm->SubMenu(2)->SubMenu(3)->CheckRadioItem(CM_SORTASEC,CM_SORTDESC,CM_SORTASEC,BY_COMMAND);
	//����ͣ�����˵�(�ڹ�����ͣ��λ�ÿհ״����Ҽ������˵�)ΪXP��ʽ
	rm = GetDockMenu();
	if(rm)
   {		
		rm->SetItemDrawFunc(FUNC_DRAWMENU_XP);
		rm->SetBkgColor(0x00ffffff,0x00ff8080);
   }

	//�����ļ����˱�־
	m_dwFilter = FILTER_FOLDER|FILTER_PIC|FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE;

	//����״̬��
	m_iconFile = NULL;
	m_statbar = NewRingObj(itemof(RingStatusBar));
	m_statbar->Create();
	int np[5] = {
		MAKESBPINT(0,SBTS_BORDER,SBPW_FITTEXT),	 //״̬�����ӿ������Ӧ���ֿ��
		MAKESBPINT(0,SBTS_BORDER,SBPW_FITTEXT),
		MAKESBPINT(0,SBTS_BORDER,SBPW_FITTEXT),
		MAKESBPINT(0,SBTS_BORDER,SBPW_FITTEXT),
		MAKESBPINT(0,SBTS_BORDER,SBPW_STRETCH)		 //״̬�����ӿ������Ӧ���ڿ��
	};
	m_statbar->SetParts(5,np);

	//����������
	m_progress = NewRingObj(itemof(RingProgress));
	m_progress->Create(PBS_SMOOTH,RTV_FITPARENTSIZE,NULL,IDC_PROGRESS,0,0,0,0);
	//���뵽״̬��
	m_statbar->InsertItem(m_progress,4);

	ringDateTime rdt;
	char szText[40];
	wsprintf(szText,"%d-%02d-%02d %02d:%02d",rdt.year(),rdt.month(),rdt.day(),
				rdt.hour(),rdt.minute());
	m_statbar->SetValue(szText,1);

	//������������RTV_FITPARENTSIZE��չ���ͱ�ʾ����Ӧ�����ڳߴ�
	m_navbar = NewRingObj(itemof(RingTab));
	m_navbar->Create(TCS_MULTILINE|TCS_HOTTRACK|TCS_OWNERDRAWFIXED,
							RTV_FITPARENTSIZE,NULL,IDC_NAVBAR,0,0,300,400);
	m_navbar->InsertTabs("�ļ���|���|�ղؼ�|����");

	//�����ļ����б�����������Ϊm_navbar����������m_navbar->NewRingObj
	m_dirtree = m_navbar->NewRingObj(itemof(RingDirTree));
	m_dirtree->Create(IDC_FOLDERTREE);
	//����Ӧ�����ڳߴ�
	m_dirtree->SetAutoSize();
	m_dirtree->ListDir();
	//���ļ����б�������һ����ǩ
	m_navbar->SetTabToWnd(0,m_dirtree);
	
	//���������
	m_navbar->EnableDockSize("������",200,400,TBS_FLAG_DOCKLEFT);

	//���������Ӵ���
	m_tab = NewRingObj(itemof(RingTabEx));
	m_tab->Create(TCS_HOTTRACK|TCS_OWNERDRAWFIXED,
						RTV_FITPARENTSIZE,NULL,IDC_NAVPANE,0,0,300,400);
	m_tab->InsertTabs("�ļ�|�鿴|����");
	
	NavPane = m_tab->NewRingObj(NavPane,TRUE);
	NavPane->Create("��������","ISeeNavPane",WS_CHILD,0,400,400,200,200);
	m_tab->SetTabToWnd(0,NavPane);
	m_tab->SetTabToWnd(1,NavPane);
	m_tab->Select(0);
	
	//��ȡ�ļ��б���
	m_fileview = NavPane->m_fileview;
			
	//����ͼƬ�����
	child = NewRingObj(child,TRUE);
	child->Create("ͼƬ�����","ISeeViwer",WS_CHILD,0,0,0,200,200);
	child->EnableDockSize("ͼƬ�����",200,200);
	//ͣ�������·�
	RECT rc;
	GetWindowRect(m_navbar->Handle(),&rc);
	ScreenToClient(m_hWnd,(LPPOINT)&rc);
	ScreenToClient(m_hWnd,(LPPOINT)&rc+1);
	rc.top = (rc.top + rc.bottom)/2;
	child->DockTo(TBS_FLAG_DOCKLEFT,&rc);
	
	return DefaultProc(param);
}

RINGMAINMSG(WM_MENUSELECT)
{
	//��״̬����ʾ�˵�������Ϣ
	return m_statbar->OnMenuHelp(param,GetMenu());
}

RINGMAINMSG(WM_ACTIVATEAPP)
{
	if(param.wParam && child && child->m_bFullScreen)
	{
		//ȫ����ʽ������ͼƬ����Ϊ����ڣ�����ô��ڽ��ղ�������
		child->SetFocus();
	}
	return DefaultProc(param);
}

RINGMAINNOTIFY(TCN_SELCHANGE)
{
	//��ǩҳ֪ͨ��Ϣ
	if(param.lpnmhdr->hwndFrom == m_navbar->Handle())
		return m_navbar->OnTabSelect(param);	//���ɿؼ��Լ�����
	else if(param.lpnmhdr->hwndFrom == m_tab->Handle())
	{
		NavPane->SetViewMode(m_tab->GetCurSel());
		return m_tab->OnTabSelect(param);
	}
	return DefaultProc(param);
}

RINGMAINNOTIFY(TVN_SELCHANGED)
{
	//�û�ѡ��Ŀ¼�Ĵ����Ƚ��ɿؼ���Ӧ
	m_dirtree->OnSelectChanged(param);
	//���״̬����Ϣ
	SetStatusFilePart(m_dirtree->GetSelFolderName());
	m_statbar->SetValue("",3);
	ListFile();
	return 0;
}

void RINGMAINMETHOD(SetStatusFilePart)(LPCTSTR lpszFile)
{
	SHFILEINFO sfi;
	if(lpszFile)
	{
		SHGetFileInfo(lpszFile, 
							0,
							&sfi, 
							sizeof(SHFILEINFO), 
							SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
	
		if(m_iconFile)
		{
			m_statbar->SetIcon(2,NULL);
			DestroyIcon(m_iconFile);
		}
		m_iconFile = sfi.hIcon;
		m_statbar->SetIcon(2,m_iconFile);
		window->m_statbar->SetValue(sfi.szDisplayName,2);

		ringFile rf = lpszFile;
		FILETIME fTime;
		ringDateTime rdt;
		rf.GetFileTime(NULL,NULL,&fTime);
		rdt = &fTime;
		char szText[40];
		wsprintf(szText,"%d-%02d-%02d %02d:%02d",rdt.year(),rdt.month(),rdt.day(),
			rdt.hour(),rdt.minute());
		m_statbar->SetValue(szText,1);
	}
}

void RINGMAINMETHOD(ListFile)()
{
	//��ȡѡ���Ŀ¼���г�ͼ���ļ�
	ringDir dir(m_dirtree->GetSelFolderName());
	ringFile rf;
	ringStr str;
	char szFile[MAX_PATH],szSize[40];
	int index,nSubIdx,nSize,nData;
	FILETIME fTime;
	ringDateTime rdt;
	DWORD dwAttr,dwCheck;
	int nFileCnt=0,nTotalSize=0;

	if(IsWindowPtr(m_fileview))		//���m_fileview�Ƿ���Ч���Ѵ���
		m_fileview->Clear();

	if(dir.BeginFindFile())
	{
		SHFILEINFO sfi;
		while(dir.FindNext(szFile))
		{
			if(strcmp(szFile,"..") == 0)
				continue;

			rf.SetFile("%s\\%s",dir.Getdir(),szFile);
			str = rf.Extname();
			str.toLower();
			dwAttr = rf.GetAttrib() & (FILTER_FOLDER|FILTER_HIDDEN|FILE_ATTRIBUTE_ARCHIVE);
			dwCheck = m_dwFilter & dwAttr;
			if(MASK_MATCH(dwAttr,FILTER_HIDDEN) && !MASK_MATCH(dwCheck,FILTER_HIDDEN))
				dwCheck = 0;
			if((dwAttr != 0xFFFFFFFF && dwCheck > 0) && (MASK_MATCH(dwCheck,FILTER_FOLDER) ||
				(MASK_MATCH(m_dwFilter,FILTER_PIC) && (str == "bmp" || str == "jpg" || 
				str == "jpeg" || str == "gif" || str == "pcx" || str == "png")) ||
				MASK_MATCH(m_dwFilter,FILTER_OTHER)))
			{				
				index = m_fileview->GetCount();				
				SHGetFileInfo((LPCTSTR)rf.Fullname(), 
					0,
					&sfi, 
					sizeof(SHFILEINFO), 
					SHGFI_SYSICONINDEX|SHGFI_SMALLICON|
					SHGFI_DISPLAYNAME|SHGFI_ATTRIBUTES|SHGFI_TYPENAME);
				
				nData = nSize = rf.Size();
				nTotalSize += nSize;
				nFileCnt ++;
				if(nSize > 0)
					nSize = nSize/1024 + 1;

				if(!MASK_MATCH(sfi.dwAttributes,SFGAO_FOLDER))
				{
					memset(szSize,0,40);					
					int x = 1000000000;
					while(x > 1)
					{
						if(nSize/x > 0)
							wsprintf(szSize,"%s%d,",szSize,(nSize/x)%1000);
						x /= 1000;
					}
					wsprintf(szSize,"%s%dKB",szSize,nSize%1000);
				}
				else
				{
					wsprintf(szSize,"\0");
					nData = -1;
				}
				//���ð����ݣ���λͼ����ţ���λΪ�ߴ����λ������newһ��
				//�ṹ������¼��������Ϣ�����ߴ�������Ҫ�ȱȽ��ַ������ȣ�
				//Ȼ���ַ�����Ȼ��nData�ĸ�λ
				nData = MAKELONG(sfi.iIcon,nData%1000);
				m_fileview->AddItem(index,sfi.szDisplayName,nData,sfi.iIcon);
				nSubIdx = 1;
				m_fileview->SetItem(index,nSubIdx++,szSize);
				m_fileview->SetItem(index,nSubIdx++,sfi.szTypeName);

				rf.GetFileTime(NULL,NULL,&fTime);
				rdt = &fTime;
				wsprintf(szSize,"%d-%02d-%02d %02d:%02d",rdt.year(),rdt.month(),rdt.day(),
							rdt.hour(),rdt.minute());
				m_fileview->SetItem(index,nSubIdx++,szSize);
			}
		}
		m_fileview->SortItemsEx(RINGCLASS(NavPane)::SortFileProc,(NavPane->m_nMarkCol==-1));
		NavPane->UpdateFileList();
	}

	int nRest=0;
	if(nTotalSize < 1000)
		wsprintf(szSize,"�ֽ�");
	else if(nTotalSize < 1000000)
	{
		nRest = (nTotalSize%1000)/100;
		nTotalSize /= 1000;
		wsprintf(szSize,"KB");
	}
	else
	{
		nRest = ((nTotalSize/1000)%1000)/100;
		nTotalSize /= 1000000;
		wsprintf(szSize,"MB");
	}
	if(nRest == 0)
		wsprintf(szFile,"�ܼ� %d ���ļ�(%d%s)",nFileCnt,nTotalSize,szSize);
	else
		wsprintf(szFile,"�ܼ� %d ���ļ�(%d.%d%s)",nFileCnt,nTotalSize,nRest+1,szSize);
	m_statbar->SetValue(szFile,0);
}

RINGMAINMSG(WM_DESTROY)
{
	//�����ڹر�ʱ��������룬�����Ӧ����Ϣ��������������Quit()��
	//��PostQuitMessage,���򴰿����٣��������������ڴ档
	if(m_iconFile)
	{
		m_statbar->SetIcon(2,NULL);
		DestroyIcon(m_iconFile);
	}
	CoUninitialize();
	return Quit();	
}

//���Ʊ�ǩ
RINGMAINMSG(WM_DRAWITEM)
{
	if(param.lpdrawitem->CtlType == ODT_TAB)
	{
		if(param.lpdrawitem->itemAction == ODA_DRAWENTIRE)
		{
			RingTab* tab = NULL;
			if(param.wParam == IDC_NAVBAR)
				tab = m_navbar;
			else if(param.wParam == IDC_NAVPANE)
				tab = m_tab;

			if(tab)
			{
				LPSTR lps = (LPSTR)tab->GetText(param.lpdrawitem->itemID);
				int len = strlen(lps);
				SetBkMode(param.lpdrawitem->hDC,TRANSPARENT);
				if(param.lpdrawitem->itemState == ODS_SELECTED)
					DrawText(param.lpdrawitem->hDC,lps,len,&param.lpdrawitem->rcItem,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
				else
					DrawText(param.lpdrawitem->hDC,lps,len,&param.lpdrawitem->rcItem,DT_SINGLELINE|DT_CENTER|DT_BOTTOM);
			}
		}
	}
	return TRUE;
}

RINGMAINCMD(OnOpen)
{
	//������Ӵ��ļ�����...	
}

RINGMAINCMD(OnExit)
{
	//���ͳ����˳���Ϣ
	SendExit();
}

RINGMAINCMD(OnHelp)
{
	//��׼�Ի�����ô���
	DlgAbout = NewRingObj(DlgAbout);
	DlgAbout->DoModal();
	//���������δ�����Զ����٣�����SetAutoDelete��������Ҫ���ٶ���	
	DelRingObj(DlgAbout);
}

RINGMAINCMD(OnSetViewMode)
{
	//�����ļ��б�鿴��ʽ
	NavPane->OnSetViewMode(event);
}

RINGMAINCMD(OnSetFileFilter)
{
	//�����ļ�����
	RingCoolMenu* rm = GetCoolMenu();
	BOOL bCheck = FALSE;
	if(rm)
	{
		rm = rm->SubMenu(2)->SubMenu(1);
		bCheck = !MASK_MATCH(rm->GetItemCheckState(event),MFS_CHECKED);
		rm->CheckItem(event,bCheck);
	
		switch(event)
		{
		case CM_SHOWFOLDER:
			if(bCheck)
				m_dwFilter |= FILTER_FOLDER;
			else
				m_dwFilter &= ~FILTER_FOLDER;
			break;
		case CM_SHOWPIC:
			if(bCheck)
				m_dwFilter |= FILTER_PIC;
			else
				m_dwFilter &= ~FILTER_PIC;
			break;
		case CM_SHOWALBUM:
			if(bCheck)
				m_dwFilter |= FILTER_ALBUM;
			else
				m_dwFilter &= ~FILTER_ALBUM;
			break;
		case CM_SHOWMEDIA:
			if(bCheck)
				m_dwFilter |= FILTER_MEDIA;
			else
				m_dwFilter &= ~FILTER_MEDIA;
			break;
		case CM_SHOWOTHER:
			if(bCheck)
				m_dwFilter |= FILTER_OTHER;
			else
				m_dwFilter &= ~FILTER_OTHER;
			break;
		case CM_SHOWHIDDEN:
			if(bCheck)
				m_dwFilter |= FILTER_HIDDEN;
			else
				m_dwFilter &= ~FILTER_HIDDEN;
			break;
		}
		ListFile();
	}
}

RINGMAINCMD(OnSetColumn)
{
	//��ʾ/�����ļ��б��У�ΪRingListView����չ����
	RingCoolMenu* rm = GetCoolMenu();
	BOOL bCheck = FALSE;
	int idx = event-CM_COLSIZE;
	int nIndex = idx + 1;
	if(rm)
	{
		rm = rm->SubMenu(2)->SubMenu(2);
		bCheck = !MASK_MATCH(rm->GetItemCheckState(event),MFS_CHECKED);
		rm->CheckItem(event,bCheck);
		m_fileview->ShowColumn(nIndex,bCheck);
	}
}

RINGMAINCMD(OnSortColumn)
{
	//ѡ������
	NavPane->OnSortColumn(event);	
}

BOOL RingTabEx::OnDrawBkg(HDC hDC)
{
	//��ȡͣ���뱳��������λ��ƫ�ƽ��л��ƣ������������ڱ���һ��
	RingDockSite* tmp = ((RingWnd*)Parent())->GetDockSite(TBS_FLAG_DOCKTOP);
	RECT rc;
	SIZE offset={0,0};
	
	if(tmp)
	{		
		Parent()->GetClientRect(&rc);
		offset.cx = rc.left;
		offset.cy = rc.top;
		OffsetRect(&rc,-rc.left,-rc.top);
		return tmp->DrawBkg(hDC,&rc,&offset);
	}
	return FALSE;
}
