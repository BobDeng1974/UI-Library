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
//ԭ�ļ�����ringedit.h
//
//���ļ�˵����RingEdit������
//
//��⹦�ܣ�Edit�ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGEDIT
#define _INC_RINGEDIT

class RingEditDrawer:public RingOwnerDrawer
{
public:
	RingEditDrawer(){};
	virtual ~RingEditDrawer(){};
	
	virtual void OnNcPaint(HDC,LPRECT){}
	virtual void OnMouseHover(HDC,LPRECT){}
	virtual void OnMouseLeave(HDC,LPRECT){}
	virtual void OnSetFocus(HDC,LPRECT){}
	virtual void OnKillFocus(HDC,LPRECT){}
};

class RingEdit:public RingControls
{
public:
	RingEdit();
   ~RingEdit();
	
   void LimitText(int nChars = 0);
   void InsertLine(LPCTSTR lpText,BOOL bFirstLine=FALSE);
	
	//void SetOwnerDraw(){};
	
	BOOL SetReadOnly(BOOL bReadOnly);
	BOOL IsReadOnly();

	LPCTSTR WndClassName(){return "Edit";}

	//��Ϣ��װ
	void SetPasswordChar(char c){SendMessage(m_hWnd,EM_SETPASSWORDCHAR,(WPARAM)c,0);}
	void SetSel(int start=0,int end=-1){SendMessage(m_hWnd,EM_SETSEL,start,end);}
	DWORD GetSel(LPDWORD lpStart,LPDWORD lpEnd)
			{return SendMessage(m_hWnd,EM_GETSEL,(WPARAM)lpStart,(LPARAM)lpEnd);}
	void Copy(){SendMessage(m_hWnd,WM_COPY,0,0);}
	void Cut(){SendMessage(m_hWnd,WM_CUT,0,0);}
	BOOL DelCurSel(){SendMessage(m_hWnd,WM_CLEAR,0,0);return TRUE;}
	BOOL Clear(){return SetValue("");}
	void Paste(){SendMessage(m_hWnd,WM_PASTE,0,0);InvalidateRect(m_hWnd,NULL,TRUE);}

protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);
	//void OnParentNotify(HWND,UINT,UINT,LONG);
	LRESULT OnBkColor(UINT,HDC);
		
//private:
//	CAN_OWNER_DRAW();	
};
#endif
