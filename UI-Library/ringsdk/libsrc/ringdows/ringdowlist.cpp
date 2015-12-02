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
//ԭ�ļ�����ringdowlist.cpp
//
//���ļ�˵����RingdowList��ʵ�ִ���
//
//��⹦�ܣ����ڶ���ָ���б��װ��ÿ�������������ʱ���
//				ָ���ŵ����б������˳�ʱ�������δ�ͷţ���
//				����ָ���б���Զ��ͷ�ÿ������
//
//�㷨ʵ�֣�
//		��������HASH���洰�ڶ���ָ�룬��Ϊ���ڶ���ָ��һ��
//		Ϊż������HASH����Ϊ������ָ���λֵ����2��ģ��16��
//		���Ա����m_pListΪ����16��ָ�����飬ָ��ʵ�����ݴ洢����
//		���ݴ洢���ɶ�̬���ӣ����׵�ַ����Ϊָ����һ����������
//
//		m_pList			�������׵�ַ��������������ʱ�ͷ�
//		| 0 |--|  	 |����|--------------------->|����|----------->...
//		| 1 |  |---->|	�� |-------		 |-------->| �� |
//		| 2 |----	 |	�� | HASH |		 |����     | �� |
//		| . |	  |	 | �� | ��ͻ |		 |����     | �� |
//		| . |	  |	 |	.	| ���� |		 |����     | .	 |
//		| . |	  |--->|	.	| �浽 |		 |����     | .	 |
//		| . |			 |	.	| ���� |		 |		     | .	 |
//		| . |			 |	.	|<-----|		 |		     | .	 |
//		| 15|			 |	.	|-------------		     | .	 |
//
**********************************************************************/

#define MAKE_SELF_LIB
//#ifndef __BORLANDC__
#include "ringdows.h"
//#else
//#include "include\ringdows.h"
//#endif

#ifdef _DEBUG

LPRINGDOWPTRBUF RingdowList::first_data = NULL;
LPRINGDOWPTR* RingdowList::m_pList = NULL;
int RingdowList::m_objCnt = 0;
CRITICAL_SECTION RingdowList::m_RingdowList_critical;
//int RingdowList::m_wndCnt = 0;

RingdowList::RingdowList()
{
   if(m_pList == NULL)
	{
		InitializeCriticalSection(&m_RingdowList_critical);
		m_pList = (LPRINGDOWPTR*)New(sizeof(LPRINGDOWPTR) << 4);
		if(m_pList)
			memset(m_pList,0,sizeof(LPRINGDOWPTR) << 4);
	}
	m_objCnt ++;
}

RingdowList::~RingdowList()
{
	//������������ע�����д����಻�ɰ�������Ϊ��Ա���ݣ�
	//(��̬��Ա����)�������δ�ͷű������Ҳ���ͷţ�����
	//������Ϊ0��Ҳ�Ͳ����Զ��ͷ�δ�ͷŶ���
	EnterCriticalSection(&m_RingdowList_critical);
	m_objCnt --;
   if(m_objCnt == 0)
	{
		LPRINGDOWPTRBUF temp,save;
		LPRINGDOWPTR del;

		temp = first_data;
		int i;
		char s[400];
		while(temp)
		{
			save = temp;

			del = &temp->data[0];
			for(i=0;i<16;i++)
			{
				if(del->winptr != NULL)
				{
					wsprintf(s,"�ļ�%s,��%d�з���Ķ���δ�ͷţ����Զ��ͷš�\n���棺�����汾�����Զ��ͷţ������޸Ĵ�����ȷ���ͷŶ���\0",del->file,del->line);
					OutputDebugString(s);
					::delete del->winptr;
				}
				del ++;
			}
			temp = temp->next;
			Del(save);
		}
		Del(m_pList);
		LeaveCriticalSection(&m_RingdowList_critical);
		DeleteCriticalSection(&m_RingdowList_critical);
	}
	else
		LeaveCriticalSection(&m_RingdowList_critical);
}

