#pragma once

#include "Type.h"
/*
�ص��������ֺ�֮ǰ�����пո񣬷�����ܱ����쳣
*/
typedef void (*Func)(void) ;//û�в����ͷ���ֵ��ί��
typedef void(*Action)(void* param) ;  //һ������û�з���ֵ��ί�У�һ��param��������Ŀ����󣬵������þ�̬������װ
typedef void(*Action2)(void*,void*) ;
typedef void(*Action3)(void*,void*,void*) ;
typedef void(*EnventHandler)(void* sender,void* param) ;//�¼�����ί�У�һ��sender��ʾ�¼������ߣ�param����Ŀ����󣬵������þ�̬������װ
typedef uint (*DataHandler)(void* sender,byte* buf,uint size_t,void* param) ;//�������ݻ�������ַ�ͳ��ȣ����з�������ʹ�øû��������������ݳ���
typedef uint (*FuncRead)();//�з���ֵ��ί��
