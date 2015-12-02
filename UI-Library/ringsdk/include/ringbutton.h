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
//ԭ�ļ�����ringbutton.h
//
//���ļ�˵����RingButton������
//
//��⹦�ܣ�Button�ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGBUTTON
#define _INC_RINGBUTTON

#define ATBTN_LEFT	0
#define ATBTN_TOP		1
#define ATBTN_RIGHT	2
#define ATBTN_BOTTOM	4

//��ť��չ���ͣ���ΪBUTTONû����չ���ͣ������������������ã�������
//ע�⣺�κ�ֵ�����λ����Ϊ1��������ΪRTV_FITPARENTSIZE�Ķ���ֵΪ1��
//���λΪ1�ᱻ����Ϊ����Ӧ�����ڳߴ�
#define RINGBTN_EX_FLAT				2
#define RINGBTN_EX_NOFOCUSRECT	4

class RingButton:public RingControls
{
public:
	RingButton();
	virtual ~RingButton();
	
   LPCTSTR WndClassName(){return "Button";}
	
	void Click();
	virtual BOOL Check(UINT uCheck=BST_CHECKED,UINT id=0);
	virtual UINT IsChecked(UINT uid=0);
	UINT GetState(){return ::SendMessage(m_hWnd,BM_GETSTATE,0,0);}
	//���ð��£�����״̬���Ի水ť����Check���ã����ʹ��m_bPushed��¼
	void SetPush(BOOL bPushed){m_bPushed = bPushed;InvalidateRect(m_hWnd,NULL,TRUE);}
	BOOL IsPushed(){return m_bPushed;}
	void SetTextColor(COLORREF crColor);
	BOOL SetBkColor(COLORREF crColor);

	HICON SetIcon(HICON hicon);
	HICON SetIcon(UINT uidIcon,int width=0,int height=0,HINSTANCE hInst=GetInstance());
	HBITMAP SetBitmap(HBITMAP hbmp);
	HBITMAP SetBitmap(UINT uidBmp,HINSTANCE hInst=GetInstance());

	//�ڰ�ť�ϼ�ͼ��
	HICON AddIcon(HICON hicon,int nWhere = ATBTN_LEFT);
	HICON AddIcon(UINT uidIcon,int nWhere = ATBTN_LEFT,int width=0,int height=0,HINSTANCE hInst=GetInstance());
	HBITMAP AddBitmap(HBITMAP hbm,int nWhere = ATBTN_LEFT);
	HBITMAP AddBitmap(UINT uidBmp,int nWhere = ATBTN_LEFT,HINSTANCE hInst=GetInstance());

	HBITMAP GetBitmap(){return (HBITMAP)SendMessage(m_hWnd,BM_GETIMAGE,IMAGE_BITMAP,0);}
	HICON GetIcon(){return (HICON)SendMessage(m_hWnd,BM_GETIMAGE,IMAGE_ICON,0);}
	//�����Ի棬����ΪTRUE��ʾ�ɸ����ڴ����Ի�
	void SetOwnerDraw(BOOL bParentDraw=FALSE)
		{RingControls::SetOwnerDraw(bParentDraw,BS_OWNERDRAW);}

	void SetExStyle(DWORD dwExStyle);

protected:
	BOOL OnDrawItem(LPDRAWITEMSTRUCT lps);
	LRESULT OnBkColor(UINT,HDC);
	LRESULT OnDestroy(RINGPARAMS& param);
	
	//�������
	virtual void DrawFrame(LPDRAWITEMSTRUCT lps,DWORD style);
	//������ְ�Χ����
	LPCTSTR GettextRect(LPDRAWITEMSTRUCT lps,DWORD style,LPRECT lprc);
	//�������ּ�ͼ���BMP
	void DrawImage_Text(LPDRAWITEMSTRUCT lps,LPCTSTR s,DWORD style,LPRECT lprc);

protected:
	BOOL m_bNeedFreeImage;
	BOOL m_bPushed;	//�Ի�ʱ��ť״̬
	int m_nImgWhere;
};

class RingCheckBox:public RingButton
{
public:
	RingCheckBox(){}
	virtual ~RingCheckBox(){};
	
   BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);
};

class RingRadioButton:public RingButton
{
public:
	RingRadioButton():m_first(0),m_last(0){}
	virtual ~RingRadioButton(){};
	
   BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);
	
	void SetGroup(UINT uFirst,UINT uLast){m_first=uFirst;m_last=uLast;}
	BOOL Check(UINT id=0,UINT first=0,UINT last=0);
	void UnCheck(UINT id=0);
	UINT IsChecked(UINT uid=0);
	
protected:
	UINT m_first,m_last;
};

#endif
