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
//˵���������Ӵ��ڴ���
//
**********************************************************************/
#include "global.h"

BEGIN_REGMSG(NavPane)
	REGMSG(WM_SIZE)
	REGNOTIFY(NM_DBLCLK)
	REGNOTIFY(LVN_COLUMNCLICK)
	REGNOTIFY(LVN_ITEMCHANGED)
	REGNOTIFY(TBN_DROPDOWN)
	REGNOTIFY(NM_RETURN)
	REGNOTIFY(LVN_SHOWCOLUMN)
	REGCMD(CM_UPPATH,OnUpParentPath)
	REGCMD(CM_ZOOMIN,OnZoomIn)
	REGCMD(CM_ZOOMOUT,OnZoomOut)
	REGCMD(CM_NEXTPIC,OnNextImg)
	REGCMD(CM_PREVPIC,OnPrevImg)
	REGCMDRANGE(CM_VIEWTD,CM_VIEWDETAIL,OnSetViewMode)
	REGCMDRANGE(CM_SHOWFOLDER,CM_SHOWHIDDEN,OnSetFileFilter)
	REGCMDRANGE(CM_SORTEXT,CM_SORTDESC,OnSortColumn)
	REGMSG(WM_DESTROY)
	REGMSG(WM_INITDIALOG)	
END_REGMSG

RINGMSG(NavPane,WM_INITDIALOG)
{
	ReplaceStyle(0,WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	
	//��������ͼIMAGELIST
	m_img = new RingImageList;
	m_img->Create(128,96,ILC_COLOR32,10,10);
	//�����ļ��б���
	m_fileview = NewRingObj(itemof(RingListView));
	m_fileview->Create(LVS_AUTOARRANGE|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_ALIGNTOP,//|LVS_OWNERDRAWFIXED,
							 WS_EX_CLIENTEDGE|RTV_FITPARENTSIZE,NULL,IDC_FILEVIEW,0,0,300,300);
	m_fileview->SetExStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);
	m_fileview->SetColumns("����|��С|����|�޸�ʱ��");
	m_fileview->SetColumnsWidth("200|80|130|130");

	//��ȡϵͳ�ļ�ͼ��IMAGELIST
	SHFILEINFO  sfi;
	m_hSysImgS = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\",0,&sfi,sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX|SHGFI_ICON|SHGFI_SMALLICON);
	m_fileview->SetImageList(m_hSysImgS,LVSIL_SMALL);
	m_hSysImgL = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\",0,&sfi,sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX|SHGFI_ICON|SHGFI_LARGEICON);
	m_fileview->SetImageList(m_hSysImgL,LVSIL_NORMAL);
	
	m_nBarBorder = 0;
	m_nMarkCol = 0;
	m_nOrder = 1;
	m_nViewMode = 0;
	m_nFileMode = CM_VIEWDETAIL;
	m_stateViewer = TBS_FLAG_DOCKLEFT;

	//���óߴ������ֿ���
	LVCOLUMN lvc;
	lvc.fmt = LVCFMT_RIGHT;
	lvc.mask = LVCF_FMT;
	m_fileview->SetColumn(1,&lvc);
	
	//������ͷ������
	m_fileview->SetColumnImageList();
	m_nMarkUp = m_fileview->AddColumnIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_COLMARKUP)));
	m_nMarkDown = m_fileview->AddColumnIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_COLMARKDOWN)));
	m_fileview->SetColumnImage(m_nMarkCol,m_nMarkUp,TRUE);
	m_hHeaderImg  = m_fileview->GetColumnImageList();
	//��������ͷ�Ҽ��˵�ѡ����ʾ/������
	RingCoolMenu* rm = m_fileview->EnableColumnMenu();
	if(rm)
   {		
		rm->SetItemDrawFunc(FUNC_DRAWMENU_XP);
		rm->SetBkgColor(0x00ffffff,0x00ff8080);
   }

	//����������
	m_toolbar = NewRingObj(itemof(RingToolBar));
	m_toolbar->Create(IDB_VIEWTOOL,TBSTYLE_FLAT);
	//������ť����������ť��ţ���ťͼ���ڹ�����ͼ������ţ������
	m_toolbar->InsertButton(0,0,CM_PREVPIC);
	m_toolbar->InsertButton(1,1,CM_NEXTPIC);
	m_toolbar->InsertSep(2);
	m_toolbar->InsertButton(3,2,CM_ZOOMOUT);
	m_toolbar->InsertButton(4,3,CM_ZOOMIN);
	m_toolbar->InsertSep(5);
	m_toolbar->InsertButton(6,5,3);
	m_toolbar->EnableDock("�鿴������",TBS_EX_NONE);
	m_toolbar->Show(SW_HIDE);

	m_navtool = NewRingObj(itemof(RingToolBar));
	m_navtool->Create(IDB_NAVTOOL,TBSTYLE_FLAT|TBSTYLE_LIST,
		TBSTYLE_EX_DRAWDDARROWS|TBSTYLE_EX_HIDECLIPPEDBUTTONS|TBSTYLE_EX_MIXEDBUTTONS);
	//������ť����������ť��ţ���ťͼ���ڹ�����ͼ������ţ������
	m_navtool->InsertButton(0,0,CM_UPPATH);
	m_navtool->InsertSep(1);
	m_navtool->InsertButton(2,5,CM_FILTER,"������",
		TBSTYLE_BUTTON|BTNS_AUTOSIZE|BTNS_WHOLEDROPDOWN|BTNS_SHOWTEXT);
	m_navtool->InsertButton(3,6,CM_SORTER,"����",
		TBSTYLE_BUTTON|BTNS_AUTOSIZE|BTNS_WHOLEDROPDOWN|BTNS_SHOWTEXT);
	m_navtool->InsertButton(4,7,CM_MODE,"�鿴",
		TBSTYLE_BUTTON|BTNS_AUTOSIZE|BTNS_WHOLEDROPDOWN|BTNS_SHOWTEXT);
	m_navtool->InsertSep(5);
	m_navtool->InsertButton(6,24,3);
	m_navtool->EnableDock("�ļ�������",TBS_EX_NONE);
	
	SetDockBackGround(MAKEINTRESOURCE(IDB_BKG),TRUE,-1,window->Handle());

	//�����̶߳���
	m_thread = new RingThread;
	return FALSE;
}

