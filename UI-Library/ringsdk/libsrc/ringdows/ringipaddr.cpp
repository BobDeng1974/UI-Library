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
//ԭ�ļ�����ringipaddr.cpp
//
//˵����IP��ַ�ؼ�����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingIPAddr::RingIPAddr()
{
	InitIPControl();
}

BOOL RingIPAddr::Create(UINT uId,int x,int y,int width,int height)
{
	return Create(0,0,NULL,uId,x,y,width,height);
}

BOOL RingIPAddr::Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height)
{
	return RingControls::Create(dwStyle,dwExStyle,szInitText,uId,x,y,width,height);
}

int RingIPAddr::GetInt(int)
{
	DWORD dwRes;
	SendMessage(m_hWnd,IPM_GETADDRESS,0,(LPARAM)&dwRes);
	return dwRes;
}

BOOL RingIPAddr::SetValue(LPCTSTR lpszVal,int)
{
	char szip[16];
	LPSTR lps = (LPSTR)lpszVal,lpi = lps;
	int i,j,nCnt = 0;
	int ip[4] = {0,0,0,0};

	if(lpszVal)
	{
		strncpy(szip,lpszVal,15);
		lps = szip;
	
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(*lps == '\0' || *lps == '.')
				{
					if(*lps == '.')
						*lps++ = '\0';
					
					ip[i] = atoi(lpi);
					lpi = lps;
					break;
				}
				else
					lps ++;
			}
			if(*lps == '\0')
				break;
		}
		SendMessage(m_hWnd,IPM_SETADDRESS,0,MAKEIPADDRESS(ip[0],ip[1],ip[2],ip[3]));
		return TRUE;
	}
	return FALSE;
}

int RingIPAddr::GetItemText(int,LPTSTR lpBuf,int size)
{
	DWORD dw;
	SendMessage(m_hWnd,IPM_GETADDRESS,0,(LPARAM)&dw);
	if(size >= 16)
	{
		wsprintf(lpBuf,"%d.%d.%d.%d\0",FIRST_IPADDRESS(dw),SECOND_IPADDRESS(dw),
				THIRD_IPADDRESS(dw),FOURTH_IPADDRESS(dw));
		return strlen(lpBuf);
	}
	else
		return 0;
}