LPRINGDOWPTR RingdowList::operator[](RingBaseWnd* w)
{
	int index = GetHashKey((DWORD)w);

	if(first_data == NULL)
	{
		first_data = (LPRINGDOWPTRBUF)New(sizeof(RINGDOWPTRBUF));
		if(first_data)
			memset(first_data,0,sizeof(RINGDOWPTRBUF));
	}

	LPRINGDOWPTR temp = FindEmpty();
	
	if(m_pList[index] == NULL)		
		m_pList[index] = temp;
	else
	{
		LPRINGDOWPTR prev = m_pList[index];
		while(prev->next)
			prev = prev->next;

		prev->next = temp;
	}

	return temp;
}

LPRINGDOWPTR RingdowList::FindEmpty()
{
	LPRINGDOWPTR temp;
	LPRINGDOWPTRBUF save,pt = first_data;
	int i;

	while(pt)
	{
		temp = &pt->data[0];
		for(i=0;i<16;i++)
		{
			if(temp->winptr == NULL)
				return temp;
			temp ++;
		}
		save = pt;
		pt = pt->next;
	}
	pt = save;
	pt->next = (LPRINGDOWPTRBUF)New(sizeof(RINGDOWPTRBUF));

	if(pt->next)
	{
		memset(pt->next,0,sizeof(RINGDOWPTRBUF));
		return &pt->next->data[0];
	}

	return NULL;
}

//int RingdowList::GetwinptrCnt()
//{
//	return m_winptr->m_winptrCnt;
//}

LPRINGDOWPTR RingdowList::GetItem(RingBaseWnd* w)
{
	EnterCriticalSection(&m_RingdowList_critical);
	LPRINGDOWPTR temp = m_pList[GetHashKey((DWORD)w)];
	
	while(temp)
	{
		if(temp->winptr == w)
		{
			LeaveCriticalSection(&m_RingdowList_critical);
			return temp;
		}
		temp = temp->next;
	}
	LeaveCriticalSection(&m_RingdowList_critical);
   return NULL;
}

void RingdowList::AddItem(RingBaseWnd* winptr,LPCTSTR file,int line)
{
   if(winptr && GetItem(winptr) == NULL)
	{
		EnterCriticalSection(&m_RingdowList_critical);
   	LPRINGDOWPTR temp = (*this)[winptr];
		temp->winptr = winptr;
		temp->file = file;
		temp->line = line;		
		LeaveCriticalSection(&m_RingdowList_critical);
	}
}

void RingdowList::DelItem(RingBaseWnd* w)
{
	if(w)
	{
		EnterCriticalSection(&m_RingdowList_critical);
		LPRINGDOWPTR prev,temp;
		int index = GetHashKey((DWORD)w);
		temp = m_pList[index];
		prev = NULL;

		while(temp)
		{
			if(temp->winptr)
			{
				temp->winptr = NULL;
				if(prev == NULL)
					m_pList[index] = NULL;
				else
					prev->next = temp->next;
				return;
			}
			prev = temp;
			temp = temp->next;
		}
		LeaveCriticalSection(&m_RingdowList_critical);
	}
}

#endif

/*
void RingdowList::Record(RingCtrlWnd* wnd)
{
	ringFile rf="ringdowlist.txt";
	int i;

	rf.Create(RF_NEW);
	rf.Write("m_pList:\r\n",10);

	char s[200];
	wsprintf(s,"%x\r\n\0\0",wnd);
	rf.Write(s,strlen(s));

	for(i=0;i<16;i++)
	{
		wsprintf(s,"[%d]:\t%x\r\n\0\0",i,m_pList[i]);
		rf.Write(s,strlen(s));
	}

	rf.Write("\r\ndata:\r\n",9);

	LPRINGDOWPTRBUF pt = first_data;
	i = 0;
	int j;

	while(pt)
	{
		wsprintf(s,"addr[%d]:\t%x\r\n\0\0",i,pt->next);
		rf.Write(s,strlen(s));

		for(j=0;j<16;j++)
		{
			wsprintf(s,"addr[%d-%d]:\t%x\tdata:%x\r\n\0\0",i,j,&pt->data[j],pt->data[j].winptr);
			rf.Write(s,strlen(s));
		}
		i++;
		pt = pt->next;
	}

	rf.Close();
}
*/