RINGMSG(NavPane,WM_DESTROY)
{
	//��WM_DESTROY����Close()���������WM_DESTROY��Ϣ��������жϴ����Ƿ��ѱ�����
	if(IsWindow())
	{
		delete m_thread;
		delete m_img;
		Close();
	}
	return FALSE;
}

RINGMSG(NavPane,WM_SIZE)
{
	//����ListView�͹�����������Ứ
	if(IsWindowPtr(m_fileview))
		m_fileview->Update(m_fileview->GetCount()-1);
	if(IsWindowPtr(m_toolbar))
		InvalidateRect(m_toolbar->Handle(),NULL,TRUE);
	if(IsWindowPtr(m_navtool))
		InvalidateRect(m_navtool->Handle(),NULL,TRUE);
	return DefaultProc(param);
}

//�����ڵ����ߴ磬������¼���֪ͨ�Ӵ��ڣ��Ӵ��������Ҫ���ڸú�������Ӧ�¼�
BOOL RINGMETHOD(NavPane,OnParentNotify)(HWND,UINT uMsg,UINT,LONG)
{
	if(uMsg == WM_SIZE)
	{
		RECT rc;
		Parent()->GetClientRect(&rc);
		MoveWindow(m_hWnd,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,FALSE);
	}
	return TRUE;
}

RINGNOTIFY(NavPane,TBN_DROPDOWN)
{
	//������������ť�˵�������Ĳ˵�ֱ�Ӱǵ������ڲ˵����Ӳ˵�
	LPNMTOOLBAR lpb = (LPNMTOOLBAR)param.lParam;
	if(lpb->iItem == CM_FILTER || lpb->iItem == CM_SORTER || lpb->iItem == CM_MODE)
	{
		RingCoolMenu* rm = window->GetCoolMenu();
		if(rm)
		{
			RECT rc;
			int idx,idxm;
			if(lpb->iItem == CM_FILTER)
			{
				idx = 2;
				idxm = 1;
			}
			else if(lpb->iItem == CM_SORTER)
			{
				idx = 3;
				idxm = 3;
			}
			else
			{
				idx = 4;
				idxm = 0;
			}
			m_navtool->GetButtonRect(idx,&rc);
			//ת������Ļ����
			ClientToScreen(m_navtool->Handle(),(LPPOINT)&rc);
			ClientToScreen(m_navtool->Handle(),(LPPOINT)&rc+1);
			//��ȡ�Ӳ˵�
			rm = rm->SubMenu(2)->SubMenu(idxm);
			//�˵�����ʱ���⸲�Ǹù�������ť���ܿ�ʱ���ȿ��Ǵ�ֱλ��
			rm->PopupEx(m_hWnd,rc.left,rc.bottom,FALSE,&rc);
		}
	}
	return TBDDRET_DEFAULT;
}

