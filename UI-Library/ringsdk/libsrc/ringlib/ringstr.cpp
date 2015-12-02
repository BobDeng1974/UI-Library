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
//ԭ�ļ�����ringstr.h
//
//˵����
//    �ַ��������� ringStr ʵ��
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringlib.h"

void exwsprintf(LPSTR s,LPCTSTR format,...)
{
	va_list ap;

   if(format)
	{
		va_start(ap, format);		
		vsprintf(s,format,ap);
		va_end(ap);
	}
}

ringStr::ringStr(LPCTSTR str,DWORD size/*=0*/)
{
   if(str)
   {
      m_len = (DWORD)lstrlen(str);
      if(m_len > size)
	      if(m_len < RSTR_INIT)
   	   	m_size = RSTR_INIT;
      	else
      		m_size = m_len + sizeof(TCHAR);
      else
      	m_size = size;

   	m_data = (LPTSTR)New(m_size * sizeof(TCHAR));

      if(m_data)
      	lstrcpy(m_data,str);
      else
      	init();
   }
   else
   {
   	m_len = m_size = 0;
      m_data = NULL;
   }
	m_wData = NULL;
}

ringStr::ringStr(int size)
{
   m_wData = NULL;
	memset(x,0,4);
	
	m_data = NULL;
	m_size = m_len = 0;

   if(size <= RSTR_PCNAME && size >= RSTR_WINPATH)
   	LoadSpecial(size);
	else
	{
		if(size < 0)
			size = 0;

		m_size = size;
		m_len = 0;
 		m_data = (LPTSTR)New(size * sizeof(TCHAR));
		if(!m_data)
   		init();
	}
}

BOOL ringStr::LoadSpecial(int nFlag)
{
	if(m_data && m_size < RSTR_STEP)
		m_data = (LPTSTR)Del(m_data);
	
   if(m_data == NULL)
	{
		m_data = (LPTSTR)New(RSTR_STEP * sizeof(TCHAR));
		m_size = m_data?RSTR_STEP:0;
	}
   else
		Clear();

	m_len = 0;

	if(m_data)
	{		
		switch(nFlag)
		{
		case RSTR_WINPATH:
			m_len = GetWindowsDirectory(m_data,m_size);
			break;
		case RSTR_SYSPATH:
			m_len = GetSystemDirectory(m_data,m_size);
			break;
		case RSTR_TEMPPATH:
			m_len = GetTempPath(m_size,m_data) - 1;
			m_data[m_len] = 0;
			break;
		case RSTR_CURRPATH:
		{
			m_len = GetModuleFileName(GetInstance(),m_data,m_size);
			TCHAR c;
			for(int i=m_len-1;i>1;i--)
			{
				c = m_data[i];
				m_data[i] = 0;
				m_len --;
				if(c == '\\')
					break;
			}
			break;
		}
		case RSTR_MYFILENAME:
			m_len = GetModuleFileName(GetInstance(),m_data,m_size);
			break;
		case RSTR_PCNAME:
			GetComputerName(m_data,&m_len);
			break;
		}
	}
	return (BOOL)m_len;
}

ringStr::ringStr(const ringStr& src)
{
   m_wData = NULL;
	if(&src == this || src.m_data == NULL)
   {
		m_data = NULL;
      m_len = 0;
      m_size = 0;
   	return;
	}
   else
   {
		m_data = (LPSTR)New(src.m_size);
    	if(m_data)
      {
         lstrcpy(m_data,src.m_data);
      	m_len = src.m_len;
         m_size = src.m_size;
      }
   }
}

ringStr::~ringStr()
{
   Release();
}

void ringStr::Release()
{
	m_data = (LPSTR)Del(m_data);
   m_wData = (WCHAR*)Del(m_wData);
	m_len = m_size = 0;
}

void ringStr::Clear()
{
   if(m_data)
		memset(m_data,0,m_size * sizeof(TCHAR));
   m_len = 0;
}

