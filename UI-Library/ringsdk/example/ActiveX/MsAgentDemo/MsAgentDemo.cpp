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
//							RingSDK��ʾ����
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
//ԭ�ļ�����msagentdemo.cpp
//																						 
//˵������ʾ��ActiveX�ؼ�MsAgent(����)�ĵ��á�
//																						 
*********************************************************************/
#include "ringcontainer.h"
																	 
//#import "C:\WINNT.0\msagent\agentctl.dll"			 
//���������import�����ͻ����tlh��tlb�ļ����Ժ�ֻҪinclude tlh�Ϳ��ԣ�
//����ÿ�α��붼����������tlh��tlb�ļ���				 
#include "agentctl.tlh"										 
using namespace AgentObjects;								 
																	 
BEGIN_SETUPMAINWINDOW(IS_WINDOW)							 
	//��Ҫ֧��ActiveX����������BEGIN_DEF_OCXPTR��	 
	//�ú�ʵ����CreateContainer����(����ActiveX����)
	BEGIN_DEF_OCXPTR
		DEFCONTAINER(AgentCon);
		RINGOCXPTR(IAgentCtlEx) gAgent;										
		IAgentCtlCharactersPtr pChars;										
		IAgentCtlCharacterExPtr pMerlin;										
		IAgentCtlRequestPtr pRequest;											
		IConnectionPoint *pCP;													
		IUnknown *pSinkUnk;														
		DWORD dwCookie;															
	END_DEF_OCXPTR																	
																						
	BEGIN_DEFMSG																	
		DEFMSG(WM_CREATE)																
		DEFMSG(WM_CLOSE)																
	END_DEFMAINMSG																	
END_SETUPMAINWINDOW																
																						
BEGIN_REGMAINMSG																	
	REGMSG(WM_CREATE)																	
	REGMSG(WM_CLOSE)																	
END_REGMSG																			
																						