RINGNOTIFY(NavPane,LVN_ITEMCHANGED)
{
	//ѡ���ļ�����ʾͼƬ
	if(param.lpnmlistview->iItem != -1 &&
		MASK_MATCH(param.lpnmlistview->uNewState,LVIS_SELECTED))
	{
		char szFile[MAX_PATH];		
		int nData = m_fileview->GetData(param.lpnmlistview->iItem);
		LPCTSTR lps = m_fileview->GetText(param.lpnmlistview->iItem,0);
		
		wsprintf(szFile,"%s\\%s\0",window->m_dirtree->GetSelFolderName(TRUE),lps);
		window->SetStatusFilePart(szFile);
		if(HIWORD(m_fileview->GetData(param.lpnmlistview->iItem)) != 0xFFFF)
		{
			//���ļ��У���ͼ��ʾͼƬ��������ͼģʽ�������̼߳���Ŀ¼������ͼƬ��
			//��RingDIB��֧�ֶ��̣߳����������Ҫͬ��
			m_guard.Enter();
			try
			{
				child->View(szFile);
			}
			catch(...)
			{
			}
			m_guard.Leave();
		}
	}
	return 0;
}

RINGNOTIFY(NavPane,NM_RETURN)
{
	//��Ӧ�س�����ȫ����ʾͼƬ
	int nIndex = m_fileview->GetCurSel();
	
	if(nIndex >= 0)
	{
		char szFile[MAX_PATH];
		wsprintf(szFile,"%s\\%s\0",window->m_dirtree->GetSelFolderName(),
			m_fileview->GetText(nIndex,0));
		if(m_fileview->GetData(nIndex) != -1)
		{
			child->SetFullScreen(TRUE);
			child->View(szFile);
		}
	}
	return 0;
}

RINGNOTIFY(NavPane,NM_DBLCLK)
{
	////˫���¼���Ӧ���ж��ǽ�����Ŀ¼����ȫ����ʾͼƬ
	if(param.lpnmhdr->hwndFrom == m_fileview->Handle())
	{
		char szFile[MAX_PATH];
		LPNMITEMACTIVATE lpv = (LPNMITEMACTIVATE)param.lParam;
		wsprintf(szFile,"%s\\%s\0",window->m_dirtree->GetSelFolderName(),
					m_fileview->GetText(lpv->iItem,0));
		if(HIWORD(m_fileview->GetData(lpv->iItem)) == 0xFFFF)
		{
			//Ŀ¼
			HTREEITEM hti = window->m_dirtree->GetSelection();
			window->m_dirtree->Expand(hti);
			hti = window->m_dirtree->SearchItem(m_fileview->GetText(lpv->iItem,0),hti);
			if(hti)
				window->m_dirtree->Select(hti);
		}
		else
		{
			child->SetFullScreen(TRUE);
			child->View(szFile);
		}
	}
	return 0;
}

RINGNOTIFY(NavPane,LVN_COLUMNCLICK)
{
	//��ͷ�����Ӧ��ѡ������
	if(param.lpnmlistview->hdr.hwndFrom == m_fileview->Handle())
	{
		RingCoolMenu* rm = window->GetCoolMenu();
		if(rm)
			rm = rm->SubMenu(2)->SubMenu(3);

		if(m_nMarkCol == param.lpnmlistview->iSubItem)
		{
			m_nOrder = -m_nOrder;
			if(rm)
				rm->CheckRadioItem(CM_SORTASEC,CM_SORTDESC,m_nOrder>0?CM_SORTASEC:CM_SORTDESC,BY_COMMAND);
		}
		else
		{
			m_fileview->SetColumnImage(m_nMarkCol,-1);
			m_nMarkCol = param.lpnmlistview->iSubItem;
			if(rm)
				rm->CheckRadioItem(CM_SORTEXT,CM_SORTTIME,m_nMarkCol + CM_SORTNAME,BY_COMMAND);
		}
		
		m_fileview->SetColumnImage(m_nMarkCol,m_nOrder>0?m_nMarkUp:m_nMarkDown,TRUE);
		m_fileview->SortItemsEx(SortFileProc,0);
	}
	return 0;
}

