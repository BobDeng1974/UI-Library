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
//ԭ�ļ�����ringcombo.h
//
//˵�����������б�ؼ��ķ�װ
//
**********************************************************************/
#ifndef _INC_RINGCOMBO
#define _INC_RINGCOMBO

class RingCombo:public RingControls
{
public:
	RingCombo(){m_windowType = ITIS_COMBO;}
	virtual ~RingCombo(){}
	
   LPCTSTR WndClassName(){return "ComboBox";}

	BOOL Clear(){SendMessage(m_hWnd,CB_RESETCONTENT,0,0);return TRUE;}

	BOOL SetValue(LPCTSTR lpszVal,int index=INDEX_CURSEL);

	//�����������ʽ��item1|item2|item3|...
	int AddStrings(LPCTSTR lpstr);

	virtual int AddString(LPCTSTR lpString,LONG lData=0);
	virtual int InsertString(LPCTSTR lpString,int index,LONG lData=0);
	int DelString(int index){return SendMessage(m_hWnd,CB_DELETESTRING,index,0);}
	int DelString(LPCTSTR lpString){return DelString(FindString(lpString));}
	int FindString(LPCTSTR lpszSearch,int nStart=-1){return SendMessage(m_hWnd,CB_FINDSTRING,nStart,(LPARAM)lpszSearch);}
	int FindMatchString(LPCTSTR lpszSearch,int nStart=-1)
	{
		return SendMessage(m_hWnd,CB_FINDSTRINGEXACT,nStart,(LPARAM)lpszSearch);
	}

	int Select(int index){return SendMessage(m_hWnd,CB_SETCURSEL,index,0);}
	int Select(LPCTSTR lpStr,int index=-1){return SendMessage(m_hWnd,CB_SELECTSTRING,index,(LPARAM)lpStr);}
	int GetCurSel(){return SendMessage(m_hWnd,CB_GETCURSEL,0,0);}
	LPCTSTR GetLBText(int index=INDEX_CURSEL,int size=RBUF_SIZE,LPTSTR lpBuf=NULL);
   int GetData(int index){return SendMessage(m_hWnd,CB_GETITEMDATA,index,0);}
	BOOL SetData(int index,LONG data){return (SendMessage(m_hWnd,CB_SETITEMDATA,index,data)!=CB_ERR);}

protected:
	//int GetItemText(int index,LPTSTR lpBuf,int size);

private:
	BOOL bPainted;

};

#endif