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
//							RingSDK ��ʾ����(ISeeͼ�������)
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
//˵��������ͷ�ļ�
//
//����������һ����ʾ�����������ʾ��
//		����XP��ʽ�Ĳ˵�
//		���������
//		�����ļ������
//		ListView�ؼ�ʹ����ʾ
//		������Ϣ�Ի�����ʾ
//
**********************************************************************/
#ifndef _INC_GLOBAL_H
#define _INC_GLOBAL_H
#include "ringmain.h"
#include "ringdib.h"
#include "resource.h"

#define IDC_FOLDERTREE			12001
#define IDC_FILEVIEW				12002

//MDIFrame�������趨
BEGIN_SETUPMAINWINDOW(IS_MDIFRAME)
	BEGIN_DEF_VAR(NO_INIT)
		RingStatusBar* m_statbar;
		RingDirTree* m_dirtree;
		RingListView* m_fileview;
	END_DEF_VAR
	BEGIN_DEFMSG
		DEFMSG(WM_CREATE)
		DEFMSG(WM_DESTROY)
		DEFMSG(WM_MENUSELECT)
		DEFNOTIFY(TVN_SELCHANGED)
		DEFNOTIFY(NM_DBLCLK)
		DEFCMD(OnOpen)
		DEFCMD(OnExit)
		DEFCMD(OnHelp)
	END_DEFMAINMSG
END_SETUPMAINWINDOW

//MDI�Ӵ����趨
BEGIN_SETUPWINDOW(child,IS_MDICHILD)
	BEGIN_DEF_VAR(NO_INIT)
		RingDIB m_dib;
	END_DEF_VAR

	BEGIN_DEFMSG
		DEFMSG(WM_CREATE)
		DEFMSG(WM_PAINT)
		DEFMSG(WM_ERASEBKGND)
		DEFMSG(WM_SIZE)		
	END_DEFMSG(child)
END_SETUPWINDOW(child)
#endif