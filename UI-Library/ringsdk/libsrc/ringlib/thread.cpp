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
//							RingSDK��� ringlib.lib
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
//ԭ�ļ�����thread.cpp
//
//˵�����߳���ʵ�ִ���
//
**********************************************************************/
#define MAKE_SELF_LIB

#include "ringlib.h"

RingThread::RingThread()
{
	m_handle = NULL;
	m_info = NULL;
	m_bQuit = FALSE;
	m_event = NULL;
	m_dwId = 0;
	m_dwExitCode = 0xFFFFFFFF;
}

RingThread::~RingThread()
{
	//�ȴ�30����߳��Զ��˳�������ǿ���˳���	
	Quit();
	if(m_event)
		CloseHandle(m_event);
}
	
BOOL RingThread::Start(THREADPROC proc,LONG param)
{
	if(m_handle)
		return TRUE;	//�����߳�����
	if(proc)
		m_handle = NewThread((THREADPROC)proc,(LPVOID)param);
	return (BOOL)m_handle;
}

BOOL RingThread::CanContinue(DWORD dwMilSec/* = INFINITE*/)
{
	if(m_event == NULL)
		m_event = CreateEvent(NULL,TRUE,TRUE,NULL);

	return (WAIT_OBJECT_0 == WaitForSingleObject(m_event,dwMilSec));
}

void RingThread::Continue()
{
	m_crl.Enter();
	
	m_nEvCnt ++;
	
	if(m_event)
		SetEvent(m_event);
	else
		m_event = CreateEvent(NULL,TRUE,TRUE,NULL);

	m_crl.Leave();
}

void RingThread::Wait(BOOL bForceWait/*=FALSE*/)
{
	m_crl.Enter();

	if(bForceWait)
		m_nEvCnt = 0;
	else
		m_nEvCnt --;
	
	if(m_nEvCnt <= 0)
	{
		m_nEvCnt = 0;
		
		if(m_event)
			ResetEvent(m_event);
		else
			m_event = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	m_crl.Leave();
}

void RingThread::Pause()
{
	if(m_handle)
		SuspendThread(m_handle);
}

void RingThread::Resume()
{
	if(m_handle)
		ResumeThread(m_handle);
}

BOOL RingThread::SetPriority(int nPriority)
{
	if(m_handle)
		return SetThreadPriority(m_handle,nPriority);
	else
		return FALSE;
}

BOOL RingThread::PostMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return PostThreadMessage(m_dwId,uMsg,wParam,lParam);
}

void RingThread::Quit(int nWaitSecond/*=30*/)
{
	if(m_handle == NULL)
		return;

	m_nWaitSecond = nWaitSecond;
	WaitQuit();
}

void RingThread::ThreadQuit(int nWaitSecond/*=30*/)
{
	if(m_bQuit || m_handle == NULL)	//m_bQuit = TRUE��ʾ���˳��߳�������
		return;

	//�����˳��̣߳�����������˳��̣߳�һ���˳���������Ҫ�˳����߳��е��ã��ᵼ��
	//��ʱ�������������ȴ��߳��˳���ʱ�䡣
	m_nWaitSecond = nWaitSecond;
	HANDLE handle = NewThread((THREADPROC)QuitProc,this);
	CloseHandle(handle);
}

DWORD RingThread::NotifyThreadQuit()
{
	m_crl.Enter();
	try
	{
		if(m_handle)
			CloseHandle(m_handle);

		//���ò������Ա���һ������	
		m_handle = NULL;
		delete m_info;
		m_info = NULL;
		m_bQuit = FALSE;
		m_nEvCnt = 0;
		
		if(m_event)
			SetEvent(m_event);
	}
	catch(...)
	{
	}
	m_crl.Leave();
	return m_dwExitCode;
}

DWORD WINAPI RingThread::ThreadProc(RingThInfo* obj)
{	
	if(obj)
	{
		try
		{
			obj->m_thread->m_dwId = GetCurrentThreadId();
			obj->m_thread->m_dwExitCode = 0xFFFFFFFF;
			try
			{
				obj->m_thread->m_dwExitCode = (obj->m_obj->*(obj->m_lpthreadFunc))(obj->m_param);
			}
			catch(...)
			{
			}
			return obj->m_thread->NotifyThreadQuit();			
		}
		catch(...)
		{
		}
	}
	return 0;
}

DWORD WINAPI RingThread::QuitProc(RingThread* obj)
{
	if(obj)
		return obj->WaitQuit();
	return 0;
}

DWORD RingThread::GetExitCode()
{
	if(m_handle == NULL)
		return m_dwExitCode;
	else
	{
		DWORD Code;
		if(GetExitCodeThread(m_handle,&Code))
			return Code;
		else
			return 0xFFFFFFFF;
	}	
}

DWORD RingThread::WaitQuit()
{
	try
	{
		DWORD code = STILL_ACTIVE;
		int cnt = 0;
		int max = m_nWaitSecond * 1000;	//����
		//BOOL bAlive;

		do
		{
			if(code != STILL_ACTIVE)
				break;

			m_bQuit = TRUE;
			
			if(m_event)
				SetEvent(m_event);

			Sleep(1);
			cnt ++;
			if(cnt > max)
			{
				TerminateThread(m_handle,0xFFFFFFFF);
				break;
			}
			//m_crl.Enter();
			//bAlive = (m_handle && GetExitCodeThread(m_handle,&code));
			//m_crl.Leave();
		}while(m_handle && GetExitCodeThread(m_handle,&code));
		
		return NotifyThreadQuit();		
	}
	catch(...)
	{
	}
	return 0;
}
