#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "Device\I2C.h"
#include "Storage\Storage.h"

// EEPROM
class AT24CXX : public CharStorage
{
public:
    I2C* IIC;		// I2Cͨ�ſ�
	byte Address;	// �豸��ַ

    AT24CXX();
    virtual ~AT24CXX();

	void Init();
	bool Write(ushort addr, byte data);
	byte Read(ushort addr);

	virtual bool Write(uint addr, const Buffer& bs) const;
	virtual bool Read(uint addr, Buffer& bs) const;
};

#endif
