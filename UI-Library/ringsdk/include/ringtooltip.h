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
//ԭ�ļ�����ringtooltip.h
//
//���ļ�˵������ʾ���ඨ��
//
**********************************************************************/
#ifndef _INC_RINGTOOLTIP
#define _INC_RINGTOOLTIP
#include <commctrl.h>

typedef struct tagRingTipText
{
	int m_id;
	LPCTSTR lpszTip;
	int m_nSize;
	tagRingTipText* m_next;
}RINGTIPTEXT,*LPRINGTIPTEXT;

class RingToolTip:public RingControls
{
public:
	RingToolTip();
	virtual ~RingToolTip();
	
	virtual BOOL Create(DWORD dwStyle=TTS_NOPREFIX|TTS_ALWAYSTIP);

	LPCTSTR WndClassName(){return TOOLTIPS_CLASS;}
	
	void SetTipStyle(DWORD dwStyle){m_dwTipStyle = dwStyle;}
	DWORD GetTipStyle(){return m_dwTipStyle;}

	BOOL AddTool(HWND hWnd,LPCTSTR lpszTip,LPRECT lprc=NULL);
	BOOL AddTool(UINT uId,LPCTSTR lpszTip,LPRECT lprc=NULL);
	LPCTSTR AddTipText(UINT uId,LPCTSTR lpszText);

	//�麯������
	BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);
	//�����Attach����Attach��������ָ��Ҫ��ʾ��ʾ��Ϣ��Ŀ�괰�ڣ��ɵ��ö��ָ�����Ŀ�괰��
	BOOL Attach(HWND hWnd);
	BOOL Show(int nCmdShow=SW_SHOW){return ::SendMessage(m_hWnd,TTM_ACTIVATE,(nCmdShow==SW_SHOW),0);}
	BOOL SetPos(int x,int y,int width=-999,int height=-999,HWND hPos = NULL,UINT uFlags = SWP_NOZORDER);
	BOOL SetValue(int val,int index=INDEX_CURSEL);
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);
	
protected:
	//LRESULT RingdowProc(HWND,RINGPARAMS);
	//LRESULT OnNotify(RINGPARAMS param);
	//LRESULT OnEraseBkg(RINGPARAMS param);
		
private:
	DWORD m_dwTipStyle;
	LPRINGTIPTEXT m_tiptext;
};

#endif