void ringStr::init()
{
	m_data = (LPTSTR)New(RSTR_INIT * sizeof(TCHAR));
   if(m_data)
   {
   	m_size = RSTR_INIT;
      m_len = 0;
   }
   else
   	m_len = m_size = 0;
}

int ringStr::GetLength()
{
	if(m_data)
		m_len = lstrlen(m_data);
	else
		m_len = 0;
	return m_len;
}

TCHAR& ringStr::operator [](int index)
{
	if((DWORD)index < m_len)
		return m_data[index];
	else
	{
		x[0] = '\0';
		return x[0];
	}
}

int ringStr::Load(UINT ident,HINSTANCE hInst)
{
   if(m_data && m_size < RSTR_STEP)
   	m_data = (LPTSTR)Del(m_data);

   if(m_data == NULL)
   {
		m_data = (LPTSTR)New(RSTR_MAX * sizeof(TCHAR));
  	   m_size = RSTR_MAX;
   }
   else
   	Clear();

   m_len = LoadString(hInst,ident,m_data,m_size);

   return m_len;
}

int ringStr::printf(LPCTSTR format,...)
{
   TCHAR s[RSTR_MAX];
   va_list ap;

   if(format)
	{
		va_start(ap, format);
		_vsntprintf(s,RSTR_MAX-2,format,ap);
		//vsprintf(s, format, ap);
		va_end(ap);
	}

   m_len = lstrlen(s);
	Clear();

   if(m_len < m_size)
   	lstrcpy(m_data,s);
   else
   {
   	Del(m_data);
      if(m_len < RSTR_STEP)
      	m_size = RSTR_STEP;
      else
         m_size = RSTR_MAX;

      m_data = (LPTSTR)New(m_size * sizeof(TCHAR));
      if(m_data)
      	lstrcpy(m_data,s);
      else
      	init();
   }
   return m_len;
}

int ringStr::strcat(LPCTSTR format,...)
{
   char s[RSTR_MAX];
   va_list ap;

   if(format)
	{
		va_start(ap, format);
		_vsntprintf(s,RSTR_MAX-2,format,ap);
		//vsprintf(s, format, ap);
		va_end(ap);
	}

   m_len += lstrlen(s);

   if(m_len < m_size)
   	::lstrcat(m_data,s);
   else
   {
      LPTSTR temp = m_data;
   	//Del(m_data);
      if(m_len < RSTR_STEP)
      	m_size = RSTR_STEP;
      else if(m_len < RSTR_MAX)
         m_size = RSTR_MAX;
      else
      	m_size = m_len + sizeof(TCHAR);

      m_data = (LPTSTR)New(m_size * sizeof(TCHAR));
      if(m_data)
      	exwsprintf(m_data,"%s%s\0",temp,s);
      else
      	init();

      Del(temp);
   }
   return m_len;
}

//////////////////////////////////////////////////
//
//����m_data����
//		bClear:������ݱ�־
//		=FALSE������������ԭ��m_data���ȴ���size��������m_data���ȣ�
//				  �ض��������ݵ�size����
//		=TRUE:�����������
//
//
//////////////////////////////////////////////////
DWORD ringStr::SetLength(DWORD size,BOOL bClear/*=FALSE*/)
{
	if(m_data)
   {
      LPTSTR temp;

   	if(size > m_size)
      {
         temp = m_data;
      	m_data = (LPTSTR)New(size * sizeof(TCHAR));
         if(m_data)
         {
            m_size = size;
            if(!bClear)
	         	lstrcpy(m_data,temp);
            else
            	m_len = 0;

            Del(temp);
         }
         else
         	m_data = temp;
      }
      else
      {
      	if(bClear)
         	Clear();
			else
				memset(m_data + size,0,(m_size - size)*sizeof(TCHAR));			
      }
   }
   else
   {
		m_data = (LPTSTR)New(size * sizeof(TCHAR));
      if(m_data)
      {
      	m_size = size;
         m_len = 0;
      }
      else
      	init();
   }
   return m_size;
}

