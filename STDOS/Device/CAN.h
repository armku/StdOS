#ifndef __CAN_H__
#define __CAN_H__

#include "Kernel\Sys.h"

// CAN��
class Can
{
public:
    typedef enum
    {
        Mode_Send = 0x00,        // ����ģʽ
        STD_Data = 0x01,    // ֻ���ձ�׼����֡
        STD_Remote = 0x02,  // ֻ���ձ�׼Զ��֡
        STD = 0x03,         // ��׼֡���ᱻ���˵�
        EXT_Data = 0x04,    // ֻ������չ����֡
        EXT_Remote = 0x05,  // ֻ������չԶ��֡
        EXT = 0x06,         // ��չ֡���ᱻ���˵�
        Mode_ALL = 0x07          // ������������
    }Mode_TypeDef;

    Mode_TypeDef Mode;  // ����ģʽ

    Can(CAN index = Can1, Mode_TypeDef mode = Mode_Send, int remap = 1);
    ~Can();

	void Open();
    void Send(byte* buf, uint len);

private:
	CAN		_index;
    int		Remap;

    void*	_Tx;
    void*	_Rx;
	
	void OnOpen();
};

#endif
