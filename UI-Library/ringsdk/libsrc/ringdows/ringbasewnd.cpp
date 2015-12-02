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
//ԭ�ļ�����ringbasewnd.cpp
//
//���ļ�˵����������ʵ�ִ��룬���ļ�������
//					RingBaseWnd��   -- ���࣬���д�����Ӹ�������
//
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdows.h"

//���ݴ��ھ���õ����������ָ�룬�������ý���ⴴ���Ĵ���
//���д�������֧�ָú���
RingBaseWnd* GetWindowPtr(HWND hWnd)
{
	return (RingBaseWnd*)SendMessage(hWnd,MYWM_GETCLASS,0xCCCCDDDD,GetCurrentProcessId());
}

//��ⴰ�ڶ����Ƿ���Ч
BOOL IsWindowPtr(RingBaseWnd* obj)
{
	if(obj == NULL)
		return FALSE;

	try
	{
		return (obj == GetWindowPtr(obj->Handle()));
	}
	catch(...)
	{
		return FALSE;
	}
	return FALSE;
}

//for dockbar only
BOOL IsVert(int state)
{
	return (state > TBS_FLAG_DOCKBOTTOM && state <= TBS_FLAG_DOCKRIGHT);
}

BOOL IsHorz(int state)
{
	return (state <= TBS_FLAG_DOCKBOTTOM || state == TBS_FLAG_NONE);
}

BOOL IsFloat(int state)
{
	return (state == TBS_FLAG_FLY);
}

void InitIPControl()
{
	InitCommonCtl32(ICC_INTERNET_CLASSES);
}

void InitBarControl()
{
	InitCommonCtl32(ICC_BAR_CLASSES);
}

void InitCommonCtl32(DWORD dwICC)
{
	INITCOMMONCONTROLSEX cx = {sizeof(INITCOMMONCONTROLSEX),dwICC};
	InitCommonControlsEx(&cx);
}

//RINGSYSCOLOR RingBaseWnd::m_crSys;
//RingdowList RingWnd::m_windowList;
//RingMsgs* RingWnd::m_msgs = NULL;

WNDPROC CreateThunk(RingBaseWnd* w,WNDPROC thunkProc);

//�ýṹ�������ֽڶ���
#pragma pack(push,1)
struct RingThunk {
	BYTE				Call;
	int				Offset;
	WNDPROC			Proc;
	BYTE				Code[5];
	RingBaseWnd*   Window;
	BYTE				Jmp;
	BYTE				ECX;	
};
#pragma pack(pop)

/******************************************************
//
//��ʼ��m_thunk
//		WNDCLASSDE��lpfnWndProc��Ա��ҪWNDPROC�ĺ���ָ�룬����
//		���Ա���������Ǿ�̬�ģ������޷�ת����WNDPROC����������
//		��������WNDPROC���͵ĳ�Ա���ݣ���m_thunk���ֽţ�����ָ
//		��һС�λ����룬�Ѵ��ڶ���thisָ���¼������
//		�������øöδ���ת�򵽾�̬stdProc�ص�������ͬʱ
//		�Ѵ��ڶ����thisָ��ŵ�eax��������stdProc��Ϳ��Դ�
//		eaxȡ�ض���ָ�룬��λ��RingdowProc�ˣ�����VC��DEBUG����
//		���ں�������ʱ��ȡ��eaxֵ֮ǰ����eax��ֵ0xCCCCCCCC
//		:(�����ǲ��ý�thisָ���滻��HWND�ķ����������ע�⣺
//		������Ϣ��������HWND�����Ƕ���ָ��!!!������HWND����
//		��һ�ε���thunk֮ǰ��¼�����������ô��ڶ����Handle()
//		�õ���
//
******************************************************/
#define OFFSET_OF(st_name,md_name)  (size_t)&(((st_name *)0)->md_name)
static const int g_Offset = OFFSET_OF(RingThunk,Code) - OFFSET_OF(RingThunk,Proc);

WNDPROC CreateThunk(RingBaseWnd* w, WNDPROC thunkProc)
{
	RingThunk*  thunk = (RingThunk*)New(sizeof(RingThunk));
	
	///////////////////////////////////////////////
	//
	//ϵͳ����m_thunkʱ�Ķ�ջ��
	//ret HWND MSG WPARAM LPARAM
	//-------------------------------------------
	//ջ��											 ջ��
	///////////////////////////////////////////////

	//call g_Offset
	//����code[0]��callִ��ʱ�����һ��ָ��ѹջ������Procѹջ
	thunk->Call = 0xE8;        // call [rel]32
	thunk->Offset = g_Offset;  // ƫ����������Proc��Code[0]
	thunk->Proc = thunkProc;
	
	//pop ecx��Proc��ѹջ������Proc��ecx	
	thunk->Code[0] = 0x59;		//pop ecx
	
	//mov dword ptr [esp+0x4],w
	//Proc�ѵ�����ջ���Ƿ��ص�ַ�������ž���HWND�ˡ�
	//[esp+0x4]����HWND
	thunk->Code[1] = 0xC7;		//	mov
	thunk->Code[2] = 0x44;		//	dword ptr
	thunk->Code[3] = 0x24;		//	disp8[esp]
	thunk->Code[4] = 0x04;		//	+4
	thunk->Window = w;
	
	//͵�������ɹ�����ת��Proc
	//jmp [ecx]
	thunk->Jmp = 0xFF;     // jmp [r/m]32
	thunk->ECX = 0x21;     // [ecx]
	
	return (WNDPROC)thunk;
}
/*
_RingSysColors::_RingSysColors()
{
	cr3DDKSHADOW = GetSysColor(COLOR_3DDKSHADOW);
	crBTNFACE = GetSysColor(COLOR_BTNFACE);
	cr3DLIGHT = GetSysColor(COLOR_3DLIGHT);
	cr3DSHADOW = GetSysColor(COLOR_3DSHADOW);
	crBTNTEXT = GetSysColor(COLOR_BTNTEXT);
	crGRAYTEXT = GetSysColor(COLOR_GRAYTEXT);
	crHIGHLIGHT = GetSysColor(COLOR_HIGHLIGHT);
	crHIGHLIGHTTEXT = GetSysColor(COLOR_HIGHLIGHTTEXT);
	crMENU = GetSysColor(COLOR_MENU);
	crMENUTEXT = GetSysColor(COLOR_MENUTEXT);
	crWINDOWTEXT = GetSysColor(COLOR_WINDOWTEXT);
}
*/
//ringFile rf = "debug.txt";
//char f[80];

