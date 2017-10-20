#include "Stream.h"

#ifdef DEBUG
byte buftest[]={0,1,2,3,4,5,6,7,8,9};
Stream streambuf(buftest,ArrayLength(buftest));
byte bufbyte=0x0d;
ushort bufshort=0x1234;
uint bufuint = 0x12345678;
UInt64 bufuint64 = 0x12345678ABCDEF;
void streamtest()
{
	streambuf.Little=false;
	streambuf.SetCapacity(15);
//	streambuf.Write(bufbyte);
//	streambuf.Write(bufshort);
//	streambuf.Write(bufuint);
	streambuf.Write(bufuint64);
	
	streambuf.SetPosition(0);
//	bufuint64=streambuf.ReadByte();
//	bufuint64=streambuf.ReadUInt16();
//	bufuint64=streambuf.ReadUInt32();
	bufuint64=streambuf.ReadUInt64();
}
#endif
