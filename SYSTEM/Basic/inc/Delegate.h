#pragma once

#include "Type.h"

//û�в����ͷ���ֵ��ί��
typedef void (*Func)(void);
//һ������û�з���ֵ��ί�У�һ��param��������Ŀ����󣬵������þ�̬������װ
typedef void (*Action)(void* param);
typedef void(*Action2)(void*,void*);
typedef void(*Action3)(void*,void*,void*);
//�¼�����ί�У�һ��sender��ʾ�¼������ߣ�param����Ŀ������������þ�̬������װ
typedef void(*EnventHandler)(void* sender,void* param);
//�������ݻ�������ַ�ͳ��ȣ����з�������ʹ�øû��������������ݳ���
typedef uint (*DataHandler)(void* sender,byte* buf,uint size_t,void* param);
