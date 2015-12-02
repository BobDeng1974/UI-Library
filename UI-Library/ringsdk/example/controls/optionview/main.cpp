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
//							�ٷ������������ʾ
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
//���ļ�˵������ʾ������OptionView��ʹ�á�
//
//������˵����	�����������ƺ�ֵ������ʾ��������ֵ�༭��ʽ���༭������
//					���༭��ʽ�����б��ѡ������루ѡ��ʽ�����༭������
//					������Աߵİ�ť���и���ϸ���ã�������뷽ʽ����
//					����ֵ����ʾֵ������ֵ��һ����������ֵ���ַ�������ʾ
//					ֵ������ֵ��һ�µģ��������ֵ���������Ҫ����������ֵ��
//					����[��ֹ�޸�|����]����ѡ�"��ֹ�޸�"Ϊ��ʾ���ƣ�"��
//					��"Ϊ��ʾֵ���������Ҫ����1Ϊ����ֵ�Է�������Ľ����
//					����ˣ���ȡѡ��ֵ�����¼��ַ�������[��ֹ�޸�|����]��
//					����ֵ1Ϊ������
//					GetName:��ȡѡ�����ƣ�����"��ֹ�޸�"
//					GetText:��ȡ��ʾֵ������"����"
//					GetInt:��ȡ����ֵ������1
//
**********************************************************************/
#include "ringmain.h"
#include "resource.h"

#define IOP_SELFILE	0
#define IOP_SELFONT	1
#define IOP_SHOWMSG	2

#define IDX_NAME				0
#define IDX_CANEDITNAME		1
#define IDX_TYPE				2
#define IDX_DATATYPE			3
#define IDX_DEFDATA			4
#define IDX_PACKLIST			5

BEGIN_SETUPMAINWINDOW(IS_DIALOG)
	BEGIN_DEF_VAR(NO_INIT)
		RingOptionView m_list,m_option;
		RingOptionItemSel m_itemSel;
		RingOptionItemList m_itemType,m_itemPackList,m_itemBro;
		RingOptionItemEdit m_itemVal;
		int m_index;
	END_DEF_VAR
	
	BOOL OnValChange(RingOptionItem* item)
	{
		int nIdx = m_option.GetSelectIndex(IDX_CANEDITNAME);
		if(nIdx == 1)
			//����FALSEΪ�����޸�
			return FALSE;
		else
			return TRUE;
	}

	BEGIN_DEFMSG
		DEFMSG(WM_INITDIALOG)
		DEFCMD(OnAdd)
		DEFCMD(OnDel)
		DEFCMD(OnClear)
		DEFNOTIFY(ROPVN_SELCHANGE)
		DEFNOTIFY(ROPVN_BUTTONCLICK)
		DEFNOTIFY(ROPVN_VALUECHANGE)
		DEFCMD(OnClose)
	END_DEFMAINMSG
END_SETUPMAINWINDOW

BEGIN_REGMAINMSG
	REGCMD(IDOK,OnAdd)
	REGCMD(IDCANCEL,OnDel)
	REGCMD(IDC_CLEAR,OnClear)
	REGCMD(IDC_CLOSE,OnClose)
	REGMSG(WM_INITDIALOG)
	REGNOTIFY(ROPVN_VALUECHANGE)
	REGNOTIFY(ROPVN_SELCHANGE)
	REGNOTIFY(ROPVN_BUTTONCLICK)
END_REGMSG

BEGIN_SETUPWINDOW(listdlg,IS_DIALOG)
	BINDID(IDD_LISTOPTION)
	BEGIN_DEF_VAR(NO_INIT)
		ringStr m_str;
		RingEdit* m_edit;
	END_DEF_VAR

	BEGIN_DEFMSG
		DEFMSG(WM_INITDIALOG)
		DEFCMD(OnOK)
	END_DEFMSG(listdlg)
END_SETUPWINDOW(listdlg)

BEGIN_REGMSG(listdlg)
	REGMSG(WM_INITDIALOG)
	REGCMD(IDOK,OnOK)
	REGCMD(IDCANCEL,Close)
END_REGMSG

int RingMain()
{
	APP_CREATEDIALOG(MAKEINTRESOURCE(IDD_MAIN));
	return RingApp.Run();
}

