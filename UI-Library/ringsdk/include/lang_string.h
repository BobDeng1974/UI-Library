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
//							RingSDK��� ��ͬ�����ַ�������
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
//ԭ�ļ�����lang_string.h
//
//˵������ͬ�����ַ������塣
//
**********************************************************************/
#ifndef _INC_LANGUAE_STRING_HEADER
#define _INC_LANGUAE_STRING_HEADER

#if defined(RINGSDK_VER_ENGLISH)
//English strings
//for RingComm:
#define LANSTR_COMM_OPENFAIL	"Open communications device[%s] fail - "
#define LANSTR_COMM_EVENTFAIL		"CreateEvent for communications device fail - "
#define LANSTR_COMM_EVENTSETFAIL	"SetCommMask fail - "
#define LANSTR_COMM_GETTIMEOUTFAIL	"GetCommTimeouts fail - "
#define LANSTR_COMM_SETTIMEOUTFAIL	"SetCommTimeouts fail - "
#define LANSTR_COMM_CLEARERRORFAIL	"ClearCommError fail - "
#define LANSTR_COMM_GETDCBFAIL	"Unable to obtain DCB information - "
#define LANSTR_COMM_SETDCBFAIL	"Unable to set DCB information - "
#define LANSTR_COMM_WRITETIMEOUT	"Write data timeout - "

#define LANSTR_WRITEFILEERROR	"WriteFile error - "

#define LANSTR_FILEOPEN "Open"
#define LANSTR_SELDIR "Choose a folder"
////������ַ���
#define LANSTR_MENUBARNAME	"Menu bar"

//#elif defined()
//other languages string define here
//...
#else
//Ĭ������
//RingComm�õ����ַ�����
#define LANSTR_COMM_OPENFAIL	"�򿪴���[%s]ʧ�� - "
#define LANSTR_COMM_EVENTFAIL		"�������ڼ����¼�ʧ�� - "
#define LANSTR_COMM_EVENTSETFAIL	"���ô����¼�ʧ�� - "
#define LANSTR_COMM_GETTIMEOUTFAIL	"��ȡ���ڳ�ʱ����ʧ�� - "
#define LANSTR_COMM_SETTIMEOUTFAIL	"���ô��ڳ�ʱʧ�� - "
#define LANSTR_COMM_CLEARERRORFAIL	"ClearCommErrorʧ�� - "
#define LANSTR_COMM_GETDCBFAIL	"��ȡDCB��Ϣʧ�� - "
#define LANSTR_COMM_SETDCBFAIL	"����DCB��Ϣʧ�� - "
#define LANSTR_COMM_WRITETIMEOUT	"д�����ݳ�ʱ - "

#define LANSTR_WRITEFILEERROR	"д���ļ�ʧ�� - "

#define LANSTR_FILEOPEN "��"
#define LANSTR_SELDIR "ѡ��·��"

//������ַ���
#define LANSTR_MENUBARNAME	"�˵���"

#endif

#endif
