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
//ԭ�ļ�����ringheader.cpp
//
//˵����header�ؼ�����
//
//������չ����ΪListView��Header��������/��ʾ�У�Ϊ����ListViewɾ���Ͳ���
//				����Ҫ���¸���ÿһ��Ĵ��ۣ����������п�Ϊ0�ķ�ʽ�������û�
//				�϶���ͷ����ʾ�����ص��У�����ڴ��ڹ������˴�������һ����
//				����һ�в���ɾ����������ͷ������һ��LONG[2]���飬LONG[0]Ϊ
//				�û����õ����ݣ�LONG[1]Ϊ[���ر�־1�����أ�0����ʾ�����λ��]
//				[ԭ��ţ�ORDER��ţ�3BIT��][ԭ��ȣ���4λ��]����Ϊ�û��϶���
//				ʱ��������HDM_HITTEST��Ϣ��������λ��ʲôλ�ã����Բ���ͨ��
//				����HDM_HITTEST��Ϣ�ı䷵��ֵ���ﵽĿ�ġ�ֻ�ܴ���Ϊ������ȫ��
//				�ƶ�����ǰ�棬�������ֻ���������ʱ�Ż��Ϊ����״̬����ʱ����
//				WM_LBUTTONDOWN��WM_LBUTTONDBCLK��Ϣ��ͨ��HDM_HITTEST��Ϣ���
//				�����HHT_ONDIVOPEN�Ҹ���������ֱ�ӷ��أ������ʹﵽ��Ŀ�ġ�
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdows.h"
#include <malloc.h>

RingHeader::RingHeader()
{
	m_hImage = NULL;
	m_CoolMenu = NULL;
	m_bCanHideColumn = FALSE;
	m_nHideColCnt = 0;
}

RingHeader::~RingHeader()
{
	if(m_hImage)
		ImageList_Destroy(m_hImage);
	delete m_CoolMenu;
}

void RingHeader::InitImageList(int cx,int cy)
{
	if(m_hImage == NULL)
	{
		m_hImage = ImageList_Create(cx,cy,ILC_COLOR32|ILC_MASK,5,5);
		Header_SetImageList(m_hWnd,m_hImage);
	}
}

////////////////////////////////////////////////////
//
//���ͼ�꣬���������ù�IMAGELIST����ù�InitImageList(),
//
//���أ�ͼ�����
//
////////////////////////////////////////////////////
int RingHeader::AddIcon(LPCTSTR lpszIcon,HINSTANCE hInst)
{
	//��Ϊ�п��ܵ��ù�SetImageList����˲���ʹ�����õ�m_hImage,
	//����Header_GetImageList
	HIMAGELIST himl = Header_GetImageList(m_hWnd);
	HICON hicon;
	int x,y;
	int nIndex = -1;
	
	if(himl == NULL)
		InitImageList();

	if(himl)
	{
		if(!ImageList_GetIconSize(himl,&x,&y))
			x = y = 16;
		
		hicon = (HICON)LoadImage(hInst,lpszIcon,IMAGE_ICON,x,y,LR_DEFAULTCOLOR);
		if(hicon)
			nIndex = ImageList_AddIcon(himl,hicon);
	}
	
	return nIndex;
}

int RingHeader::AddIcon(UINT uidIcon,HINSTANCE hInst)
{
	return AddIcon(MAKEINTRESOURCE(uidIcon),hInst);
}

int RingHeader::AddIcon(HICON hicon)
{
	if(hicon == NULL)
		return -1;
	
	int nIndex = -1;
	HIMAGELIST himl = Header_GetImageList(m_hWnd);
	if(himl == NULL)
		InitImageList();

	if(himl)
		nIndex = ImageList_AddIcon(himl,hicon);
	
	return nIndex;
}

BOOL RingHeader::SetItemWidth(int iCol,int nWidth)
{
	HDITEM hdi;
	hdi.mask = HDI_WIDTH;
	hdi.cxy = nWidth;
	return Header_SetItem(m_hWnd,iCol,&hdi);
}

LONG RingHeader::GetData(int index)
{
	HDITEM hdi;

	hdi.mask = HDI_LPARAM;
	hdi.lParam = 0;

	Header_GetItem(m_hWnd,index,&hdi);
	return hdi.lParam;
}

