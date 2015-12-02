/**********************************************************************
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
//ԭ�ļ�����edit.cpp
//
//˵����Edit�ؼ�����
//
**********************************************************************/
/*
#define MAKE_SELF_LIB
#include "ringdows.h"

RingCHECKBOX::RingCHECKBOX(HWND hParent,UINT uId)
{
	m_hParent = hParent;
   m_TimerCnt ++;
   timerId = m_TimerCnt;
   currStyle = EDITSTYLE_FLAT;
   bTimerSetted = FALSE;
   m_preProc = NULL;

   if(uId != 0)
   {
   	m_hWnd = GetDlgItem(hParent,uId);
      m_ID = uId;
      if(RegCtrl(m_hWnd))
      {
	      m_preProc = (WNDPROC)GetWindowLong(m_hWnd,GWL_WNDPROC);
   		SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)RingControls::RingCtrlProc);
      }
   }
}

RingCHECKBOX::~RingCHECKBOX()
{
   KillTimer(m_hWnd,timerId);
   if(m_preProc != NULL)
		SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)m_preProc);
}

BOOL RingCHECKBOX::Create(LPCTSTR szCaption,DWORD dwStyle,int x,int y,int width,int height)
{
	m_hWnd = CreateWindow("edit",szCaption,
									WS_CHILD | WS_VISIBLE | dwStyle,
									x,y,width,height,
									m_hParent,
									(HMENU)m_ID,
									m_hInst,NULL);

   if(RegCtrl(m_hWnd))
   {
      m_preProc = (WNDPROC)GetWindowLong(m_hWnd,GWL_WNDPROC);
  		SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)RingControls::RingCtrlProc);
      return TRUE;
   }
   return FALSE;
}

LRESULT WINAPI RingCHECKBOX::DefCtrlProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	switch(uMsg)
   {
      case WM_NCPAINT:
      {
         if(!bPainted)
         {
				GetWindowRect(hWnd,&rcItem);

            rcItem.right -= (rcItem.left);
  	      	rcItem.left = 0;
	     	   rcItem.bottom -= (rcItem.top);
   	     	rcItem.top = 0;

            hDC = GetWindowDC(hWnd);
            if(currStyle == EDITSTYLE_FLAT)
            {
              	hpen = CreatePen(PS_SOLID,1,m_crSys.crBTNFACE);
		         SelectObject(hDC,hpen);
   		      SelectObject(hDC,GetStockObject(NULL_BRUSH));
      		   Rectangle(hDC,rcItem.left,rcItem.top,rcItem.right,rcItem.bottom);
	         	DeleteObject(hpen);
            }
            else
         		DrawEdge(hDC,&rcItem,BDR_SUNKENOUTER,BF_RECT);

            ReleaseDC(hWnd,hDC);
            bPainted = TRUE;
      	}
      	return 0;
      }
      case WM_MOUSEMOVE:
         if(!bTimerSetted)
         {
         	SetTimer(hWnd,timerId,10,NULL);
            bTimerSetted = TRUE;
         }
      	return m_preProc(hWnd,uMsg,wParam,lParam);
      case WM_SETFOCUS:
			bLocked = TRUE;
			return m_preProc(hWnd,uMsg,wParam,lParam);
      case WM_KILLFOCUS:
      	bLocked = FALSE;
         SendMessage(hWnd,WM_NCPAINT,0,0);
         return m_preProc(hWnd,uMsg,wParam,lParam);
      case WM_TIMER:
      {
         GetWindowRect(hWnd,&rcItem);
         GetCursorPos(&pt);
         if(PtInRect(&rcItem,pt))
         {
         	if(currStyle == EDITSTYLE_FLAT)
            {
            	bPainted = FALSE;
               currStyle = EDITSTYLE_NORMAL;
               SendMessage(hWnd,WM_NCPAINT,0,0);
               return 0;
            }
         }
         else
         {
         	if(!bLocked && currStyle == EDITSTYLE_NORMAL)
	         {
            	bPainted = FALSE;
               currStyle = EDITSTYLE_FLAT;
               SendMessage(hWnd,WM_NCPAINT,0,0);
               return 0;
            }
            else if(bLocked && currStyle == EDITSTYLE_FLAT)
	         {
            	bPainted = FALSE;
               currStyle = EDITSTYLE_NORMAL;
               SendMessage(hWnd,WM_NCPAINT,0,0);
               return 0;
            }
         }
         return 0;
      }
      default:
         return m_preProc(hWnd,uMsg,wParam,lParam);
   }
}
*/