//ATOM WINAPI RegRingdowClass(LPWNDCLASSEX lpwc)
BOOL WINAPI RegRingdowClass(LPWNDCLASSEX lpwc)
{
	WNDCLASSEX wndex;
	wndex.cbSize = sizeof(WNDCLASSEX);

	if(GetClassInfoEx(GetInstance(),lpwc->lpszClassName,&wndex))
		return TRUE;
	else
	{
		ATOM atom = RegisterClassEx(lpwc);
      if(!atom)
			atom = RegisterClass((LPWNDCLASS)lpwc->style);
		return (BOOL)atom;
	}

/*
	//ԭʼ�汾����ΪAddAtom��û�м�¼ATOM��
	//�����˳�ʱ�޷�DeleteAtom����ڴ�й¶��	
	ATOM atom = FindAtom(lpwc->lpszClassName);

	if(atom == NULL)
	{
      atom = RegisterClassEx(lpwc);
      if(!atom)
		   atom = RegisterClass((LPWNDCLASS)lpwc->style);

		if(atom)
			AddAtom(lpwc->lpszClassName);
	}
	return atom;
*/
}

//#ifdef _DEBUG
//RingdowList RingBaseWnd::m_RingdowList;
//void* RingBaseWnd::operator new(size_t size,char *file/*=__FILE__*/,int line/*=__LINE__*/)
//#else
void* RingBaseWnd::operator new(size_t size)
//#endif
{
	char *s = ::new char[size];
   if(s)
   {
		RingBaseWnd* temp = (RingBaseWnd*)s;
      temp->bNeedDestroy = 0x12345678;
//#ifdef _DEBUG
//		m_RingdowList.AddItem(temp,file,line);
//#endif
   }
   return s;
}

//#ifdef _DEBUG
//void RingBaseWnd::operator delete(void* w,char *file/*=__FILE__*/,int line/*=__LINE__*/)
//{
//	delete w;
//}
//#endif
/*
void RingBaseWnd::operator delete(void* w)
{
	if(w == NULL)
		return;

	//if(((RingBaseWnd*)w)->bNeedDestroy == 0x12345678)
	{	
//#ifdef _DEBUG
//		m_RingdowList.DelItem((RingBaseWnd*)w);
//#endif
		::delete (RingBaseWnd*)w;
	}
}
//*/
RingBaseWnd::RingBaseWnd()
{
	m_hWnd = NULL;
	m_thunk = NULL;
	m_DefProc = NULL;
	m_brush = NULL;
	m_bKillOnExit = FALSE;
	m_parent = this;
	//m_bKeyDown = FALSE;

	if(bNeedDestroy != 0x12345678)
		bNeedDestroy = 0;	
}

RingBaseWnd::~RingBaseWnd()
{
	if(bNeedDestroy == 0)
	{
		//��new�����Ķ����п���Attachһ�����ڣ��ڴ��ڹر�
		//ǰ�ö���ͱ����٣���ҪDetach,��ԭ���ڹ��̹黹��		
		Detach();
	}
	bNeedDestroy = -1;
	m_hWnd = NULL;
	m_thunk = (WNDPROC)Del((RingThunk*)m_thunk);	
}

HWND RingBaseWnd::SetParent(HWND hParentWnd)
{
	return ::SetParent(m_hWnd,hParentWnd);	
}

COLORREF RingBaseWnd::GetBkColor(HDC hDC/*=NULL*/)
{
	if(m_hWnd)
	{
		COLORREF cr;

		if(hDC == NULL)
		{
			hDC = GetDC(m_hWnd);
			cr = ::GetBkColor(hDC);
			ReleaseDC(m_hWnd,hDC);
		}
		else
			cr = ::GetBkColor(hDC);
		return cr;
	}
	return CLR_INVALID;
}

HBRUSH RingBaseWnd::SetBkgColor(COLORREF crColor)
{
	if(m_brush)
		DeleteObject(m_brush);
	
	m_brush = CreateSolidBrush(crColor);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)m_brush);
	return m_brush;
}

HBRUSH RingBaseWnd::SetBkgBitmap(HBITMAP hbm)
{	
	if(hbm)
	{
		if(m_brush)
			DeleteObject(m_brush);
		
		m_brush = CreatePatternBrush(hbm);
		SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)m_brush);
		return m_brush;
	}
	return NULL;
}

HBRUSH RingBaseWnd::SetBkgBitmap(UINT uId,HINSTANCE hInst)
{
	HBITMAP bmp = LoadBitmap(hInst,MAKEINTRESOURCE(uId));
	SetBkgBitmap(bmp);
	DeleteObject(bmp);
	return m_brush;
}

//�����˳������һ����Ϣ
void RingBaseWnd::OnNcDestroy()
{
	try
	{
		if(m_brush)
			DeleteObject(m_brush);
	}
	catch(...)
	{
	}
	if(bNeedDestroy == 0x12345678 && m_bKillOnExit)
		delete this;	
}

WNDPROC RingBaseWnd::InitThunk()
{
	if(m_thunk == NULL)
		m_thunk = CreateThunk(this,stdProc);
	return m_thunk;
}

LRESULT CALLBACK WINAPI RingDialog::InitDlgProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{   
	if(uMsg == WM_INITDIALOG)
	{
		RingWnd *w = ((LPRINGWNDPARAM)lParam)->WndObj;
		
		if(w)
		{
			//�Ѵ�����Ϣ����������Ϊw->m_thunk����thunkת��stdProc
			w->SubClassWindow(hWnd);
			
			//��ֹ�ٴν��뱾��Ϣѭ��
			if(w->m_DefProc == InitDlgProc || w->m_DefProc == NULL)
				w->m_DefProc = DefaultDlgProc;
			return (*(WNDPROC)(w->GetThunk()))(hWnd,uMsg,wParam,lParam);//((LPRINGWNDPARAM)lParam)->dwInitParam);
		}
	}
	return FALSE;
}

