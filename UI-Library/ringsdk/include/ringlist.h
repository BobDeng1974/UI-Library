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
//ԭ�ļ�����ringlist.h
//
//˵�������б�ؼ��ķ�װ
//
**********************************************************************/
#ifndef _INC_RINGLIST
#define _INC_RINGLIST

class RingListBox:public RingControls
{
public:
	RingListBox();
	~RingListBox();
		
	//virtual LRESULT OnDrawItem(HWND,UINT,LONG);

   LPCTSTR WndClassName(){return "ListBox";}

	//�ַ�������
	virtual int AddString(LPCTSTR lpString,LONG lData=0);
	//�����������ʽ��item1|item2|item3|...
	int AddStrings(LPCTSTR lpstr);
	virtual int InsertString(LPCTSTR lpString,int index,LONG lData=0);		
	BOOL SetValue(int val,int index=INDEX_CURSEL);
	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);

	void SetOwnerDraw(BOOL bParentDraw=FALSE)
			{RingControls::SetOwnerDraw(bParentDraw,LBS_OWNERDRAWFIXED);}

	//��Ϣ��װ(����ȫ���Ժ������)
	int Select(int index){return SendMessage(m_hWnd,LB_SETCURSEL,index,0);}
	int Select(LPCTSTR lpStr,int index=-1){return SendMessage(m_hWnd,LB_SELECTSTRING,index,(LPARAM)lpStr);}
	int GetCurSel(){return SendMessage(m_hWnd,LB_GETCURSEL,0,0);}
	BOOL SetCurSel(int index){return (SendMessage(m_hWnd,LB_SETCURSEL,index,0) != LB_ERR);}

	BOOL IsSelected(int index){return (BOOL)(SendMessage(m_hWnd,LB_GETSEL,index,0) > 0);}

	int GetTextLen(int index){return SendMessage(m_hWnd,LB_GETTEXTLEN,index,0);}
	
	virtual LONG GetData(int index){return SendMessage(m_hWnd,LB_GETITEMDATA,index,0);}
	virtual BOOL SetData(int index,LPARAM lpData){return (SendMessage(m_hWnd,LB_SETITEMDATA,index,lpData) != LB_ERR);}
	
	BOOL Clear(){SendMessage(m_hWnd,LB_RESETCONTENT,0,0);return TRUE;}
	int DelString(int index){return SendMessage(m_hWnd,LB_DELETESTRING,index,0);}
	int DelString(LPCTSTR lpString){return DelString(FindString(lpString));}
	int FindString(LPCTSTR lpszSearch,int nStart=-1){return SendMessage(m_hWnd,LB_FINDSTRING,nStart,(LPARAM)lpszSearch);}
	int FindMatchString(LPCTSTR lpszSearch,int nStart=-1)
			{return SendMessage(m_hWnd,LB_FINDSTRINGEXACT,nStart,(LPARAM)lpszSearch);}
	int GetCount(){return SendMessage(m_hWnd,LB_GETCOUNT,0,0);}
	int GetTopIndex(){return SendMessage(m_hWnd,LB_GETTOPINDEX,0,0);}
	int SetTopIndex(int index){return SendMessage(m_hWnd,LB_SETTOPINDEX,index,0);}
	int GetItemHeight(int index=0){return SendMessage(m_hWnd,LB_GETITEMHEIGHT,index,0);}
	BOOL SetItemHeight(int height,int index=0){return (SendMessage(m_hWnd,LB_SETITEMHEIGHT,index,height) != LB_ERR);}
	BOOL GetItemRect(int index,LPRECT lprc);
	int ItemFromPoint(int x,int y){return SendMessage(m_hWnd,LB_ITEMFROMPOINT,0,MAKELONG(x,y));}
	int Dir(UINT flag,LPCTSTR lpszFile){return SendMessage(m_hWnd,LB_DIR,flag,(LPARAM)lpszFile);}
	
protected:
	LRESULT OnBkColor(UINT,HDC);
	int GetItemText(int index,LPTSTR lpBuf,int size);
   //int m_style;
	//virtual LRESULT RingdowProc(HWND,UINT,UINT,LONG);

private:
	//BOOL bPainted;
};

#endif
