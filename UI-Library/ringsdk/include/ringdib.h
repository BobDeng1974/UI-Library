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
//ԭ�ļ�����ringdib.h
//
//˵����RingDIB.lib��ͷ�ļ���ʹ�ø�������������ļ�
//
**********************************************************************/
#ifndef _INC_RINGDIB
#define _INC_RINGDIB
//#pragma warn -pck
#include "ringlib.h"

#include <mmsystem.h>
#include <math.h>
#include <vfw.h>

#define RC_BMP				0x730001	//��Դ���ͣ�BMP
#define RC_DATA         0x730002	//��Դ���ͣ�����

//#define DIB_BMP		6600000
//#define DIB_PCX		6600001
//#define DIB_JPG		6600002
//#define DIB_GIF		6600003
//#define DIB_MIX		6600004

#define PCX		"pcx"
#define GIF		"gif"
#define JPG		"jpg"
#define PNG		"png"

#define DIB_LOADDIB			0
#define DIB_QUERYSUPPORT	1
#define DIB_GETFORMAT		2

#define FAIL_SUCCESS	8800001
#define FAIL_OTHER	8800002

typedef struct tagTRUECOLOR{
	BYTE r;
   BYTE g;
   BYTE b;
   BYTE alpha;
}RGBCOLOR,*LPRGBCOLOR;

#pragma pack(push,1)
typedef struct tagAniGifImage{		//����GIFͼ����Ϣ
	tagAniGifImage* prev;            //��һ��ͼ����Ϣ
   COLORREF* curr;                  //��ǰͼ������
   tagAniGifImage* next;            //��һ��ͼ����Ϣ
   COLORREF keycolor;					//͸��ɫ����
   int x,y,width,height;
   WORD delay;								//ͼ����ʱ
   BYTE docase;							//ͼ������
}ANIGIFPIC,*LPANIGIFPIC;
#pragma pack(pop)

typedef void (*ANIFILTER_FUNC)(void);

typedef struct tagLOCALGIFINFO{
	RingThread* AniThread;				//�����߳̾��
   COLORREF bgC;					 		//����ɫ
   BOOL bNeedBak;							//��������Ҫ��¼����ͼ��(docase = 0 or 1)
   BOOL bActive;							//������
   BOOL bDibReady;						//ͼ���Ƿ�׼���ÿɻ��Ƶ�����
   ANIFILTER_FUNC funcFilter;			//ͼ��׼���ú�����Ҫ��������øú�������������������ͼ��
}LOCALGIFINFO,*LPLOCALGIFINFO;

class RingDIB;

extern "C" {
//RingDIB���ͼ��������
BOOL LoadGIF(LPCTSTR szFilename,RingDIB* lprd,int iFlag=DIB_LOADDIB,LPSTR lpszFormat=NULL);
BOOL LoadPCX(LPCTSTR szFilename,RingDIB* lprd,int iFlag=DIB_LOADDIB,LPSTR lpszFormat=NULL);
BOOL LoadJPG(LPCTSTR szFilename,RingDIB* rdib,int iFlag=DIB_LOADDIB,LPSTR lpszFormat=NULL);
BOOL LoadPNG(LPCTSTR szFilename,RingDIB* rdib,int iFlag=DIB_LOADDIB,LPSTR lpszFormat=NULL);

BOOL SaveJPG(LPCTSTR szFilename,RingDIB* rdib,BOOL bRGB=TRUE,int quality=50);
BOOL SaveBMP(LPCTSTR szFilename,RingDIB* rdib);

//����GIF����ͼ����ƺú�Ĭ�ϵĴ��������ú���������ͼ��
void NoneFilter();

//��ͼ���ϻ��ƾ����á���DragRect��Ҫ���ã�
//������ͼ���ϵľ���λ������Ƶ���Ļ��λ��
BOOL Get_OnDCRect(RingDIB* dib,int rate,CONST RECT* lpsrc,LPRECT lpdst);
//���ݻ��Ƶ���Ļ��λ������ͼ���ϵľ���λ��
BOOL Get_OnDIBRect(RingDIB* dib,int rate,CONST RECT* lpsrc,LPRECT lpdst);
BOOL Get_OnDCPt(RingDIB* dib,int rate,CONST POINT* lpsrc,LPPOINT lpdst);
BOOL Get_OnDIBPt(RingDIB* dib,int rate,CONST POINT* lpsrc,LPPOINT lpdst);

//��ͼ�����ű��ʣ�ֻ����������
int GetDIBShowRate(RingDIB* dib);
}