BOOL RingHeader::SetData(int index,LONG lParam)
{
	HDITEM hdi;
	
	hdi.mask = HDI_LPARAM;
	hdi.lParam = lParam;
	
	return Header_SetItem(m_hWnd,index,&hdi);
}

LPLONG RingHeader::GetRealData(int index)
{
	HDITEM hdi;
	
	hdi.mask = HDI_LPARAM;
	hdi.lParam = 0;
	
	CallWindowProc(m_DefProc,m_hWnd,HDM_GETITEM,index,(LPARAM)&hdi);
	return (LPLONG)hdi.lParam;
}

BOOL RingHeader::SetRealData(int index,LONG lParam)
{
	HDITEM hdi;
	
	hdi.mask = HDI_LPARAM;
	hdi.lParam = lParam;
	
	return CallWindowProc(m_DefProc,m_hWnd,HDM_SETITEM,index,(LPARAM)&hdi);
}

LPLONG RingHeader::GetHDInfo(int index)
{
	LPLONG lpl = GetRealData(index);
	if(lpl)
		return lpl + 1;
	else
		return NULL;
}

//�������
int RingHeader::AddItem(int index,LPCTSTR szText,LPARAM lParam,int idIcon)
{
	HDITEM hdi;
	
	hdi.mask = 0;
	
	if(idIcon >= 0)
	{
		hdi.mask = HDI_IMAGE;
		hdi.iImage = idIcon;
	}
	
	if(szText)
	{
		hdi.mask |= HDI_TEXT;
		hdi.pszText = (LPSTR)szText;
		hdi.cchTextMax = strlen(szText);
	}	
	return Header_InsertItem(m_hWnd,index,&hdi);
}

int RingHeader::AddItem(int index,int nVal,LPARAM lParam,int idIcon)
{
	char s[16];
	wsprintf(s,"%d\0",nVal);
	return AddItem(index,s,lParam,idIcon);
}

void RingHeader::Select(int index,int idIcon)
{
	HDITEM hdi;
	
	hdi.mask = HDI_IMAGE;
	hdi.iImage = idIcon;
	
	Header_SetItem(m_hWnd,index,&hdi);
}

//�麯������
BOOL RingHeader::SetValue(int val,int index)
{
	char szText[16];
	wsprintf(szText,"%d\0",val);
	return SetValue(szText,index);
}

BOOL RingHeader::SetValue(LPCTSTR lpszVal,int index)
{
	HDITEM hdi;
	
	hdi.mask = 0;
	
	if(lpszVal)
	{
		hdi.mask = HDI_TEXT;
		hdi.pszText = (LPSTR)lpszVal;
		hdi.cchTextMax = strlen(lpszVal);
	}	
	return Header_SetItem(m_hWnd,index,&hdi);
}

int RingHeader::GetOrder(int index)
{
	HDITEM hdi;
	
	hdi.mask = HDI_ORDER;
	
	if(Header_GetItem(m_hWnd,index,&hdi))
		return hdi.iOrder;
	else
		return -1;
}

int RingHeader::GetItemText(int index,LPTSTR lpBuf,int size)
{
	HDITEM hdi;
	
	hdi.mask = HDI_TEXT;
	hdi.pszText = (LPSTR)lpBuf;
	hdi.cchTextMax = size;

	if(Header_GetItem(m_hWnd,index,&hdi))
		return lstrlen(lpBuf);
	else
		return 0;
}

BOOL RingHeader::IsColumnHide(int iCol)
{
	LPLONG lpl = GetRealData(iCol);
	if(lpl)
		return HDCOL_ISHIDDEN(lpl[1]);
	else
		return FALSE;
}

//��ȡ��ͷ��ȣ�nMode:�����ͷδ���أ�����ʵ�ʿ�ȣ�
//�����ͷ��������nMode=SW_HIDE������δ����ǰ�Ŀ��
int RingHeader::GetItemWidth(int iCol,int nMode/*=SW_SHOW*/)
{
	HDITEM hd;
	hd.mask = HDI_WIDTH|HDI_LPARAM;

	if(CallWindowProc(m_DefProc,m_hWnd,HDM_GETITEM,iCol,(LPARAM)&hd))
	{
		if(m_bCanHideColumn && nMode == SW_HIDE)
		{
			LPLONG lpl = (LPLONG)hd.lParam;
			if(lpl)
				return LOWORD(lpl[1]);
		}
		else
			return hd.cxy;
	}
	return -1;
}

