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
//							RingSDK��ý����� ringdib.lib
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
//ԭ�ļ�����ringavi.cpp
//
//˵�����������Ų���RingAvi��ʵ��
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"

RingAvi::RingAvi()
{
   m_hMCIWnd=NULL;
   X=Y=0;
   nWindowStyle=0;
   nCenter=0;
}

RingAvi::~RingAvi()
{
   X=Y=0;
   nWindowStyle=0;
   nCenter=0;
	if(m_hMCIWnd!=NULL)
   {
   	MCIWndDestroy(m_hMCIWnd);
      m_hMCIWnd=NULL;
   }
}

void RingAvi::Open(LPCTSTR szPath)
{
	if(szPath!=NULL)
   	wsprintf(szAviFilename,szPath);
}

void RingAvi::Close()
{
   X=Y=0;
   nWindowStyle=0;
   nCenter=0;
	if(m_hMCIWnd!=NULL)
   {
   	MCIWndDestroy(m_hMCIWnd);
      m_hMCIWnd=NULL;
   }
}

void RingAvi::SetWindow(int nflag,int x,int y,int pos)
{
	X=x;Y=y;

   switch(nflag)
   {
   	case AVIW_NONE:
			nWindowStyle=MCIWNDF_NOPLAYBAR;
         break;
      case AVIW_BAR:
      	nWindowStyle=MCIWNDF_NOMENU;
         break;
      case AVIW_MENUBAR:
      	nWindowStyle=0;
         break;
      case AVIW_NEW:
         nWindowStyle=WS_DISABLED | WS_SYSMENU | WS_THICKFRAME |
				          WS_MINIMIZEBOX | WS_MAXIMIZEBOX | MCIWNDF_SHOWNAME;
      	break;
   }
   if(pos!=0)
   	nCenter=pos;
}

HWND RingAvi::Play(int width,int height)
{
   if(m_hMCIWnd==NULL)
		if((m_hMCIWnd=MCIWndCreate(GetMainWnd(),GetInstance(),nWindowStyle,NULL))==NULL)
   		return NULL;

   MCIWndOpen(m_hMCIWnd,szAviFilename,0);
   MCIWndPlay(m_hMCIWnd);
   CentreWindow(m_hMCIWnd,width,height);
   EnableWindow(m_hMCIWnd,TRUE);
   ShowWindow(m_hMCIWnd,SW_SHOWNORMAL);

   MCIWndUseTime(m_hMCIWnd);
   lAviLen=MCIWndGetLength(m_hMCIWnd);

   return m_hMCIWnd;
}

HWND RingAvi::PlayFullScreen()
{
   if(m_hMCIWnd==NULL)
		if((m_hMCIWnd=MCIWndCreate(GetMainWnd(),GetInstance(),nWindowStyle,NULL))==NULL)
   		return NULL;
   RECT rt;

   MCIWndOpen(m_hMCIWnd,szAviFilename,0);

   MCIWndGetDest(m_hMCIWnd,&rt);
   m_width=rt.right-rt.left;
   m_height=rt.bottom-rt.left;

   int w=GetSystemMetrics(SM_CXSCREEN),h=GetSystemMetrics(SM_CYSCREEN);
   float t;

   if(m_width>=m_height)
   {
		t=(float)m_height*(float)w/(float)m_width;
      h=(int)t;
   }
   else
   {
   	t=(float)m_width*(float)h/(float)w;
      w=(int)t;
   }

   MCIWndPlay(m_hMCIWnd);
   nCenter=AVIW_CENTER;
   CentreWindow(m_hMCIWnd,w,h);
   EnableWindow(m_hMCIWnd,TRUE);
   ShowWindow(m_hMCIWnd,SW_SHOWNORMAL);

   MCIWndUseTime(m_hMCIWnd);
   lAviLen=MCIWndGetLength(m_hMCIWnd);

   return m_hMCIWnd;
}

void RingAvi::CentreWindow(HWND hwnd,int width,int height)
{
	RECT rect;

   if(width==0 && height==0)
		GetWindowRect(hwnd,&rect);
   else
   {
   	rect.top=rect.left=0;
      rect.right=width;
      rect.bottom=height;
   }

   switch(nCenter)
   {
   	case 0:
      	SetWindowPos(hwnd,NULL,X,Y,rect.right-rect.left,rect.bottom-rect.top,SWP_SHOWWINDOW);
         break;
      case AVIW_DESKCENTER:
			X=(GetSystemMetrics(SM_CXSCREEN)-(rect.right-rect.left))/2;
			Y=(GetSystemMetrics(SM_CYSCREEN)-(rect.bottom-rect.top))/2;
         SetWindowPos(hwnd,NULL,X,Y,rect.right-rect.left,rect.bottom-rect.top,SWP_SHOWWINDOW);
         break;
		case AVIW_CENTER:
      {
      	RECT rt;

         GetWindowRect(GetMainWnd(),&rt);

         X=((rt.right-rt.left)-(rect.right-rect.left))/2;
			Y=((rt.bottom-rt.top)-(rect.bottom-rect.top))/2;

         SetWindowPos(hwnd,NULL,X,Y,rect.right-rect.left,rect.bottom-rect.top,SWP_SHOWWINDOW);
         break;
      }
   }
   w_x=X;
   w_y=Y;
   w_width=rect.right-rect.left;
   w_height=rect.bottom-rect.top;
}

void RingAvi::ResetWindow(int x,int y,int pos)
{
	if(x==0 && y==0 && pos==0)
		SetWindowPos(m_hMCIWnd,NULL,w_x,w_y,w_width,w_height,SWP_SHOWWINDOW);
   else if(x!=0 && y!=0)
   	SetWindowPos(m_hMCIWnd,NULL,w_x,w_y,x,y,SWP_SHOWWINDOW);
}

void RingAvi::NextFast(int skip)
{
   if(m_hMCIWnd==NULL)
   	return;

	lAviPos=MCIWndGetPosition(m_hMCIWnd);
   if((lAviPos+skip)>lAviLen)
   	lAviPos=lAviLen-skip;
   MCIWndSeek(m_hMCIWnd,skip);
   //MCIWndPlayFrom(m_hMCIWnd,lAviPos+skip);
}

void RingAvi::PrevFast(int skip)
{
   if(m_hMCIWnd==NULL)
   	return;

	lAviPos=MCIWndGetPosition(m_hMCIWnd);
   if((lAviPos-skip)<0)
   	lAviPos=skip;
   MCIWndSeek(m_hMCIWnd,-skip);
   //MCIWndPlayFrom(m_hMCIWnd,lAviPos-skip);
}
