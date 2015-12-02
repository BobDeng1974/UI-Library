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
//�汾��0.1
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����ringpropsheet.h
//
//���ļ�˵��������ҳ�ඨ��
//
//��⹦�ܣ�����ҳ�ඨ�塣
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingPropSheet::RingPropSheet()
{
	m_windowType = ITIS_PROPSHEET;
	m_psh.dwSize = sizeof(PROPSHEETHEADER);
	//m_psh.dwFlags = PSH_DEFAULT;
	m_psh.hwndParent = m_parent->Handle();
	m_psh.hInstance = GetInstance();
	m_psh.nPages = 0;
	m_psh.pfnCallback = NULL;

	m_lppsp = NULL;
	m_PageSize = 0;
	SetAutoDelete();
}

RingPropSheet::~RingPropSheet()
{
	m_lppsp = (HPROPSHEETPAGE*)Del(m_lppsp);
}

HWND RingPropSheet::Create(LPCTSTR lpszTitle/*=NULL*/,LPARAM dwInitParam/* = 0*/)
{
	return NULL;
}

BOOL RingPropSheet::DoModal(LPCTSTR lpszTitle/*=NULL*/,LPARAM dwInitParam/* = 0*/)
{
	m_psh.dwFlags &= ~PSH_MODELESS;

	if(m_lppsp == NULL || m_psh.nPages <= 0)
		return FALSE;

	m_psh.phpage = m_lppsp;
	m_psh.dwFlags = PSH_PROPTITLE;
	m_psh.hwndParent = m_parent->Handle();

	m_psh.pszCaption = lpszTitle;

	PROPSHEETHEADER psh;
	psh.dwSize = sizeof(PROPSHEETHEADER) ;
	psh.dwFlags = PSH_PROPTITLE ;
	psh.hwndParent = m_parent->Handle();
	psh.hInstance = GetInstance();
	psh.pszCaption = lpszTitle;
	psh.nPages = m_psh.nPages;
	psh.phpage = m_lppsp ;

	return PropertySheet(&psh);
}

BOOL RingPropSheet::AddPage(RingPropPage* page,LPARAM dwInitParam/*=0*/,
									 HINSTANCE hInst/*=GetInstance()*/)
{
	if(page == NULL)
		return FALSE;

	page->SetParentObj(this);
	HPROPSHEETPAGE hpsp = page->CreatePage(0,NULL,dwInitParam,hInst);
	if(hpsp == NULL)
		return FALSE;

	if(m_lppsp == NULL)
	{
		m_lppsp = (HPROPSHEETPAGE*)New(sizeof(HPROPSHEETPAGE) * PAGE_ADDSTEP);
		if(m_lppsp)
		{
			m_PageSize = PAGE_ADDSTEP;
		}
		else
			return NULL;
	}
	
	m_psh.nPages ++;
	if(m_psh.nPages >= (UINT)m_PageSize)
	{
		m_PageSize += PAGE_ADDSTEP;
		HPROPSHEETPAGE* temp = (HPROPSHEETPAGE*)New(sizeof(HPROPSHEETPAGE) * m_PageSize);
		if(temp)
		{
			memcpy(temp,m_lppsp,sizeof(HPROPSHEETPAGE) * m_psh.nPages);
			Del(m_lppsp);
			m_lppsp = temp;
		}
	}
	m_lppsp[m_psh.nPages-1] = hpsp;
	return TRUE;
}

LRESULT RingPropSheet::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	LRESULT res = CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
	if(param.uMsg == WM_NCDESTROY)
		OnNcDestroy();
	return res;
}

////////////////////////////////////////////////////////////////////////
//
//										����ҳ
//
////////////////////////////////////////////////////////////////////////
RingPropPage::RingPropPage()
{
	m_windowType = ITIS_PROPPAGE;
}

RingPropPage::~RingPropPage()
{
}
/*	
LRESULT RingPropPage::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	return RingDialog::RingdowProc(hWnd,param);
}
*/	
HPROPSHEETPAGE RingPropPage::CreatePage(UINT uIconId/*=0*/,LPCTSTR lpTemplate/*=NULL*/,
													 LPARAM dwInitParam/*=0*/,HINSTANCE hInst/*=GetInstance()*/)
{
   if(m_hWnd)
		return NULL;

	m_InitParam.WndObj = this;
	m_InitParam.dwInitParam = dwInitParam;

	if(lpTemplate == NULL)
	{
		SetId();	//�������������ʱ������BINDID��SetId����m_id������m_id=0
		if(m_id != 0)
			lpTemplate = MAKEINTRESOURCE(m_id);
	}

	PROPSHEETPAGE psp;

	memset(&psp,0,sizeof(PROPSHEETPAGE));
	psp.dwSize = sizeof(PROPSHEETPAGE);		
	psp.dwFlags = PSP_USEICONID;
	psp.hInstance = hInst;	
	psp.pszTemplate = lpTemplate;
	psp.pszIcon = MAKEINTRESOURCE(uIconId);
	psp.pfnDlgProc = (DLGPROC)InitPageProc;
	psp.lParam = (LPARAM)&m_InitParam;
	psp.pfnCallback = NULL;

	return CreatePropertySheetPage(&psp);
}
