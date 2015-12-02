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
//ԭ�ļ�����ringscrollbar.cpp
//
//���ļ�˵����RingScrollBar��ʵ�ִ���
//
//��⹦�ܣ��������ؼ���װ
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingScrollBar::RingScrollBar()
{
	m_windowType = ITIS_SCROLLBAR;	
}

BOOL RingScrollBar::GetScrollRange(LPSIZE lpSize)
{
	if(lpSize)
		return ::GetScrollRange(m_hWnd,SB_CTL,(LPINT)&lpSize->cx,(LPINT)&lpSize->cy);
	else
		return FALSE;
}

BOOL RingScrollBar::SetScrollInfo(int nMask,int nPosMin,int nPosMax,int nPage,int nPos,
											 int nTrackPos,BOOL bRedraw)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = nMask;
	si.nMin = nPosMin;
	si.nMax = nPosMax;
	si.nPage = nPage;
	si.nPos = nPos;
	si.nTrackPos = nTrackPos;
	
	return ::SetScrollInfo(m_hWnd,SB_CTL,&si,bRedraw);
}

BOOL RingScrollBar::GetScrollInfo(LPSCROLLINFO lpsi)
{
	if(lpsi)
		return ::GetScrollInfo(m_hWnd,SB_CTL,lpsi);
	else
		return FALSE;
}

BOOL RingScrollBar::SetScrollInfo(LPSCROLLINFO lpsi,BOOL bRedraw)
{
	if(lpsi)
		return ::SetScrollInfo(m_hWnd,SB_CTL,lpsi,bRedraw);
	else
		return FALSE;
}

int RingScrollBar::GetScrollLimit()
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE;
	if(::GetScrollInfo(m_hWnd,SB_CTL,&si))
		return si.nMax;
	else
		return 0;
}