//�����¼�������
class CSink : public IDispatch												
{																						
public:																				
	// ����/��������																
	CSink():m_uRef(0){}
	virtual ~CSink() {}
	// IUnknown�ӿ�ʵ��															
	STDMETHODIMP QueryInterface( REFIID iid, void **ppvObject )
	{
		if ( iid == __uuidof( _AgentEvents ) )
		{
			*ppvObject = (_AgentEvents *)this;
			AddRef();
			return S_OK;
		}
		if ( iid == IID_IUnknown )
		{
			*ppvObject = (IUnknown *)this;
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef()
	{
		m_uRef++;
		return m_uRef;
	}
	ULONG STDMETHODCALLTYPE Release()
	{
		ULONG u = m_uRef--;
		if ( 0 == m_uRef )
		{
			delete this;
		}
		return u;
	}
	// IDispatch�ӿ�ʵ��
	STDMETHODIMP GetTypeInfoCount( UINT *pctinfo )
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetTypeInfo( UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetIDsOfNames( REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,
		DISPPARAMS *pDispParams,VARIANT *pVarResult,
		EXCEPINFO *pExcepInfo,UINT *puArgErr)
	{
		HRESULT hr = S_OK;
		
		if ( NULL != pDispParams)
		{
			if(7 == dispIdMember && 2 == pDispParams->cArgs)
			{
				if(VT_I2 == pDispParams->rgvarg[0].vt && VT_BSTR == pDispParams->rgvarg[1].vt)
				{
					OnHide( pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].iVal );
				}
				else // ���ʹ���
				{
					hr = DISP_E_TYPEMISMATCH;
				}
			}
			else if(dispIdMember == 6)
			{
				if(pDispParams->cArgs == 5)
				OnDragComplete(pDispParams->rgvarg[4].bstrVal,
					pDispParams->rgvarg[3].iVal,
					pDispParams->rgvarg[2].iVal,
					pDispParams->rgvarg[1].iVal,
					pDispParams->rgvarg[0].iVal);
			}
			else // ������������
			{
				hr = DISP_E_BADPARAMCOUNT;
			}
		}
		return hr;
	}
	// Ҫ�����_AgentEvents�¼�
	STDMETHODIMP OnHide( _bstr_t CharacterID, short Cause )
	{
		PostMessage(GetMainWnd(),WM_CLOSE,0,0);
		return S_OK;
	}
	STDMETHODIMP OnDragComplete(_bstr_t CharacterID,short Button,short Shift,short x,short y)
	{
		window->pMerlin->Speak(CComVariant("��㣬��㣬�����ӡ�"));
		return S_OK;
	}
private:
	ULONG m_uRef;
};

int RingMain()
{
	InitRingContainer();

	APP_CREATEWINDOW("MsAgentDemo","RINGSDK_COMAGENTCLASS",WS_OVERLAPPEDWINDOW,(LPSTR)NULL);
	RingApp.Run(FALSE);

	return FreeRingContainer();
}

RINGMAINMSG(WM_CREATE)
{
	//����������
	if(CreateContainer(AgentCon,OLESTR("{D45FD31B-5C6E-11D1-9EC1-00C04FD7081F}"),0))
	{
		HRESULT hr = AgentCon.QueryControl(__uuidof(IAgentCtlEx),(void**)&gAgent);
	
		if(SUCCEEDED(hr))
		{
			pCP = NULL;
			pSinkUnk = NULL;
			dwCookie = 0;

			// ��ȡ��ɫ�ļ�·��
			TCHAR szPath[MAX_PATH];
			GetWindowsDirectory( szPath, MAX_PATH );
			lstrcat(szPath, _T("\\msagent\\chars\\merlin.acs"));
			
			// ��������
			gAgent->PutConnected((VARIANT_BOOL)-1);
			
			// �������ӵ�Ĺ��̿�ʼ
			IConnectionPointContainer *pCPC = NULL;
			// ��ѯ���ӵ�����
			hr = gAgent->QueryInterface( IID_IConnectionPointContainer, (void **)&pCPC );
			if ( FAILED( hr ) )
			{
				Errmsg("��ѯ���ӵ�����ʧ�ܣ�");
				return 0;
			}
			// �������ӵ�
			hr = pCPC->FindConnectionPoint(__uuidof(_AgentEvents),&pCP);
			if(FAILED(hr))
			{
				Errmsg("�������ӵ�ʧ�ܣ�");
				return 0;
			}
			// ��ʱ���ӵ������Ѿ�û���ˣ��ͷ�֮
			pCPC->Release();
			pCPC = NULL;
			// ����֪ͨ����
			CSink* pSink = new CSink;
			hr = pSink->QueryInterface(IID_IUnknown,(void **)&pSinkUnk);
			if(FAILED(hr))
			{
				Errmsg("�ӿڲ�ѯʧ�ܣ�");
				return 0;
			}
			// �����ӵ��������
			hr = pCP->Advise(pSinkUnk,&dwCookie);
			if ( FAILED( hr ) )
			{
				Errmsg("���ӵ�����ʧ�ܣ�");
				return 0;
			}

			// ��ý�ɫ�б�
			pChars = gAgent->GetCharacters();
			// װ�ؽ�ɫ
			pRequest = pChars->Load( _bstr_t("merlin"), CComVariant(szPath) );
			pMerlin = pChars->Character( _bstr_t("merlin") );
			//pMerlin->put_AutoPopupMenu((VARIANT_BOOL)0);
			// ��ʾ��ɫ
			pMerlin->Show();
			// ������Ļ�������꣬���ƶ�
			short x = ( GetSystemMetrics( SM_CXFULLSCREEN ) - pMerlin->GetWidth() ) / 2;
			short y = ( GetSystemMetrics( SM_CYFULLSCREEN ) - pMerlin->GetHeight() ) / 2;
			pRequest = pMerlin->MoveTo( x, y );
			pRequest = pMerlin->Speak( CComVariant("�Ҽ������ң�ѡ�����ء��Խ�������") );
		}
	}
	return 0;
}

RINGMAINMSG(WM_CLOSE)
{
	pCP->Unadvise(dwCookie);
	pCP->Release();
	// ����pSinkUnk������QueryInterface�����Ա���Release֮
	pSinkUnk->Release();
	DestroyWindow(m_hWnd);
	return Quit();
}