RINGMAINMSG(WM_INITDIALOG)
{
	AssignItem(m_list,IDC_LISTFILE);
	AssignItem(m_option,IDC_OPTION);
	m_list.SetBkColor(0xFFDEDA);

	m_index = 1;
	//���������
	m_option.AddOption("��������","Item1");
	//������б������OPTYPE_READONLY��ֻ��ѡ�񲻿�����
	m_option.AddOption("�����޸�","����",0,OPTYPE_LIST|OPTYPE_READONLY,(LPARAM)"����|��ֹ|ֻ��");
	m_option.AddOption("��������","����",0,OPTYPE_LIST|OPTYPE_READONLY,(LPARAM)"����|���ݰ�|���ݰ�����");
	//��Ŀ��������
	m_itemType.SetName("��������");
	//ͬ������Ϊֻ��ѡ�񲻿����룬��ѡ��Ӧ���ڼ���ǰ����
	m_itemType.SetReadOnly(TRUE);
	m_itemType.SetSelection("Ĭ��|�б�|���");
	m_itemType.SetSelectIndex(0);
	m_option.AddOption(&m_itemType);

	m_itemVal.SetName("Ĭ������");
	m_option.AddOption(&m_itemVal);
	
	//��Ŀ�������ã�������ӵ��б���
	m_itemPackList.SetName("���뵽���ݰ�");
	m_itemPackList.SetReadOnly(TRUE);
	m_itemSel.SetName("�б�ѡ��");
	m_itemSel.SetValue("��|��");
	m_itemBro.SetName("���ѡ��");
	m_itemBro.SetReadOnly(TRUE);
	m_itemBro.SetSelection("����ļ�|�������|��ʾ��Ϣ|������");
	m_itemBro.SetSelectIndex(3);

	//����������������ֵ�ı�Ĵ�����
	m_option.SetItemValChangeFunc(0,(RINGVCLASS*)this,(ROPV_CHECKVALFUNC)OnValChange);
	
	return FALSE;
}

RINGMAINCMD(OnAdd)
{
	ringStr strName = m_option.GetText(IDX_NAME);
	char s[20];
	int index;

	if(strName.len() > 0)
	{
		index = m_option.GetSelectIndex(IDX_TYPE);
		
		if(index == 1)
		{
			//���ݰ�
			m_list.AddOptionPack(strName.string());				
				
			m_index ++;				
			wsprintf(s,"���ݰ�%d",m_index);
			m_option.SetValue(s,0);
			
			m_itemPackList.AddSelection(strName.string());
			if(strlen(m_itemPackList.GetText()) == 0)
				m_itemPackList.SetValue(strName.string());				
			return;
		}
		else
		{
			ringStr strDef = m_option.GetText(IDX_DEFDATA);
			LONG lVal = 0,lOption = 0;
			int type = OPTYPE_EDIT;

			switch(m_option.GetSelectIndex(IDX_DATATYPE))
			{
			case 1://�б�
				type = OPTYPE_LIST;
				lOption = (LPARAM)m_itemSel.GetText();
				break;
			case 2://���
				type = OPTYPE_BROWSE;
				lVal = m_itemBro.GetSelectIndex();
				break;				
			}
			if(index == 0)		//����
				m_list.AddOption(strName.string(),strDef.string(),lVal,type,lOption);
			else	//���ݰ�����
				m_list.AddSubOption(m_option.GetText(IDX_PACKLIST),strName.string(),strDef.string(),lVal,type,lOption);
		}
		m_index ++;
		wsprintf(s,"item%d",m_index);
		m_option.SetValue(s,0);
	}
}

RINGMAINCMD(OnDel)
{
	RingOptionItem* item = m_list.GetOptionItem();
	if(item)
	{
		if(item->IsPackItem())
			m_itemPackList.DelSelection(item->GetName());
		m_list.DelOption(item);
	}
}

RINGMAINCMD(OnClear)
{
	m_itemBro.GetInt();
	m_list.Clear();
	m_itemPackList.Clear();
}