RINGNOTIFY(NavPane,LVN_SHOWCOLUMN)
{
	RingCoolMenu* rm = window->GetCoolMenu();
	LPNMHEADER hd = (LPNMHEADER)param.lParam;
	if(rm)
	{
		rm = rm->SubMenu(2)->SubMenu(2);
		rm->CheckItem(hd->iItem-1,hd->iButton,BY_POSITION);
	}
	return 0;
}

//���ϲ�Ŀ¼
RINGCMD(NavPane,OnUpParentPath)
{
	HTREEITEM hti = window->m_dirtree->GetSelection();
	if(hti)
		hti = window->m_dirtree->GetParentItem(hti);
	
	if(hti)
		window->m_dirtree->Select(hti);
}

RINGCMD(NavPane,OnZoomIn)
{
	child->ZoomIn();
}

RINGCMD(NavPane,OnZoomOut)
{
	child->ZoomOut();
}

RINGCMD(NavPane,OnNextImg)
{
	child->ViewNextImg();
	m_fileview->EnsureVisible(m_fileview->GetCurSel());
}

RINGCMD(NavPane,OnPrevImg)
{
	child->ViewNextImg(FALSE);
	m_fileview->EnsureVisible(m_fileview->GetCurSel());
}

RINGCMD(NavPane,OnSetFileFilter)
{
	window->OnSetFileFilter(event);
}

//����
RINGCMD(NavPane,OnSortColumn)
{
	RingCoolMenu* rm = window->GetCoolMenu();
	if(rm)
	{
		rm = rm->SubMenu(2)->SubMenu(3);
		if(event < CM_SORTASEC)
		{
			rm->CheckRadioItem(CM_SORTEXT,CM_SORTTIME,event,BY_COMMAND);
			if(event == CM_SORTEXT)
			{
				m_fileview->SetColumnImage(m_nMarkCol,-1);
				m_nMarkCol = -1;
			}
			else
			{
				m_fileview->SetColumnImage(m_nMarkCol,-1);
				m_nMarkCol = event - CM_SORTNAME;
			}
		}
		else
		{
			rm->CheckRadioItem(CM_SORTASEC,CM_SORTDESC,event,BY_COMMAND);
			m_nOrder = (event==CM_SORTASEC)?1:-1;
		}
	}	
	m_fileview->SetColumnImage(m_nMarkCol,m_nOrder>0?m_nMarkUp:m_nMarkDown,TRUE);
	m_fileview->SortItemsEx(SortFileProc,(m_nMarkCol==-1));

}

//�����ļ��б����鿴��ʽ
RINGCMD(NavPane,OnSetViewMode)
{
	if(m_nFileMode == (int)event)
		return;

	int nOldMode = m_nFileMode;
	BOOL bUpdate = FALSE;
	RingCoolMenu* rm = window->GetCoolMenu();
	if(rm)
	{
		rm = rm->SubMenu(2)->SubMenu(0);
		rm->CheckRadioItem(CM_VIEWTD,CM_VIEWDETAIL,event,BY_COMMAND);
	}
	
	m_nFileMode = event;
	switch(event)
	{
	case CM_VIEWBIGICON:
		m_fileview->SetViewOf(LVS_ICON);
		if(nOldMode <= CM_VIEWD)
			bUpdate = TRUE;
		break;
	case CM_VIEWSMALLICON:
		m_fileview->SetViewOf(LVS_SMALLICON);
		if(nOldMode <= CM_VIEWD)
			bUpdate = TRUE;
		break;
	case CM_VIEWLIST:
		m_fileview->SetViewOf(LVS_LIST);
		if(nOldMode <= CM_VIEWD)
			bUpdate = TRUE;
		break;
	case CM_VIEWDETAIL:
		if(nOldMode == CM_VIEWTD)
			m_fileview->EnsureVisible(0);
		m_fileview->SetViewOf(LVS_REPORT);
		if(nOldMode <= CM_VIEWD)
			bUpdate = TRUE;
		break;
	case CM_VIEWD:
		m_fileview->SetViewOf(LVS_ICON);
		if(nOldMode == CM_VIEWTD)	//����ͼ���л���UPDATE����
			m_fileview->Update(m_fileview->GetCount()-1);
		else
			bUpdate = TRUE;
		break;
	case CM_VIEWTD:
		if(nOldMode == CM_VIEWDETAIL)
			m_fileview->EnsureVisible(0);
		m_fileview->SetViewOf(LVS_REPORT);
		if(nOldMode == CM_VIEWD)	//����ͼ���л���UPDATE����
			m_fileview->Update(m_fileview->GetCount()-1);
		else
			bUpdate = TRUE;
		break;
	}
	if(bUpdate)
	{
		//����ֹ�����������е��߳�
		m_thread->Quit(1);
		//����
		m_thread->Start(this,(THREADFUNC)ViewThumbProc,nOldMode);
	}
}

