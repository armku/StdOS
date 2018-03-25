#ifndef _PARITY_H
#define _PARITY_H
    #include "Type.h"
    class Parity
    {
        public:
            static bool Parity::OddParity(uint8_t *buf, uint32_t len);
            static bool Parity::EvenParity(uint8_t *buf, uint32_t len);

    }; // ∆Ê≈º–£—È	
#endif
