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
//�汾��1.0
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����ringbasewnd.cpp
//
//���ļ�˵�����Ի�����ʵ�ִ��롣
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingDialog::RingDialog()
{
   m_windowType = ITIS_DIALOG;
	m_id = 0;	
}

RingDialog::~RingDialog()
{
}

int RingDialog::DoModal(LPCTSTR lpTemplate/*=NULL*/,LPARAM dwInitParam/*=0*/,
								HWND hParentWnd/* = NULL*/,HINSTANCE hInst/*=GetInstance()*/)
{
   if(IsWindow())
	{
		SetActiveWindow(m_hWnd);
		return 0;
	}

	RINGWNDPARAM lParam = {this,dwInitParam};
	if(lpTemplate == NULL)
	{
		SetId();	//�������������ʱ������BINDID��SetId����m_id������m_id=0
		if(m_id != 0)
			lpTemplate = MAKEINTRESOURCE(m_id);
	}
	if(hParentWnd == NULL)
		hParentWnd = m_parent->Handle();
	else
		SetParentObj(GetWindowPtr(hParentWnd));
	
	m_modal = TRUE;	
   return DialogBoxParam(hInst,lpTemplate,hParentWnd,(DLGPROC)InitDlgProc,(LPARAM)&lParam);
}

HWND RingDialog::Create(LPCTSTR lpTemplate/*=NULL*/,LPARAM dwInitParam/*=0*/,
								HWND hParentWnd/*=NULL*/,HINSTANCE hInst/*=GetInstance()*/)
{
   if(IsWindow())
		return m_hWnd;
	
	RINGWNDPARAM lParam = {this,dwInitParam};
	if(lpTemplate == NULL)
	{
		SetId();	//�������������ʱ������BINDID��SetId����m_id������m_id=0
		if(m_id != 0)
			lpTemplate = MAKEINTRESOURCE(m_id);
	}
	m_modal = FALSE;
	
	if(hParentWnd == NULL)
		hParentWnd = m_parent->Handle();
	else
		SetParentObj(GetWindowPtr(hParentWnd));

   CreateDialogParam(hInst,lpTemplate,hParentWnd,(DLGPROC)InitDlgProc,(LPARAM)&lParam);
	Show(SW_SHOW);
	return m_hWnd;
}

int RingDialog::Close(int nResult)
{
	if(IsWindow())
	{
		int res;
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		if(m_modal)
			res = EndDialog(hWnd,nResult);
		else
			res = DestroyWindow(hWnd);
		return res;
	}
	return 0;
}

WNDPROC RingDialog::SubClassWindow(HWND hWnd)
{
	if(!::IsWindow(hWnd))
		return NULL;
	
	m_hWnd = hWnd;
	m_DefProc = (WNDPROC)SetWindowLong(hWnd,DWL_DLGPROC,(LONG)InitThunk());
	return m_DefProc;
}

LRESULT RingDialog::OnCtlColorDlg(HWND,RINGPARAMS& param)
{
	SetBkMode(param.hdc,TRANSPARENT);
	return (LONG)m_brush;
}

LRESULT RingDialog::RingdowProc(HWND hWnd,RINGPARAMS param)
{
   if(param.uMsg == WM_INITDIALOG)
   {
		param.lParam = param.lParam?((LPRINGWNDPARAM)param.lParam)->dwInitParam:0;
		OnCreate(hWnd);
	}

	LRESULT res = RM_NOTPROCESS;
   if(NotifyChildProc(hWnd,param.uMsg,param.wParam,param.lParam))
		return TRUE;

	if(m_msgs)
		res = DispMessage((RINGVCLASS*)this,m_msgs,hWnd,param);
	else
		if(param.uMsg == WM_COMMAND && m_DefProc == DefaultDlgProc)
			switch(param.shortval.low)
			{
				case IDOK:
				case IDCANCEL:
					SendExit();
					return FALSE;
			}

	if(res == RM_NOTPROCESS)
	{
		if(m_windowType == ITIS_DIALOG)
		{
			if(m_brush && (param.uMsg == WM_CTLCOLORDLG || param.uMsg == WM_CTLCOLORSTATIC))
				res = OnCtlColorDlg(m_hWnd,param);
			else if(((m_modal && param.uMsg == WM_CLOSE) || 
						(!m_modal && param.uMsg == WM_DESTROY)) &&
						m_DefProc == DefaultDlgProc)
				res = Close(0);
			else
				res = DefaultProc(param);
		}
		else
			res = DefaultProc(param);
	}
	
	if(res > 1 || res < 0)	//��TRUE��FALSE�����÷���ֵ
		SetWindowLong(hWnd,DWL_MSGRESULT,res);
	return res;
}

HWND RingDialog::Create(LPCTSTR szTitle,LPCTSTR szClass,
								DWORD dwStyle,DWORD dwExStyle,
								int x,int y,int width,int height,
								HWND hParentWnd,HMENU hMenu/*=NULL*/,LPARAM lParam/*=0*/)
{
	return CreateIndirect(szTitle,dwStyle,x,y,width,height,hParentWnd,lParam);
}

HWND RingDialog::CreateIndirect(LPCTSTR szCaption,DWORD dwStyle,
										  int x,int y,int width,int height,
										  HWND hParentWnd/*=NULL*/,LPARAM lParam/*=0*/)
{
	if(IsWindow())
		return m_hWnd;

	if(hParentWnd == NULL)
		hParentWnd = m_parent->Handle();
	else
		SetParentObj(GetWindowPtr(hParentWnd));
	
	m_modal = FALSE;

	RINGWNDPARAM initParam = {this,lParam};
	LPDLGTEMPLATE lpdt = (LPDLGTEMPLATE)New(400);
	CreateTemplate(lpdt,szCaption,dwStyle,x,y,width,height);
	m_hWnd = CreateDialogIndirectParam(GetInstance(),lpdt,hParentWnd,
								(DLGPROC)InitDlgProc,(LPARAM)&initParam);
	Del(lpdt);
	ShowWindow(m_hWnd,SW_SHOW);
	return m_hWnd;
}

int RingDialog::DialogIndirect(LPCTSTR szCaption,DWORD dwStyle,
										 int x,int y,int width,int height,
										 HWND hParentWnd/*=NULL*/,LPARAM lParam/*=0*/)
{
	if(hParentWnd == NULL)
		hParentWnd = m_parent->Handle();
	else
		SetParentObj(GetWindowPtr(hParentWnd));
	
	RINGWNDPARAM initParam = {this,lParam};
	LPDLGTEMPLATE lpdt = (LPDLGTEMPLATE)New(400);
	CreateTemplate(lpdt,szCaption,dwStyle,x,y,width,height);
	int ret = DialogBoxIndirectParam(GetInstance(),lpdt,hParentWnd,
								(DLGPROC)InitDlgProc,(LPARAM)&initParam);
	Del(lpdt);
	return ret;
}

void RingDialog::CreateTemplate(LPDLGTEMPLATE lpdt,LPCTSTR szCaption,DWORD dwStyle,int x,int y,int width,int height)
{
	if(lpdt)
	{
		LPWORD lpw;
		LPWSTR lpwsz;
		
		lpdt->style = dwStyle;
		lpdt->cdit = 0;
		lpdt->x = x;  lpdt->y = y;
		lpdt->cx = width; lpdt->cy = height;
		
		lpw = (LPWORD) (lpdt + 1);
		*lpw++ = 0;   // no menu
		*lpw++ = 0;   // predefined dialog box class (by default)
		
		lpwsz = (LPWSTR) lpw;
		MultiByteToWideChar(CP_ACP,0,szCaption,-1,lpwsz,80);
	}	
}
/*
HBRUSH RingDialog::SetBkgColor(COLORREF crColor)
{
	RingBaseWnd::SetBkgColor(crColor);
	//if(m_brush)
	//	InitRingControls();
	return m_brush;	
}

HBRUSH RingDialog::SetBkgBitmap(HBITMAP hbm)
{	
	RingBaseWnd::SetBkgBitmap(hbm);
	//if(m_brush)
	//	InitRingControls();
	return m_brush;	
}
*/

/************************************
*
*��ʼ���ؼ��࣬�����пؼ���ʼ����
*
************************************/
void RingDialog::InitRingControls()
{
	RingStatic* objStatic = NULL;
	//RingButton*	objButton = NULL;
	char szName[80];
	HWND hWnd = GetChild();
	while(hWnd)
	{
		memset(szName,0,80);
		if(GetClassName(hWnd,szName,80))
		{
			if(strcmpi(szName,"Static") == 0)
			{
				objStatic = (RingStatic*)GetWindowPtr(hWnd);
				if(objStatic == NULL)
				{
					objStatic = NewRingObj(objStatic);
					if(objStatic)
						objStatic->Attach(hWnd);
				}
				if(objStatic)
					objStatic->SetTransparent();
			}
		}
		hWnd = GetNextChild(hWnd);
	}
}

LRESULT RingOFNHookDialog::RingdowProc(HWND hWnd,RINGPARAMS param)
{   
	if(param.uMsg == WM_NCDESTROY)
	{
		//��������໯�����ڣ����򸸴��ڷ��ʱ����󣬶�������������
		SetWindowLong(m_hWndParent,DWL_DLGPROC,(LONG)m_OFNProc);
	}
	return RingDialog::RingdowProc(hWnd,param);
}

