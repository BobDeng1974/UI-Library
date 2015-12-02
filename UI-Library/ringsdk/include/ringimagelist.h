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
//ԭ�ļ�����ringimagelist.h
//
//���ļ�˵����RingImageList������
//
//��⹦�ܣ�ImageList��װ
//
**********************************************************************/
#ifndef _INC_RINGIMAGELIST
#define _INC_RINGIMAGELIST

class RingImageList
{
public:
	RingImageList():m_handle(NULL){}
	virtual ~RingImageList(){Destroy();}
		
	HIMAGELIST Handle(){return m_handle;}
	operator	HIMAGELIST() const{return m_handle;}
	int Count(){return ImageList_GetImageCount(m_handle);}
	
	BOOL Clear(){return ImageList_RemoveAll(m_handle);}
	HIMAGELIST Create(int cx,int cy,UINT flags,int cInitial,int cGrow);
	BOOL Destroy();
	int Add(HBITMAP hbm,HBITMAP hbmMask=NULL){return ImageList_Add(m_handle,hbm,hbmMask);}
	int AddMasked(HBITMAP hbm,COLORREF crMask){return ImageList_AddMasked(m_handle,hbm,crMask);}
	BOOL BeginDrag(int idx,int x,int y){return ImageList_BeginDrag(m_handle,idx,x,y);}

	HICON GetIcon(int idx,UINT flags){return ImageList_GetIcon(m_handle,idx,flags);}
	BOOL GetIconSize(int *cx,int *cy){return ImageList_GetIconSize(m_handle,cx,cy);}

protected:

private:
	HIMAGELIST m_handle;	
};

#endif