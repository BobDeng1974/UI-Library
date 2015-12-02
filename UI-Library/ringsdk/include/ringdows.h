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
//ԭ�ļ�����ringdows.h
//
//
//˵�����Դ��ڼ��Ի���ķ�װ���������⽫���ڱ���⣬���������ļ���������
//      WinMain�������Լ�ʵ�֡�
//
**********************************************************************/
#ifndef _INC_RINGDOWS
#define _INC_RINGDOWS

//#define WINVER	0x0500
#if(!defined(_WIN32_IE) || (defined(_WIN32_IE) && _WIN32_IE < 0x0501))
#define _WIN32_IE 0x0501
#endif

#if (!defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0400))
#define _WIN32_WINNT 0x0400
#endif

#include "ringlib.h"
#include "ringdowbase.h"
#include "ringcontrols.h"
#include "ringimagelist.h"
#include "ringpropsheet.h"
#include "ringdockbar.h"
#include "ringmenu.h"
#include "ringedit.h"
#include "ringbutton.h"
#include "ringstatic.h"
#include "ringcombo.h"
#include "ringlist.h"
#include "ringchecklist.h"
#include "ringheader.h"
#include "ringlistview.h"
#include "ringoptionview.h"
#include "ringtree.h"
#include "ringtoolbar.h"
#include "ringcoolbar.h"
#include "ringmenubar.h"
#include "ringstatus.h"
#include "ringtab.h"
#include "ringoutlook.h"
#include "ringipaddr.h"
#include "ringtrackbar.h"
#include "ringprogress.h"
#include "ringscrollbar.h"
#include "ringspinbutton.h"
#include "ringtooltip.h"
//#include "ringdataspool.h"

#ifndef MAKE_SELF_LIB
#ifdef _DEBUG
#pragma comment(lib, "ringdowsd.lib")
#else
#pragma comment(lib, "ringdows.lib")
#endif
#pragma comment(lib, "comctl32.lib")
#endif

#endif
