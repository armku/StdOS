#include "RC4.h"

#if 0
/// <summary>加密</summary>
/// <param name="data">数据</param>
/// <param name="pass">密码</param>
/// <returns></returns>
byte *RC4::Encrypt(byte *data, byte *pass, int datalen, int passlen)
{
    if (datalen == 0 || passlen == 0)
        return NULL;

    byte *output = new byte[datalen];
    int i = 0;
    int j = 0;
    byte *box = GetKey(pass, passlen, 256);
    // 加密  
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

/// <summary>打乱密码</summary>  
/// <param name="pass">密码</param>  
/// <param name="len">密码箱长度</param>  
/// <returns>打乱后的密码</returns>  
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