void RINGMETHOD(NavPane,UpdateFileList)()
{
	//�������ͼ
	m_img->Clear();

	if(m_nFileMode <= CM_VIEWD)
	{
		//����ֹ�����������е��߳�
		m_thread->Quit(1);
		//����
		m_thread->Start(this,(THREADFUNC)ViewThumbProc,m_nFileMode);
	}
}

/************************************************************
//
//��������ʾ����ͼ�߳�
//
//��������������������̣߳�
//1��������ͼ��ʾģʽ�л�������ͼ��ʾģʽ������ͼ������ͼ+��ϸ���ϣ�
//2������ͼ��ʾģʽ�л���������ͼ��ʾģʽ
//3������ͼ��ʾģʽ���л��ļ��У���ʱnOldMode==m_nFileMode��
//������ͼ��ʾģʽ���л�������ͼ��ʾģʽ���л����������߳�
//
//���߳���ɹ�����
//��δ��������ͼ��m_img->Count()==0������������ͼ�����뵽m_img
//������������ͼ����Ҫ�л�IMAGELIST���л�IMAGELIST��ȡԭ��¼��
//IMAGE��Ž������ã�ͬʱ��¼ÿһ��ԭIMAGE����Ա���һ���л�
//
//**********************************************************/
DWORD RINGMETHOD(NavPane,ViewThumbProc)(LONG nOldMode)
{
	int cnt = m_fileview->GetCount();
	int nData,nImg;
	LVITEM lvm;

	lvm.mask = LVIF_IMAGE|LVIF_PARAM;
	lvm.iSubItem = 0;

	//������ͼ��ʾģʽ���л�������ͼ��ʾģʽ���л����������̣߳�
	//��˿���m_nFileMode��nOldMode��С���ж�
	if(m_nFileMode > nOldMode)	//����ͼ��ʾģʽ�л���������ͼ��ʾģʽ
	{
		//�ָ�ԭIMAGELIST
		m_fileview->SetImageList(m_hSysImgS,LVSIL_SMALL);
		m_fileview->SetImageList(m_hSysImgL,LVSIL_NORMAL);
		//����IMAGELIST��ͬʱ������ͷ��IMAGELIST��������ٴ����û���
		m_fileview->SetColumnImageList(m_hHeaderImg);
		m_fileview->SetColumnImage(m_nMarkCol,m_nOrder>0?m_nMarkUp:m_nMarkDown,TRUE);

		//��¼��ǰͼ�꣬�ָ�ԭϵͳͼ��
		for(int i=0;i<cnt;i++)
		{
			lvm.iItem = i;
			m_fileview->GetItemInfo(&lvm);
			nData = lvm.lParam;
			lvm.lParam = MAKELONG(lvm.iImage,HIWORD(nData));
			lvm.iImage = LOWORD(nData);
			m_fileview->SetItem(&lvm);			
		}
		m_fileview->Update(i-1);
		m_fileview->EnsureVisible(m_fileview->GetCurSel());
		return 0x900D;
	}
	else
	{
		//����ͼ��ʾģʽ
		m_fileview->SetImageList(*m_img,LVSIL_SMALL);
		m_fileview->SetImageList(*m_img,LVSIL_NORMAL);
		//����IMAGELIST��ͬʱ������ͷ��IMAGELIST��������ٴ����û���
		m_fileview->SetColumnImageList(m_hHeaderImg);
		m_fileview->SetColumnImage(m_nMarkCol,m_nOrder>0?m_nMarkUp:m_nMarkDown,TRUE);

		if(m_img->Count() > 0 && nOldMode != m_nFileMode)
		{
			//������ͼ��ʾģʽ�л�������ͼ��ʾģʽ������������ͼ��
			//��¼��ǰͼ�꣬�ָ�ԭ����ͼͼ��
			for(int i=0;i<cnt;i++)
			{
				lvm.iItem = i;
				m_fileview->GetItemInfo(&lvm);
				nData = lvm.lParam;
				lvm.lParam = MAKELONG(lvm.iImage,HIWORD(nData));
				lvm.iImage = LOWORD(nData);
				m_fileview->SetItem(&lvm);
				m_fileview->Update(i);
			}
			m_fileview->EnsureVisible(m_fileview->GetCurSel());
			return 0x900D;					
		}			

		//��������ͼ
		RingDIB dibPic;
		char szFile[MAX_PATH];
		LPSTR lps = (LPSTR)window->m_dirtree->GetSelFolderName();
		int w,h;
		HDC hDC = GetDC(NULL);
		HDC hMemDC = CreateCompatibleDC(hDC);
		HDC hBkgDC = CreateCompatibleDC(hDC);
		HBITMAP hobm,hbm = CreateCompatibleBitmap(hDC,128,96);
		HBITMAP hbmBkg = LoadBitmap(GetInstance(),MAKEINTRESOURCE(IDB_THUMBBKG));
		HBRUSH hbr = (HBRUSH)GetStockObject(GRAY_BRUSH);
		RECT rc;
		BOOL bLoadOK;

		hobm = (HBITMAP)SelectObject(hMemDC,hbm);
		SelectObject(hBkgDC,hbmBkg);

		m_img->Clear();
		
		window->m_progress->SetRange(0,cnt-1);
		window->m_progress->SetStep(1);

		for(int i=0;i<cnt;i++)
		{
			if(!m_thread->IsRunning())		  //��ѯ�߳��˳���־
				break;

			nImg = -1;

			SetRect(&rc,0,0,128,96);
			BitBlt(hMemDC,0,0,128,96,hBkgDC,0,0,SRCCOPY);
			DrawEdge(hMemDC,&rc,BDR_RAISEDINNER,BF_RECT);

			wsprintf(szFile,"%s\\%s\0",lps,m_fileview->GetText(i,0));
			lvm.iItem = i;
			m_fileview->GetItemInfo(&lvm);
			nData = lvm.lParam;
			//Ϊ���û���ʱ����ļ��鿴����Ҫ����ͬ��
			m_guard.Enter();
			try
			{
				bLoadOK = (HIWORD(nData) != 0xFFFF && dibPic.Load(szFile));
			}
			catch(...)
			{
				bLoadOK = FALSE;
			}
			m_guard.Leave();
			
			if(bLoadOK)
			{
				//��������ͼ
				if(dibPic.Width() > dibPic.Height())
				{
					w = 120;
					h = 120 * dibPic.Height() / dibPic.Width();
				}
				else
				{
					h = 90;
					w = 90 * dibPic.Width() / dibPic.Height();
				}
				rc.left = (128-w)/2 - 1;
				rc.top = (96-h)/2 - 1;
				rc.right = rc.left + w + 1;
				rc.bottom = rc.top + h + 1;

				dibPic.Draw(hMemDC,rc.left + 1,rc.top + 1,0,0,w,h);
				
				lvm.lParam = MAKELONG(lvm.iImage,HIWORD(nData));
			}
			else
			{
				//Ŀ¼������ͼ��ʧ�ܣ������������ɵ�ͼ������ͼ
				h = lvm.iImage;

				for(int j=0;j<i;j++)
				{
					lvm.iItem = j;
					m_fileview->GetItemInfo(&lvm);
					if(h == LOWORD(lvm.lParam))
					{
						//��ǰIMAGE�����ǰ�����¼��ԭIMAGE�����ͬ������
						//ƥ���������ͼ���
						nImg = lvm.iImage;
						break;
					}
				}
				lvm.lParam = MAKELONG(h,HIWORD(nData));
				if(nImg == -1)
				{
					//����ͼ������ͼ
					ImageList_GetIconSize(m_hSysImgL,&w,&h);
					rc.left = (128-w)/2 - 1;
					rc.top = (96-h)/2 - 1;
					rc.right = rc.left + w + 1;
					rc.bottom = rc.top + h + 1;
					ImageList_Draw(m_hSysImgL,LOWORD(nData),hMemDC,rc.left+1,rc.top+1,ILD_TRANSPARENT);
				}				
			}
			DrawEdge(hMemDC,&rc,BDR_SUNKENINNER,BF_RECT);
			
			SelectObject(hMemDC,hobm);
			if(nImg == -1)
				lvm.iImage = m_img->Add(hbm);
			else
				lvm.iImage = nImg;
			lvm.iItem = i;
			m_fileview->SetItem(&lvm);
			m_fileview->Update(i);

			hobm = (HBITMAP)SelectObject(hMemDC,hbm);
			window->m_progress->StepIt();
			Sleep(1);
		}
		window->m_progress->Clear();
		//m_fileview->Update(i-1);

		DeleteObject(hbmBkg);
		DeleteObject(hbm);
		DeleteDC(hBkgDC);
		DeleteDC(hMemDC);		
		ReleaseDC(m_hWnd,hDC);		
	}
	return 0xDEAD;
}