//��/�����ļ��Ի���HOOK�Ի���
LRESULT CALLBACK WINAPI RingOFNHookDialog::InitDlgProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	if(uMsg == WM_INITDIALOG)
	{
		try
		{
			LPOPENFILENAME lpofn = (LPOPENFILENAME)lParam;
			RingOFNHookDialog *w = (RingOFNHookDialog*)lpofn->lCustData;
		
			if(w)
			{
				//�Ѵ�����Ϣ����������Ϊw->m_thunk����thunkת��stdProc
				w->SubClassWindow(hWnd);
				
				//��ֹ�ٴν��뱾��Ϣѭ��
				if(w->m_DefProc == InitDlgProc || w->m_DefProc == NULL)
					w->m_DefProc = DefaultDlgProc;

				LRESULT res = (*(WNDPROC)(w->GetThunk()))(hWnd,uMsg,wParam,lParam);//((LPRINGWNDPARAM)lParam)->dwInitParam);
				//���໯�����ڣ��縸���ڿɸı�ߴ磬��ϵͳĬ�϶���Ϊ�޸Ŀؼ�λ�ã�
				//�п��ܻ��ҡ�����HOOK�Ի���ϵͳĬ�϶���Ϊ�ı�öԻ���ߴ磬������
				//�ؼ�Ų���ļ��б��·����û��϶��ı�ߴ�ʱ�ı�öԻ���ߴ磬�ƶ�����
				//�ؼ���������Ի�������ӦCDN_INITDONE��Ϣʱ��ԭΪ���ʱ�ߴ粢��ԭ
				//�ؼ�λ�ã����Ի����ƶ����ļ��б��ҷ������϶��ı�ߴ�ʱϵͳ�ж�
				//�ؼ�λ�����󣬲����ƶ��ؼ��������Ի����޷�֪�������ڳߴ��Ѹı䣬
				//��������໯�����ڣ��ڸı�ߴ�ʱ����WM_SIZE�����Ի���
				try
				{
					w->m_thunkParent = CreateThunk(w,OFNDlgProc);
					w->m_hWndParent = ::GetParent(hWnd);
					w->m_OFNProc = (WNDPROC)SetWindowLong(w->m_hWndParent,DWL_DLGPROC,
																		(LONG)w->m_thunkParent);
				}
				catch(...)
				{
				}
				return res;
			}
		}
		catch(...)
		{
		}
	}
	return FALSE;
}

LRESULT CALLBACK WINAPI RingOFNHookDialog::OFNDlgProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	RingOFNHookDialog* w = (RingOFNHookDialog*)hWnd;
	
	if(uMsg == WM_SIZE)
	{
		LRESULT res = CallWindowProc(w->m_OFNProc,w->m_hWndParent,uMsg,wParam,lParam);
		::SendMessage(w->Handle(),WM_SIZE,0,0);
		return res;
	}
	return CallWindowProc(w->m_OFNProc,w->m_hWndParent,uMsg,wParam,lParam);
}

LRESULT CALLBACK WINAPI RingPropPage::InitPageProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{   
	if(uMsg == WM_INITDIALOG)
	{		
		RingPropPage *w = NULL;
		
		w = (RingPropPage*)((LPRINGWNDPARAM)((LPPROPSHEETPAGE)lParam)->lParam)->WndObj;
		
		char szTitle[80];
		GetWindowText(hWnd,szTitle,80);
		((LPPROPSHEETPAGE)lParam)->pszTitle = szTitle;

		if(w)
		{
			w->SubClassWindow(hWnd);
			RingBaseWnd* parent = w->Parent();
			if(parent->GetWindowType() == ITIS_PROPSHEET)
				parent->Attach(w->GetParent());
			
			if(w->m_DefProc == InitPageProc || w->m_DefProc == NULL)
				w->m_DefProc = DefaultDlgProc;
			return (*(WNDPROC)(w->GetThunk()))(hWnd,uMsg,wParam,((LPRINGWNDPARAM)((LPPROPSHEETPAGE)lParam)->lParam)->dwInitParam);
		}
	}
	return FALSE;
}

LRESULT CALLBACK WINAPI RingBaseWnd::InitWndProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{   
	if(uMsg == WM_NCCREATE)
	{
		RingBaseWnd *w = NULL;
		w = (RingBaseWnd*)((LPRINGWNDPARAM)((LPCREATESTRUCT)lParam)->lpCreateParams)->WndObj;
		if(w)
		{
			w->SubClassWindow(hWnd);
			
			if(w->m_DefProc == InitWndProc || w->m_DefProc == NULL)
				w->m_DefProc = DefWindowProc;

			//((LPCREATESTRUCT)lParam)->lpCreateParams = 
			//	(LPVOID)((LPRINGWNDPARAM)((LPCREATESTRUCT)lParam)->lpCreateParams)->dwInitParam;

			return (*(WNDPROC)(w->GetThunk()))(hWnd,uMsg,wParam,lParam);
		}
	}	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CALLBACK WINAPI RingMDIChildWnd::InitChildProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
   if(uMsg == WM_NCCREATE)
	{
		MDICREATESTRUCT *ms = (MDICREATESTRUCT*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		RingMDIChildWnd* w = (RingMDIChildWnd*)((LPRINGWNDPARAM)ms->lParam)->WndObj;
		if(w)
		{
			w->SubClassWindow(hWnd);
			
			if(w->m_DefProc == InitChildProc || w->m_DefProc == NULL)
				w->m_DefProc = DefMDIChildProc;
			
			ms->lParam = ((LPRINGWNDPARAM)((RingMDIChildWnd*)ms->lParam))->dwInitParam;
			//((LPCREATESTRUCT)lParam)->lpCreateParams = 
			//	(LPVOID)((LPRINGWNDPARAM)((RingMDIChildWnd*)ms->lParam))->dwInitParam;

			return (*(WNDPROC)(w->GetThunk()))(hWnd,uMsg,wParam,lParam);
		}
   }
   
   return DefMDIChildProc(hWnd,uMsg,wParam,lParam);
}

//���ص�������HWND����֮ǰ����m_thunk�滻Ϊ����ָ��
LRESULT CALLBACK WINAPI RingBaseWnd::stdProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam)
{
	RingBaseWnd* w = (RingBaseWnd*)hWnd;
	
	if(uMsg == MYWM_GETCLASS && wParam == 0xCCCCDDDD && (DWORD)lParam == GetCurrentProcessId())
	{
		//�Ի��򣬱������÷���ֵ
		if(w->GetWindowType() <= ITIS_DIALOG && w->GetWindowType() >= ITIS_PROPSHEET)
			SetWindowLong(w->Handle(),DWL_MSGRESULT,(LONG)w);
		return (LONG)w;
	}
	//else if(uMsg == WM_KEYDOWN)
	//	w->m_bKeyDown = TRUE;
	
	w->m_param.uMsg = uMsg;
	w->m_param.wParam = wParam;
	w->m_param.lParam = lParam;
	/*if(uMsg == WM_KEYUP && w->m_bKeyDown)
	{
		HWND hWnd = w->m_hWnd;

		w->m_bKeyDown = FALSE;		
		LRESULT res = w->RingdowProc(w->m_hWnd,w->m_param);
		if(::IsWindow(hWnd))		
			//����WM_KEYPRESS��Ϣ
			::SendMessage(hWnd,WM_KEYPRESS,wParam,lParam);
		return res;
	}
	*/
	return w->RingdowProc(w->m_hWnd,w->m_param);	
}

