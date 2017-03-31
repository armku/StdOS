#include "RC4.h"

#if 0
/// <summary>����</summary>
/// <param name="data">����</param>
/// <param name="pass">����</param>
/// <returns></returns>
byte *RC4::Encrypt(byte *data, byte *pass, int datalen, int passlen)
{
    if (datalen == 0 || passlen == 0)
        return NULL;

    byte *output = new byte[datalen];
    int i = 0;
    int j = 0;
    byte *box = GetKey(pass, passlen, 256);
    // ����  
    for (int k = 0; k < datalen; k++)
    {
        i = (i + 1) % 256;
        j = (j + box[i]) % 256;
        byte temp = box[i];
        box[i] = box[j];
        box[j] = temp;
        byte a = data[k];
        byte b = box[(box[i] + box[j]) % 256];
        output[k] = (byte)(a ^ b);
    }

    return output;
}

/// <summary>��������</summary>  
/// <param name="pass">����</param>  
/// <param name="len">�����䳤��</param>  
/// <returns>���Һ������</returns>  
byte *RC4::GetKey(byte *pass, int passlen, int len)
{
    byte *box = new byte[len];
    for (int i = 0; i < len; i++)
    {
        box[i] = (byte)i;
    }
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        j = (j + box[i] + pass[i % passlen]) % len;
        byte temp = box[i];
        box[i] = box[j];
        box[j] = temp;
    }
    return box;
}
#endif
