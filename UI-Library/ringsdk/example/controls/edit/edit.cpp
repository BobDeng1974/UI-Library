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
//							�ٷ������������ʾ
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
//���ļ�˵������ʾ�༭�����ʹ�á�
//
//��������ʾ��
//		��������
//		�����༭��
//		���ñ༭��Ϊֻ��������ɫΪ��ɫ������Ӧ�����ڳߴ�
//		��Ϣ���ʹ�ã���ʾ���ڳߴ磬���������Ϊ�༭��
//
//ʵ�����Ϲ���ֻ��Ҫ����45�д��루��ע�ͣ����򵥰ɣ�
//
**********************************************************************/
#include "ringmain.h"

BEGIN_SETUPMAINWINDOW(IS_WINDOW)	//����������ͨ����
	BEGIN_DEF_VAR(NO_INIT)
		RingEdit m_edit;
		RingMsg m_msg;
	END_DEF_VAR

	BEGIN_DEFMSG
		DEFMSG(WM_CREATE)
		DEFMSG(WM_SIZE)		
	END_DEFMAINMSG
END_SETUPMAINWINDOW

BEGIN_REGMAINMSG
	REGMSG(WM_CREATE)
	REGMSG(WM_SIZE)	
END_REGMSG

int RingMain()
{
	APP_CREATEWINDOW("RingSDK �༭�ؼ���ʾ","RingSDK_EDIT_DEMO",WS_OVERLAPPEDWINDOW,(LPCTSTR)NULL);
	return RingApp.Run();
}

RINGMAINMSG(WM_CREATE)
{
	//����EDIT�ؼ��ĸ�����
	m_edit.SetParentObj(this);
	//����EDIT�ؼ���RTV_FITPARENTSIZEΪʼ������Ӧ�����ڳߴ�
	m_edit.Create(WS_BORDER|ES_MULTILINE|WS_VSCROLL|ES_AUTOHSCROLL|ES_READONLY,
						RTV_FITPARENTSIZE,"",123,0,0,0,0);
	//���ñ���ɫΪ��ɫ��ES_READONLY����Ĭ��Ϊ��ɫ��
	m_edit.SetBkColor(0x00FFFFFF);
	//������Ϣ��ʾ��EDIT�ؼ�
	m_msg.SetMsgMessage(m_edit.Handle(),EM_REPLACESEL);
	
	return DefaultProc(param);
}

RINGMAINMSG(WM_SIZE)
{
	m_msg.MsgOut("���ڴ�С��%d * %d",param.mousept.x,param.mousept.y);
	return DefaultProc(param);
}
