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
//							RingSDK��ý����� ringdib.lib
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
//ԭ�ļ�����dragrect.cpp
//
//˵�������ƶ�/�϶�����RingDragRect����ʵ�֡�
//
//		RingDragRect����PhotoShop��ѡ����Σ���ͼ���ϻ���һ��
//		���Σ��Զ�����ʾ�����ɸ�ͼ���ƶ������š���Ϊͼ���ڴ���
//		���Թ��������Ը�������¼�����������꣬һ����ͼ����
//		�ľ�������m_rcDIB��������ֻҪ������Ч���ǲ����ġ�һ
//		��������ʾ�����ڵ�����m_rcDC��ֻҪͼ���й��������ţ�
//		������ͱ��������ͼ���ϵ��������¼��㣬Ȼ����ʾ��
//		���ڡ���Ϊ�Ƕ�����ʾ���Σ�������һ���̸߳���þ���
//		�Ļ��ơ�
//		��Ϊ���ε���ʾ�����ڲ�����أ����������µĴ�����Ϣ
//		�б��������Ӧ�ĳ�Ա������
//			WM_LBUTTONDOWN		OnLButtonDown(LOWORD(lParam),HIWORD(lParam));
//			WM_MOUSEMOVE		OnMouseMove(LOWORD(lParam),HIWORD(lParam));
//			WM_LBUTTONUP		OnLButtonUp(LOWORD(lParam),HIWORD(lParam));
//			WM_SIZE				OnParentSize(LOWORD(lParam),HIWORD(lParam));
//			WM_PAINT				OnPaint(ͼ�����ű��ʣ�˵����dib_global.cpp��);
//			�ر�˵����ͼ�����Ų���ʱ�������SetRate���ú�����Ȼ
//			����Ϊpublic����ֻҪ��WM_PAINT�е���OnPaint���Զ�
//			���á�һ��������á�
//
**********************************************************************/
#define MAKE_SELF_LIB
#include "ringdib.h"

RingDragRect::RingDragRect()
{
	Init();	
}

RingDragRect::~RingDragRect()
{
	DeleteObject(m_brush);
	if(hevent)
	{
		bQuitDraw = TRUE;
		SetEvent(hevent);		
	}
	if(handleDraw)
		CloseHandle(handleDraw);
	CloseHandle(hevent);
}

void RingDragRect::Init()
{
	memset(&m_rcDIB,0,sizeof(RECT));
	memset(&m_rcDC,0,sizeof(RECT));
	bInDrag = FALSE;
	bInMove = FALSE;
	bActived = FALSE;
	bInFirstAct = FALSE;

	m_nInSide = -1;
	scrx = scry = -9999;
	pt.x = -1;
	pt.y = -1;
	m_hWnd = NULL;
	m_dib = NULL;
	handleDraw = NULL;

	WORD b[16] = {0xf0f0,0xf0f0,0xf0f0,0xf0f0,
					 0x0f0f,0x0f0f,0x0f0f,0x0f0f};
	BITMAP bm;
	bm.bmType = 0;
	bm.bmWidth = 8;
	bm.bmHeight = 8;
	bm.bmWidthBytes = 2;
	bm.bmPlanes = 1;
	bm.bmBitsPixel = 1;
	bm.bmBits = b;
	HBITMAP hbm = CreateBitmapIndirect(&bm);
	m_brush = CreatePatternBrush(hbm);
	DeleteObject(hbm);

	bQuitDraw = FALSE;
	hevent = CreateEvent(NULL,TRUE,TRUE,NULL);
}

int RingDragRect::left(int isDC/* = ON_DC*/)
{
	if(isDC == ON_DC)
		return m_rcDC.left;
	else
		return m_rcDIB.left;
}

int RingDragRect::right(int isDC/* = ON_DC*/)
{
	if(isDC == ON_DC)
		return m_rcDC.right;
	else
		return m_rcDIB.right;
}