#define MAX_FILTER 	10			//���֧���˾������

//RingDIB�˾����������
class dibFilterALPHA;
class dibFilterEFFECT;

//RingDIB�˾������Ļ���
typedef struct DibFilter
{
	DibFilter():m_rdib(NULL){}
	virtual ~DibFilter()=0;		//���麯���������������rdib.cpp
protected:
	RingDIB* m_rdib;
}DIBFILTER,*LPDIBFILTER;

typedef struct _tagDibFilterInfo
{
	LPVOID m_filter[MAX_FILTER];
   //�µ�RingDIB�˾���������������
	//��������dibFilterXXX* m_lpXXX
	//XXXΪ�������������ALPHA,EFFECT
   dibFilterALPHA* m_lpALPHA;
	dibFilterEFFECT* m_lpEFFECT;
}DIBFILTERINFO,*LPDIBFILTERINFO;

//��ʱ������
class RingTimer
{
public:
	RingTimer(){}
	virtual ~RingTimer()=0{}
	
   void Reset();		//���¿�ʼ��ʱ
   void Count();     //��ʱ
   int PassedTime();	//�Կ�ʼ��ʱ�����øú���ʱ������ʱ�䣨���룩
};

//RingTimer::~RingTimer(){}

//���뼶��ʱ��
class RingMSTimer:public RingTimer
{
public:
	RingMSTimer():m_initTime(timeGetTime()),m_passedTime(0),m_currTime(0){}
	~RingMSTimer(){}

   void Reset();		//���¿�ʼ��ʱ
   void Count();     //��ʱ
   int PassedTime();	//�Կ�ʼ��ʱ�����øú���ʱ������ʱ�䣨���룩

private:
	int m_initTime,m_passedTime,m_currTime;
};

#pragma warning(disable:4035)
//���뼶��ʱ��,ժ�ԡ�Windowsͼ�α�̡�
class RingNSTimer:public RingTimer
{
public:
	RingNSTimer()
	{
		m_cpu = 1;
		m_overHead = 0;
		Reset();
		m_overHead = PassedTime();
		Reset();
		Sleep(100);
		m_cpu = PassedTime()/10000;
	}
	~RingNSTimer(){}

	inline __int64 GetCycleCount()
	{
		_asm _emit 0x0F
		_asm _emit 0x31
	}

	void Reset(){m_startCycle = GetCycleCount();}
	void Count(){}
	//����΢��,1Сʱ�ڱ���Reset�����򷵻�ֵ�ض�
	int PassedTime(){return (int)((GetCycleCount() - m_startCycle - m_overHead)/m_cpu)/100;}

private:
	unsigned __int64 m_startCycle,m_overHead,m_cpu;
};
#pragma warning(default:4035)

/*
class RingDraw
{
public:
	static HDRAWDIB Open();
	static void Close ();

   static BOOL Draw(RingDIB* rdib,UINT wFlags=DDF_HALFTONE);
	static BOOL Draw(HDC hDC,RingDIB* rdib,UINT wFlags=DDF_HALFTONE);
	static BOOL Paint(RingDIB* rdib,PAINTSTRUCT* lps,UINT wFlags=DDF_HALFTONE);
   static BOOL DrawTo(RingDIB* src,RingDIB* dest,BOOL bTrans=TRUE);
   static BOOL StretchTo(RingDIB* src,RingDIB* dest,BOOL bAll);
   static BOOL CopyToBack(RingDIB* rdib);

   static BOOL CreateBack(HWND hWnd,int x,int y);     //�����ڴ�ͼ��
	static void FreeBack();                			//�ͷ��ڴ�ͼ��

public:
	static HDRAWDIB m_hDrawDib;
   static BITMAPINFO m_bmif;
   static int m_count;
   static BOOL m_bWithBack;
};
*/
typedef BOOL (*DECODER_FUNC)(LPCTSTR,RingDIB*,int,LPSTR);
typedef void (*FILTER_FUNC)(RingDIB*);