/*
RINGMSG(NavPane,WM_DRAWITEM)
{
	if(param.lpdrawitem->CtlType == ODT_LISTVIEW)
	{
		LPCTSTR lpc = m_fileview->GetText(param.lpdrawitem->itemID,0,MAX_PATH);
		RECT rc;
		int x=1,y;
		
		if((param.lpdrawitem->itemID & 0x1) == 1)
		{
			if((param.lpdrawitem->itemState & ODS_SELECTED))
			{
				FillRect(param.lpdrawitem->hDC,&param.lpdrawitem->rcItem,m_brsel);
				SetTextColor(param.lpdrawitem->hDC,m_crSel);
			}
			else
			{
				FillRect(param.lpdrawitem->hDC,&param.lpdrawitem->rcItem,m_br1);
				SetTextColor(param.lpdrawitem->hDC,0);
			}
			y = 1;
		}
		else
		{
			if((param.lpdrawitem->itemState & ODS_SELECTED))
			{
				FillRect(param.lpdrawitem->hDC,&param.lpdrawitem->rcItem,m_brsel);
				SetTextColor(param.lpdrawitem->hDC,m_crSel);
			}
			else
			{
				FillRect(param.lpdrawitem->hDC,&param.lpdrawitem->rcItem,m_br2);
				SetTextColor(param.lpdrawitem->hDC,0);
			}
			y = 0;
		}
		
		for(int i=0;i<20;i++)
		{
			lpc = m_fileview->GetText(param.lpdrawitem->itemID,i,MAX_PATH);
			if(lpc && strlen(lpc) > 0)
			{
				ListView_GetSubItemRect(m_fileview->Handle(),
					param.lpdrawitem->itemID,
					i,LVIR_BOUNDS,&rc);
				TextOut(param.lpdrawitem->hDC,
					rc.left+2,
					rc.top+2,
					lpc,
					strlen(lpc));
				//if(x > 0)
				InflateRect(&rc,x,y);
				if(x == 1)
					x = 0;
				else
					x = 1;
				
				FrameRect(param.lpdrawitem->hDC,&rc,m_brline);
			}
			else
				break;
		}
		return TRUE;
	}
	return DefaultProc(param);
}
*/

