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
//ԭ�ļ�����ringscrollbar.h
//
//���ļ�˵����RingScrollBar������
//
//��⹦�ܣ��������ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGSCROLL
#define _INC_RINGSCROLL

class RingScrollBar:public RingControls
{
public:
	RingScrollBar();
   virtual ~RingScrollBar(){}
	
   LPCTSTR WndClassName(){return "SCROLLBAR";}

	//��Ϣ��װ
	int GetScrollPos(){return ::GetScrollPos(m_hWnd,SB_CTL);}
	int SetScrollPos(int nPos,BOOL bRedraw=TRUE){return ::SetScrollPos(m_hWnd,SB_CTL,nPos,bRedraw);}
	BOOL GetScrollRange(LPSIZE lpSize);
	BOOL SetScrollRange(int nPosMin,int nPosMax,BOOL bRedraw=TRUE)
	{
		return ::SetScrollRange(m_hWnd,SB_CTL,nPosMin,nPosMax,bRedraw);
	}	
	BOOL EnableScrollBar(UINT wArrows){return ::EnableScrollBar(m_hWnd,SB_CTL,wArrows);}
	BOOL GetScrollInfo(LPSCROLLINFO lpsi);
	BOOL SetScrollInfo(LPSCROLLINFO lpsi,BOOL bRedraw=TRUE);
	BOOL SetScrollInfo(int nMask,int nPosMin,int nPosMax,int nPage,int nPos,int nTrackPos,
							BOOL bRedraw=TRUE);	
	int GetScrollLimit();
};

#endif
