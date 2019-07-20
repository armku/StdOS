#include "Type.h"

#include "Buffer.h"
#include "Array.h"
#include "ByteArray.h"
#include "SString.h"

#include <typeinfo>
using namespace ::std;

/******************************** Object ********************************/

// ���������ַ�����ʾ��ʽ
String& Object::ToStr(String& str) const
{
	auto name = typeid(*this).name();
	while(*name >= '0' && *name <= '9') name++;

	str	+= name;

	return str;
}

// ���������ַ�����ʾ��ʽ��֧��RVO�Ż���
// �÷���ֱ�ӷ��ظ���һ��String��Ϊ��ʼֵ��ֻ��һ�ι��죬û�ж��๹�졢������������
String Object::ToString() const
{
	String str;
	ToStr(str);

	return str;
}

void Object::Show(bool newLine) const
{
	// Ϊ�˼��ٶѷ��䣬���ýϴ��ջ������
	char cs[0x200];
	String str(cs, ArrayLength(cs));
	str.SetLength(0);
	ToStr(str);
	str.Show(newLine);
}

const Type Object::GetType() const
{
	auto p = (int*)this;

	//return Type(&typeid(*this), *(p - 1));
	
	Type type;
	
	type._info	= &typeid(*this);
	type.Size	= *(p - 1);
	
	return type;
}

/******************************** Type ********************************/

Type::Type()
{
	//_info	= ti;

	//Size	= size;
}

const String Type::Name() const
{
	auto ti	= (const type_info*)_info;
	auto name = ti->name();
	while(*name >= '0' && *name <= '9') name++;

	return String(name);
}

void assert_failed2(cstring msg, cstring file, unsigned int line)
{
    debug_printf("%s Line %d, %s\r\n", msg, line, file);

//	TInterrupt::Halt();
}
