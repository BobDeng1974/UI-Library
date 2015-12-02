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
//ԭ�ļ�����ringtrackbar.h
//
//���ļ�˵����RingTrackBar������
//
//��⹦�ܣ�TrackBar�ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGTRACKBAR
#define _INC_RINGTRACKBAR
#include <commctrl.h>

class RingTrackBar:public RingControls
{
public:
	RingTrackBar(){}
	virtual ~RingTrackBar(){}
	
   LPCTSTR WndClassName(){return TRACKBAR_CLASS;}
	void SetTransparent(){m_brush = (HBRUSH)GetStockObject(NULL_BRUSH);}

	//���ص��麯��
	BOOL Clear(){::SendMessage(m_hWnd,TBM_CLEARSEL,TRUE,0);return TRUE;}
	int GetInt(int index=INDEX_CURSEL){return GetPosition();}
	BOOL SetValue(int val,int index=INDEX_CURSEL){SetPosition(val,(BOOL)index);return TRUE;}
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL)
		{SetPosition(atoi(lpszVal),(BOOL)index);return TRUE;}
	LRESULT OnBkColor(UINT uMsg,HDC hDC)
		{return (uMsg == WM_CTLCOLORSTATIC)?SetColors(hDC):0;}

	//����
	void ClearSel(BOOL bRedraw = TRUE){::SendMessage(m_hWnd,TBM_CLEARSEL,bRedraw,0);}
	void ClearTics(BOOL bRedraw = TRUE){::SendMessage(m_hWnd,TBM_CLEARTICS,bRedraw,0);}
	
	//��ȡ����/����
	HWND GetBuddy(BOOL bFirst){return (HWND)::SendMessage(m_hWnd,TBM_GETBUDDY,bFirst,0);}
	void GetChannelRect(LPRECT lprc)
		{if(lprc)::SendMessage(m_hWnd,TBM_GETCHANNELRECT,0,(LPARAM)lprc);}
	int GetPosition(){return ::SendMessage(m_hWnd,TBM_GETPOS,0,0);}
	int GetRangeMin(){return ::SendMessage(m_hWnd,TBM_GETRANGEMIN,0,0);}
	int GetRangeMax(){return ::SendMessage(m_hWnd,TBM_GETRANGEMAX,0,0);}
	int GetLineSize(){return ::SendMessage(m_hWnd,TBM_GETLINESIZE,0,0);}
	int GetPageSize(){return ::SendMessage(m_hWnd,TBM_GETPAGESIZE,0,0);}
	int GetSelStart(){return ::SendMessage(m_hWnd,TBM_GETSELSTART,0,0);}
	int GetSelEnd(){return ::SendMessage(m_hWnd,TBM_GETSELEND,0,0);}
	LPDWORD GetTics(){return (LPDWORD)::SendMessage(m_hWnd,TBM_GETPTICS,0,0);}
	int GetTicCount(){return ::SendMessage(m_hWnd,TBM_GETNUMTICS,0,0);}
	int GetThumbLength(){::SendMessage(m_hWnd,TBM_GETTHUMBLENGTH,0,0);}
	void GetThumbRect(LPRECT lprc)
		{if(lprc)::SendMessage(m_hWnd,TBM_GETTHUMBRECT,0,(LPARAM)lprc);}
	int GetTic(int nIndex){::SendMessage(m_hWnd,TBM_GETTIC,nIndex,0);}
	int GetTipPos(int nIndex){::SendMessage(m_hWnd,TBM_GETTICPOS,nIndex,0);}
	HWND GetToolTip(){::SendMessage(m_hWnd,TBM_GETTOOLTIPS,0,0);}
	
	//��������/����
	void SetPosition(int nPos,BOOL bRedraw = TRUE){::SendMessage(m_hWnd,TBM_SETPOS,bRedraw,nPos);}
	void SetRange(int nMin,int nMax,BOOL bRedraw = TRUE)
		{::SendMessage(m_hWnd,TBM_SETRANGE,bRedraw,MAKELPARAM(nMin,nMax));}
	int SetLineSize(int nSize){return ::SendMessage(m_hWnd,TBM_SETLINESIZE,0,nSize);}
	int SetPageSize(int nSize){return ::SendMessage(m_hWnd,TBM_SETPAGESIZE,0,nSize);}
	void SetSel(int nMin,int nMax,BOOL bRedraw = TRUE)
		{::SendMessage(m_hWnd,TBM_SETSEL,bRedraw,MAKELPARAM(nMin,nMax));}
	BOOL SetTic(int nPos){return ::SendMessage(m_hWnd,TBM_SETTIC,0,nPos);}
	void SetThumbLength(int nLength){::SendMessage(m_hWnd,TBM_SETTHUMBLENGTH,nLength,0);}
	void SetTicFreq(int nFreq){::SendMessage(m_hWnd,TBM_SETTICFREQ,nFreq,0);}
	int SetTipSide(int nFlag){::SendMessage(m_hWnd,TBM_SETTIPSIDE,nFlag,0);}
	void SetToolTip(HWND hwndTip){::SendMessage(m_hWnd,TBM_SETTOOLTIPS,(WPARAM)hwndTip,0);}
};

#endif