LRESULT RingBaseWnd::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	return DefaultProc(param);
}

LRESULT RingBaseWnd::CtlColorProc(HWND hWnd,RINGPARAMS& param)
{	
	switch(param.uMsg)
	{
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
		{
			RingBaseWnd* obj = GetWindowPtr((HWND)param.lParam);
			if(obj)
				return obj->OnBkColor(param.uMsg,param.hdc);
		}	
	}	
	return RM_NOTPROCESS;
}

//֪ͨ�Ӵ��ڴ�����Ϣ����WM_SIZE�ȣ��Ӵ��ڿ��Ը��游���ڵ����ߴ�
void RingBaseWnd::NotifyChild(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HWND hChildWnd = GetChild();
	RingBaseWnd* tmp;
	while(hChildWnd)
	{
		tmp = GetWindowPtr(hChildWnd);
		if(tmp)// && tmp->Parent() == this )//&& tmp->IsControls())
			tmp->OnParentNotify(hWnd,uMsg/* + MYWM_BASEEX*/,wParam,lParam);
		hChildWnd = GetNextChild(hChildWnd);
	}
}

BOOL RingBaseWnd::NotifyChildDrawItem(HWND hParentWnd,HWND hWnd,UINT uMsg,
												  WPARAM wParam,LPARAM lParam)
{
	RingBaseWnd* tmp = GetWindowPtr(hWnd);

	if(tmp && tmp->OnParentNotify(hParentWnd,uMsg,wParam,lParam))
		return TRUE;
	else
		return FALSE;
}

//���໯���ڹ���
WNDPROC RingBaseWnd::SubClassWindow(HWND hWnd)
{
	if(!::IsWindow(hWnd))
		return NULL;

	m_hWnd = hWnd;
	m_DefProc = (WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(LONG)InitThunk());
	return m_DefProc;
}

BOOL RingBaseWnd::Attach(HWND hWnd)
{
	if(m_hWnd && m_DefProc)	//�����󵽴���
	{
		if(m_hWnd != hWnd)	//Ҫ��������Ĵ���
		{
			RingBaseWnd* w = GetWindowPtr(hWnd);
			if(w != NULL)		//Ŀ�괰���ѱ�����,�����滻
				return FALSE;
			else
				if(!Detach())	//��������ǰ����
					return FALSE;
		}
		else
			return TRUE;
	}
	
	m_DefProc = SubClassWindow(hWnd);
	if(m_DefProc)
	{
		m_hWnd = hWnd;
		if(m_parent == this)
			SetParentObj(GetParent());
		return TRUE;
	}
	else
	{
		m_hWnd = NULL;
		return FALSE;
	}
}

BOOL RingBaseWnd::Detach()
{
	if(m_hWnd && m_DefProc && IsWindow())
	{
		BOOL bOK;
		if(m_windowType <= ITIS_DIALOG && m_windowType >= ITIS_PROPSHEET)
			bOK = SetWindowLong(m_hWnd,DWL_DLGPROC,(LONG)m_DefProc);
		else
			bOK = SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)m_DefProc);
		if(bOK)
		{
			m_DefProc = NULL;
			m_hWnd = NULL;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		m_DefProc = NULL;
		m_hWnd = NULL;
		return TRUE;
	}	
}
/*
RingOwnerDrawer* RingBaseWnd::SetItemOwnerDraw(RingOwnerDrawer* drawer)
{
	return RingControls::SetDrawer(drawer);
}
*/
void RingBaseWnd::SetTitle(LPCTSTR szTitle)
{
   if(szTitle)
		SetWindowText(m_hWnd,szTitle);
}

void RingBaseWnd::SetCursor(HCURSOR hCur)
{
   if(hCur == NULL)
		SetClassLong(m_hWnd,GCL_HCURSOR,NULL);
	else
		::SetCursor(hCur);
}

BOOL RingBaseWnd::SetPos(int x,int y,int width,int height,HWND hPos,UINT uFlags)
{
	if(width==-999 && height == -999)
		uFlags |= SWP_NOSIZE;

	return SetWindowPos(m_hWnd,hPos,x,y,width,height,uFlags);
}

