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
//ԭ�ļ�����ringtoolbar.cpp
//
//���ļ�˵����������ʵ�ִ��룬������RingToolBar��RingCoolBar
//				  RingToolBar:��ͨ�������������ƶ�����϶�
//				  RingCoolBar:����IE�Ĺ������������϶����������ɶ��
//								  ��������ؼ���֧�����䷶Χ�ڵ���λ�á�
//
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdows.h"

////////////////////////////////////////////////////
//
//����IE�Ĺ�����
//
////////////////////////////////////////////////////
RingCoolBar::RingCoolBar()
{
	init();
}

void RingCoolBar::init()
{
	m_windowType = ITIS_COOLBAR;
}

RingCoolBar::~RingCoolBar()
{
}

BOOL RingCoolBar::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height)
{
   if(m_hWnd)
   	return FALSE;

//	m_State = TBS_FLAG_DOCKTOP;

	DWORD style = 0;
/*
	if(m_State > TBS_FLAG_DOCKBOTTOM)
	{
		m_winpos.cx = 32;
		m_winpos.cy = 300;
		style = CCS_VERT;
	}
	else
	{
		m_winpos.cx = 400;
		m_winpos.cy = 32;
	}
*/
	INITCOMMONCONTROLSEX icex;		
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);		
	// Load the coolbar. 
	icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);

	RingCtrlWnd::Create(NULL,
		REBARCLASSNAME,		
		WS_VISIBLE | WS_BORDER | WS_CHILD | WS_CLIPCHILDREN |
      WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NOPARENTALIGN |
      RBS_VARHEIGHT | RBS_AUTOSIZE | RBS_BANDBORDERS | style,
		m_parent->Handle(),0,(HMENU)gdwDockBarId++,
      0,0,width,height);	

	REBARINFO rbi;
	
	rbi.cbSize = sizeof(REBARINFO);  // Required when using this struct.
   rbi.fMask  = 0;
   rbi.himl   = (HIMAGELIST)NULL;
   if(!SendMessage(m_hWnd,RB_SETBARINFO,0,(LPARAM)&rbi))
      return FALSE;
/*	
	m_parentBar = ((RingWnd*)m_parent)->CreateDockSite(m_State);
	if(m_parentBar)
	{
		SetParent(m_parentBar->Handle());
		SetPos(0,0);
		((RingDockSite*)m_parentBar)->AddBar(this);
	}
*/
	return (BOOL)m_hWnd;
}

BOOL RingCoolBar::AddBand(RingCtrlWnd* child)
{
	if(child == NULL)
		return FALSE;

	REBARBANDINFO rbBand;
	   
   // Initialize REBARBANDINFO for all coolbar bands.
   rbBand.cbSize = sizeof(REBARBANDINFO);
   rbBand.fMask = RBBIM_CHILD |                     // hwndChild is valid
      RBBIM_CHILDSIZE |                 // cxMinChild and cyMinChild are valid
      RBBIM_STYLE |                     // fStyle is valid
      RBBIM_ID;// |                              // wID is valid      
   rbBand.fStyle =  RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;
   rbBand.hwndChild = child->Handle();
   rbBand.wID = child->m_ID;
   rbBand.cxMinChild = 100;//MIN_TBCX;
   rbBand.cyMinChild = 26;//MIN_CY;
	
   // Insert band into coolbar.
	//SetPos(0,0,400,32);
   return SendMessage(m_hWnd,RB_INSERTBAND,(UINT) -1,
		(LPARAM)(LPREBARBANDINFO)&rbBand);
	/*
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	OffsetRect(&rc,-rc.left,-rc.top);
	if(m_lineInfo && m_lineInfo->m_maxSize != nSize)
	{
		nSize -= m_lineInfo->m_maxSize;
		m_lineInfo->m_maxSize += nSize;
		((RingDockSite*)m_parentBar)->UpdateLine(m_lineInfo,nSize);
	}
	*/
}
/*
void RingCoolBar::ChangeParent(RingDockBar* parent/*=NULL)
{
	if(parent == NULL)
	{
		if(m_parentBar)
		{		
			m_parentBar->Show();
			m_CtrlBar = m_parentBar;			
		}
	}
	else
	{
//		if(!IsWindowVisible(m_hWnd))
//			parent->Show();
		m_CtrlBar = parent;
		if(m_parentBar && m_parentBar != parent)
			m_parentBar->Show(SW_HIDE);
	}
	//DefWindowProc(m_hWnd,TB_SETPARENT,(WPARAM)m_CtrlBar->Handle(), 0);
	SetParent(m_CtrlBar->Handle());
	SetParentObj(m_CtrlBar);
}
*/
LRESULT RingCoolBar::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	switch(param.uMsg)
	{
		case WM_PAINT:
		case WM_PRINTCLIENT:
		case WM_ERASEBKGND:
			break;//return TRUE;

/*
		case RB_INSERTBAND:
		{
			LRESULT res = DefaultProc(param);
			RECT rc;
			GetWindowRect(m_hWnd,&rc);
			OffsetRect(&rc,-rc.left,-rc.top);
			UpdateSize(rc.right,rc.bottom);
			return res;
		}
		case WM_ERASEBKGND:
			return OnEraseBkg(param);
*/
	}
	return DefaultProc(param);//RingReBar::RingdowProc(hWnd,param);
}

//ע�⣺HWND �Ǹ����ڵ�HWND
//wParamΪLPRINGBARLINEINFO
BOOL RingCoolBar::OnParentNotify(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	switch(uMsg)
	{
		case WM_SIZE:	//�����ڸı�ߴ�
		{
			SetPos(0,0,LOWORD(lParam),24);//HIWORD(lParam));			
		}
	}
	return 0;
}
