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
//ԭ�ļ�����ringstatus.h
//
//���ļ�˵����״̬����װ
//
**********************************************************************/
#ifndef _INC_RINGSTATUS
#define _INC_RINGSTATUS
#include <commctrl.h>

//״̬��������
//�������
#define SBPS_STATICW		0x0				//�̶����
#define SBPS_FITTEXT		0x1				//����Ӧ���ֿ��
#define SBPS_STRETCH		0x2				//�䳤����Ӧ���ڿ��
//#define SBPS_MENUHELP	0x3				//�䳤����ʾ�˵�����
//״̬
#define SBPF_UPDATE		0x00010000		//��Ҫ��������
//��ʾ����
#define SBTS_BORDER		0x0				//�б߿�
#define SBTS_NOBORDER	0x1000			//�ޱ߿�(SBT_NOBORDERS<<4)
#define SBTS_POPOUT		0x2000			//ͻ����ʾ(SBT_POPOUT<<4)
#define SBTS_RTL			0x4000			//�����Ķ�˳��(SBT_RTLREADING<<4)

//#define SPS_TEXT			0x10		  //����
//#define SPS_PROGRESS		0x40		  //��������RINGSDK��չ��
//#define SPS_TIMER			0x80		  //ʱ������RINGSDK��չ��
//�������
#define SBPW_FITTEXT		0x0FFE
#define SBPW_STRETCH		0x0FFF
//���÷ָ���ʱ���ɷָ�����Ϣ��
//strid:Ҫ��ʾ���ֵ���ԴID��
//type:������SBTS_XXXֵ֮һ
//width:�ָ�����ȣ����ɳ���4095����ΪSBPW_XXXֵ֮һ
#define MAKESBPINT(strid,type,width) ((strid << 16) | type | (width&SBPW_STRETCH))
//ȡMAKESBPINT��type��������������ֵ֮һ��SBT_NOBORDERS��SBT_POPOUT��SBT_RTLREADING
#define SBPWSTYLE(val) ((val & 0xF000) >> 4)

typedef struct _tagRingStatusPart
{
	int m_nIndex;
	UINT m_uStyle;
	int m_nWidth;
	ringStr* m_str;
	RingBaseWnd* m_child;
/*	union
	{
		int m_nStep;
		RingThread* m_Timer;
	};
*/
}RINGSTATUSPANE,*LPRINGSTATUSPANE;

class RingStatusBar:public RingControls
{
public:
	RingStatusBar();
	virtual ~RingStatusBar();
	
	BOOL Create(DWORD dwStyle = SBARS_SIZEGRIP);

	LPCTSTR WndClassName(){return STATUSCLASSNAME;}

	BOOL SetParts(int parts,LPINT lpArr = NULL);
	UINT GetPartStyle(int nPart);
	//BOOL SetPartStyle(int index,UINT uStyle,LPCTSTR lpszText=NULL,int nWidth=0);
	//BOOL SetTimerFormat(int index,LPCTSTR lpszFormat);
	//BOOL SetProgress(int index,int nStep);	
	virtual BOOL SetValue(int nPart,LPCTSTR lpszVal,int uStyle=-1,int nWidth=0);
	int OnMenuHelp(RINGPARAMS &param,HMENU hMenu);
	BOOL InsertItem(RingBaseWnd* wnd,int iPart);

	//���ص��麯��
	BOOL SetValue(int val,int index=INDEX_CURSEL);
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);
	BOOL SetBkColor(COLORREF crColor){return ::SendMessage(m_hWnd,SB_SETBKCOLOR,0,(LPARAM)crColor);}

	int GetPartWidth(int nPart);

	//��Ϣ��װ
	int GetParts(int nPart,LPINT lpCoord=NULL)
	{return lpCoord==NULL?m_nPartCount:(::SendMessage(m_hWnd,SB_GETPARTS,nPart,(LPARAM)lpCoord));}
	HICON GetIcon(int nPart){return (HICON)::SendMessage(m_hWnd,SB_GETICON,nPart,0);}
	BOOL SetIcon(int nPart,HICON hIcon)
	{return ::SendMessage(m_hWnd,SB_SETICON,nPart,(LPARAM)hIcon);}
	BOOL GetBorders(LPINT lpInt){return ::SendMessage(m_hWnd,SB_GETBORDERS,0,(LPARAM)lpInt);}
	BOOL GetRect(int nPart,LPRECT lpRect)
	{return ::SendMessage(m_hWnd,SB_GETRECT,nPart,(LPARAM)lpRect);}
	int GetTextLength(int nPart)
	{return LOWORD(::SendMessage(m_hWnd,SB_GETTEXTLENGTH,nPart,0));}
	UINT GetBorderStyle(int nPart)
	{return HIWORD(::SendMessage(m_hWnd,SB_GETTEXTLENGTH,nPart,0));}
	BOOL IsSimple(){return ::SendMessage(m_hWnd,SB_ISSIMPLE,0,0);}
	void SetSimple(BOOL bSimple){::SendMessage(m_hWnd,SB_SIMPLE,bSimple,1);}
	void ToggleSimple(){SetSimple(!IsSimple());}

	//DWORD m_TimerProc(LPRINGSTATUSPANE);
	
protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);
	BOOL OnParentNotify(HWND,UINT,UINT,LONG);
	void UpdateAllParts();
	LPRINGSTATUSPANE GetPanePtr(int index);
	int GetItemText(int index,LPTSTR lpBuf,int size);

private:	
	BOOL CreatePaneInfo(int nParts);
	void FreeAllPart();
	//BOOL SetTimerFormat(LPRINGSTATUSPANE lpPart,LPCTSTR lpszFormat);	
	BOOL SetPaneText(LPRINGSTATUSPANE lpPart,LPCTSTR lpszVal,UINT uStyle);
	//void DestroyTimerPart(LPRINGSTATUSPANE lpPart);

protected:
	RingDockBar* m_dockbar;

private:
	LPRINGSTATUSPANE m_pane;
	int m_nPartCount;
};

#endif
