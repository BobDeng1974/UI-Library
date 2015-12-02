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
//ԭ�ļ�����dib_global.cpp
//
//˵����ȫ�ְ�������ʵ��
//
**********************************************************************/

#define MAKE_SELF_LIB
#include "ringdib.h"

/////////////////////////////////////////////////
//
//	���º���Ӧ����DIBͼ�����Ż��Ʊ���Ϊ����������
//	������С1/3��������������ͼ�����ű��ʴ�С����
//	������ã�������ݲ�׼�ģ���
//
//	����rate˵����
//		rateΪͼ�����ű��ʣ�Ӧ����ͼ�����Ż��Ƶ����
//		rate��ȡֵ�����������¹���
//
//			rate >= 0,�Ŵ���Ϊradio+1,
//						��ԭͼ��һ�����ػ��Ƶ���ĻΪradio+1������
//			rate == -1,��С����Ϊ1/3,
//						��ԭͼ��4�����ػ��Ƶ���ĻΪ3������,���ƺ�
//						Ϊԭͼ���66.7%
//			rate < -1,��С����Ϊ(-radio)��
//						��ԭͼ��(-radio)�����ػ��Ƶ���ĻΪһ������
//
//		����ʱӦ������rate = GetDIBShowRate(dib);
//		��Ȼһ��Ӧ����Ҫ�õ��˹��ܶ���Ϊͼ������һ��rate,
//		���ü���rate�����Ǹ���rate����ͼ��Ļ���Ŀ�ľ��Ρ�
//
/////////////////////////////////////////////////

//������ͼ���ϵľ���λ������Ƶ���Ļ��λ��
BOOL Get_OnDCRect(RingDIB* dib,int rate,CONST RECT* lpsrc,LPRECT lpdst)
{
	if(dib == NULL || lpsrc == NULL || lpdst == NULL)
		return FALSE;

	RECT src,dst;
	
	dib->GetSrcRect(&src);
	dib->GetDestRect(&dst);

	//rate >= 0,�Ŵ���Ϊradio+1
	//rate == -1,��С����Ϊ3/4
	//rate < -1,��С����Ϊ(-radio)

	if(rate >= 0)
	{
		rate ++;
		lpdst->left = (lpsrc->left - src.left) * rate + dst.left;
		lpdst->top = (lpsrc->top - src.top) * rate + dst.top;
		lpdst->right = (lpsrc->right - src.left) * rate + dst.left;
		lpdst->bottom = (lpsrc->bottom - src.top) * rate + dst.top;
	}
	else if(rate == -1)
	{
		lpdst->left = (lpsrc->left - src.left) * 3/4 + dst.left;
		lpdst->top = (lpsrc->top - src.top) * 3/4 + dst.top;
		lpdst->right = (lpsrc->right - src.left) * 3/4 + dst.left;
		lpdst->bottom = (lpsrc->bottom - src.top) * 3/4 + dst.top;
	}
	else
	{
		rate = -rate;
		lpdst->left = (lpsrc->left - src.left) / rate + dst.left;
		lpdst->top = (lpsrc->top - src.top) / rate + dst.top;
		lpdst->right = (lpsrc->right - src.left) / rate + dst.left;
		lpdst->bottom = (lpsrc->bottom - src.top) / rate + dst.top;
	}
	return TRUE;
}

