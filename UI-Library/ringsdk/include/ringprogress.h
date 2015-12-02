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
//ԭ�ļ�����ringchecklist.h
//
//���ļ�˵����RingProgress������
//
//��⹦�ܣ��������ؼ���װ��
//
**********************************************************************/
#ifndef _INC_RINGPROGRESS
#define _INC_RINGPROGRESS

class RingProgress:public RingControls
{
public:
	RingProgress(){InitCommonControls();}
	virtual ~RingProgress(){}

	LPCTSTR WndClassName(){return PROGRESS_CLASS;}

	//�麯������
	int GetInt(int index=INDEX_CURSEL){return GetPos();}
	BOOL SetValue(int val,int index=-1){return SetProgress(val);}
	BOOL SetBkColor(COLORREF crColor){SendMessage(m_hWnd,PBM_SETBKCOLOR,0,crColor);return TRUE;}
	BOOL Clear(){SetProgress(0);return TRUE;}

	//��Ϣ��װ
	int DeltaPos(int nIncrement){return SendMessage(m_hWnd,PBM_DELTAPOS,nIncrement,0);}
	int GetPos(){return SendMessage(m_hWnd,PBM_GETPOS,0,0);}
	int GetRange(BOOL bLowLimit,PPBRANGE ppb=NULL);
	COLORREF SetBarColor(COLORREF crColor){return SendMessage(m_hWnd,PBM_SETBARCOLOR,0,crColor);}
	int SetProgress(int nPos){return SendMessage(m_hWnd,PBM_SETPOS,nPos,0);}
	DWORD SetRange(int nMin,int nMax){return SendMessage(m_hWnd,PBM_SETRANGE,0,MAKELONG(nMin,nMax));}
	DWORD SetRange32(int nMin,int nMax){return SendMessage(m_hWnd,PBM_SETRANGE,nMin,nMax);}
	int SetStep(int nStep){return SendMessage(m_hWnd,PBM_SETSTEP,nStep,0);}
	int StepIt(){return SendMessage(m_hWnd,PBM_STEPIT,0,0);}

};

#endif