int RingDragRect::top(int isDC/* = ON_DC*/)
{
	if(isDC == ON_DC)
		return m_rcDC.top;
	else
		return m_rcDIB.top;
}

int RingDragRect::bottom(int isDC/* = ON_DC*/)
{
	if(isDC == ON_DC)
		return m_rcDC.bottom;
	else
		return m_rcDIB.bottom;
}

BOOL RingDragRect::GetRect(LPRECT lprc,int isDC/* = ON_DC*/)
{
	if(lprc)
		if(isDC == ON_DC)
			return CopyRect(lprc,&m_rcDC);
		else
			return CopyRect(lprc,&m_rcDIB);
	return FALSE;
}

void RingDragRect::OnLButtonDown(int x,int y)
{
	pt.x = x;
	pt.y = y;

	ResetEvent(hevent);
	RECT rc;
	rc.left = m_rcDC.left;
	rc.top = m_rcDC.top;
	rc.right = m_rcDC.right + 1;
	rc.bottom = m_rcDC.bottom + 1;

	bInFirstAct = TRUE;
	InvalidateRect(m_hWnd,&rc,TRUE);	
	
	if(bActived && m_dib)
		if(m_nInSide != -1)
		{
			if(m_nInSide == DRC_INSIDE)
				StartMove(x,y);
			else
			{
				SetCapture(m_hWnd);
/*
				RECT rect;
				m_dib->GetDestRect(&rect);

				scrx = GetScrollPos(m_hWnd,SB_HORZ);
				scry = GetScrollPos(m_hWnd,SB_VERT);

				m_currX = m_dib->GetSrcX();
				m_currY = m_dib->GetSrcY();
*/
				bActived = TRUE;
				bInDrag = TRUE;
				//bInFirstAct = TRUE;
			}
			return;
		}
		
	StartDrag(x,y);
}

void RingDragRect::StartDrag(int x,int y)
{	
	if(m_dib)
	{
		//SetCursor(LoadCursor(NULL,IDC_CROSS));
		//if(bActived)// && bInFirstAct)
		//	Draw();

		SetCapture(m_hWnd);

		RECT rect;
		m_dib->GetDestRect(&rect);

		scrx = GetScrollPos(m_hWnd,SB_HORZ);
		scry = GetScrollPos(m_hWnd,SB_VERT);

		//���߽�
		x = max(x,rect.left);
		x = min(x,rect.right);
		y = max(y,rect.top);
		y = min(y,rect.bottom);
		
		pt.x = x;
		pt.y = y;

		m_currX = m_dib->GetSrcX();
		m_currY = m_dib->GetSrcY();

		m_rcDC.left = m_rcDC.right = x;
		m_rcDC.top = m_rcDC.bottom = y;
			

		//���������ͼ������
		Get_OnDIBRect(m_dib,m_rate,&m_rcDC,&m_rcDIB);
		if(m_rate >=0)
			Get_OnDCRect(m_dib,m_rate,&m_rcDIB,&m_rcDC);
		//setDIBRect((LPPOINT)&rc,&pt);
	
		ptOrg.x = m_rcDC.left;
		ptOrg.y = m_rcDC.top;

		bActived = TRUE;
		bInDrag = TRUE;

		Draw();
	}
}