///////////////////////////////////////////////////////////
//
//�����ڹ�����Ҫ�԰����ݽ��д�������������ʾ/������ͷ��
//���ÿ����ͷ��DATA����һ��LONG[2]���飬��һ��Ϊ�û�����
//�����ݣ��ڶ���Ϊ��¼��ͷ��Ⱥ���ʾ�����
//
//��һ�в�������
//
///////////////////////////////////////////////////////////
LRESULT RingHeader::RingdowProc(HWND hWnd,RINGPARAMS param)
{
	LRESULT res;
	switch(param.uMsg)
	{
		case HDM_DELETEITEM:
			if(m_bCanHideColumn && param.wParam != 0)
			{
				//������ͷ��Ⱥ���ʾ����ǣ��ָ��û�����
				LPLONG lpl = GetRealData(param.wParam);
				if(lpl)
				{
					LONG lUserData = *lpl;
					Del(lpl);
					SetRealData(param.wParam,lUserData);
				}
			}
			break;
		case HDM_INSERTITEM:
			res = CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
			if(res != -1 && m_bCanHideColumn)
			{
				//��ʼ���û�����
				LPLONG lpl = (LPLONG)New(sizeof(LONG)*2);
				if(lpl)
				{
					LPHDITEM phdi = (LPHDITEM)param.lParam;
					if(MASK_MATCH(phdi->mask,HDI_LPARAM))
						*lpl = phdi->lParam;
					phdi->mask = HDI_LPARAM;
					phdi->lParam = (LONG)lpl;
					lpl[1] = MAKELONG(GetItemWidth(res),res);
					CallWindowProc(m_DefProc,m_hWnd,HDM_SETITEM,res,(LPARAM)phdi);
				}
			}
			return res;
		case HDM_GETITEM:
			res = CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,param.lParam);
			if(m_bCanHideColumn && param.wParam != 0)
			{
				//�����û����ݷ���
				LPHDITEM lphd = (LPHDITEM)param.lParam;
				if(res && lphd && MASK_MATCH(lphd->mask,HDI_LPARAM) && lphd->lParam)
					lphd->lParam = *((LPLONG)lphd->lParam);
			}
			return res;			
		case HDM_SETITEM:
			if(m_bCanHideColumn && param.wParam != 0)
			{
				LPHDITEM lphd = (LPHDITEM)param.lParam;
				if(lphd && (lphd->mask & (HDI_LPARAM|HDI_WIDTH)))
				{
					//�������ݣ���Ҫ��ȡԭ��������
					BOOL bHide = FALSE;
					HDITEM hd;
					hd.mask = HDI_LPARAM|HDI_WIDTH;
					hd.lParam = 0;
					if(CallWindowProc(m_DefProc,m_hWnd,HDM_GETITEM,param.wParam,(LONG)&hd))
					{
						if(hd.lParam)
						{
							LPLONG lpl = (LPLONG)hd.lParam;
							if(MASK_MATCH(lphd->mask,HDI_LPARAM))
								*lpl = lphd->lParam;
							//���ÿ�ȣ���¼
							if(MASK_MATCH(lphd->mask,HDI_WIDTH))
							{
								bHide = HDCOL_ISHIDDEN(lpl[1]);
								lpl[1] &= 0xFFFF0000;
								lpl[1] |= lphd->cxy;
							}
						}						
					}
					memcpy(&hd,lphd,sizeof(HDITEM));
					//���������ã�ȡ���ñ�־
					hd.mask &= ~HDI_LPARAM;
					if(bHide && hd.cxy != 0)	//�������������ÿ�Ȳ�Ϊ0���������������ÿ�ȣ���ʵ������
						hd.mask &= ~HDI_WIDTH;					
					return CallWindowProc(m_DefProc,m_hWnd,param.uMsg,param.wParam,(LPARAM)&hd);
				}
			}
			break;
		case HDM_ENABLECOLUMNHIDE:
			//�Զ�����Ϣ������/��ֹ������ͷ��wParamΪ����/��ֹ��־
			if((m_bCanHideColumn && !param.wParam) || (param.wParam && !m_bCanHideColumn))
			{
				HDITEM hdi;				
				hdi.mask = HDI_LPARAM;
				int i,nCnt = GetCount();
				LPLONG lpl;
				//״̬ת��
				if(m_bCanHideColumn)
				{
					//ԭ����ɾ��������ͷ�ڲ���Ϣ����
					for(i=1;i<nCnt;i++)
					{
						if(CallWindowProc(m_DefProc,m_hWnd,HDM_GETITEM,i,(LPARAM)&hdi))
						{
							if(hdi.lParam)
							{
								lpl = (LPLONG)hdi.lParam;
								hdi.lParam = *lpl;
								CallWindowProc(m_DefProc,m_hWnd,HDM_SETITEM,i,(LPARAM)&hdi);
								Del(lpl);
							}
						}
					}
				}
				else
				{
					//ԭ��ֹ������������ͷ�ڲ���Ϣ����
					for(i=1;i<nCnt;i++)
					{
						if(CallWindowProc(m_DefProc,m_hWnd,HDM_GETITEM,i,(LPARAM)&hdi))
						{
							lpl = (LPLONG)New(sizeof(LONG)*2);
							if(lpl)
							{
								*lpl = hdi.lParam;
								hdi.lParam = (LONG)lpl;
								lpl[1] = GetItemWidth(i) | (i << 16);
								CallWindowProc(m_DefProc,m_hWnd,HDM_SETITEM,i,(LPARAM)&hdi);
							}
						}
					}
				}
				m_bCanHideColumn = param.wParam;
			}
			return 0;
		case HDM_SHOWCOLUMN:
			//��ָ����ͷ��ʾ/���ر��,��һ�в�������
			if(param.wParam > 0)
			{
				if(!m_bCanHideColumn)
					SendMessage(m_hWnd,HDM_ENABLECOLUMNHIDE,TRUE,0);
				
				HDITEM hd;
				hd.mask = HDI_LPARAM;
				hd.lParam = 0;
				if(CallWindowProc(m_DefProc,m_hWnd,HDM_GETITEM,param.wParam,(LPARAM)&hd))
				{
					LPLONG lpl = (LPLONG)hd.lParam+1;
					if(lpl)
					{
						if(m_CoolMenu)
							m_CoolMenu->CheckItem(param.wParam,HDCOL_ISHIDDEN(*lpl),BY_POSITION);

						res = LOWORD(*lpl);
						//��ȡ���
						hd.mask = HDI_ORDER;
						hd.iOrder = param.wParam;
						CallWindowProc(m_DefProc,m_hWnd,HDM_GETITEM,param.wParam,(LPARAM)&hd);

						//��ʾlParamΪTRUE������¼ʱ����ΪTRUE������跴һ��
						if(param.lParam)
						{
							if(HDCOL_ISHIDDEN(*lpl))
							{
								//��ʾ,�ƶ���ԭλ��
								int pre=m_nHideColCnt,cnt = GetCount();
								LPINT lpi = (LPINT)_alloca(cnt*sizeof(int));
								GetOrderArray(cnt,lpi);
								hd.iOrder = HDCOL_EXTRACTORDER(*lpl);
								for(int i=m_nHideColCnt;i<cnt;i++)
								{
									if(hd.iOrder > lpi[i])
										pre = i;
									else
									{
										i = pre;
										break;
									}
								}
								m_nHideColCnt --;
								//hd.iOrder = HDCOL_EXTRACTORDER(*lpl) + m_nHideColCnt;
								*lpl = MAKELONG(res,hd.iOrder);
								hd.iOrder = min(i,cnt-1);								
							}
						}
						else
						{
							if(!HDCOL_ISHIDDEN(*lpl))
							{
								//����
								*lpl = res | HDCOL_HIDEMASK | (HDCOL_EXTRACTORDER(*lpl)<<16);//((max(0,hd.iOrder-m_nHideColCnt)<<16)&0x0FFF0000);
								//�Ƶ���ǰ��
								hd.iOrder = 0;
								m_nHideColCnt++;
							}
						}
						CallWindowProc(m_DefProc,m_hWnd,HDM_SETITEM,param.wParam,(LPARAM)&hd);
						return res;
					}
				}
			}
			return 0;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if(IsMouseOnHiddenCol(param.mousept.x,param.mousept.y))
				return 0;
			break;
		case WM_RBUTTONDOWN:
			if(m_bCanHideColumn && m_CoolMenu)
				m_CoolMenu->Popup(m_hWnd);
			break;
		case WM_COMMAND:
			if(m_bCanHideColumn && m_CoolMenu && param.shortval.high == 0)
			{
				//�˵���Ϣ
				NMHEADER nmhd;
				int nIndex = param.shortval.low;
				BOOL bShow = (m_CoolMenu->GetItemCheckState(nIndex) == MF_UNCHECKED);
				HWND hWndP;
				if(Parent()->GetWindowType() == ITIS_LISTVIEW)
				{
					((RingListView*)Parent())->ShowColumn(nIndex - HDMENU_BASEID,bShow);
					nmhd.hdr.code = LVN_SHOWCOLUMN;
					nmhd.hdr.hwndFrom = Parent()->Handle();
					nmhd.hdr.idFrom = ((RingListView*)Parent())->GetId();
					hWndP = Parent()->GetParent();
				}
				else
				{
					ShowColumn(nIndex - HDMENU_BASEID,bShow);
					nmhd.hdr.code = HDN_SHOWCOLUMN;
					nmhd.hdr.hwndFrom = m_hWnd;
					nmhd.hdr.idFrom = GetId();
					hWndP = GetParent();
				}
				nmhd.iItem = nIndex - HDMENU_BASEID;
				nmhd.iButton = bShow;
				nmhd.pitem = NULL;
				SendMessage(hWndP,WM_NOTIFY,nmhd.hdr.idFrom,(LPARAM)&nmhd);
				return 0;
			}
			break;
		case WM_MEASUREITEM:      	
			if(m_CoolMenu)
				m_CoolMenu->MeasureItem(param.lpmeasureitem);
         break;
      case WM_DRAWITEM:
			if(m_CoolMenu)
				m_CoolMenu->Draw(param.lpdrawitem);
         break;
		case WM_DESTROY:
			SendMessage(m_hWnd,HDM_ENABLECOLUMNHIDE,FALSE,0);
			break;
	}
	return RingControls::DefaultProc(param);
}

