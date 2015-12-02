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
//�汾��0.2
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����ringmdichild.cpp
//
//˵������MDI�Ӵ��ڵķ�װʵ�֡�
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

//ringFile rfc = "child.txt";
//char cc[80];
HWND RingMDIChildWnd::Create(LPCTSTR szTitle,LPCTSTR szClass,
									  DWORD dwStyle,DWORD dwExStyle,
									  int x,int y,int width,int height,
									  HWND hParentWnd/*=NULL*/,HMENU hMenu/*=NULL*/,
									  LPARAM lParam/*=0*/)
{
   if(!Register(szClass,
		(HBRUSH)(COLOR_WINDOW + 1),
		m_windowInfo.lpszMenu,
		m_windowInfo.hIconBig,
		m_windowInfo.hCursor,
		m_windowInfo.hIconSmall,
		ITIS_MDICHILD))
		return NULL;
	
   if(m_parent->GetWindowType() == ITIS_MDIFRAME)
		((RingMDIFrameWnd*)m_parent)->CreateChild(szTitle,this,FALSE,dwStyle | WS_CHILD,
																lParam,(LPSTR)szClass);

   return m_hWnd;
}

LRESULT RingMDIChildWnd::RingdowProc(HWND hWnd,RINGPARAMS param)
{
   LRESULT res;
	//����ı�WM_CREATE�Ĳ�����InitChildProc���Ѹı�
	if(param.uMsg == WM_MOUSEACTIVATE)//WM_ACTIVATE && param.wParam == WA_CLICKACTIVE)
	{
		res = DispMessage((RINGVCLASS*)this,m_msgs,hWnd,param);
		RingCoolMenu* rm = ((RingMDIFrameWnd*)m_parent)->GetCoolMenu();
		if(m_parent->GetMenu() == NULL && rm)
		{
			RingMDIClient* client = ((RingMDIFrameWnd*)m_parent)->GetClientObj();
			client->RefreshMenu(rm,m_hWnd);
			client->UpdateMenuBar(m_hWnd);
		}
		return res;
	}
	else if(param.uMsg == WM_SIZE)
	{
		if(param.wParam == SIZE_MAXIMIZED 
			|| param.wParam == SIZE_RESTORED 
			|| param.wParam == SIZE_MINIMIZED)
		{
			//LRESULT res = DispMessage((RINGVCLASS*)this,m_msgs,hWnd,param);
			RingCoolMenu* rm = ((RingMDIFrameWnd*)m_parent)->GetCoolMenu();
			if(m_parent->GetMenu() == NULL && rm)
			{
				RingMDIClient* client = ((RingMDIFrameWnd*)m_parent)->GetClientObj();
				client->UpdateMenuBar(m_hWnd);
			}
			//return res;
		}
	}

	NotifyChildProc(hWnd,param.uMsg,param.wParam,param.lParam);
   return DispMessage((RINGVCLASS*)this,m_msgs,hWnd,param);	
}


