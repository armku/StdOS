
#if 0
    int HSE_VALUE = 8000000; // weak
    int SystemCoreClock = 72000000; // weak

    unsigned int SetSysClock(unsigned int result, unsigned int a2)
    {
        unsigned int v2; // r3@5
        int v3; // [sp+4h] [bp-10h]@1

        v3 = 0;

        return result;
    }
    signed int SystemInit()
    {
        signed int result; // r0@1

        HSE_VALUE = 8000000;
        SystemCoreClock = 72000000;
        SetSysClock(0x44AA200u, 0x7A1200u);
        result = 0x8000000;
        return result;
    }
#endif
