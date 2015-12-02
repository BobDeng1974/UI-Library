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
//���ļ�˵������ʾ��ť���ʹ�á�
//
**********************************************************************/
#include "ringmain.h"
#include "resource.h"

BEGIN_SETUPMAINWINDOW(IS_DIALOG)	//�������ǶԻ���
	BEGIN_DEF_VAR(NO_INIT)
		RingButton m_btIcon,m_btBmp,m_btCust,m_btDef;
		RingButton* m_lpbtn;
	END_DEF_VAR

	BEGIN_DEFMSG
		DEFMSG(WM_INITDIALOG)
		DEFCMD(OnBtBmpClick)
		DEFCMD(OnAlignClick)
		DEFCMD(OnIconPosClick)
		DEFCMD(OnExit)		
	END_DEFMAINMSG
END_SETUPMAINWINDOW

BEGIN_REGMAINMSG
	REGMSG(WM_INITDIALOG)
	REGCMD(IDC_BTNBMP,OnBtBmpClick)
	REGCMD(IDC_BTNCUST,OnAlignClick)
	REGCMD(IDC_BTNOWNIMG,OnIconPosClick)
	REGCMD(IDOK,OnExit)
	REGCMD(IDCANCEL,OnExit)
END_REGMSG

int RingMain()
{
	APP_CREATEDIALOG(MAKEINTRESOURCE(IDD_MAIN));
	return RingApp.Run();
}

RINGMAINMSG(WM_INITDIALOG)
{
	//ֱ�����ɵĶ�����AssignItem����
	AssignItem(m_btDef,IDC_STANDARD);
	AssignItem(m_btIcon,IDC_BTNICON);
	AssignItem(m_btBmp,IDC_BTNBMP);
	AssignItem(m_btCust,IDC_BTNCUST);
	//��������ָ����GetItem
	m_lpbtn = GetItem(itemof(RingButton),IDC_BTNOWNIMG);
	//��m_lpbtn = GetItem(m_lpbtn,IDC_BTNOWNIMG);
	
	//����ͼ��
	m_btIcon.SetIcon(IDI_ICON1,128,128);
	//��������Ϊ�޽�����ο�
	m_btIcon.SetExStyle(RINGBTN_EX_NOFOCUSRECT);

	//����ͼ��
	m_btBmp.SetBitmap(IDB_BEER);
	
	m_btCust.AddIcon(IDI_ICON1,ATBTN_LEFT,16,16);
	m_btCust.SetBkColor(0x007f7fff);
	m_btCust.SetTextColor(0x004f4eFF);
	m_lpbtn->AddBitmap(IDB_TIME,ATBTN_LEFT);
	m_lpbtn->SetExStyle(RINGBTN_EX_FLAT);
	
	m_btDef.SetBkColor(0x00FF0000);
	m_btDef.SetTextColor(0x000000FF);
	return FALSE;
}

RINGMAINCMD(OnAlignClick)
{
	//eventΪ�������Ĳ�����ΪWM_COMMAND��Ϣ���HIWORD(wParam)��RINGCMD�������˸ò���.
	//���ù�������������ɫ��BUTTONΪOWNERDRAW���ͣ��ᷢ��BN_PUSHED����Ϣ��������ж�
	//�Ƿ����˰�ť��������Ϣ��
	if(event == BN_CLICKED)	
	{
		if(strcmp(m_btCust.GetText(),"���������") == 0)
		{
			m_btCust.ReplaceStyle(BS_LEFT,BS_CENTER);
			m_btCust.SetValue("���־���");
		}
		else if(strcmp(m_btCust.GetText(),"���־���") == 0)
		{
			m_btCust.ReplaceStyle(BS_CENTER,BS_RIGHT);
			m_btCust.SetValue("�����Ҷ���");
		}
		else		
		{
			m_btCust.ReplaceStyle(BS_RIGHT,BS_LEFT);
			m_btCust.SetValue("���������");
		}
	}
}

RINGMAINCMD(OnIconPosClick)
{
	if(m_lpbtn && event == BN_CLICKED)
	{		
		if(strcmp(m_lpbtn->GetText(),"ͼ������") == 0)
		{
			m_lpbtn->AddBitmap(IDB_TIME,ATBTN_TOP);
			m_lpbtn->SetValue("ͼ������");
		}
		else if(strcmp(m_lpbtn->GetText(),"ͼ������") == 0)
		{
			m_lpbtn->AddBitmap(IDB_TIME,ATBTN_RIGHT);
			m_lpbtn->SetValue("ͼ������");
		}
		else if(strcmp(m_lpbtn->GetText(),"ͼ������") == 0)
		{
			m_lpbtn->AddBitmap(IDB_TIME,ATBTN_BOTTOM);
			m_lpbtn->SetValue("ͼ������");
		}
		else if(strcmp(m_lpbtn->GetText(),"ͼ������") == 0)
		{
			m_lpbtn->AddBitmap(IDB_TIME,ATBTN_LEFT);
			m_lpbtn->SetValue("ͼ������");
		}		
	}
}

RINGMAINCMD(OnBtBmpClick)
{
	if(MASK_MATCH(m_btBmp.GetStyle(),BS_BITMAP))
	{		
		m_btBmp.SetIcon(IDI_ICON1,16,16);
		m_btBmp.SetPos(0,0,32,32,NULL,SWP_NOMOVE);
	}
	else
	{
		m_btBmp.SetBitmap(IDB_BEER);
		m_btBmp.SetPos(0,0,42,128,NULL,SWP_NOMOVE);
	}
}

RINGMAINCMD(OnExit)
{
	Close(0);
}
