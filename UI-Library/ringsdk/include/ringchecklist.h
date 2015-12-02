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
//ԭ�ļ�����ringchecklist.h
//
//���ļ�˵����RingCheckList������
//
//��⹦�ܣ�CheckList�ؼ���װ��CheckList�ǽ�������Ƶ�
//				�ؼ���ListBox�ؼ�����չ��ÿһ���б�ѡ��ǰ��
//				һ��ѡ���ɹ�ѡ��
//
**********************************************************************/
#ifndef _INC_RINGCHECKLIST
#define _INC_RINGCHECKLIST

typedef struct _tagCheckListData
{
	BOOL m_bChecked;
	LPARAM m_data;
}RINGCHECKLISTDATA,*LPRINGCHECKLISTDATA;

class RingCheckList:public RingListBox
{
public:
	RingCheckList();
	virtual ~RingCheckList();
	
	BOOL Create(DWORD dwStyle,DWORD dwExStyle,LPCTSTR szInitText,UINT uId,int x,int y,int width,int height);
	BOOL Create(UINT uId,int x=0,int y=0,int width=-999,int height=-999);

	//��������Attach(HWND),�������ظú��������������أ�Ҫ��RingControls::���ܵ���
	BOOL Attach(UINT uId,RingBaseWnd* parent=NULL){return RingControls::Attach(uId,parent);}
	BOOL Attach(HWND);
	
	//�����Ի棬����ΪTRUE��ʾ�ɸ����ڴ����Ի�
	void SetOwnerDraw(BOOL bParentDraw=FALSE)
		{RingControls::SetOwnerDraw(bParentDraw,LBS_OWNERDRAWFIXED);}
	
	//���ѡ��״̬
	BOOL IsItemChecked(int nIndex = -1);
	//�л�ѡ��/��ѡ��״̬��ToggleItemCheck������״̬��SetItemCheck����ԭ״̬
	BOOL ToggleItemCheck(int nIndex = -1);
	BOOL SetItemCheck(int nIndex,BOOL bCheck);

protected:
	LRESULT RingdowProc(HWND,RINGPARAMS);
	BOOL OnDrawItem(LPDRAWITEMSTRUCT lpds);
	void CreateCheckBmp(HDC hDC,int nSize);
	LRESULT OnDestroy(RINGPARAMS& param);

private:
	void OnDelString(int index);
	LPRINGCHECKLISTDATA GetPriData(int nIndex);
	BOOL SetPriData(int nIndex,LPRINGCHECKLISTDATA lpData);

protected:
	HBITMAP m_hbmp;
	int m_nCheckSize;
};

#endif