void RingDragRect::Drag(int x,int y)
{	
	bInFirstAct = FALSE;
	if(m_dib && bActived && bInDrag)
	{
		Draw();

		RECT rtd,rts;
		m_dib->GetDestRect(&rtd);
		m_dib->GetSrcRect(&rts);
				
		pt.x = x;
		pt.y = y;

		x = max(x,rtd.left);
		x = min(x,rtd.right);
		y = max(y,rtd.top);
		y = min(y,rtd.bottom);

		if(PtInRect(&rtd,pt))
		{
			pt.x = x;
			pt.y = y;

			setDCRect(x,y);
		}
		else
		{
			//SetCursor(LoadCursor(NULL,IDC_CROSS));

			int scx=0,scy = 0;
			
			if(pt.x < rtd.left && rts.left > 0)
				scx = -999;
			else if(pt.x > rtd.right && rts.right < m_dib->Width())
				scx = 999;

			if(pt.y < rtd.top && rts.top > 0)
				scy = -999;
			else if(pt.y > rtd.bottom && rts.bottom < m_dib->Height())
				scy = 999;
			
			if(scx != 0 || scy != 0)
			{				
				ScrollParent(scx,scy);
				int sx = scrx;
				int sy = scry;

				scrx = GetScrollPos(m_hWnd,SB_HORZ);
				scry = GetScrollPos(m_hWnd,SB_VERT);

				sx -= scrx;// - sx;
				sy -= scry;// - sy;
				OffsetRect(&m_rcDC,sx,sy);
				ptOrg.x += sx;
				ptOrg.y += sy;
			}
			pt.x = x;
			pt.y = y;
			setDCRect(x,y);
		}
		Get_OnDIBRect(m_dib,m_rate,&m_rcDC,&m_rcDIB);
		if(m_rate >=0)
			Get_OnDCRect(m_dib,m_rate,&m_rcDIB,&m_rcDC);
		Draw();
	}
}

void RingDragRect::StopDrag(int x,int y)
{
	ReleaseCapture();
	
	RECT rtd;
	m_dib->GetDestRect(&rtd);
	
	x = max(x,rtd.left);
	x = min(x,rtd.right);
	y = max(y,rtd.top);
	y = min(y,rtd.bottom);

	pt.x = x;
	pt.y = y;

	setDCRect(x,y);
	Get_OnDIBRect(m_dib,m_rate,&m_rcDC,&m_rcDIB);
	if(m_rate >=0)
		Get_OnDCRect(m_dib,m_rate,&m_rcDIB,&m_rcDC);
	
	bInDrag = FALSE;

	if(m_rcDC.left == m_rcDC.right || m_rcDC.top == m_rcDC.bottom)
	{
		bActived = FALSE;
		ResetEvent(hevent);
	}
	else
		SetEvent(hevent);
}

int RingDragRect::OnMouseMove(int x,int y)
{
	if(bActived)
	{
		if(bInDrag)
			Drag(x,y);
		else if(bInMove)
			Move(x,y);
		else
		{
			RECT rect;
			CopyRect(&rect,&m_rcDC);
			InflateRect(&rect,3,3);

			pt.x = x;
			pt.y = y;

			if(PtInRect(&rect,pt))
			{
				InflateRect(&rect,-6,-6);
				if(!PtInRect(&rect,pt))
				{
					if(pt.x < rect.left)
					{
						if(pt.y < rect.top)
						{
							ptOrg.y = m_rcDC.bottom;
							m_nInSide = DRC_LU;
						}
						else if(pt.y > rect.bottom)
						{
							ptOrg.y = m_rcDC.top;
							m_nInSide = DRC_LD;
						}
						else
						{
							m_nInSide = -1;
							return -1;
						}
						ptOrg.x = m_rcDC.right;
					}
					else if(pt.x > rect.right)
					{
						if(pt.y < rect.top)
						{
							ptOrg.y = m_rcDC.bottom;
							m_nInSide = DRC_RU;
						}
						else if(pt.y > rect.bottom)
						{
							ptOrg.y = m_rcDC.top;
							m_nInSide = DRC_RD;
						}
						else
						{
							m_nInSide = -1;
							return -1;
						}
						ptOrg.x = m_rcDC.left;
					}					
				}
				else
					m_nInSide = DRC_INSIDE;
				return m_nInSide;
			}
			else
				m_nInSide = -1;
		}
	}	
	return m_nInSide;
}

void RingDragRect::StartMove(int x,int y)
{
	SetCapture(m_hWnd);
	pt.x = x;
	pt.y = y;
	bInMove = TRUE;	
	//bInFirstAct = TRUE;
}

