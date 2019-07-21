#ifndef _PARITY_H
#define _PARITY_H
    #include "Type.h"
    class Parity
    {
        public:
            static bool OddParity(byte *buf, uint32_t len);
            static bool EvenParity(byte *buf, uint32_t len);

    }; // ∆Ê≈º–£—È	
#endif
