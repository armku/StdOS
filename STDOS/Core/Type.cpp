#include "Type.h"
#include "SString.h"

void assert_failed(uint8_t *file, uint32_t line, char *errstr)
{
    StdPrintf("%s(%d):    %s\n", file, line, errstr);
}