BOOL RingBaseWnd::CenterWindow(BOOL bWithParent)
{
   RECT    rcChild, rcParent;
   int     cxChild, cyChild, cxParent, cyParent;
   int     cxScreen, cyScreen, xNew, yNew;
   HDC     hdc;
   HWND 	  hwndParent;

   GetWindowRect(m_hWnd,&rcChild);
   cxChild = rcChild.right - rcChild.left;
   cyChild = rcChild.bottom - rcChild.top;

   if(bWithParent && m_parent != this)
   	hwndParent = m_parent->m_hWnd;
   else
     	hwndParent = GetDesktopWindow();

   GetWindowRect(hwndParent,&rcParent);
   cxParent = rcParent.right - rcParent.left;
   cyParent = rcParent.bottom - rcParent.top;

   hdc = GetDC(m_hWnd);
   cxScreen = GetDeviceCaps(hdc,HORZRES);
   cyScreen = GetDeviceCaps(hdc,VERTRES);
   ReleaseDC(m_hWnd,hdc);

   xNew = rcParent.left + ((cxParent - cxChild) / 2);
   if(xNew < 0)
   	xNew = 0;
   else if((xNew + cxChild) > cxScreen)
	   xNew = cxScreen - cxChild;

   yNew = rcParent.top  + ((cyParent - cyChild) / 2);
   if(yNew < 0)
	   yNew = 0;
   else if((yNew + cyChild) > cyScreen)
      yNew = cyScreen - cyChild;

	return SetPos(xNew,yNew,0,0,NULL,SWP_NOSIZE | SWP_NOZORDER);
}

void RingBaseWnd::SetExStyle(DWORD dwExStyle)
{
  	SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle);
}

void RingBaseWnd::SetStyle(DWORD dwStyle)
{
  	SetWindowLong(m_hWnd,GWL_STYLE,dwStyle);
}

void RingBaseWnd::SetParentObj(RingBaseWnd* parent)
{
	if(parent)
   	m_parent = parent;
   else
   	m_parent = this;
}

BOOL RingBaseWnd::SetParentObj(HWND hParentWnd)
{
	RingBaseWnd* tmp = GetWindowPtr(hParentWnd);
	if(tmp)
	{
		m_parent = tmp;
		return TRUE;
	}
   else
	{
		m_parent = this;
		return FALSE;
	}
}

void RingBaseWnd::ReplaceStyle(DWORD dwsRemove,DWORD dwsReplace)
{
	DWORD style = GetWindowLong(m_hWnd,GWL_STYLE);
   style &= ~dwsRemove;
   style |= dwsReplace;
   SetWindowLong(m_hWnd,GWL_STYLE,style);
}

HWND RingBaseWnd::CreateEx(LPCTSTR szTitle,LPCTSTR szClass,DWORD dwStyle,
									DWORD dwExStyle,HWND hParentWnd/*=NULL*/,HMENU hMenu,
									int x,int y,int width,int height,LPARAM lParam/*=0*/)
{
	if(IsWindow())
		return m_hWnd;

	if(hParentWnd == NULL)
		hParentWnd = m_parent->Handle();
	else
		SetParentObj(GetWindowPtr(hParentWnd));

	RINGWNDPARAM wndParam = {(RingWnd*)this,lParam};

	CreateWindowEx(
				  		dwExStyle,
                  szClass,
                  szTitle,
                  dwStyle,
                  x,
                  y,
                  width,
                  height,
                  hParentWnd,
                  hMenu,
                  GetInstance(),
                  &wndParam
                  );
	//m_hWnd����InitProc�м�¼
   return m_hWnd;
}

BOOL RingBaseWnd::Popup(RingBaseWnd* obj,LPCTSTR lpDlgOrTitle,LPARAM lParamOrClass,DWORD dwStyle,DWORD dwExStyle/* = 0*/)
{
   if(obj)
   {
		if(::IsWindow(obj->Handle()))
		{
			SetActiveWindow(obj->Handle());
			return TRUE;
			//SetForegroundWindow(obj->Handle());
		}
		else
		{
			if(obj->GetWindowType() == ITIS_DIALOG)
			{
				obj->SetParentObj(this);
				return ((RingDialog*)obj)->DoModal(lpDlgOrTitle,lParamOrClass);
			}
			else
			{
				obj->SetAutoDelete();
				return (BOOL)((RingWnd*)obj)->Create(lpDlgOrTitle,(LPCTSTR)lParamOrClass,dwStyle,dwExStyle);
			}
		}
	}
   return FALSE;
}

RingControls* RingBaseWnd::InsertItem(RingControls* item,DWORD dwStyle/*=0*/,int left/*=0*/,int top/*=0*/,int width/*=0*/,int height/*=0*/)
{
	if(item)
   {
      RingBaseWnd* w = item;
		w->SetParentObj(this);
   	
		if(item->Handle() == NULL)
			item->Create(dwStyle,0,NULL,item->GetId(),left,top,width,height);
		else
		{
			if(width == 0)
				width = item->Width();
			if(height)
				height = item->Height();

			item->SetParent(m_hWnd);
			item->SetPos(left,top,width,height);
		}
   }
   return item;
}

int RingBaseWnd::ClientWidth()
{
	RECT rc;
	GetClientRect(&rc);
	return rc.right - rc.left;
}

int RingBaseWnd::ClientHeight()
{
	RECT rc;
	GetClientRect(&rc);
	return rc.bottom - rc.top;
}

//�õ��������꣨����ڸ����ڣ��޸�������Ϊ��Ļ���꣩
BOOL RingBaseWnd::GetRect(LPRECT lprc)
{
	if(lprc)
	{
		if(GetWindowRect(m_hWnd,lprc))
		{
			if(m_parent && m_parent != this)
			{
				ScreenToClient(m_parent->Handle(),(LPPOINT)lprc);
				ScreenToClient(m_parent->Handle(),(LPPOINT)lprc+1);
			}
			return TRUE;
		}
	}
	return FALSE;
}