void ringStr::resizeBuf(LPCTSTR str)
{
   if(str)
   {
      DWORD len = (DWORD)lstrlen(str);
      if(len < m_size)
      {
         Clear();
      	lstrcpy(m_data,str);
         m_len = len;
      }
      else
      {
         LPTSTR temp = m_data;
   		m_data = (LPTSTR)New((len + 1) * sizeof(TCHAR));
      	if(m_data)
	      {
            m_len = len;
            m_size = len + 1;
            lstrcpy(m_data,str);
            Del(temp);
         }
      	else
         	m_data = temp;
   	}
   }
	else
		Clear();
}

//ANSI��UNICODEͨ�ð汾���������͸��ݰ汾ΪLPCSTR��LPCWSTR
const ringStr& ringStr::operator=(LPCTSTR str)
{
	if(str != m_data)
		resizeBuf(str);
   return *this;
}

const ringStr& ringStr::operator=(const ringStr& str)
{
	if(this == &str)
   	return *this;

	return operator=(str.m_data);
}

const ringStr& ringStr::operator=(const int number)
{
   if(!m_data)
   	init();
   else if(m_size < RSTR_INIT)
	   SetLength(RSTR_INIT,TRUE);

	wsprintf(m_data,"%d\0\0",number);
   m_len = lstrlen(m_data);
   return *this;
}

#ifndef _UNICODE
//ANSI�汾ר��
const ringStr& ringStr::operator=(LPCWSTR str)
{
	if(str)
	{
		int z = WideCharToMultiByte(CP_ACP,WC_SEPCHARS|WC_COMPOSITECHECK,(LPWSTR)str,-1,NULL,0,NULL,NULL);
	
		SetLength(z+1,TRUE);
		WideCharToMultiByte(CP_ACP,WC_SEPCHARS|WC_COMPOSITECHECK,(LPWSTR)str,-1,m_data,z,NULL,NULL);
		
		GetLength();
	}
   return *this;
}
#endif

const ringStr& ringStr::operator=(TCHAR ch)
{
	TCHAR s[2] = {ch,'\0'};
	resizeBuf(s);
	return *this;
}

const ringStr& ringStr::operator=(const unsigned char* psz)
{
	*this = (LPCSTR)psz;
	return *this;
}

const ringStr& ringStr::operator+=(const int number)
{
	TCHAR s[16];
	wsprintf(s,"%d\0\0",number);
	return operator+=(s);
}

const ringStr& ringStr::operator+=(LPCTSTR s)
{
   if(!s)
   	return *this;

   m_len += lstrlen(s);

   if(m_len < m_size)
   	::lstrcat(m_data,s);
   else
   {
      if(m_len < RSTR_STEP)
      	m_size = RSTR_STEP;
      else if(m_len < RSTR_MAX)
         m_size = RSTR_MAX;
      else
      	m_size = m_len + sizeof(TCHAR);

      LPTSTR temp = (LPSTR)New(m_size);
		if(m_data == NULL)
			lstrcpy(temp,s);
		else
			exwsprintf(temp,"%s%s\0",m_data,s);
		Del(m_data);
      m_data = temp;
   }
   return *this;
}

const ringStr& ringStr::operator+=(const ringStr& str)
{
   return operator+=(str.m_data);
}

const ringStr& ringStr::operator+=(TCHAR ch)
{
	TCHAR s[2] = {ch,'\0'};
	return operator+=(s);
}

BOOL ringStr::operator ==(const ringStr& str) const
{
   if(str.m_data && m_data)
		return !lstrcmp(m_data,str.m_data);
   else
		return (!str.m_data && !m_data);   	
}

BOOL ringStr::operator ==(LPCTSTR str) const
{
	if(m_data && str)
		return !lstrcmp(m_data,str);
   else
		return (!m_data && !str);
}

BOOL ringStr::operator !=(const ringStr& str) const
{
	if(str.m_data && m_data)
		return lstrcmp(m_data,str.m_data);
   else
		return (str.m_data || m_data);
}

BOOL ringStr::operator !=(LPCTSTR str) const
{
	if(m_data && str)
		return lstrcmp(m_data,str);
   else
		return (m_data || str);
}