//ͼ����ת/��ת��־
//���ɣ�pos = DIB_LFPOS_UL;
//��ת��pos = (BYTE)(pos+DIB_LFPOS_DL);
//��ת��pos = (BYTE)(pos-DIB_LFPOS_DL);
//����ת��pos ^= DIB_LFPOS_UR;
//����ת��pos ^= DIB_LFPOS_DL;
//���մ˹�����ͼ����ת/��ת���ٴΣ�����ԭʼͼ�����
//RotateTo��FlipTo����Ȼ����֪��ͼ���Ƿ��ߵ�������
//��pos��Ϊ������ͼ����ת/��ת���ʼ����ȷ��
#define DIB_LFPOS_UL	0x00			//ԭͼ�����Ͻ���ת�������Ͻ�
#define DIB_LFPOS_DL	0x40        //ԭͼ�����Ͻ���ת�������½�
#define DIB_LFPOS_DR	0x80        //ԭͼ�����Ͻ���ת�������½�
#define DIB_LFPOS_UR	0xc0        //ԭͼ�����Ͻ���ת�������Ͻ�

class RingDIB
{
public:
	RingDIB();
	RingDIB(HWND hWnd);
	~RingDIB();

	//����������ͼ�����
   BOOL Create(int width,int height);
   BOOL Load(LPCTSTR szPicFilename);
   BOOL Load(UINT uBmpId);
	BOOL Import(HBITMAP hbm);
	BOOL Import(LPBYTE lpdata,int width,int height);
   BOOL Select(BOOL bAllFile=FALSE,HWND hWnd=GetMainWnd(),LPCTSTR dlgEx=NULL,LPOFNHOOKPROC hookproc=NULL);
	BOOL IsImage(LPSTR szPicFilename);

	//ͼ�����Ե���
	COLORREF* Data(){return m_Bits;}
   int Width(){return m_width;}
   int Height(){return m_height;}
	int GetSrcX(){return m_rcSrc.left;}
	int GetSrcY(){return m_rcSrc.top;}
	int GetDestX(){return m_rcDest.left;}
	int GetDestY(){return m_rcDest.top;}
	BOOL GetSrcRect(LPRECT lpdst){return lpdst?CopyRect(lpdst,&m_rcSrc):FALSE;}
	BOOL GetDestRect(LPRECT lpdst){return lpdst?CopyRect(lpdst,&m_rcDest):FALSE;}
	HWND GetDestWindow(){return m_hWnd;}
	RingDIB* GetDestDIB(){return m_target;}
	RingDIB* GetCanvas(){return m_dibCanvas;}
	LPANIGIFPIC GetAniGifPic(){return m_aniGifPic;}
   LPLOCALGIFINFO GetLocalGifInfo();
	//����ͼ���ʽ����"gif","jpeg"�ȵ�
	LPSTR GetFormat(LPSTR lpszFormat);
	
	//����
	LPANIGIFPIC AddAniGifPic(LPANIGIFPIC prev,BYTE docase,WORD delay,BOOL bTrans);
	void SetTarget(HWND hWnd){m_hWnd = hWnd;}
	void SetTarget(RingDIB* dib){m_target = dib;}
   static BOOL RegFormat(LPCSTR ext,DECODER_FUNC ext_func);
   
   void SetSrc(int x,int y,int width=0,int height=0);
   void SetDest(int x,int y,int width=0,int height=0);
	
   void SetColorKey(COLORREF crkey){m_crKey=crkey;}
	COLORREF GetColorKey(){return m_crKey;}

	//DrawDibDraw���Ʋ���
   BOOL Draw(BOOL bStretch=TRUE,UINT wFlags=DDF_HALFTONE);
	BOOL Draw(HDC hDC,BOOL bStretch=TRUE,UINT wFlags=DDF_HALFTONE);
   BOOL Draw(int dx,int dy,int sx,int sy,int dw=0,int dh=0,int sw=0,int sh=0,UINT wFlags=DDF_HALFTONE);
	BOOL Draw(HDC hDC,int dx,int dy,int sx,int sy,int dw=0,int dh=0,int sw=0,int sh=0,UINT wFlags=DDF_HALFTONE);
   //Paint����ҪHDC�������иò����ɵ���Draw(hDC,...)
	BOOL Paint(BOOL bStretch=TRUE,UINT wFlags=DDF_HALFTONE);
   BOOL Paint(int dx,int dy,int sx,int sy,int dw=0,int dh=0,int sw=0,int sh=0,UINT wFlags=DDF_HALFTONE);
	