//lprcPos����Ϊ����ڸ����ڣ�������ͣ���Ĵ���
RingDockBar* RingBaseWnd::EnableDock(LPCTSTR szTitle,
										UINT uDockStyle/* = TBS_EX_DOCKABLE*/,
										UINT uDockFlag/* = TBS_FLAG_DOCKTOP*/,
										LPRECT lprcPos/* = NULL*/)
{
	if(!IsWindow() || !IsChild(m_parent->Handle(),m_hWnd))
		return NULL;
	
	if(!m_parent->IsControls() && m_parent->IsWindow())
	{
		UINT flag = uDockFlag;
		if(uDockFlag == TBS_FLAG_FLY)		//�����������ȴ���һ��DockSite
			flag = TBS_FLAG_DOCKTOP;
		
		RingDockSite* pSite = ((RingWnd*)m_parent)->CreateDockSite(flag);
		if(pSite)
			return pSite->AddBar(this,szTitle,uDockStyle,uDockFlag,lprcPos);
	}
	return NULL;
}

RingDockBar* RingBaseWnd::EnableDockSize(LPCTSTR szTitle,int nWidth,int nHeight,
											UINT uDockFlag/* = TBS_FLAG_DOCKTOP*/,
											UINT uDockStyle/* = TBS_EX_DOCKABLE*/)
{
	if(!IsWindow() || !IsChild(m_parent->Handle(),m_hWnd))
		return NULL;
	
	if(!m_parent->IsControls() && m_parent->IsWindow())
	{
		UINT flag = uDockFlag;
		if(uDockFlag == TBS_FLAG_FLY)		//�����������ȴ���һ��DockSite
			flag = TBS_FLAG_DOCKTOP;
		
		RingDockSite* pSite = ((RingWnd*)m_parent)->CreateDockSite(flag);
		if(pSite)
		{
			SetPos(0,0,nWidth,nHeight,NULL,SWP_NOMOVE);
			return pSite->AddBar(this,szTitle,uDockStyle,uDockFlag,NULL,TRUE);
		}
	}
	return NULL;
}

RingDockBar* RingBaseWnd::GetDockBar()
{
	//DockBar�ĸ����ڶ���m_parent���븸���ڣ���DockSite������ͬһ����
	//��˱��������·����õ������ĸ����ڶ���(DockSite��FlyBar)
	RingBaseWnd* wnd = GetWindowPtr(GetParent());
	RingDockBar* bar = NULL;

	if(wnd)
	{
		if(wnd->GetWindowType() == ITIS_DOCKSITE)
			bar = ((RingDockSite*)wnd)->GetDockBar(this);
		else if(wnd->GetWindowType() == ITIS_FLYBAR)
			bar = ((RingFlyBar*)wnd)->GetDockBar();
	}
	return bar;
}

void RingBaseWnd::DisableDock()
{
	RingBaseWnd* wnd = GetWindowPtr(GetParent());
	
	if(wnd)
	{
		if(wnd->GetWindowType() == ITIS_FLYBAR)
		{
			RingDockBar* bar = ((RingFlyBar*)wnd)->GetDockBar();
			wnd = bar->GetDockSite();
		}
		if(wnd->GetWindowType() == ITIS_DOCKSITE)
			((RingDockSite*)wnd)->ReleaseDockBar(this);
	}
}

int RingBaseWnd::GetDockState()
{
	RingDockBar* bar = GetDockBar();
	if(bar)
		return bar->GetState();
	else
		return -1;
}

//////////////////////////////////////
//
//lprcPos:����ڸ����ڵ�����
//
//////////////////////////////////////
BOOL RingBaseWnd::DockTo(UINT uDockPos,LPRECT lprcPos/* = NULL*/)
{
	RingDockBar* bar = GetDockBar();
	if(bar)
		bar->Dockto(uDockPos,lprcPos);

	return (BOOL)bar;
}

int RingBaseWnd::DockBack()
{
	RingDockBar* bar = GetDockBar();
	if(bar)
		return bar->DockBack();
	else
		return -1;
}

HDWP RingBaseWnd::DefDeferWindowPos(HDWP hdwp,int x,int y,int width,int height,UINT uAppendFlag)
{
	return DeferWindowPos(hdwp,m_hWnd,0,x,y,width,height,
		SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED|uAppendFlag);
}