BOOL ringStr::operator <(const ringStr& str) const
{
	if(str.m_data && m_data)
		return (lstrcmp(m_data,str.m_data) < 0);
   else
		return (BOOL)(str.m_data);   	
}

BOOL ringStr::operator >(const ringStr& str) const
{
	if(str.m_data && m_data)
		return (lstrcmp(m_data,str.m_data) > 0);
   else
		return (BOOL)(m_data);
}

BOOL ringStr::operator <=(const ringStr& str) const
{
   return !operator > (str);
}

BOOL ringStr::operator >=(const ringStr& str) const
{
	return !operator < (str);
}

ringStr ringStr::operator +(const ringStr& rhs) const
{
	ringStr temp = *this;
   temp += rhs.m_data;

   return temp;
}

ringStr ringStr::operator +(TCHAR ch) const
{
	ringStr temp = *this;
   temp += ch;
	
   return temp;
}

int ringStr::cmp(const ringStr& str) const
{
	if(str.m_data && m_data)
		return lstrcmp(m_data,str.m_data);
   else if(m_data)
   	return -1;
   else if(str.m_data)
   	return 1;
   else
   	return 0;
}

int ringStr::cmpi(const ringStr& str) const
{
	if(str.m_data && m_data)
		return lstrcmpi(m_data,str.m_data);
   else if(m_data)
   	return -1;
   else if(str.m_data)
   	return 1;
   else
   	return 0;
}

ringStr ringStr::replace(LPCTSTR str1,LPCTSTR str2,BOOL bAll/* = TRUE*/) const
{
   ringStr temp;
   replace(temp,str1,str2,bAll);
	return temp;
}

BOOL ringStr::replace(ringStr& temp,LPCTSTR str1,LPCTSTR str2,BOOL bAll/* = TRUE*/) const
{
   if(str1 == NULL || str2 == NULL || m_data == NULL || m_len == 0)
	{
		temp = m_data;
		return FALSE;
	}
	
	int l1 = lstrlen(str1);
   int l2 = lstrlen(str2);
   int addstep = l2 - l1;	//����滻���ִ������������ӵ��ִ�����
	int i,pos = 0;
   LPTSTR dest,src;
   
   pos = indexOf(str1,pos);
   src = m_data;
   
	if(addstep <= 0)
		temp.SetLength(m_size,TRUE);
	else
		temp.SetLength(m_size + (bAll?count(str1):1) * addstep + 1,TRUE);
	
	dest = temp.m_data;
	
   if(bAll)
   {
		i = 0;
		while(pos >= 0)
      {
			//COPY ƥ���ִ�ǰ����ַ�
			lstrcpyn(dest,src,pos - i + 1);
			dest += pos - i;
			src += pos - i;
			//�滻 str1 Ϊ str2
			lstrcpyn(dest,str2,l2 + 1);
			dest += l2;
			src += l1;
			i = pos + l1;
         //Ѱ����һ��ƥ��
         pos = indexOf(str1,i);
      }
   }
   else
   {
		if(pos >= 0)
      {
			//COPY ƥ���ִ�ǰ����ַ�
			lstrcpyn(dest,src,pos + 1);
			dest += pos;
			src += pos;
			//�滻 str1 Ϊ str2
			lstrcpyn(dest,str2,l2 + 1);
			dest += l2;
			src += l1;			
      }
   }
	//��������ִ�
	lstrcpyn(dest,src,m_len - (src - m_data) + 1);
	temp.GetLength();
   return TRUE;
}

void ringStr::replace(TCHAR c1,TCHAR c2,BOOL bAll/* = TRUE*/)
{
	LPTSTR tmp = m_data;
	if(tmp)
		if(bAll)
			for(DWORD i=0;i<m_len;i++)
			{
				if(*tmp == c1)
					*tmp = c2;
				tmp ++;
			}
		else
			for(DWORD i=0;i<m_len;i++)
			{
				if(*tmp == c1)
				{
					*tmp = c2;
					return;
				}
				tmp ++;
			}
}

