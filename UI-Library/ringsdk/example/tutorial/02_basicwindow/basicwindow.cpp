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
//							RingSDK ��ʾ����
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
//˵����
//
//��������ʾ��򵥵�SDI���ڣ���ʾ������ô��ڱ���ͼƬ����ʾһ������ͼ��
//�İ�Ȩ��Ϣ��Ϣ��
//
**********************************************************************/
#include "ringmain.h"
#include "resource.h"

//����������
BEGIN_SETUPMAINWINDOW(IS_WINDOW)
	BEGIN_DEFMSG
		//�����������
		DEFCMD(OnExit)
		DEFCMD(OnHelp)
		//������Ϣ������
		DEFMSG(WM_CREATE)
	END_DEFMAINMSG
END_SETUPMAINWINDOW

//��������Ϣ�������
BEGIN_REGMAINMSG
	REGMSG(WM_CREATE)
	//��������ID����Ϣ������
	REGCMD(CM_EXIT,OnExit)
	REGCMD(CM_ABOUT,OnHelp)
END_REGMSG

//������������൱��WinMain
int RingMain()
{
	//���������ڣ����������ڱ��⣬�����������������ͣ��˵���Դ
	APP_CREATEWINDOW("RingSDK�̶̳�","RingSDKApp",WS_OVERLAPPEDWINDOW,MAKEINTRESOURCE(IDM_MENU));
   //������Ϣѭ����RingAppΪ������ö���
	return RingApp.Run();
}

//��Ϣ������
RINGMAINMSG(WM_CREATE)
{
	//���ô���ͼ��
	SetIcon(LoadIcon(GetInstance(),MAKEINTRESOURCE(IDI_APP)));
	//���ô��ڱ���ͼƬ������ͼƬΪƽ����ʾ��
	SetBkgBitmap(IDB_BITMAP1);
	//����Ĭ�ϴ���paramΪ�������Ĳ�����RINGMAINMSG(WM_CREATE)��չ��Ϊ��
	//LRESULT RingWnd_window::On_WM_CREATE(HWND hWnd,RINGPARAMS& param)
	return DefaultProc(param);
}

//����������޷���ֵ
RINGMAINCMD(OnExit)
{
	SendExit();   
}

RINGMAINCMD(OnHelp)
{
	//��ʾ��ͼ���Ȩ��Ϣ��Ϣ��
	ShowHelpInfo("RingSDK ��ʾ����\n\n(C)Copyright 2007     ","RingSDK�̶̳�",MAKEINTRESOURCE(IDI_APP));
}
