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
//�汾��0.1
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����ringmenu.h
//
//���ļ�˵����ringmenu�˵��ඨ��
//
//��⹦�ܣ����ڲ˵��ඨ�壬ʵ������OICQ��OFFICE XP�Ĳ˵���
//
**********************************************************************/

#ifndef _INC_RINGMENU
#define _INC_RINGMENU
//#include "include\ringlib.h"

class RingMenu;
class RingCoolMenu;

//���¶���ΪRingCoolMenu����
typedef struct _tagRingMenuStyle
{
	HICON hIcon;				//ͼ��
   UINT uID;					//�˵���ID
	RingCoolMenu* pMenu;		//����ָ��
	LONG lData;					//�˵���󶨵����ݣ�RingDockSite�����ø�ֵ
}RINGMENUSTYLE,*LPRINGMENUSTYLE;

#define MENURECT_UP			BDR_RAISEDINNER
#define MENURECT_DOWN		BDR_SUNKENOUTER

#define MENUMARKER_EXT		24		//���ͼ������չͼ���߶ȣ�����չͼ����ΪMENU�߶�>���ͼ��ʱ
											//����ʣ�ಿ�ֶ��裬����չͼ������λ�ڱ��ͼ���������߶�Ϊ����ֵ
                                 //��Դ�ļ��еı��ͼ�����������������
#define MENUSEP_HEIGHT	  	6		//�˵��ָ�����ռ�˵���߶�
#define MENUCHECK_CHECKBOX	0		//Menu Checkͼ����CHECKBOX
#define MENUCHECK_RADIO		12    //Menu Checkͼ����RADIO

typedef struct _tagRingMenuGlobalStyle
{
   HBRUSH hbrBkg;          //������ˢ
   HBITMAP hbmHilight;		//ѡ�в˵�ͼ��
	int uHilX,uHilY;			//ѡ�в˵�ͼ������
   HBRUSH hbrHilight;      //ѡ�в˵�����ɫ
   HPEN hPen;					//�����ã�XPʽ����
   COLORREF crText;		   //ѡ�в˵���������ɫ
   COLORREF crTextDef;		//δѡ�в˵���ɫ��Ĭ����ɫ��
   UINT uSelectRect;			//ѡ�в˵���͹����£�MENURECT_UP �� MENURECT_DOWN
	//���±��ͼ����������OICQ�˵���ߵ�һ��ͼ������Ϣ
   HBITMAP hbmMarker;		//���ͼ��
   int uMarkerX,uMarkerY;	//���ͼ������
   int nMenuHeight;			//�˵��ܸ߶�
}MENUGLOBALSTYLE,*LPMENUGLOBALSTYLE;

#define BY_COMMAND 		MF_BYCOMMAND			//����˵�λ��ָ����ʽ
#define BY_POSITION 		MF_BYPOSITION

#define MAX_MENUTEXT		84							//�˵������ʾ����
#define MENU_PER_INSERT	16							//ÿ�����Ӳ˵�������(RingCoolMenu��)

#define MENUCOLOR_TRANSPARENT		0xffffffff		//͸��ɫ�����¶���˵��ã�
#define MENUCOLOR_DEF				0xfffffffe		//Ĭ��ɫ
#define MENUCOLOR_NOCHANGE			0xfffffffd		//��һ�ε���ɫ������SetColor()�в���ı����ɫ��

typedef void (*FUNC_COOLMENU_DRAWITEM)(RingCoolMenu*,LPDRAWITEMSTRUCT);
//�������

extern "C"
{
   void RingCoolMenu_DrawItem_def(RingCoolMenu* rm,LPDRAWITEMSTRUCT lps);
   void RingCoolMenu_DrawItem_XP(RingCoolMenu* rm,LPDRAWITEMSTRUCT lps);	//Office XP ���Ͳ˵��Ļ���
	void RingCoolMenu_DrawParent_def(RingCoolMenu* rm,LPDRAWITEMSTRUCT lps);
	void RingCoolMenu_DrawParent_XP(RingCoolMenu* rm,LPDRAWITEMSTRUCT lps);	//Office XP ���͸��˵��Ļ���
}

class RingMenu
{
public:
   RingMenu();
	RingMenu(LPCTSTR lpMenuname,HINSTANCE hInst=GetInstance());
   virtual ~RingMenu();

   BOOL Create();
	//���������˵�
   BOOL CreatePopup();
	//�����Ӳ˵�
	HMENU CreateSubMenu(){return CreatePopupMenu();}
   void Destroy(){DestroyMenu(m_hMenu);};

