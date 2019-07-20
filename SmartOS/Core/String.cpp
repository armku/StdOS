#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

// gcc里面strncasecmp位于strings.h
#ifdef __GNUC__
#include <strings.h>
#endif

#include "_Core.h"

#include "ByteArray.h"

#include "SString.h"