RINGMAINNOTIFY(ROPVN_SELCHANGE)
{
	if(param.lpropvnm->hdr.idFrom == IDC_OPTION)
	{		
		int index = param.lpropvnm->item->GetSelectIndex();

		switch(m_option.GetCurSel())
		{
			case IDX_TYPE:
			{	
				//�������ͱ��
				char szName[40];
				if(index == 1)
				{
					//���ݰ�
					wsprintf(szName,"���ݰ�%d",m_index);
					//�������ö�����Ҫ��ɾ��
					m_option.DelOption(&m_itemType);
					m_option.DelOption(&m_itemVal);
					m_option.DelOption(&m_itemPackList);
					m_option.DelOption(&m_itemSel);
					m_option.DelOption(&m_itemBro);
				}
				else
				{
					//���������ݰ�����
					wsprintf(szName,"Item%d",m_index);
					m_option.SetValue(szName,IDX_NAME);
					//AddOption���Զ�����Ƿ��Ѽ����
					m_option.AddOption(&m_itemType);
					m_option.AddOption(&m_itemVal);
					
					if(index == 0)
						//�������ݣ�ɾ�����ݰ��б�
						m_option.DelOption(&m_itemPackList);
					else
					{
						//�������������������ݰ��б���棬��ɾ��������˳�����
						m_option.DelOption(&m_itemSel);
						m_option.DelOption(&m_itemBro);
						//���ݰ����������ݰ��б�						
						m_option.AddOption(&m_itemPackList);
					}
						
					index = m_itemType.GetSelectIndex();
					if(index == 1)
						m_option.AddOption(&m_itemSel);	 //�б���������
					else if(index == 2)
						m_option.AddOption(&m_itemBro);	 //�����������
				}
				//������������
				m_option.SetValue(szName,IDX_NAME);
				break;
			}
			case IDX_DATATYPE:
			{
				//�������ͱ��
				if(index == 1)
				{
					//�б������б�ѡ�ɾ�����ѡ��
					m_option.AddOption(&m_itemSel);
					m_option.DelOption(&m_itemBro);
				}
				else if(index == 2)
				{
					//������������ѡ�ɾ���б�ѡ��
					m_option.DelOption(&m_itemSel);
					m_option.AddOption(&m_itemBro);
				}
				else
				{
					//Ĭ�ϣ�ɾ������Ҫ��ѡ��
					m_option.DelOption(&m_itemSel);
					m_option.DelOption(&m_itemBro);
				}
				break;
			}
			case IDX_CANEDITNAME:
			{				
				RingOptionItem* item = m_option.GetOptionItem(IDX_NAME);
				if(item)
				{
					if(index == 2)
						item->SetReadOnly(TRUE);	//ֻ��
					else
						//���޸Ļ�ʹ�޸���Ч������Ϊ��ֻ����ʹ�޸���Ч��
						//OnValChange�������ж�ʵ��
						item->SetReadOnly(FALSE);
				}
				break;
			}
		}
	}
	return 0;
}

RINGMAINNOTIFY(ROPVN_BUTTONCLICK)
{
	if(param.lpropvnm->hdr.idFrom == IDC_OPTION)
	{
		if(strcmp(param.lpropvnm->item->GetName(),"�б�ѡ��")==0)
		{
			listdlg = NewRingObj(listdlg);
			if(Popup(listdlg,NULL,(LPARAM)param.lpropvnm->item->GetText()))
				param.lpropvnm->item->SetValue(listdlg->m_str.string());
			DelRingObj(listdlg);
		}
	}
	else if(param.lpropvnm->hdr.idFrom == IDC_LISTFILE)
	{
		switch(param.lpropvnm->item->GetData())
		{
			case IOP_SELFILE:
			{
				ringFile rf;
				if(rf.Select("�����ļ�(*.*)|*.*||"))
					param.lpropvnm->item->SetValue(rf.Fullname());
				break;
			}
			case IOP_SELFONT:
			{
				ringFont font;
				if(font.Select())
				{
					LOGFONT lf;
					font.GetLogFont(&lf);
					param.lpropvnm->item->SetValue(lf.lfFaceName);
				}
				break;
			}
			case IOP_SHOWMSG:
			{
				LPSTR lpstr = (LPSTR)param.lpropvnm->item->GetText();
				if(lpstr)
					MsgBoxEx(param.lpropvnm->item->GetName(),MB_OK,strlen(lpstr)+1,lpstr);
				break;
			}
		}
	}
	return 0;
}

RINGMAINNOTIFY(ROPVN_VALUECHANGE)
{	
	return 0;
}

RINGMAINCMD(OnClose)
{
	Close();
}

RINGMSG(listdlg,WM_INITDIALOG)
{
	m_str = (LPSTR)param.lParam;
	m_str = m_str.replace("|","\r\n");
	
	m_edit = GetItem(m_edit,IDC_SLIST);
	if(m_edit)
		m_edit->SetValue(m_str.string());
	return 0;
}

RINGCMD(listdlg,OnOK)
{
	if(m_edit)
	{
		m_str = m_edit->GetText();
		m_str = m_str.replace("\r\n","|");
		Close(TRUE);
	}
	else
		Close();
}