	//ֱ��ͼ�����ݲ���
	BOOL DrawTo(RingDIB* Dest,int x=0,int y=0,BOOL bTrans=TRUE);
   BOOL DrawTo(RingDIB* Dest,int dx,int dy,int sx,int sy,int w=0,int h=0,BOOL bTrans=TRUE);
	//��ͬ��Сͼ����
	BOOL CopyTo(RingDIB* Dest);
	//����������ͼ�󣬷��ض�������ɵ���������
	RingDIB* MakeCopy(RingDIB* Dest=NULL);
   //�������
	BOOL StretchTo(RingDIB* Dest,int dx=0,int dy=0,int dw=0,int dh=0,int sx=0,int sy=0,int sw=0,int sh=0);
	BOOL Fill(COLORREF crColor);
   BOOL Fill(BYTE r,BYTE g,BYTE b);
	BOOL FillRect(COLORREF crColor,int x=-1,int y=-1,int width=0,int height=0);
	BOOL Rotate(BYTE pos);		//��ת90��
	BOOL Flip(BYTE pos);		//��ת
   
	//��������
	//�����ڴ�ͼ��(����)
	RingDIB* CreateCanvas(HWND hWnd = NULL,int width = 0,int height = 0);
	BOOL DrawToCanvas(int x = 0,int y = 0,BOOL bTrans=TRUE);
   void FreeCanvas();								//�ͷ��ڴ�ͼ��
	
	//���ݲ���
   BOOL Backup();			 					//����ͼ������
	BOOL Restore(BOOL bTrans=FALSE);		//�ָ�ͼ������
	
	//����GIFͼ����
	BOOL IsAniGif(){return (BOOL)m_aniGifPic;}
	void EnableAniGif(BOOL bActive=TRUE);
   BOOL SetAniGifFilter(ANIFILTER_FUNC func);

	//��չ����
   BOOL Capture(HWND hwnd = NULL);	//=NULL,ȫ��
	BOOL DrawText(LPCTSTR szText,int x,int y,COLORREF crText = 0xFFFFFFFF,ringFont* rFont = NULL,BOOL bAutoAdjust=FALSE);
	BOOL DrawText(LPCTSTR szText,LPRECT lprc,UINT uFormat,COLORREF crText = 0xFFFFFFFF,ringFont* rFont = NULL);
	//ת������ͼ��HBITMAP,�����ֹ�����HBITMAP��DeleteObject)
	HBITMAP GetBitmap();	

   HRGN CreateRgn(BOOL bForceNoneRect = TRUE);
   HRGN CreateCoolWindow(BOOL bForceNoneRect = TRUE);
   void MakeBkg();

public:   
   DIBFILTERINFO m_Filter;
   int pluged_filter;

private:
	static BOOL LoadBmp(LPCTSTR szFilename,RingDIB* lprdib,int iFlag=DIB_LOADDIB,LPSTR lpszFormat=NULL);
   void Destroy();
	BOOL NewSize(int nNewsize);	//����ͼ�����ݻ�����
	void init();
	BOOL DIBDraw(HDC hDC,int dx,int dy,int dw,int dh,int sx,int sy,int sw,int sh,UINT wFlags);
	//����������������޶���ͼ���ڲ�
	//ʹ��ע�⣬���ɴ����Ա�������������븴�ƺ���
	void ClipRect(int& x,int& y,int& width,int& height);
   
private:
   static int pluged_ext;			//�˾��������
	static HDRAWDIB m_hDrawDib;   
   static LONG m_nObjCount;
	static RingDIB* m_dibCanvas;	//ȫ�ֻ���
   DECODER_FUNC m_currFunc;		//��ǰ�������
	BITMAPINFO m_bmif;
	RECT m_rcSrc,m_rcDest;
   COLORREF m_crKey;
	HWND m_hWnd;						//�滭Ŀ�괰�ھ��
	HRGN m_hrgn;
   RingDIB* m_backup,*m_actbak,*m_target;
   COLORREF* m_Bits;
   int m_width,m_height;
	int m_size;							//�����ͼ�󻺳����ߴ�
	LPANIGIFPIC m_aniGifPic;
   LPLOCALGIFINFO m_lGifInfo;     //����GIF��Ϣ
	static RingDataGuard m_guard;
};

//ע��ͼ��������
#define DIBREGFORMAT(pic)			RingDIB::RegFormat(pic,Load##pic)
//ͬһ����֧�ֶ��ļ���ʽע�ᣬ��jpg,jpeg��������LoadJPG
#define DIBREGFORMATS(ext,pic)	RingDIB::RegFormat(ext,Load##pic)

