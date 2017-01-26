/*
根基类 Object
参考 C#的 Object 类，系统中大部分类都继承自 Object
主要提供对象转字符串以及对象调试打印的抽象接口。
对象转字符串 ToStr/ToString 的默认实现时显示类名。
显示对象 Show 的默认实现是通过板子的调试串口输出。

 */
#pragma once

class CType;
class String;

//根对象
class Object
{
    public:
        #if 0
            //输出对象的字符串表示方式
            virtual String &ToStr(String &str)const;
		#endif
		#if 0
        //输出对象的字符串表示方式.支持RVO优化
        virtual String ToString()const;        
		#endif
        //显示对象。默认显示 ToString
        virtual void Show(bool newLine = false)const;
        const CType GetType()const;
};
