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

#ifndef _INC_RINGPROPSHEET
#define _INC_RINGPROPSHEET

//#include "prsht.h"
class RingPropPage;

class RingPropSheet:public RingDialog
{
public:
	RingPropSheet();
	~RingPropSheet();

	enum Page_AddStep {PAGE_ADDSTEP = 8};

	BOOL AddPage(RingPropPage* page,LPARAM dwInitParam=0,HINSTANCE hInst=GetInstance());
	BOOL DelPage(RingPropPage* page);
	BOOL SetActivePage(int index);
	BOOL SetActivePage(RingPropPage* page);
	
	void SetStyle();
	void SetWizardButtons(DWORD dwFlags);
	BOOL SetWizardMode();

	HWND Create(LPCTSTR lpszTitle=NULL,LPARAM dwInitParam = 0);
	int DoModal(LPCTSTR lpszTitle=NULL,LPARAM dwInitParam = 0);

protected:
	virtual LRESULT RingdowProc(HWND hWnd,RINGPARAMS param);
	   
protected:
	PROPSHEETHEADER m_psh;
	HPROPSHEETPAGE* m_lppsp;
	int m_PageSize;				//�Ѵ�����PROPPAGE�����С���Ǽ����PROPPAGE������
};

class RingPropPage:public RingDialog
{
public:
	RingPropPage();
	~RingPropPage();

	//virtual LRESULT RingdowProc(HWND hWnd,RINGPARAMS);
	HPROPSHEETPAGE CreatePage(UINT uIconId=0,LPCTSTR lpTemplate=NULL,
									LPARAM dwInitParam=0,HINSTANCE hInst=GetInstance());

protected:
	static LRESULT CALLBACK WINAPI InitPageProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);

public:
	RINGWNDPARAM m_InitParam;
};

#endif
