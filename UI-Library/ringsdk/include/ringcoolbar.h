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
//ԭ�ļ�����ringcoolbar.h
//
//���ļ�˵����ReBar�ඨ��
//
**********************************************************************/
#ifndef _INC_RINGCOOLBAR
#define _INC_RINGCOOLBAR

class RingCoolBar:public RingControls
{
public:
	RingCoolBar();
   virtual ~RingCoolBar();
	
	LPCTSTR WndClassName(){return REBARCLASSNAME;}

	BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);
	
	BOOL AddBand(RingCtrlWnd* child);
   
protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);
	
	LRESULT OnLButtonDown(RINGPARAMS& param);
	LRESULT OnMouseMove(RINGPARAMS& param);
	LRESULT OnLButtonUp(RINGPARAMS& param);
	
	BOOL OnParentNotify(HWND,UINT,UINT,LONG);
	
private:
	void init();
	
protected:   
   int m_State;
};

#endif