ringStr ringStr::SubString(int start,int count/* = 0*/) const
{
	ringStr temp;

	if(start < 0)
		start = 0;

   if(count <= 0 || (DWORD)(count + start) >= m_len)
   	count = m_len - start;

   if(count <= 0)		//���start������Χ,m_len - start�п��� <0
   	return temp;

   temp.SetLength(count + sizeof(TCHAR));

   LPTSTR src = m_data + start;
   LPTSTR dest = temp.m_data;

   lstrcpyn(dest,src,count + 1);	
	temp.GetLength();
   return temp;
}

ringStr ringStr::SubString(LPCTSTR szStart,LPCTSTR szEnd,int nStart/*=0*/,int* nIndex/*=NULL*/) const
{
	int lens=0,s,e=0,sp;

	if(szStart)
		lens = lstrlen(szStart);
	
	s = indexOf(szStart,nStart);
	
	if(s < 0)
		sp = nStart;
	else
		sp = s + lens;

	if(szEnd)
		e = indexOf(szEnd,sp);
	
	if(e < 0)
		e = m_len;
	
	//if(e == 0 || (s == 0 && szStart) || (e - s - lens == 0))//�ϸ��жϰ汾
	if((s < 0 && szStart) || (e - s - lens == 0))	//szEnd�Ҳ������ص���β
	{
		//δ�ҵ�szStart��szEnd,eֻ��szEnd��Ч���Ҳ�����Ϊ0
		if(nIndex)
			*nIndex = e;
		return (LPTSTR)NULL;
	}
	else	//�ҵ�ƥ����ִ�
	{
		if(nIndex)
			*nIndex = e;
		return SubString(s + lens,e - s - lens);
	}	
}

/*********************************************************
//
//�����ַ���
//
//������
//	str:Ҫ�������ַ���
//	start:������ʼλ�ã�<0����ͷ��ʼ��>=0����startλ�ÿ�ʼ����
//	ntimes:��ntimesƥ�䣨��1��ʼ������
//
//���أ�-1��δ�ҵ�������>=0
//
//*******************************************************/
int ringStr::indexOf(LPCTSTR str,int start/*=-1*/,int ntimes/*=1*/) const
{
   if(!str || *str == '\0' || m_data == NULL || *m_data == '\0' || start > (int)m_len)
   	return -1;
	
	if(start < 0)
		start = 0;

	LPTSTR lpszCur = m_data + start;
	int nCnt = 1,nLen = lstrlen(str);

//������ԭʼ�汾������Ĭ�϶���_MBCS��_tcsstr��_MBCS�汾������
	//while((lpszCur - m_data) < (int)m_len && ((lpszCur = _tcsstr(lpszCur,str)) != NULL))
#ifndef _UNICODE
	while((lpszCur - m_data) < (int)m_len && ((lpszCur = strstr(lpszCur,str)) != NULL))
#else
	while((lpszCur - m_data) < (int)m_len && ((lpszCur = wcsstr(lpszCur,str)) != NULL))
#endif
	{
		if(++nCnt > ntimes)
			return lpszCur - m_data;
		else
			lpszCur += nLen;
	}
	return -1;
}

//ntimes:��ntimesƥ�䣨��1��ʼ������
int ringStr::lastIndexOf(LPCTSTR str,int ntimes/*=1*/) const
{
	if(!str || *str == '\0' || m_data == NULL || *m_data == '\0')
		return -1;

	int nLenF = lstrlen(str);
	if(nLenF > (int)m_len)
		return -1;

   LPTSTR lpszEndF = (LPTSTR)str + nLenF -1;

   LPTSTR pS = m_data + m_len - nLenF;
   LPTSTR pFind;
   LPTSTR p;
	int cnt = 1;
   //ƥ�䲻�ɹ�������ѯ��ָ�뽫�����ƶ����ַ���
   int nMove;

   while(pS >= m_data)
   {
      pFind = (LPTSTR)str;

      while(pFind <= lpszEndF)
      {
         if(*pS != *pFind)
         {
            //����ѯ���ĵ�ǰ�ַ��Ƿ��ڲ�ѯ����ǰ�ַ�������ִ��
            if(pFind == lpszEndF)
               nMove = nLenF;
            else
            {
            	p = pFind + 1;
               while(p <= lpszEndF)
               {
               	if(*p == *pS)
                  {
		               nMove = p - str;
                     break;
                  }
                  p ++;
               }
               if(p > lpszEndF)
               	nMove = nLenF;
            }
            break;
         }
         pFind ++;
         pS ++;
      }
      if(pFind > lpszEndF)
		{
			cnt ++;
			if(cnt > ntimes)
				return((pS - m_data) - nLenF);
			else
				nMove = nLenF << 1;
		}
      pS -= nMove;
   }
   return -1;
}

