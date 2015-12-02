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
//ԭ�ļ�����ringipaddr.h
//
//���ļ�˵����RingIPAddr������
//
//��⹦�ܣ�IP��ַ�ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGIPADDR
#define _INC_RINGIPADDR
#include <commctrl.h>

class RingIPAddr:public RingControls
{
public:
	RingIPAddr();
	virtual ~RingIPAddr(){}
	
   BOOL Create(UINT uId,int x,int y,int width,int height);
	BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);

	LPCTSTR WndClassName(){return WC_IPADDRESS;}

	//��ȡ,��������
	int GetInt(int index=-1);
	//�������������أ����ڲ�����GetItemText����������������GetItemText�ı�ú�����Ϊ��
	BOOL SetValue(int val,int index=-1){SendMessage(m_hWnd,IPM_SETADDRESS,0,(DWORD)val);return TRUE;}
	BOOL SetValue(LPCTSTR lpszVal,int index=-1);
	BOOL Clear(){SendMessage(m_hWnd,IPM_CLEARADDRESS,0,0);return TRUE;}
	BOOL IsBlank(){return SendMessage(m_hWnd,IPM_ISBLANK,0,0);}
	BOOL SetRange(int nField,BYTE bLow,BYTE bHigh)
	{return SendMessage(m_hWnd,IPM_SETRANGE,nField,MAKEIPRANGE(bLow,bHigh));}

protected:
	int GetItemText(int index,LPTSTR lpBuf,int size);
};

#endif
