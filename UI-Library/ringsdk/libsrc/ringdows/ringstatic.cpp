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
//ԭ�ļ�����ringstatic.cpp
//
//˵������̬�ؼ�����
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"

RingStatic::RingStatic()
{
	m_windowType = ITIS_STATIC;
	m_crHover = 0xFFFFFFFF;
	m_crNormal = m_crTextColor;
	m_cursor = NULL;	
}

RingStatic::~RingStatic()
{
	if(m_cursor)
		DestroyCursor(m_cursor);
}

void RingStatic::SetHyperlink(COLORREF crNormal/*=0x00FF0000*/,COLORREF crHover/*=0x00FF0000*/)
{
	m_crTextColor = m_crNormal = crNormal;
	m_crHover = crHover;
	
	m_brush = (HBRUSH)GetStockObject(NULL_BRUSH);

	ReplaceStyle(0,SS_NOTIFY);
	
	if(m_cursor == NULL)
	{
		ringStr str(RSTR_WINPATH);
		str += "\\winhlp32.exe";
		HMODULE hModule = LoadLibrary(str.string());
		if(hModule)
		{
			HCURSOR hCursor = LoadCursor(hModule,MAKEINTRESOURCE(106));
			if(hCursor)
				m_cursor = CopyCursor(hCursor);
		}
		FreeLibrary(hModule);
	}
}

void RingStatic::SetHyperStyle(UINT styNormal/*=0*/,UINT styHover/*=RINGFONT_UNDERLINE*/)
{
	HFONT hf = (HFONT)SendMessage(m_hWnd,WM_GETFONT,0,0);
	if(hf)
	{
		LOGFONT lf;
		if(GetObject(hf,sizeof(LOGFONT),&lf))
		{
			lf.lfUnderline = (BYTE)(styNormal & RINGFONT_UNDERLINE);
			lf.lfItalic = (BYTE)(styNormal & RINGFONT_ITALIC);
			lf.lfStrikeOut = (BYTE)(styNormal & RINGFONT_STRIKEOUT);
			if((styNormal & RINGFONT_BOLD) == RINGFONT_BOLD)
				lf.lfWeight = FW_BOLD;
			m_font.SetFont(&lf);

			lf.lfUnderline = (BYTE)(styHover & RINGFONT_UNDERLINE);
			lf.lfItalic = (BYTE)(styHover & RINGFONT_ITALIC);
			lf.lfStrikeOut = (BYTE)(styHover & RINGFONT_STRIKEOUT);
			if((styHover & RINGFONT_BOLD) == RINGFONT_BOLD)
				lf.lfWeight = FW_BOLD;
			m_fontHover.SetFont(&lf);

			m_font.SetWindowFont(m_hWnd);
			if(SendMessage(m_parent->Handle(),WM_LINKLEAVE,(WPARAM)m_hWnd,0) == 0)
				InvalidateRect(m_hWnd,NULL,TRUE);
		}
	}
}

LRESULT RingStatic::OnBkColor(UINT uMsg,HDC hDC)
{
	switch(uMsg)
	{
		case WM_CTLCOLORSTATIC:
			return SetColors(hDC);			
	}
	return 0;
}

LRESULT RingStatic::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	if(m_crHover != 0xFFFFFFFF)	//�ǳ�����
	{
		SetMouseTrack(param);

		switch(param.uMsg)
		{
			case WM_MOUSEHOVER:				
				m_crTextColor = m_crHover;
				if(m_fontHover.GetFont())
					m_fontHover.SetWindowFont(m_hWnd);
				
				if(SendMessage(m_parent->Handle(),WM_LINKHOVER,(WPARAM)m_hWnd,0) == 0)
					UpdateWindow();//InvalidateRect(m_hWnd,NULL,TRUE);
				break;
			case WM_MOUSELEAVE:
				m_crTextColor = m_crNormal;
				if(m_font.GetFont() && m_fontHover.GetFont())
					m_font.SetWindowFont(m_hWnd);
				
				if(SendMessage(m_parent->Handle(),WM_LINKLEAVE,(WPARAM)m_hWnd,0) == 0)
					UpdateWindow();//InvalidateRect(m_hWnd,NULL,TRUE);
				break;
			case WM_SETCURSOR:
				if(m_cursor)
					SetCursor(m_cursor);
				return TRUE;
		}
	}
	return RingControls::RingdowProc(m_hWnd,param);
}

BOOL RingStatic::SizeFitText()
{
	
	RECT rc;
	HDC hDC = GetDC(m_hWnd);
		
	if(m_font.GetFont())
		SelectObject(hDC,m_font.GetFont());

	GetClientRect(&rc);
	if(GetText())
		DrawText(hDC,m_buf,strlen(m_buf),&rc,DT_CALCRECT);
	ReleaseDC(m_hWnd,hDC);
	return SetPos(0,0,rc.right - rc.left,rc.bottom - rc.top,NULL,SWP_NOMOVE);
}