//ע������
#define DIBREGFILTER(dib,filter)\
{\
	RingDIB& ___rdib = dib;\
   if(___rdib.pluged_filter<MAX_FILTER && ___rdib.m_Filter.m_lp##filter==NULL){\
		try{___rdib.m_Filter.m_filter[___rdib.pluged_filter] = ___rdib.m_Filter.m_lp##filter = new dibFilter##filter(___rdib);\
         ___rdib.pluged_filter ++;}\
						catch(char *)\
      {Errmsg("RingDIB �ҽ��˾�ģ��ʧ��");abort();}\
   }\
}

#define FILTERS(filter) m_Filter.m_lp##filter

//RingDIB�˾�����ඨ��
class dibFilterALPHA:public DIBFILTER
{
public:
	~dibFilterALPHA();

	dibFilterALPHA(RingDIB& dib){m_rdib = &dib;};
	void Fill(int r,int g,int b,int a);
   void FillGlass(COLORREF color);
	void Blend(RingDIB *dib,int A);
	void Darken(RingDIB *dib);
	void Difference(RingDIB *dib);
	void Lighten (RingDIB *dib);
	void Multiply (RingDIB *dib);
	void Screen(RingDIB *dib);
	void Xor(RingDIB *dib);
};

typedef struct tagHSLDATA
{
	float m_hue;         // 0.0 .. 360.0  // Winkel
	float m_saturation;  // 0.0 .. 1.0    // Prozent
	float m_luminance;   // 0.0 .. 1.0    // Prozent
	float reserve;
}HSLDATA,*LPHSLDATA;

class dibFilterEFFECT:public DIBFILTER
{
public:
	~dibFilterEFFECT(){}

	dibFilterEFFECT(RingDIB& dib){m_rdib = &dib;}
	
	//ת��Ϊ256���Ҷ�ͼ��
	BOOL GrayScale();
	//��ƬЧ��
   BOOL Negative();
	//�����Աȶȣ�����Ϊ�ٷֱ�ȥ���ٷֺţ�������Աȶ�
	//Ϊ70%��PercentӦΪ70.������Χ0~200
	BOOL AdjustContrast(int Percent);
	//�������ȣ�����Ϊ�����������ȣ�Ϊ�����С���ȣ�
	//��ΧӦ��-255~255֮�䣬255ͼ��ȫ�ף�-255��ȫ��
	BOOL AdjustBright(int step);
	//ͬʱ�������ȺͶԱȶȣ���������ͬ�ϣ��ú���Ŀ��
	//����Ϊ���������Ȼ�Աȶ�������ɫ����0~255�ᶪ��
	//������Ϣ����ͬʱ�������п�������0~255��Χ������
	//��ʧ��Ϣ��
	BOOL AdjustBright_Contrast(int bright,int contrast);
	//����ɫ����������Χ��-180~180���ȣ�,=0��������
	//�������Ͷȣ�������Χ0~200(���飬���ֵ��>200),=100��������
	//�������ȣ����Ȳ�����Χ0~200(���飬���ֵ��>200)
	//,=100��������
	BOOL AdjustHSL(int degHue,int perSaturation,int perLuminosity);
	//δ��ɣ������Ժ��ͼ�󲿷����������
	void SetWorkArea(int x,int y,int width=0,int height=0);

protected:
	void RGBtoHSL(BYTE R,BYTE G,BYTE B,float* H,float* S,float* L);
	BYTE HueToRGB(float rm1, float rm2, float rh);
	COLORREF HSLtoRGB(float H,float S,float L);

private:
	RECT m_rcArea;		//����Χ����δ�õ�
	LPHSLDATA m_hslData;
};

//���¶���ֵ�������
#define DRC_LEFT		1
#define DRC_TOP		2
#define DRC_RIGHT		4
#define DRC_BOTTOM	8
#define DRC_LU			3
#define DRC_RU			6
#define DRC_LD			9
#define DRC_RD			12
#define DRC_INSIDE	99

#define ON_DC			1
#define ON_DIB			0
////////////////////////////////////////////////
//
//����PhotoShop��ѡ����Σ��������ƶ�
//
////////////////////////////////////////////////
class	RingDragRect
{
public:
	RingDragRect();
	virtual ~RingDragRect();

	void Init(HWND hwnd,RingDIB* dib)
	{
		m_hWnd = hwnd;
		m_dib = dib;		
	}