   virtual BOOL Load(LPCTSTR lpMenuname,HINSTANCE hInst=GetInstance());
   virtual BOOL LoadPopup(LPCTSTR lpMenuname=NULL,UINT nIndex=0,HINSTANCE hInst=GetInstance());
   virtual BOOL Attach(HMENU hMenu);
   virtual HMENU Detach();
   BOOL Popup(HWND hWnd,int x=-999,int y=-999);			//ʹ��Ĭ��ֵ������굱ǰλ�õ���
	//������������lpRectExcludeָ��������
	//bHorzAlign������Ҫ����λ�ñܿ�lpRectExcludeָ������ʱ���ȿ��Ǻ���λ��
	BOOL PopupEx(HWND hWnd,int x,int y,BOOL bHorzAlign,LPRECT lpRectExclude);

   HMENU GetMenu(){return m_hCurrMenu;};
   //HMENU GetPopup(){return m_hCurrMenu;};
   int GetItemIndex(UINT uIndex);
   int GetItemId(UINT uIndex);
	int GetItemCount(){return GetMenuItemCount(m_hCurrMenu);}
   HMENU GetItem(UINT uIndex);

   virtual BOOL Insert(HMENU hSubMenu,UINT nIndex,LPSTR szText,UINT flag = BY_POSITION);
	virtual BOOL Insert(UINT uid,UINT nIndex,LPSTR szText,UINT flag = BY_COMMAND);
   virtual BOOL InsertSep(UINT nIndex,UINT uPos=BY_COMMAND);	//����ָ���
   virtual BOOL Delete(UINT nIndex,UINT uPos=BY_COMMAND);

	UINT GetItemCheckState(UINT uIndex,UINT uFlag = BY_COMMAND);
   virtual DWORD CheckItem(UINT uIndex,BOOL bChecked = TRUE,UINT uFlag = BY_COMMAND);
   virtual BOOL CheckRadioItem(UINT idFirst,UINT idEnd,UINT uIndex,UINT uFlag = BY_POSITION);
   void SetCheckBmp(UINT uIndex,UINT uFlag,HBITMAP hbmCheck,HBITMAP hbmUncheck = NULL);
   void EnableItem(UINT uIndex,BOOL bEnable = TRUE,UINT uFlag = BY_COMMAND);
	BOOL SetDefaultItem(UINT uIndex,UINT uFlag = BY_COMMAND);
	BOOL SetItemText(UINT uIndex,LPCTSTR szText,UINT uFlag = BY_COMMAND);

   BOOL BindToWindow(HWND hWnd){return SetMenu(hWnd,m_hMenu);};
   void Update(HWND hWnd);

protected:
   HMENU m_hMenu,m_hCurrMenu;	//m_hCurrMenu:�����POPUPMENU����ֵ��m_hMenu��submenu,����=m_hMenu
	BOOL bNeedDestroy;			//�Ƿ���Ҫ����MENU
};

#define FUNC_DRAWMENU_DEF	RingCoolMenu_DrawItem_def
#define FUNC_DRAWMENU_XP	RingCoolMenu_DrawItem_XP
#define FUNC_DRAWPARENT_DEF	RingCoolMenu_DrawParent_def
#define FUNC_DRAWPARENT_XP	RingCoolMenu_DrawParent_XP

#define RINGMENU_INITED		1
#define RINGMENU_ISPARENT	2

class RingCoolMenu:public RingMenu
{
public:
	RingCoolMenu();
   RingCoolMenu(LPCTSTR lpMenuname,HINSTANCE hInst=GetInstance());
   RingCoolMenu(HMENU hMenu,BOOL bParent = TRUE);
   ~RingCoolMenu();

   BOOL Load(LPCTSTR lpMenuname,HINSTANCE hInst=GetInstance());
   BOOL Attach(HMENU hMenu);
   HMENU Detach();
   BOOL LoadPopup(LPCTSTR lpMenuname=NULL,UINT nIndex=0,HINSTANCE hInst=GetInstance());
   void Free();			//�ͷŷ���Ĳ˵�ͼ�󣨱�����ѡ��ͼ������Ȩ���ȣ�

   BOOL Draw(LPDRAWITEMSTRUCT lps);
   BOOL MeasureItem(LPMEASUREITEMSTRUCT pmis);
   BOOL Remap();			//���ò˵��������Ӳ˵�Ϊ�Ի���ʽ��
	BOOL ReScan();			//����ɨ��˵��������Ի�
   void Reset();        //���ò˵�Ϊ��ʼ״̬�����Ի棬�ͷŷ���Ĳ˵����ݣ�

   HICON SetItemIcon(UINT nIndex,HICON hicon);	//���ò˵���ͼ�꣨���ݲ˵����ID��
   HICON SetSubMenuIcon(UINT nIndex,HICON hicon);
	BOOL SetBkg(HBITMAP hbmBkg,BOOL bToAllSubMenu=TRUE);
	BOOL SetBkg(UINT idBmp,BOOL bToAllSubMenu=TRUE,HINSTANCE hInst=GetInstance());
	
	void SetBkgColor(COLORREF crBkg,COLORREF crHilight,BOOL bToAllSubMenu=TRUE);
	void SetTextColor(COLORREF crBkgText,COLORREF crSelText,BOOL bToAllSubMenu=TRUE);
   void SetColor(COLORREF crBkg,COLORREF crHilight,COLORREF crBkgText,COLORREF crSelText,BOOL bToAllSubMenu=TRUE);
	
   HBITMAP SetSelectBkg(HBITMAP hbmHilight,COLORREF crText = MENUCOLOR_NOCHANGE,BOOL bToAllSubMenu=TRUE);
   void SetSelectRectType(UINT uRectStyle,BOOL bToAllSubMenu=TRUE);
   HBITMAP SetMarker(HBITMAP hbmMarker);			//���ð�Ȩ��

   DWORD CheckItem(UINT uIndex,BOOL bChecked = TRUE,UINT uFlag = BY_COMMAND);
   BOOL CheckRadioItem(UINT idFirst,UINT idEnd,UINT uIndex,UINT uFlag = BY_POSITION);

   BOOL Insert(UINT uid,UINT nIndex,LPSTR szText,UINT flag = BY_COMMAND);
   BOOL InsertSep(UINT nIndex,UINT uPos=BY_COMMAND);	//����ָ���
   BOOL Delete(UINT nIndex,UINT uPos=BY_COMMAND);

	//bAll:�Ƿ����������Ӳ˵�
   void SetItemDrawFunc(FUNC_COOLMENU_DRAWITEM drawer,BOOL bAll=FALSE,FUNC_COOLMENU_DRAWITEM parent_drawer = NULL);	//���ò˵��Ի溯��
	FUNC_COOLMENU_DRAWITEM GetItemDrawFunc(){return m_DrawItemFunc;}
   
	RingCoolMenu* SubMenu(int i);                         //�õ��Ӳ˵�
	inline int GetSubMenuCount(){return m_nSubMenuCnt;}

	//���ư�Ȩ��
	void DrawMark(HDC hDC,HDC hMemDC,LPRECT lprc);
	//���Ʒָ���
	void DrawSeparator(HDC hDC,LPRECT lprc);
	//���Ʊ���ͼ��
	void DrawBkg(HDC hDC,LPRECT lprc);
	//����ͼ�꼰ѡ�б�־
	int DrawIconCheck(HDC hDC,HDC hMemDC,HICON hIcon,int x,int y,BOOL bSelect,
								BOOL bCheck,HPEN hPen,UINT idMenu);
	//���Ƹ�����
	void DrawHilight(HDC hDC,HDC hMemDC,HPEN hPen,LPRECT lprc);
	//��������
	void DrawText(HDC hDC,HDC hMemDC,UINT idMenu,LPRECT lprc,BOOL bGray,BOOL bSelect);

	void SetXPStyle();

private:
   void Init();                                          //���ʼ��
   void InitSubMenu();                                   //��ʼ���Ӳ˵�
   void ResetItemData();                                 //���ò˵������ݣ��������Ӳ˵���˵����ݻ����������������
	BOOL SetCheckItem(UINT uIndex,int nType = MENUCHECK_CHECKBOX);

public:
   static HBITMAP m_hbmCheck,m_hbmRadio;                 //Ĭ�ϲ˵���ѡ��ͼ��
	static int m_cyFont;
   int m_ItemIconPos;                                    //�˵���ͼ��λ�ã���ѡ��ͼ��������ʾ��������ʾ��
   MENUGLOBALSTYLE m_Style;
	int m_IconCnt;

private:
   static HFONT m_hFont;
   LPRINGMENUSTYLE m_lprms;
   RINGMENUSTYLE m_SepData;			//�ָ������ݣ����õ�ItemData)
   FUNC_COOLMENU_DRAWITEM m_DrawItemFunc;
   static int m_id;
   int m_nSubMenuCnt;
   RingCoolMenu** m_SubMenu;
   //RingCoolMenu* m_Popup;
   int m_nInitFlag;                 //�Ƿ��ѳ�ʼ��
   int m_nItemCnt;						//�˵�����(��ʵ������������DATA��)
};

#endif