char *
DecodeMessage(UINT msg)
{
    switch (msg)
    {
        case WM_NULL:               return("WM_NULL");
        case WM_CREATE:             return("WM_CREATE");
        case WM_DESTROY:            return("WM_DESTROY");
        case WM_MOVE:               return("WM_MOVE");
        case WM_SIZE:               return("WM_SIZE");
        case WM_ACTIVATE:           return("WM_ACTIVATE");
        case WM_SETFOCUS:           return("WM_SETFOCUS");
        case WM_KILLFOCUS:          return("WM_KILLFOCUS");
        case WM_ENABLE:             return("WM_ENABLE");
        case WM_SETREDRAW:          return("WM_SETREDRAW");
        case WM_SETTEXT:            return("WM_SETTEXT");
        case WM_GETTEXT:            return("WM_GETTEXT");
        case WM_GETTEXTLENGTH:      return("WM_GETTEXTLENGTH");
        case WM_PAINT:              return("WM_PAINT");
        case WM_CLOSE:              return("WM_CLOSE");
        case WM_QUERYENDSESSION:    return("WM_QUERYENDSESSION");
        case WM_QUERYOPEN:          return("WM_QUERYOPEN");
        case WM_ENDSESSION:         return("WM_ENDSESSION");
        case WM_QUIT:               return("WM_QUIT");
        case WM_ERASEBKGND:         return("WM_ERASEBKGND");
        case WM_SYSCOLORCHANGE:     return("WM_SYSCOLORCHANGE");
        case WM_SHOWWINDOW:         return("WM_SHOWWINDOW");
        case WM_WININICHANGE:       return("WM_WININICHANGE");
        case WM_DEVMODECHANGE:      return("WM_DEVMODECHANGE");
        case WM_ACTIVATEAPP:        return("WM_ACTIVATEAPP");
        case WM_FONTCHANGE:         return("WM_FONTCHANGE");
        case WM_TIMECHANGE:         return("WM_TIMECHANGE");
        case WM_CANCELMODE:         return("WM_CANCELMODE");
        case WM_SETCURSOR:          return("WM_SETCURSOR");
        case WM_MOUSEACTIVATE:      return("WM_MOUSEACTIVATE");
        case WM_CHILDACTIVATE:      return("WM_CHILDACTIVATE");
        case WM_QUEUESYNC:          return("WM_QUEUESYNC");
        case WM_GETMINMAXINFO:      return("WM_GETMINMAXINFO");
        case WM_PAINTICON:          return("WM_PAINTICON");
        case WM_ICONERASEBKGND:     return("WM_ICONERASEBKGND");
        case WM_NEXTDLGCTL:         return("WM_NEXTDLGCTL");
        case WM_SPOOLERSTATUS:      return("WM_SPOOLERSTATUS");
        case WM_DRAWITEM:           return("WM_DRAWITEM");
        case WM_MEASUREITEM:        return("WM_MEASUREITEM");
        case WM_DELETEITEM:         return("WM_DELETEITEM");
        case WM_VKEYTOITEM:         return("WM_VKEYTOITEM");
        case WM_CHARTOITEM:         return("WM_CHARTOITEM");
        case WM_SETFONT:            return("WM_SETFONT");
        case WM_GETFONT:            return("WM_GETFONT");
        case WM_SETHOTKEY:          return("WM_SETHOTKEY");
        case WM_GETHOTKEY:          return("WM_GETHOTKEY");
        case WM_QUERYDRAGICON:      return("WM_QUERYDRAGICON");
        case WM_COMPAREITEM:        return("WM_COMPAREITEM");
        case WM_COMPACTING:         return("WM_COMPACTING");
        case WM_COMMNOTIFY:         return("WM_COMMNOTIFY");
        case WM_WINDOWPOSCHANGING:  return("WM_WINDOWPOSCHANGING");
        case WM_WINDOWPOSCHANGED:   return("WM_WINDOWPOSCHANGED");
        case WM_POWER:              return("WM_POWER");
        case WM_COPYDATA:           return("WM_COPYDATA");
        case WM_CANCELJOURNAL:      return("WM_CANCELJOURNAL");
        case WM_NOTIFY:             return("WM_NOTIFY");
        case WM_INPUTLANGCHANGEREQUEST: return("WM_INPUTLANGCHANGEREQUEST");
        case WM_INPUTLANGCHANGE:    return("WM_INPUTLANGCHANGE");
        case WM_TCARD:              return("WM_TCARD");
        case WM_HELP:               return("WM_HELP");
        case WM_USERCHANGED:        return("WM_USERCHANGED");
        case WM_NOTIFYFORMAT:       return("WM_NOTIFYFORMAT");
        case WM_CONTEXTMENU:        return("WM_CONTEXTMENU");
        case WM_STYLECHANGING:      return("WM_STYLECHANGING");
        case WM_STYLECHANGED:       return("WM_STYLECHANGED");
        case WM_DISPLAYCHANGE:      return("WM_DISPLAYCHANGE");
        case WM_GETICON:            return("WM_GETICON");
        case WM_SETICON:            return("WM_SETICON");
        case WM_NCCREATE:           return("WM_NCCREATE");
        case WM_NCDESTROY:          return("WM_NCDESTROY");
        case WM_NCCALCSIZE:         return("WM_NCCALCSIZE");
        case WM_NCHITTEST:          return("WM_NCHITTEST");
        case WM_NCPAINT:            return("WM_NCPAINT");
        case WM_NCACTIVATE:         return("WM_NCACTIVATE");
        case WM_GETDLGCODE:         return("WM_GETDLGCODE");
        case WM_SYNCPAINT:          return("WM_SYNCPAINT");
        case WM_NCMOUSEMOVE:        return("WM_NCMOUSEMOVE");
        case WM_NCLBUTTONDOWN:      return("WM_NCLBUTTONDOWN");
        case WM_NCLBUTTONUP:        return("WM_NCLBUTTONUP");
        case WM_NCLBUTTONDBLCLK:    return("WM_NCLBUTTONDBLCLK");
        case WM_NCRBUTTONDOWN:      return("WM_NCRBUTTONDOWN");
        case WM_NCRBUTTONUP:        return("WM_NCRBUTTONUP");
        case WM_NCRBUTTONDBLCLK:    return("WM_NCRBUTTONDBLCLK");
        case WM_NCMBUTTONDOWN:      return("WM_NCMBUTTONDOWN");
        case WM_NCMBUTTONUP:        return("WM_NCMBUTTONUP");
        case WM_NCMBUTTONDBLCLK:    return("WM_NCMBUTTONDBLCLK");
        case WM_KEYDOWN:            return("WM_KEYDOWN");
        case WM_KEYUP:              return("WM_KEYUP");
        case WM_CHAR:               return("WM_CHAR");
        case WM_DEADCHAR:           return("WM_DEADCHAR");
        case WM_SYSKEYDOWN:         return("WM_SYSKEYDOWN");
        case WM_SYSKEYUP:           return("WM_SYSKEYUP");
        case WM_SYSCHAR:            return("WM_SYSCHAR");
        case WM_SYSDEADCHAR:        return("WM_SYSDEADCHAR");
        case WM_IME_STARTCOMPOSITION:   return("WM_IME_STARTCOMPOSITION");
        case WM_IME_ENDCOMPOSITION: return("WM_IME_ENDCOMPOSITION");
        case WM_IME_COMPOSITION:    return("WM_IME_COMPOSITION");
        case WM_INITDIALOG:         return("WM_INITDIALOG");
        case WM_COMMAND:            return("WM_COMMAND");
        case WM_SYSCOMMAND:         return("WM_SYSCOMMAND");
        case WM_TIMER:              return("WM_TIMER");
        case WM_HSCROLL:            return("WM_HSCROLL");
        case WM_VSCROLL:            return("WM_VSCROLL");
        case WM_INITMENU:           return("WM_INITMENU");
        case WM_INITMENUPOPUP:      return("WM_INITMENUPOPUP");
        case WM_MENUSELECT:         return("WM_MENUSELECT");
        case WM_MENUCHAR:           return("WM_MENUCHAR");
        case WM_ENTERIDLE:          return("WM_ENTERIDLE");
        case WM_CTLCOLORMSGBOX:     return("WM_CTLCOLORMSGBOX");
        case WM_CTLCOLOREDIT:       return("WM_CTLCOLOREDIT");
        case WM_CTLCOLORLISTBOX:    return("WM_CTLCOLORLISTBOX");
        case WM_CTLCOLORBTN:        return("WM_CTLCOLORBTN");
        case WM_CTLCOLORDLG:        return("WM_CTLCOLORDLG");
        case WM_CTLCOLORSCROLLBAR:  return("WM_CTLCOLORSCROLLBAR");
        case WM_CTLCOLORSTATIC:     return("WM_CTLCOLORSTATIC");
        case WM_MOUSEMOVE:          return("WM_MOUSEMOVE");
        case WM_LBUTTONDOWN:        return("WM_LBUTTONDOWN");
        case WM_LBUTTONUP:          return("WM_LBUTTONUP");
        case WM_LBUTTONDBLCLK:      return("WM_LBUTTONDBLCLK");
        case WM_RBUTTONDOWN:        return("WM_RBUTTONDOWN");
        case WM_RBUTTONUP:          return("WM_RBUTTONUP");
        case WM_RBUTTONDBLCLK:      return("WM_RBUTTONDBLCLK");
        case WM_MBUTTONDOWN:        return("WM_MBUTTONDOWN");
        case WM_MBUTTONUP:          return("WM_MBUTTONUP");
        case WM_MBUTTONDBLCLK:      return("WM_MBUTTONDBLCLK");
        case WM_MOUSEWHEEL:         return("WM_MOUSEWHEEL");
        case WM_PARENTNOTIFY:       return("WM_PARENTNOTIFY");
        case WM_ENTERMENULOOP:      return("WM_ENTERMENULOOP");
        case WM_EXITMENULOOP:       return("WM_EXITMENULOOP");
        case WM_NEXTMENU:           return("WM_NEXTMENU");
        case WM_SIZING:             return("WM_SIZING");
        case WM_CAPTURECHANGED:     return("WM_CAPTURECHANGED");
        case WM_MOVING:             return("WM_MOVING");
        case WM_POWERBROADCAST:     return("WM_POWERBROADCAST");
        case WM_DEVICECHANGE:       return("WM_DEVICECHANGE");
        case WM_MDICREATE:          return("WM_MDICREATE");
        case WM_MDIDESTROY:         return("WM_MDIDESTROY");
        case WM_MDIACTIVATE:        return("WM_MDIACTIVATE");
        case WM_MDIRESTORE:         return("WM_MDIRESTORE");
        case WM_MDINEXT:            return("WM_MDINEXT");
        case WM_MDIMAXIMIZE:        return("WM_MDIMAXIMIZE");
        case WM_MDITILE:            return("WM_MDITILE");
        case WM_MDICASCADE:         return("WM_MDICASCADE");
        case WM_MDIICONARRANGE:     return("WM_MDIICONARRANGE");
        case WM_MDIGETACTIVE:       return("WM_MDIGETACTIVE");
        case WM_MDISETMENU:         return("WM_MDISETMENU");
        case WM_ENTERSIZEMOVE:      return("WM_ENTERSIZEMOVE");
        case WM_EXITSIZEMOVE:       return("WM_EXITSIZEMOVE");
        case WM_DROPFILES:          return("WM_DROPFILES");
        case WM_MDIREFRESHMENU:     return("WM_MDIREFRESHMENU");
        case WM_IME_SETCONTEXT:     return("WM_IME_SETCONTEXT");
        case WM_IME_NOTIFY:         return("WM_IME_NOTIFY");
        case WM_IME_CONTROL:        return("WM_IME_CONTROL");
        case WM_IME_COMPOSITIONFULL:    return("WM_IME_COMPOSITIONFULL");
        case WM_IME_SELECT:         return("WM_IME_SELECT");
        case WM_IME_CHAR:           return("WM_IME_CHAR");
        case WM_IME_KEYDOWN:        return("WM_IME_KEYDOWN");
        case WM_IME_KEYUP:          return("WM_IME_KEYUP");
        case WM_MOUSEHOVER:         return("WM_MOUSEHOVER");
        case WM_MOUSELEAVE:         return("WM_MOUSELEAVE");
        case WM_CUT:                return("WM_CUT");
        case WM_COPY:               return("WM_COPY");
        case WM_PASTE:              return("WM_PASTE");
        case WM_CLEAR:              return("WM_CLEAR");
        case WM_UNDO:               return("WM_UNDO");
        case WM_RENDERFORMAT:       return("WM_RENDERFORMAT");
        case WM_RENDERALLFORMATS:   return("WM_RENDERALLFORMATS");
        case WM_DESTROYCLIPBOARD:   return("WM_DESTROYCLIPBOARD");
        case WM_DRAWCLIPBOARD:      return("WM_DRAWCLIPBOARD");
        case WM_PAINTCLIPBOARD:     return("WM_PAINTCLIPBOARD");
        case WM_VSCROLLCLIPBOARD:   return("WM_VSCROLLCLIPBOARD");
        case WM_SIZECLIPBOARD:      return("WM_SIZECLIPBOARD");
        case WM_ASKCBFORMATNAME:    return("WM_ASKCBFORMATNAME");
        case WM_CHANGECBCHAIN:      return("WM_CHANGECBCHAIN");
        case WM_HSCROLLCLIPBOARD:   return("WM_HSCROLLCLIPBOARD");
        case WM_QUERYNEWPALETTE:    return("WM_QUERYNEWPALETTE");
        case WM_PALETTEISCHANGING:  return("WM_PALETTEISCHANGING");
        case WM_PALETTECHANGED:     return("WM_PALETTECHANGED");
        case WM_HOTKEY:             return("WM_HOTKEY");
        case WM_PRINT:              return("WM_PRINT");
        case WM_PRINTCLIENT:        return("WM_PRINTCLIENT");
        case WM_USER:               return("WM_USER");
        case WM_USER+1:             return("WM_USER+1");
        case WM_USER+2:             return("WM_USER+2");
        case WM_USER+3:             return("WM_USER+3");
        case WM_USER+4:             return("WM_USER+4");
    }

    return("");
}
