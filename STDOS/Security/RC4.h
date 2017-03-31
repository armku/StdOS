#pragma once

#include "Type.h"

class RC4
{
    public:
        static byte *Encrypt(byte *data, byte *pass, int datalen, int passlen);
    private:
        static byte *GetKey(byte *pass, int passlen, int len);
};
