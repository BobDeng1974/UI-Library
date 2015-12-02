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
//							RingSDK��� ringservice.lib
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
//ԭ�ļ�����ringservlogfile.cpp
//
//˵������������־�ļ���ķ�װʵ��
//
**********************************************************************/
#define MAKE_SELF_LIB

#include "ringlib.h"

RingServLogFile::RingServLogFile()
{
	m_thread = NULL;
	m_nMaxFileSize = 0xFFFFFFFF;
	m_bNewLine = TRUE;
	m_nCurrDay = 0;
	m_lpszErr = NULL;
	m_rf = new ringFile;
}

RingServLogFile::~RingServLogFile()
{
	delete m_thread;	
	Del(m_lpszErr);
	delete m_rf;
}
	
void RingServLogFile::SetLogFile(LPCTSTR lpszLogfile,int nMaxSize,BOOL bWithDate/*=FALSE*/)
{
	if(m_rf)
	{
		m_ctl.Enter();

		try
		{
			if(lpszLogfile)
				m_rf->SetFile(lpszLogfile);
			
			if(nMaxSize > 0)
				m_nMaxFileSize = nMaxSize;
			
			if(bWithDate)
			{
				memset(m_szFilePrefix,0,MAX_PATH);
				strncpy(m_szFilePrefix,m_rf->Pathname(),MAX_PATH-1);
				strcat(m_szFilePrefix,"\\");
				strcat(m_szFilePrefix,m_rf->Titlename());

				m_nCurrDay = 0;

				if(m_thread == NULL)
					m_thread = new RingThread;
				if(m_thread)
				{
					//�̺߳���ѭ���������ȣ�wait�������ǿ��Ƿ�ʱ
					m_thread->Wait();
					m_thread->Start(this,(THREADFUNC)MonDateProc);
				}
			}
			else
				m_rf->Create(RF_APPEND);
		}
		catch(...)
		{
		}
		m_ctl.Leave();
	}
}

BOOL RingServLogFile::WriteLog(LPCTSTR lpBuf,LPCTSTR lpBufNext)
{
	if(lpBuf)
		return WriteLog((LPSTR)lpBuf,strlen(lpBuf),(LPSTR)lpBufNext,
								lpBufNext?strlen(lpBufNext):0,FALSE,FALSE);
	else
		return FALSE;
}

BOOL RingServLogFile::WriteLog(LPVOID lpBuf,int nSize)
{
	if(lpBuf)
		return WriteLog(lpBuf,nSize,NULL,0,FALSE,FALSE);
	else
		return FALSE;
}

BOOL RingServLogFile::WriteError(DWORD dwErrCode)
{
	BOOL bOK = FALSE;

	if(m_rf)
	{
		m_ctl.Enter();
		if(m_lpszErr == NULL)
			m_lpszErr = (LPSTR)New(1024*sizeof(TCHAR));
		
		if(m_lpszErr)
			if(GetErrMessage(dwErrCode,m_lpszErr,1024))
			{
				if(m_rf->Size() > m_nMaxFileSize)
				{
					m_rf->SeekToBegin();
					m_rf->SetEndOfFile();
				}
				bOK = m_rf->Write(m_lpszErr,strlen(m_lpszErr));
				wsprintf(m_lpszErr,"[%d]\0",dwErrCode);
				m_rf->WriteLine(m_lpszErr);
				m_bNewLine = TRUE;
			}
		m_ctl.Leave();
	}
	return bOK;
}

BOOL RingServLogFile::WriteLog(LPVOID lpBuf,int nSize1,LPVOID lpBufNext,int nSize2,
										 BOOL bSetNewLine,BOOL bAddWrap)
{
	BOOL bOK;
	m_ctl.Enter();
	if(m_rf->Size() > m_nMaxFileSize)
	{
		m_rf->SeekToBegin();
		m_rf->SetEndOfFile();
	}

	if(m_bNewLine)
	{
		wsprintf(m_szDate,"[%d-%02d-%02d %02d:%02d:%02d.%03d]\0",m_rdt.year(),m_rdt.month(),
				m_rdt.day(),m_rdt.hour(),m_rdt.minute(),m_rdt.second(),m_rdt.ms());
		m_rf->Write(m_szDate,strlen(m_szDate));
	}
	
	bOK = m_rf->Write(lpBuf,nSize1);
	if(lpBufNext)
		bOK = m_rf->Write(lpBufNext,nSize2);

	m_bNewLine = bSetNewLine;
	if(bAddWrap)
		m_rf->Write("\r\n",2);

	m_ctl.Leave();
	return bOK;
}

BOOL RingServLogFile::WriteLogLine(LPCTSTR lpBuf,LPCTSTR lpBufNext)
{
	if(lpBuf)
		return WriteLog((LPSTR)lpBuf,strlen(lpBuf),(LPSTR)lpBufNext,
								lpBufNext?strlen(lpBufNext):0,TRUE,TRUE);
	else
		return FALSE;
}
	
void RingServLogFile::OnDateChange()
{
	m_ctl.Enter();
	try
	{
		char ext[40];
		memset(ext,0,40);
		wsprintf(ext,m_rf->Extname());
		m_rf->SetFile("%s%d%02d%02d.%s\0",m_szFilePrefix,m_rdt.year(),m_rdt.month(),m_rdt.day(),ext);
		m_rf->Create(RF_APPEND);
	}
	catch(...)
	{
	}
	m_ctl.Leave();
}

DWORD RingServLogFile::MonDateProc(LONG)
{
	while(m_thread->IsRunning())
	{
		if(m_rdt.day() != m_nCurrDay)
		{
			m_nCurrDay = m_rdt.day();
			OnDateChange();
		}
		if(m_thread->CanContinue(20000))
			break;
	}
	return 0xDead;
}
