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
//							Shell������� ringShell.lib
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
//ԭ�ļ�����ringshell.h
//
//˵����Shell��������
//
**********************************************************************/
#ifndef _INC_RINGSHELL
#define _INC_RINGSHELL
#include "ringlib.h"

#include <shellAPI.h>
//#include <initguid.h>
#include <shlguid.h>
#include <shlobj.h>
#include <mapi.h>
//#include <ISGUIDS.H>
/*
#define DEFINE_MYGUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#pragma option -b

#undef  INTERFACE
#define INTERFACE   ITaskbarList
DECLARE_INTERFACE_(ITaskbarList, IUnknown)
{
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
   STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
   STDMETHOD_(ULONG,Release) (THIS) PURE;
   STDMETHOD(ActivateTab)(HWND) PURE;
	STDMETHOD(AddTab)(HWND) PURE;
	STDMETHOD(DeleteTab)(HWND) PURE;
	STDMETHOD(HrInit)(void) PURE;
};
typedef ITaskbarList *LPITaskbarList;

DEFINE_GUID(CLSID_TaskbarList, 0x56fdf344, 0xfd6d, 0x11d0, 0x95, 0x8a, 0x0, 0x60, 0x97, 0xc9, 0xa0, 0x90);
DEFINE_GUID(IID_ITaskbarList, 0x56fdf342, 0xfd6d, 0x11d0, 0x95, 0x8a, 0x0, 0x60, 0x97, 0xc9, 0xa0, 0x90);
#pragma option -b.
*/

extern "C" {

//������ݷ�ʽ
HRESULT CreateShortcut( LPCSTR pszPathObj, LPSTR pszParam,LPSTR pszPath,LPSTR pszPathLink, LPSTR pszDesc);
//���ÿ������
BOOL RunControlPanel(LPSTR szCplName);
//�ɵ��������ϵİ�ť
//extern void KillTaskButton(void);

//������ؼ�����
//�����º������õ���ϵͳ�ؼ��ĺ���SelectDir���档��Ϊ�����õ�����ת����ringlib��
//����Ϊ�ǵ���ϵͳ�ؼ��İ汾����Ƕ��Ի������У�������ϵͳ����

extern BOOL Init_Browser(HWND hWnd,UINT nID);        //��ʼ��������ؼ�
extern UINT Response_Browser(LONG lParam);           //��Ӧ�ؼ�����(in WM_NOTIFY)
extern LPSTR Get_Browser_selFoldername(void);        //�õ�ѡ�е�Ŀ¼·��

}

#ifndef MAKE_SELF_LIB
#ifdef _DEBUG
#pragma comment(lib, "ringshelld.lib")
#else
#pragma comment(lib, "ringshell.lib")
#endif
//#pragma comment(linker,"/NODEFAULTLIB:libc.lib")
#endif

#endif
