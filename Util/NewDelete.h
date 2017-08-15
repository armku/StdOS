#ifndef _NEWDELETE_H
#define _NEWDELETE_H

#include "Type.h"

#ifdef DEBUG

    void *operator new(uint size);
    void *operator new[](uint size);
    void operator delete (void *p);
    void operator delete [](void *p);

#endif
#endif