//////////////////////////////////////////////////////////
//
//�ı�鿴ģʽ����ΪNavPane���������ڣ�����������ڲ�ͬ����
//ͣ����Ҫ������ԭ���ڣ�����ͣ����Ȼ����ͣ����Ŀ�괰�ڡ�
//
//ע��ͬһ�����ڲ�ͬ����ͣ��������ͣ�����ڵķǵ����÷���һ��
//		�ò��š�
//
//////////////////////////////////////////////////////////
void RINGMETHOD(NavPane,SetViewMode)(int iFlag)
{
	if(iFlag != m_nViewMode)
	{
		//�ı�鿴ģʽ
		if(iFlag == 0)
		{
			//�ļ��б�ģʽ
			m_toolbar->Show(SW_HIDE);

			if(child->Parent() == this)
			{
				child->DisableDock();
				child->SetParent(window->Handle());
				child->SetParentObj(window->Handle());

				child->EnableDockSize("ͼƬ�����",
											m_RectViewer.right-m_RectViewer.left-2*m_nBarBorder,
											m_RectViewer.bottom-m_RectViewer.top-2*m_nBarBorder,
											m_stateViewer);
				child->DockTo(m_stateViewer,&m_RectViewer);
			}
			m_nViewMode = iFlag;
		}
		else if(iFlag == 1)
		{
			//�鿴ģʽ
			if(child->Parent() == window)
			{
				//�������������ͣ��λ�ã��Ա��ٴ�ͣ����ȥ
				RingDockBar *bar = child->GetDockBar();
				if(bar)
				{
					bar->GetDockRect(&m_RectViewer);
					m_stateViewer = bar->GetState();
					m_nBarBorder = bar->GetBarBorder();
				}
				m_toolbar->Show();
				child->DisableDock();
				child->SetParent(m_hWnd);
				child->SetParentObj(m_hWnd);				
				child->EnableDockSize("ͼƬ�����",200,200,TBS_FLAG_DOCKTOP,TBS_EX_NONE);
				
				//����ͣ�����������ǰ��߶����߼��㣬ͼƬ��ʾ���ڽϸߣ���ͣ�������һ�У����
				//����Ҫ����ʹ�ļ�����������ͣ�������һ��
				RECT rc;
				GetWindowRect(m_navtool->Handle(),&rc);
				OffsetRect(&rc,-rc.left,child->Height());
				m_navtool->DockTo(TBS_FLAG_DOCKTOP,&rc);
			}
			m_nViewMode = iFlag;
		}
	}	
}