void RingDragRect::Move(int x,int y)
{
	bInFirstAct = FALSE;
	if(x > m_width || x < 0)
		x = pt.x;
	if(y > m_height || y < 0)
		y = pt.y;

	Draw();
	
	if(m_rate >=0)
	{
		int ox,oy;
		ox = (x - pt.x)/(m_rate + 1);
		oy = (y - pt.y)/(m_rate + 1);
		if(ox != 0)
			pt.x = x;
		if(oy != 0)
			pt.y = y;

		if(ox != 0 || oy != 0)
		{
			OffsetRect(&m_rcDIB,ox,oy);
			Get_OnDCRect(m_dib,m_rate,&m_rcDIB,&m_rcDC);
		}		
	}
	else
	{
		OffsetRect(&m_rcDC,x - pt.x,y - pt.y);	
		pt.x = x;
		pt.y = y;
	}

	Draw();
}

void RingDragRect::StopMove(int x,int y)
{
	ReleaseCapture();	
	Get_OnDIBRect(m_dib,m_rate,&m_rcDC,&m_rcDIB);
	if(m_rate >=0)
		Get_OnDCRect(m_dib,m_rate,&m_rcDIB,&m_rcDC);
	bInMove = FALSE;
	bInFirstAct = FALSE;
}

void RingDragRect::OnLButtonUp(int x,int y)
{
	m_nInSide = -1;
	if(bActived)
	{
		if(bInDrag)
			StopDrag(x,y);
		else if(bInMove)
			StopMove(x,y);

		if(handleDraw == NULL)
			handleDraw = NewThread((THREADPROC)DragRectDrawProc,this);

		Draw();
		SetEvent(hevent);
	}
}

void RingDragRect::Draw()
{	
	if(bActived)
	{
		if(m_rcDC.left == m_rcDC.right && m_rcDC.top == m_rcDC.bottom)
			return;

		hDC = GetDC(m_hWnd);
		DrawFocusRect(hDC,&m_rcDC);
		ReleaseDC(m_hWnd,hDC);		
	}
}

//���������
void RingDragRect::OnPaint(int rate)
{
	if(m_dib && bActived)		
	{
		if(!bInDrag)
		{
			if(rate != m_rate)
			{
				SetRate(rate);
				scrx = GetScrollPos(m_hWnd,SB_HORZ);
				scry = GetScrollPos(m_hWnd,SB_VERT);
			}
			else
			{
				int sx = scrx;
				int sy = scry;
		
				scrx = GetScrollPos(m_hWnd,SB_HORZ);
				scry = GetScrollPos(m_hWnd,SB_VERT);
				if(scrx == 0 || scry == 0)
					SetRate(rate);
				else
					OffsetRect(&m_rcDC,sx - scrx,sy - scry);
			}		
		}
		if(bInFirstAct)
			Draw();
	}
}

//�������Ρ�������EnableDraw(FALSE)�����Ч	
void RingDragRect::Erase()
{
	RECT rc;
	rc.left = m_rcDC.left;
	rc.top = m_rcDC.top;
	rc.right = m_rcDC.right + 1;
	rc.bottom = m_rcDC.bottom + 1;

	InvalidateRect(m_hWnd,&rc,TRUE);	
}

void RingDragRect::Disable()
{
	ResetEvent(hevent);
	Erase();
	bActived = FALSE;
}

void RingDragRect::setDCRect(int x,int y)
{
//	BOOL bSetAll = FALSE;

	if(ptOrg.x < x)
	{
		m_rcDC.left = ptOrg.x;
		m_rcDC.right = x;
	}
	else
	{
		m_rcDC.right = ptOrg.x;
		m_rcDC.left = x;
	}
	if(ptOrg.y < y)
	{
		m_rcDC.top = ptOrg.y;
		m_rcDC.bottom = y;
	}
	else
	{
		m_rcDC.bottom = ptOrg.y;
		m_rcDC.top = y;
	}
	//if(bSetAll)
	//	setDIBRect((LPPOINT)&rc,(LPPOINT)&m_rcDC);

	//setDIBRect((LPPOINT)&rc+1,(LPPOINT)&m_rcDC+1);
}