//���ݻ��Ƶ���Ļ��λ������ͼ���ϵľ���λ��
BOOL Get_OnDIBRect(RingDIB* dib,int rate,CONST RECT* lpsrc,LPRECT lpdst)
{
	if(dib == NULL || lpsrc == NULL || lpdst == NULL)
		return FALSE;

	RECT src,dst;
	
	dib->GetSrcRect(&src);
	dib->GetDestRect(&dst);

	//rate >= 0,�Ŵ���Ϊradio+1
	//rate == -1,��С����Ϊ3/4
	//rate < -1,��С����Ϊ(-radio)

	if(rate >= 0)
	{
		rate ++;
		lpdst->left = (lpsrc->left - dst.left) / rate + src.left;
		lpdst->top = (lpsrc->top - dst.top) / rate + src.top;
		lpdst->right = (lpsrc->right - dst.left) / rate + src.left;
		lpdst->bottom = (lpsrc->bottom - dst.top) / rate + src.top;
	}
	else if(rate == -1)
	{
		lpdst->left = (lpsrc->left - dst.left) * 4/3 + src.left;
		lpdst->top = (lpsrc->top - dst.top) * 4/3 + src.top;
		lpdst->right = (lpsrc->right - dst.left) * 4/3 + src.left;
		lpdst->bottom = (lpsrc->bottom - dst.top) * 4/3 + src.top;
	}
	else
	{
		rate = -rate;
		lpdst->left = (lpsrc->left - dst.left) * rate + src.left;
		lpdst->top = (lpsrc->top - dst.top) * rate + src.top;
		lpdst->right = (lpsrc->right - dst.left) * rate + src.left;
		lpdst->bottom = (lpsrc->bottom - dst.top) * rate + src.top;
	}
	return TRUE;
}

int GetDIBShowRate(RingDIB* dib)
{
	if(dib == NULL)
		return 0;

	RECT rcd,rcs;
	int dw,sw;

	dib->GetDestRect(&rcd);
	dib->GetSrcRect(&rcs);
	dw = rcd.right - rcd.left;
	sw = rcs.right - rcs.left;

	if(dw >= sw)
		return (dw / sw) - 1;
	else
		if(dw * 4/3 == sw)
			return -1;
		else
			return -(sw / dw);
}

//������ͼ���ϵľ���λ������Ƶ���Ļ��λ��
BOOL Get_OnDCPt(RingDIB* dib,int rate,CONST POINT* lpsrc,LPPOINT lpdst)
{
	if(dib == NULL || lpsrc == NULL || lpdst == NULL)
		return FALSE;

	RECT src,dst;
	
	dib->GetSrcRect(&src);
	dib->GetDestRect(&dst);

	//rate >= 0,�Ŵ���Ϊradio+1
	//rate == -1,��С����Ϊ3/4
	//rate < -1,��С����Ϊ(-radio)

	if(rate >= 0)
	{
		rate ++;
		lpdst->x = (lpsrc->x - src.left) * rate + dst.left;
		lpdst->y = (lpsrc->y - src.top) * rate + dst.top;		
	}
	else if(rate == -1)
	{
		lpdst->x = (lpsrc->x - src.left) * 3/4 + dst.left;
		lpdst->y = (lpsrc->y - src.top) * 3/4 + dst.top;		
	}
	else
	{
		rate = -rate;
		lpdst->x = (lpsrc->x - src.left) / rate + dst.left;
		lpdst->y = (lpsrc->y - src.top) / rate + dst.top;		
	}
	return TRUE;
}

//���ݻ��Ƶ���Ļ��λ������ͼ���ϵľ���λ��
BOOL Get_OnDIBPt(RingDIB* dib,int rate,CONST POINT* lpsrc,LPPOINT lpdst)
{
	if(dib == NULL || lpsrc == NULL || lpdst == NULL)
		return FALSE;

	RECT src,dst;
	
	dib->GetSrcRect(&src);
	dib->GetDestRect(&dst);

	//rate >= 0,�Ŵ���Ϊradio+1
	//rate == -1,��С����Ϊ3/4
	//rate < -1,��С����Ϊ(-radio)

	if(rate >= 0)
	{
		rate ++;
		lpdst->x = (lpsrc->x - dst.left) / rate + src.left;
		lpdst->y = (lpsrc->y - dst.top) / rate + src.top;		
	}
	else if(rate == -1)
	{
		lpdst->x = (lpsrc->x - dst.left) * 4/3 + src.left;
		lpdst->y = (lpsrc->y - dst.top) * 4/3 + src.top;		
	}
	else
	{
		rate = -rate;
		lpdst->x = (lpsrc->x - dst.left) * rate + src.left;
		lpdst->y = (lpsrc->y - dst.top) * rate + src.top;		
	}
	return TRUE;
}
