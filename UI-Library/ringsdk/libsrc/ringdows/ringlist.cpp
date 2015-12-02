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
//ԭ�ļ�����ringlist.cpp
//
//˵����ListBox�ؼ�����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingListBox::RingListBox()
{
	m_windowType = ITIS_LISTBOX;	
}

RingListBox::~RingListBox()
{
}

int RingListBox::InsertString(LPCTSTR lpString,int index,LONG lData/*=0*/)
{
	if(lpString)
	{
		int nIndex = SendMessage(m_hWnd,LB_INSERTSTRING,index,(LPARAM)lpString);
		if(nIndex != LB_ERR && lData != 0)
			SetData(nIndex,lData);
		
		return nIndex;
	}
	return LB_ERR;
}

int RingListBox::AddString(LPCTSTR lpString,LONG lData/*=0*/)
{
	if(lpString)
	{
		int nIndex = SendMessage(m_hWnd,LB_ADDSTRING,0,(LPARAM)lpString);
		if(nIndex != LB_ERR && lData != 0)
			SetData(nIndex,lData);
		
		return nIndex;
	}
	return LB_ERR;
}

LRESULT RingListBox::OnBkColor(UINT uMsg,HDC hDC)
{
	switch(uMsg)
	{
	case WM_CTLCOLORLISTBOX:
		return SetColors(hDC);			
	}
	return 0;
}

int RingListBox::GetItemText(int index,LPTSTR lpBuf,int size)
{
	if(index == INDEX_CURSEL)
		index = GetCurSel();
	int nTextLen = SendMessage(m_hWnd,LB_GETTEXTLEN,index,0);
	if(nTextLen > size)
	{
		if(lpBuf != (LPTSTR)m_buf)
			return 0;
		else
		{
			//�ڲ���������������С
			if(GetBuffer(nTextLen + 1))
				lpBuf = m_buf;
			else
				return 0;
		}
	}
	return SendMessage(m_hWnd,LB_GETTEXT,index,(LPARAM)lpBuf);
}

BOOL RingListBox::GetItemRect(int index,LPRECT lprc)
{
	if(lprc == NULL)
		return FALSE;

	return (LB_ERR != SendMessage(m_hWnd,LB_GETITEMRECT,index,(LPARAM)lprc));
}

BOOL RingListBox::SetValue(int val,int index/*=INDEX_CURSEL*/)
{
	char szText[16];
	wsprintf(szText,"%d\0",val);
	return SetValue(szText,index);
}

BOOL RingListBox::SetValue(LPCTSTR lpszVal,int index/*=INDEX_CURSEL*/)
{
	if(index == INDEX_CURSEL)
		index = GetCurSel();

	if(lpszVal && index >= 0 && index < GetCount())
	{
		LONG lData = GetData(index);
		SetData(index,0);		//���ⷢ��WM_DELETEITEM��Ϣ
		DelString(index);
		index = max(0,index-1);
		return InsertString(lpszVal,index,lData);
	}
	return FALSE;
}

//�����������ʽ��item1|item2|item3|...
//���ؼ���ĵ�һ���������ţ����ListBox�Զ����򣬸÷���ֵ��һ��׼ȷ��
int RingListBox::AddStrings(LPCTSTR lpstr)
{
	if(m_hWnd == NULL)
		return LB_ERR;
	
	int index,i,cnt,c = 0;
	ringStr str;
	LPTSTR s;
	
	str = lpstr;
	cnt = str.count("|") + 1;
	
	// Add the item
	for(i=0;i<cnt;i++) 
	{ 
		s = (LPTSTR)str.string() + c;
		c = str.indexOf("|",c) + 1;
		
		//str��operator[]���ǰ�ȫ��
		if(str[c-2] == '\\')
			continue;
		str[c-1] = '\0';		
		if(*s != '\0')
			index = AddString(s);
		str[c-1] = '|';		
	}
	
	return index - (cnt - 1);
}

/*
UINT RingListBox::GetString(LPCTSTR buf,int len)
{
	return SendMessage(m_hWnd,WM_GETTEXT,len,(LPARAM)buf);
}
*/
/*LRESULT RingListBox::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	return DefaultProc(param);
}
*/
