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
//							RingSDK��� ringmain.lib
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
//ԭ�ļ�����ringdowapp.cpp
//
//˵������WinMain�ķ�װʵ�֣��������⽫���ڱ����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringmain.h"

RingdowApp RingApp;

IDLEPROC RingdowApp::OnIdleProc = (IDLEPROC)RingdowApp::OnIdle;
int RingdowApp::m_CmdShow = SW_SHOW;
LPSTR RingdowApp::m_szCommand = NULL;
HACCEL RingdowApp::m_hAccel = NULL;
RingBaseWnd* RingdowApp::m_window = NULL;
int RingdowApp::m_Result = 0;

RingdowApp::~RingdowApp()
{
	try
	{
		delete m_window;
		m_window = NULL;
	}
	catch(...)
	{
	}
}

BOOL RingdowApp::SetAccelTable(LPCTSTR lpTableName)
{
	m_hAccel = LoadAccelerators(GetInstance(),lpTableName);
   return (BOOL)m_hAccel;
}

int RingdowApp::Run(BOOL bShow)
{
   if(m_window)
   {
      if(m_window->m_windowType == ITIS_DIALOG)
			return m_Result;
		else
		{
			MSG msg;
			HWND hwndMDIClient = NULL;
			int nRet;
			
			if(bShow)
				ShowWindow(m_window->m_hWnd,m_CmdShow);
			else
				ShowWindow(m_window->m_hWnd,SW_HIDE);
			
			UpdateWindow(m_window->m_hWnd);
			
			if(m_window->m_windowType == ITIS_MDIFRAME)
			{
				hwndMDIClient = ((RingMDIFrameWnd*)m_window)->GetClient();
				((RingMDIFrameWnd*)m_window)->SetMonExtClientWnd(&hwndMDIClient);
			}

			while(TRUE)
			{
      		if(!PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
      			OnIdleProc();
            
            nRet = GetMessage(&msg,NULL,0,0);
				if(nRet && nRet != -1)
				{				
					if((hwndMDIClient == NULL || !TranslateMDISysAccel(hwndMDIClient, &msg))
						&&	!TranslateAccelerator(msg.hwnd,m_hAccel,&msg))
	        		{
   	        		TranslateMessage(&msg);  // Translates virtual key codes
      				DispatchMessage(&msg);   // Dispatches message to window
  		   		}
				}
   			else
					return msg.wParam;
			}
		}
   }
   else
   	return 0;
}

LRESULT RingdowApp::OnIdle(void)
{
   return 0;
}

void RingdowApp::SetIdleProc(IDLEPROC IdleProc)
{
	OnIdleProc = IdleProc;
}