BOOL RingHeader::IsMouseOnHiddenCol(int x,int y)
{
	if(m_bCanHideColumn)
	{
		HDHITTESTINFO hdii;
		hdii.pt.x = x;
		hdii.pt.y = y;
		LRESULT res = CallWindowProc(m_DefProc,m_hWnd,HDM_HITTEST,0,(LPARAM)&hdii);
		if(res != -1 && MASK_MATCH(hdii.flags,HHT_ONDIVOPEN))
		{
			LPLONG lpl = GetHDInfo(res);
			if(lpl && HDCOL_ISHIDDEN(*lpl))
				return TRUE;
		}
	}
	return FALSE;
}

//��ʾ/������ͷ��iColΪȫ����ͷ��ʾ״̬�µ����,��ʾ/���ز���������Ч��
//����������ÿ�Ȳſ��ԣ�����������ΪListView�����п��ͬʱ���ñ�HEADER���п�
//��������ԭ��ʾ״̬���п�
int RingHeader::ShowColumn(int iCol,BOOL bShow)
{
	int nWidth = SendMessage(m_hWnd,HDM_SHOWCOLUMN,iCol,bShow);
	if(Parent()->GetWindowType() != ITIS_LISTVIEW)
	{
		if(bShow)
			SetItemWidth(iCol,nWidth);
		else
			SetItemWidth(iCol,0);
		InvalidateRect(m_hWnd,NULL,TRUE);
	}
	return nWidth;
}

//�����Ҽ��˵�ѡ����ʾ/������ͷ
RingCoolMenu* RingHeader::EnableColumnMenu(BOOL bEnable)
{
	if(bEnable)
	{
		EnableHideColumn();

		if(m_CoolMenu == NULL)
		{
			m_CoolMenu = new RingCoolMenu;
			if(m_CoolMenu)
			{
				if(m_CoolMenu->CreatePopup())
				{
					int nCnt = GetCount();
					if(nCnt > 0)
					{
						m_CoolMenu->Insert(HDMENU_BASEID,0,(LPSTR)GetText(0),BY_POSITION);
						m_CoolMenu->EnableItem(0,FALSE,BY_POSITION);
						m_CoolMenu->CheckItem(0,TRUE,BY_POSITION);

						for(int i=1;i<nCnt;i++)
						{
							m_CoolMenu->Insert(HDMENU_BASEID+i,i,(LPSTR)GetText(i),BY_POSITION);
							m_CoolMenu->CheckItem(i,!IsColumnHide(i),BY_POSITION);
						}
					}
				}
			}
		}
	}
	else
	{
		delete m_CoolMenu;
		m_CoolMenu = NULL;
	}
	return m_CoolMenu;
}