//���㺬�ж���str�Ӵ�
int ringStr::count(LPCTSTR str) const
{
   if(!str || *str == '\0' || m_data == NULL || *m_data == '\0')
		return 0;
	
	int nCnt = 0,nLen = lstrlen(str);
	LPTSTR lpszCur = m_data;

//������ԭʼ�汾������Ĭ�϶���_MBCS��_tcsstr��_MBCS�汾������
	//while((lpszCur - m_data) < (int)m_len && ((lpszCur = _tcsstr(lpszCur,str)) != NULL))
#ifndef _UNICODE
	while((lpszCur - m_data) < (int)m_len && ((lpszCur = strstr(lpszCur,str)) != NULL))
#else
	while((lpszCur - m_data) < (int)m_len && ((lpszCur = wcsstr(lpszCur,str)) != NULL))
#endif	
	{
		nCnt ++;
		lpszCur += nLen;
	}

	return nCnt;
}

ringStr ringStr::Insert(const ringStr& str, int index)
{
   ringStr temp;
   int i,cnt = str.len();
   LPTSTR src,dest;

	if(cnt == 0 || isClear())
	{
		temp = *this;
		return temp;
	}

   if((cnt + m_len) > m_size)
   	temp.SetLength(cnt + m_len + sizeof(TCHAR),TRUE);
   else
   	temp.SetLength(m_size,TRUE);

   src = m_data;
   dest = temp.m_data;

   for(i=0;i<index;i++)
   	*dest++ = *src++;

   for(i=0;i<cnt;i++)
   	*dest++ = str.m_data[i];

   cnt = m_len - index;

   for(i=0;i<cnt;i++)
   	*dest++ = *src++;

   return temp;
}

ringStr ringStr::Delete(int index, int count)
{
	ringStr temp = *this;
   LPTSTR src,dest;
   int i;

	if(isClear())
		return temp;

   src = m_data;
   dest = temp.m_data;

   if(index > (int)m_len)
   	index = m_len;

   for(i=0;i<index;i++)
   	*dest++ = *src++;

   if((index + count) > (int)m_len)
   	count = m_len - index;

   src += count;

   for(i=index+count;i<(int)m_len;i++)
   	*dest++ = *src++;

	return temp;
}

void ringStr::toLower()
{
	if(m_data && m_len > 0)
	{
		LPTSTR tmp = m_data + m_len - 1;
		while(tmp >= m_data)
		{
			*tmp = (char)tolower(*tmp);
			tmp --;
		}
	}
}

void ringStr::toUpper()
{
	if(m_data && m_len > 0)
	{
		LPTSTR tmp = m_data + m_len - 1;
		while(tmp >= m_data)
		{
   		*tmp = (char)toupper(*tmp);
			tmp --;
		}
	}
}

void ringStr::Trim()
{
	RTrim();
	LTrim();   
}

void ringStr::LTrim()
{
   LPTSTR start = m_data,tmp = m_data,end = m_data + m_len - 1;

   while(start <= end)
   {
   	if(*start == ' ')
      {
			start ++;
			m_len --;
		}
      else
      	break;
   }
   if(start > m_data)
   {
		while(start <= end)
      {
      	*tmp++ = *start++;
      }
		memset(tmp,0,(end - (tmp - 1)) * sizeof(TCHAR));
	}
}

