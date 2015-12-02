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
//							RingSDK��� ringmain.lib
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
//ԭ�ļ�����ringmain.h
//
//˵������WinMain�ķ�װ���������⽫���ڱ���⣬�������ļ�����ʵ��
//      WinMain��������ΪRingMain�������Լ�ʵ�֡�
//
**********************************************************************/
#ifndef _INC_RINGCONTAINER
#define _INC_RINGCONTAINER

#include "ringmain.h"

#include <atlbase.h>
// Required ATL module
extern CComModule _Module;

#include <atlcom.h>
#include <atlwin.h>
#include <atlhost.h>

extern "C"
{
	void InitRingContainer();
	int FreeRingContainer();
	//BOOL CreateContainer();
}

//���µĺ���Ҫ���ڴ������ú�֮�У���BEGIN_SETUPWINDOW...END_SETUPWINDOW֮��
#define BEGIN_DEF_OCXPTR \
	BOOL CreateContainer(CAxWindow2& Container,LPCOLESTR ocxName,UINT uStyle=WS_HSCROLL|WS_VSCROLL,LPRECT lprc=NULL)\
	{\
		RECT rect;\
		if(lprc)\
			CopyRect(&rect,lprc);\
		else\
			GetClientRect(&rect);\
		HWND hwndContainer = Container.Create(m_hWnd,&rect,0,WS_CHILD|WS_VISIBLE|uStyle);\
		if(!hwndContainer) return FALSE;\
   	HRESULT hr = Container.CreateControl(ocxName);\
		return SUCCEEDED(hr);\
	}\

#define DEFCONTAINER(container)	CAxWindow2 container
#define RINGHTML	CComPtr<IWebBrowser2>
#define RINGOCXPTR(ptr)  CComPtr<ptr>
#define END_DEF_OCXPTR

template <typename TBase = CWindow>
class CAxWindowT2 : public CAxWindowT<TBase>
{
public:
	CAxWindowT2(HWND hwnd = 0):CAxWindowT<TBase>(hwnd){}

	BOOL IsCreated(){return (m_hWnd && ::IsWindow(m_hWnd));}
	
   HRESULT CreateControl(LPCOLESTR lpszName, IStream* pStream = NULL, IUnknown** ppUnkContainer = NULL)
   {
		return CreateControlEx(lpszName, pStream, ppUnkContainer);
   }
    
   HRESULT CreateControl(DWORD dwResID, IStream* pStream = NULL, IUnknown** ppUnkContainer = NULL)
   {
		return CreateControlEx(dwResID, pStream, ppUnkContainer);
   }
    
   HRESULT CreateControlEx(DWORD dwResID,  IStream* pStream = NULL, 
                            IUnknown** ppUnkContainer = NULL, IUnknown** ppUnkControl = NULL,
                            REFIID iidSink = IID_NULL, IUnknown* punkSink = NULL)
   {
      TCHAR szModule[_MAX_PATH];
      GetModuleFileName(_Module.GetModuleInstance(), szModule, _MAX_PATH);
        
      CComBSTR bstrURL(OLESTR("res://"));
      bstrURL.Append(szModule);
      bstrURL.Append(OLESTR("/"));
      TCHAR szResID[11];
      wsprintf(szResID, _T("%0d"), dwResID);
      bstrURL.Append(szResID);
        
      return CreateControlEx(bstrURL, pStream, ppUnkContainer, ppUnkControl, iidSink, punkSink);
   }
    
   HRESULT CreateControlEx(LPCOLESTR lpszName, IStream* pStream = NULL, 
                            IUnknown** ppUnkContainer = NULL, IUnknown** ppUnkControl = NULL,
                            REFIID iidSink = IID_NULL, IUnknown* punkSink = NULL)
   {
      ATLASSERT(::IsWindow(m_hWnd));
        
      HRESULT hr = E_FAIL;
      CComPtr<IAxWinHostWindow> spAxWindow;
        
      // Reuse existing CAxHostWindow
      hr = QueryHost(&spAxWindow);
      if(SUCCEEDED(hr))
      {
         CComPtr<IUnknown> spunkControl;
         hr = spAxWindow->CreateControlEx(lpszName,m_hWnd,pStream,&spunkControl,iidSink,punkSink);
         if(FAILED(hr))
				return hr;
        
         if( ppUnkControl ) (*ppUnkControl = spunkControl)->AddRef();
         if( ppUnkContainer ) (*ppUnkContainer = spAxWindow)->AddRef();
      }
      // Create a new CAxHostWindow
      else
      {
			return AtlAxCreateControlEx(lpszName, m_hWnd, pStream, ppUnkContainer, ppUnkControl, iidSink, punkSink);
      }
      return S_OK;
	}
    
	HRESULT AttachControl(IUnknown* pControl, IUnknown** ppUnkContainer = 0)
   {
		ATLASSERT(::IsWindow(m_hWnd));
        
      HRESULT hr = E_FAIL;
      CComPtr<IAxWinHostWindow> spAxWindow;
        
      // Reuse existing CAxHostWindow
      hr = QueryHost(&spAxWindow);
      if(SUCCEEDED(hr))
      {
         hr = spAxWindow->AttachControl(pControl, m_hWnd);
         if(FAILED(hr))
				return hr;
        
         if(ppUnkContainer)(*ppUnkContainer = spAxWindow)->AddRef();
      }
      // Create a new CAxHostWindow
      else
      {
    		return AtlAxAttachControl(pControl,m_hWnd,ppUnkContainer);
      }
      return S_OK;
	}
};

typedef CAxWindowT2<CWindow> CAxWindow2;

#endif
