#ifndef B64_H
#define B64_H

#include <stdlib.h>
#include <sys/types.h>

u_int8_t* encodeToB64(size_t lenght, const u_int8_t* data);

#endif