void ringStr::RTrim()
{
   LPTSTR tmp = m_data + m_len - 1;
	while(tmp >= m_data)
   {
   	if(*tmp == ' ')
      {
      	*tmp-- = 0;
         m_len --;
      }
      else
      	break;
   }
}

WCHAR* ringStr::wstring(int* nLen)
{
	int len = (m_len << 1) + 2;
	if(m_wData)
		Del(m_wData);

	m_wData = (WCHAR*)New(len * sizeof(WCHAR));
	if(m_wData && m_data)
	{
		len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_data,-1,m_wData,len);
		if(nLen)
			*nLen = len;
		return m_wData;
	}
	return L"";
}

ringStr operator +(LPTSTR lhs, const ringStr& rhs)
{
	ringStr temp(lhs);
   temp += rhs.string();
   return temp;
}

//���count��c�ַ�����β
void ringStr::Append(TCHAR c,int count)
{
	LPTSTR s = (LPTSTR)New((count+1) * sizeof(TCHAR));
	LPTSTR p = s;
	if(s)
	{
		for(int i=0;i<count;i++)
			*p++ = c;
		operator+=(s);
	}
	Del(s);
}

//���lpstr�����ɸ�c�ַ�����β��������ַ���Ϊcount
//���lpstr���ȴ���count���ض�lpstrΪcount���ַ�
void ringStr::AppendSize(LPCTSTR lpstr,TCHAR c,int count)
{
	if(count <= 0)
		return;

	int n = lstrlen(lpstr);
	LPTSTR p,s = (LPTSTR)New((count+2) * sizeof(TCHAR));
	
	if(s)
	{
		if(lpstr)
			lstrcpyn(s,lpstr,count+1);	//+1:����'\0'

		p = s + lstrlen(s);
		for(int i=p-s;i<count;i++)
			*p++ = c;		
		operator+=(s);
	}
	Del(s);
}


/***********�������룬���вο���ֵ���ַ��������㷨***********************
//ntimes:��ntimesƥ�䣨��1��ʼ������
//���أ�-1��δ�ҵ�������>=0
int ringStr::indexOf(LPCTSTR str,int start,int ntimes) const
{
   if(!str || m_len < 1)
		return -1;
	
	if(start < 0)
		start = 0;
	
	int nLenF = lstrlen(str);
	if(nLenF < 1)
		return -1;
	
   LPTSTR lpszEndF = (LPSTR)str + nLenF -1;
   LPTSTR lpszEndS = m_data + m_len -1;
	
   LPSTR pS = m_data + start + nLenF -1;
   LPSTR pFind;
   LPSTR p;
   //ƥ�䲻�ɹ�������ѯ��ָ�뽫�����ƶ����ַ���
   int nMove;
	int cnt = 1;
   BOOL bEnd = FALSE;
	
   while(pS <= lpszEndS)
   {
		//��������ƥ�䡣
      pFind = lpszEndF;
      //�Ƚϱ���ѯ���ĵ�ǰ�ַ��Ƿ�Ͳ�ѯ�������ұ���
      //��ƥ�䣬���ƥ������ݱȽ�
      if(pS == lpszEndS)
			bEnd = TRUE;
		
      while(pFind >= str)
      {
         if(*pS != *pFind)
         {
            //����ѯ���ĵ�ǰ�ַ��Ƿ��ڲ�ѯ����ǰ�ַ�ǰ����ִ��
            if(pFind == str)
               nMove = nLenF;
            else
            {
					p = pFind - 1;
               while(p >= str)
               {
						if(*p == *pS)
                  {
							nMove = lpszEndF - p;
                     break;
                  }
                  p --;
               }
               if(p < str)
						nMove = nLenF;
            }
            break;
         }
         // ƥ��ɹ�һ�����������
         pFind --;
         pS --;
      }
      if(pFind < str)
      {
			cnt ++;
			if(cnt > ntimes)
				return((pS - m_data) + 2);
			else
				nMove = nLenF << 1;
		}
		
      if(bEnd)
			return 0;
		
      pS += nMove;
   }
   return 0;
}
**********************************************************/