	BOOL IsActive(){return bActived;}
	BOOL IsMoving(){return bInMove;}
	BOOL IsDraging(){return bInDrag;}

	int left(int isDC = ON_DC);
	int top(int isDC = ON_DC);
	int right(int isDC = ON_DC);
	int bottom(int isDC = ON_DC);
	BOOL GetRect(LPRECT lprc,int isDC = ON_DC);
		
	//�����ڳߴ�ı�
	void OnParentSize(int width,int height)
	{
		m_width = width;
		m_height = height;		
	}
	virtual void SetRate(int rate);
	void ScrollParent(int dx,int dy);

	virtual void StartDrag(int x,int y);
	virtual void Drag(int x,int y);
	virtual void StopDrag(int x,int y);
	//virtual void StartMoveSide(int x,int y);
	virtual void StartMove(int x,int y);
	virtual void Move(int x,int y);
	virtual void StopMove(int x,int y);
	//void Resize(int x,int y);
	
	virtual void OnLButtonDown(int x,int y);
	virtual void OnLButtonUp(int x,int y);
	virtual int OnMouseMove(int x,int y);
	virtual void OnPaint(int rate);
	
	virtual void Draw(/*BOOL setClear*/);

	void setDIBRect(LPPOINT lpd,LPPOINT lps);//����ͼ�����
	virtual void setDCRect(int x,int y);	//���û��ƾ���	

	int GetDCWidth(){return m_rcDC.right-m_rcDC.left;}
	int GetDCHeight(){return m_rcDC.bottom-m_rcDC.top;}
	
	virtual void EnableDraw(BOOL bDraw = TRUE);
	//�������Ρ�������EnableDraw(FALSE)�����Ч	
	virtual void Erase();
	void Disable();

protected:
	virtual void Init();

protected:
	HANDLE handleDraw,hevent;
	RECT m_rcDIB,m_rcDC;	//rc:���ͼ�����꣬rectDC:��Դ�������
	//RECT m_dcDIBDest;	//ͼ��
	BOOL bActived;		//�Ƿ񼤻�
	BOOL bInDrag,bInMove,bInFirstAct;
	BOOL bShow;			//�Ƿ�����ʾ
	HDC hDC;
	POINT pt,ptOrg;
	HWND m_hWnd;
	RingDIB* m_dib;
	//m_width,m_height:�����ڿ�ߣ�
	//m_currX,m_currY:��ǰͼ��src����,
	//m_w,m_h:���ο��
	int m_width,m_height,m_currX,m_currY,m_w,m_h;
	int m_nInSide,scrx,scry;//scrx,scry������λ��
	int m_rate;
	HBRUSH m_brush;
	
	BOOL bQuitDraw;
	//DragRect���������߳�
	friend DWORD DragRectDrawProc(RingDragRect* obj);
};

#define AVIW_NONE			0xFFFFFFFF
#define AVIW_NEW			0x00000001
#define AVIW_BAR			0x00000010
#define AVIW_MENUBAR 	0x00000100
#define AVIW_CENTER  	0x10000000
#define AVIW_DESKCENTER 0x01000000

class RingAvi
{
public:
	RingAvi();
   ~RingAvi();

   void Open(LPCTSTR szPath=NULL);
   void SetWindow(int nflag,int x=0,int y=0,int pos=0);
   HWND Play(int width=0,int height=0);
   HWND PlayFullScreen();
   void ResetWindow(int x=0,int y=0,int pos=0);
   void NextFast(int skip=10000);
   void PrevFast(int skip=10000);
   void Close();

private:
   void CentreWindow(HWND hwnd,int width,int height);

public:
	HWND m_hMCIWnd;
   char szAviFilename[MAX_PATH];
   int nCenter,nWindowStyle;
   int X,Y,m_width,m_height,w_x,w_y,w_width,w_height;

private:
	LONG lAviPos;	//AVI��ǰ����λ��
   LONG lAviLen;	//AVI����
};

#ifndef MAKE_SELF_LIB
#ifdef _DEBUG
#pragma comment(lib, "ringdibd.lib")
#pragma comment(lib, "jpegd.lib")
#pragma comment(lib, "zlibd.lib")
#pragma comment(lib, "pngd.lib")
#else
#pragma comment(lib, "ringdib.lib")
#pragma comment(lib, "jpegd.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "png.lib")
#endif
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "vfw32.lib")
#endif

#endif

