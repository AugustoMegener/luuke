#ifndef KGP_H
#define KGP_H

#include <stdio.h>
#include <sys/types.h>

#include "graphic_flags.h"
#include "image.h"

#define GFlag(NAME, name, f, q, type)   \
    GFEntry name##Flag(type value);  
GraphicFlags
#undef GFlag

#define putFlag(code, flag) code = addFlag(code, flag);

#define escapeCode "\x1B"

typedef struct {
    u_int8_t* data;
    size_t flagCount;
    GFEntry flags[];
} GCode;

GCode* newGCode();

GCode* addFlag(GCode* code, GFEntry flag);

GCode* b64GCode(u_int8_t* data, size_t format, Size size);

GCode* fileGCode(char* filePath, size_t format, Size size);

GCode* shmGCode(char* shm_name, size_t format, Size size);

void freeGCode(GCode* code);

char* flagStrFlag(GFEntry flag);

char* flagStrChar(GFEntry flag);

size_t flagStrSize(GFEntry flag);

void fprintFlagValue(FILE* out, GFEntry flag);

void fprintGCode(FILE* out, GCode* code);

#endif 
