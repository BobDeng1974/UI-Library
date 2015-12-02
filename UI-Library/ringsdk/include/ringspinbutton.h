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
//							RingSDK���������� ringdows.lib
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
//ԭ�ļ�����ringspinbutton.h
//
//˵������up-down�ؼ��ķ�װ
//
**********************************************************************/
#ifndef _INC_RINGSPINBUTTON
#define _INC_RINGSPINBUTTON

class RingSpinButton:public RingControls
{
public:
	RingSpinButton(){}
	virtual ~RingSpinButton(){}
		
	LPCTSTR WndClassName(){return UPDOWN_CLASS;}
	
	virtual BOOL Create(DWORD dwStyle,UINT uId,RingControls* objbuddy,int nUpper,int nLower,int nPos,
								int x,int y,int cx,int cy)
	{
		if(m_hWnd && IsWindow())
			return TRUE;
		
		m_hWnd = CreateUpDownControl(dwStyle,x,y,cx,cy,m_parent->Handle(),uId,GetInstance(),
							objbuddy->Handle(),nUpper,nLower,nPos);
		RingBaseWnd::Attach(m_hWnd);
		m_ID = uId;
		return (BOOL)m_hWnd;
	}
	//�麯������
	BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height)
	{
		return RingControls::Create(dwStyle,dwExStyle,szInitText,uId,x,y,width,height);
	}
	int GetInt(int index=INDEX_CURSEL)
	{
		DWORD res = ::SendMessage(m_hWnd,UDM_GETPOS,0,0);
		if(HIWORD(res) == 0)
			return LOWORD(res);
		else
			return 0;
	}
	BOOL SetValue(int val,int index=INDEX_CURSEL)
	{
		return ::SendMessage(m_hWnd,UDM_SETPOS,0,MAKELONG((short)val,0));
	}
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL)
	{
		if(lpszVal)
			return SetValue(atoi(lpszVal));
		else
			return FALSE;
	}
		
	//��Ϣ��װ
	int GetAccel(int nAccel=0,LPUDACCEL paAccels=NULL)
	{
		return ::SendMessage(m_hWnd,UDM_GETACCEL,nAccel,(LPARAM)paAccels);
	}
	BOOL SetAccel(int nAccel,LPUDACCEL paAccels)
	{
		return ::SendMessage(m_hWnd,UDM_SETACCEL,nAccel,(LPARAM)paAccels);
	}
	//10��16����
	int GetBase(){return ::SendMessage(m_hWnd,UDM_GETBASE,0,0);}
	int SetBase(int nBase){return ::SendMessage(m_hWnd,UDM_SETBASE,nBase,0);}
	HWND GetBuddy(){return (HWND)::SendMessage(m_hWnd,UDM_GETBUDDY,0,0);}
	RingControls* GetBuddyObj();
	HWND SetBuddy(HWND hwnd){return (HWND)::SendMessage(m_hWnd,UDM_SETBUDDY,(WPARAM)hwnd,0);}
	DWORD GetRange(){return (DWORD)::SendMessage(m_hWnd,UDM_GETRANGE,0,0);}
	void SetRange(int nMin,int nMax)
	{::SendMessage(m_hWnd,UDM_SETRANGE,0,MAKELONG((short)nMax,(short)nMin));}
	void GetRange32(int* pMin,int* pMax){::SendMessage(m_hWnd,UDM_GETRANGE32,(WPARAM)pMin,(LPARAM)pMax);}
	void SetRange32(int nMin,int nMax){::SendMessage(m_hWnd,UDM_SETRANGE32,nMin,nMax);}
	int GetPos32()
	{
		BOOL bErr = FALSE;
		int nPos = ::SendMessage(m_hWnd,UDM_GETPOS32,0,(LPARAM)&bErr);
		if(!bErr)
			return nPos;
		else
			return 0;
	}
	int SetPos32(int nPos){return ::SendMessage(m_hWnd,UDM_SETPOS32,0,nPos);}

protected:
	int GetItemText(int index,LPTSTR lpBuf,int size){return 0;}
};

#endif