void RingDragRect::setDIBRect(LPPOINT lpd,LPPOINT lps)
{
	if(m_dib)
	{
		RECT rect;
		int x,y;

		m_dib->GetDestRect(&rect);
		x = lps->x;
		y = lps->y;

		if(m_rate >= 0)
		{
			x = (x - rect.left)/(m_rate+1);
			y = (y - rect.top)/(m_rate+1);
			
			lpd->x = m_dib->GetSrcX() + x;
			lpd->y = m_dib->GetSrcY() + y;			
		}
		else
		{
			if(m_rate == -1)
			{
				lpd->x = m_dib->GetSrcX() + 
					(x - rect.left)*4/3;
				lpd->x = m_dib->GetSrcY() + 
					(y - rect.top)*4/3;
			}
			else
			{
				lpd->x = m_dib->GetSrcX() + 
					(x - rect.left)*(-m_rate);
				lpd->y = m_dib->GetSrcY() + 
					(y - rect.top)*(-m_rate);
			}
		}
	}
}

void RingDragRect::ScrollParent(int dx,int dy)
{
	if(dx == 999)
		SendMessage(m_hWnd,WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,0),0);
	else if(dx == -999)
		SendMessage(m_hWnd,WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,0),0);		
	else
	{
		dx += GetScrollPos(m_hWnd,SB_HORZ);
		SendMessage(m_hWnd,WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(short)max(0,dx)),0);
	}

	if(dy == 999)
		SendMessage(m_hWnd,WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,0),0);		
	else if(dx == -999)
		SendMessage(m_hWnd,WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,0),0);
	else
	{
		dy += GetScrollPos(m_hWnd,SB_VERT);
		SendMessage(m_hWnd,WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(short)max(0,dy)),0);
	}
}

void RingDragRect::EnableDraw(BOOL bDraw/* = TRUE*/)
{
	if(hevent)
		if(bDraw)
			SetEvent(hevent);
		else
			ResetEvent(hevent);
}

void RingDragRect::SetRate(int rate)
{
	m_rate = rate;
	Get_OnDCRect(m_dib,rate,&m_rcDIB,&m_rcDC);	
}

DWORD DragRectDrawProc(RingDragRect* obj)
{
	if(obj)
	{
		int x = 0,y = 0;
		HBRUSH oldbrush;

		while(WAIT_OBJECT_0	== WaitForSingleObject(obj->hevent,INFINITE))
		{
			if(!obj->bQuitDraw)
			{
				HDC hDC = GetDC(obj->m_hWnd);
				oldbrush = (HBRUSH)SelectObject(hDC, obj->m_brush);

				SetBrushOrgEx(hDC,x,0,NULL);
				PatBlt(hDC,obj->m_rcDC.left,obj->m_rcDC.top,obj->m_rcDC.right - obj->m_rcDC.left,1,PATCOPY);
				PatBlt(hDC,obj->m_rcDC.left,obj->m_rcDC.bottom,obj->m_rcDC.right - obj->m_rcDC.left,1,PATCOPY);

				SetBrushOrgEx(hDC,0,y,NULL);
				PatBlt(hDC,obj->m_rcDC.right,obj->m_rcDC.top,1,obj->m_rcDC.bottom - obj->m_rcDC.top,PATCOPY);
				PatBlt(hDC,obj->m_rcDC.left,obj->m_rcDC.top,1,obj->m_rcDC.bottom - obj->m_rcDC.top,PATCOPY);

				x ++;
				y ++;
				
				if(x > 8)
				{
					x -= 8;
					y -= 8;
				}
				SelectObject(hDC,oldbrush);
				ReleaseDC(obj->m_hWnd,hDC);
			}
			else
				break;
			Sleep(250);
		}
	}
	return 100;
}