//�ļ�����ص�����
int CALLBACK RINGMETHOD(NavPane,SortFileProc)(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	char szCmp[MAX_PATH];
	int nRes;
	memset(szCmp,0,MAX_PATH);
	if(lParamSort)
	{
		//��չ������
		ringFile rf1 =	NavPane->m_fileview->GetText(lParam1,0,MAX_PATH);
		ringFile rf2 =	NavPane->m_fileview->GetText(lParam2,0,MAX_PATH);
		nRes = strcmpi(rf1.Extname(),rf2.Extname());
	}
	else
	{
		//����
		NavPane->m_fileview->GetText(lParam1,NavPane->m_nMarkCol,MAX_PATH,szCmp);
		if(HIWORD(NavPane->m_fileview->GetData(lParam1)) == 0xFFFF || 
			HIWORD(NavPane->m_fileview->GetData(lParam2)) == 0xFFFF)
		{
			//�ļ���
			nRes = (short)HIWORD(NavPane->m_fileview->GetData(lParam1)) - (short)HIWORD(NavPane->m_fileview->GetData(lParam2));
			if(nRes == 0)
				nRes = strcmpi(szCmp,NavPane->m_fileview->GetText(lParam2,NavPane->m_nMarkCol,MAX_PATH));
		}
		else if(NavPane->m_nMarkCol == 1)
		{
			//�ߴ�
			nRes = strlen(szCmp) - strlen(NavPane->m_fileview->GetText(lParam2,NavPane->m_nMarkCol,MAX_PATH));
			if(nRes == 0)
			{
				nRes = strcmpi(szCmp,NavPane->m_fileview->GetText(lParam2,NavPane->m_nMarkCol,MAX_PATH));
				if(nRes == 0)
					//data��λΪ�ߴ�����(�ļ���Ϊ-1����С)
					nRes = (short)HIWORD(NavPane->m_fileview->GetData(lParam1)) - 
							 (short)HIWORD(NavPane->m_fileview->GetData(lParam2));
			}
		}
		else
			nRes = strcmpi(szCmp,NavPane->m_fileview->GetText(lParam2,NavPane->m_nMarkCol,MAX_PATH));
	}
	return nRes * NavPane->m_nOrder;
}

