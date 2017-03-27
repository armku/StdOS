#pragma once
#include "Type.h"

class STMFLASH
{
    public:       
		STMFLASH();
		void SetFlashSize(uint flashsize);
		int Read(uint addr,void* pBuf,int len);//读取
		int Write(uint addr,void* pBuf,int len,bool saveold=true);//写入
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		uint sectorSize;//扇区大小
		uint flashSize;//Flash容量大小，单位K
		ushort readHalfWord(uint faddr); //读出半字  
        void writeNoCheck(uint addr, ushort *pBuffer, ushort len);	
		void write(uint addr, ushort *pBuffer, ushort len); //从指定地址开始写入指定长度的数据
		int writeSector(uint addr);//写入整个扇区
        void read(uint addr, ushort *pBuffer, ushort len); //从指定地址开始读出指定长度的数据
		bool addrValid(uint addr);//地址正确，在上下限范围内
		bool addrSectorFirst(uint addr);//地址是否是扇区起始地址
};
