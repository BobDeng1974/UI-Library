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
//�汾��0.2
//
//������������������ʹ�ö���������������κλر���������ϣ���ܵõ�
//		  ��Ĺ�����֧�֡�����Զ����Դ�������޸ĺ͸Ľ�����ϣ��������
//		  �޸ĵ�ͬʱ������һ��ͬ���ĸ�����
//		  ����ⲻ�������κ���ҵ��;����ȷʵ��Ҫ������������ϵ��
//
//e-mail:ringphone@sina.com
//
//ԭ�ļ�����ringstatic.h
//
//���ļ�˵����RingStatic������
//
//��⹦�ܣ���̬�ؼ���װ
//
**********************************************************************/
#ifndef _INC_RINGSTATIC
#define _INC_RINGSTATIC

class RingStatic:public RingControls
{
public:
	RingStatic();
	~RingStatic();
	
   LPCTSTR WndClassName(){return "Static";}
	void SetHyperlink(COLORREF crNormal = 0x00FF0000,COLORREF crHover = 0x00FF0000);
	void SetHyperStyle(UINT styNormal=0,UINT styHover=RINGFONT_UNDERLINE);
	void SetTransparent(){m_brush = (HBRUSH)GetStockObject(NULL_BRUSH);}
	BOOL SizeFitText();

protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);
	//void OnParentNotify(HWND,UINT,UINT,LONG);
	LRESULT OnBkColor(UINT,HDC);

protected:
	COLORREF m_crNormal,m_crHover;
	ringFont m_fontHover;
	HCURSOR m_cursor;
};

#endif
