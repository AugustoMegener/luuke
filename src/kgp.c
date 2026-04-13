#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "graphic_flags.h"
#include "image.h"
#include "b64.h"

#define GFlag(NAME, name, f, q, type)   \
    GFEntry name##Flag(type value) { \
        GFEntry new;                 \
        new.flag = NAME;                \
        new.name = value;               \
        return new;                     \
    }; 

GraphicFlags
#undef GFlag

#define putFlag(code, flag) code = addFlag(code, flag);

#define escapeCode "\x1B"
#define gpPrefix "_G"

typedef struct {
    u_int8_t* data;
    size_t flagCount;
    GFEntry flags[];
} GCode;

GCode* newGCode() {
    GCode* new = malloc(sizeof(GCode));
    new->data = NULL;
    new->flagCount = 0;
    return new;
}

GCode* addFlag(GCode* code, GFEntry flag) {
    size_t newCount = code->flagCount + 1;

    GCode* new = realloc(code, sizeof(GCode) + (newCount * sizeof(GFEntry)));

    new->flags[newCount - 1] = flag;
    new->flagCount = newCount;

    return new;
}

GCode* b64GCode(u_int8_t* data, size_t format, Size size) {
    GCode* new = newGCode();

    new->data = data;
    new->flagCount = 0;

    putFlag(new, formatFlag(format));
    putFlag(new, widthFlag(size.width));
    putFlag(new, heightFlag(size.height));

    return new;
}

GCode* fileGCode(char* filePath, size_t format, Size size) {
    GCode* new = newGCode();

    new->data = (u_int8_t*)filePath;
    new->flagCount = 0;

    putFlag(new, formatFlag(format));
    putFlag(new, widthFlag(size.width));
    putFlag(new, heightFlag(size.height));

    return new;
}

GCode* shmGCode(char* shm_name, size_t format, Size size) {
    GCode* new = newGCode();

    new->data = (u_int8_t*)shm_name + 1;
    new->flagCount = 0;

    putFlag(new, formatFlag(format));
    putFlag(new, widthFlag(size.width));
    putFlag(new, heightFlag(size.height));

    return new;
}


char* flagStrFlag(GFEntry flag) {
    switch (flag.flag) {
#define GFlag(NAME, name, _f, q, type) \
        case NAME : return q; break;
        GraphicFlags
#undef GFlag
    }
}

char* flagStrChar(GFEntry flag) {
    switch (flag.flag) {
#define GFlag(NAME, name, f, q, type) \
        case NAME : return f; break;
        GraphicFlags
#undef GFlag
    }
}

size_t flagStrSize(GFEntry flag) {
    size_t size;

    switch (flag.flag) {
#define GFlag(NAME, name, _f, q, type) \
        case NAME : size = snprintf(NULL, 0, q, flag.name); break;
        GraphicFlags
#undef GFlag
    }

    return size;
}

void fprintFlagValue(FILE* out, GFEntry flag) {
    switch (flag.flag) {
#define GFlag(NAME, name, _f, q, type) \
        case NAME : fprintf(out, q, flag.name); break;
        GraphicFlags
#undef GFlag
    }
}

void fprintGCode(FILE* out, GCode* code) {
    fprintf(out, escapeCode);
    fprintf(out, gpPrefix);

    for (size_t i = 0; i < code->flagCount; i++) {
        GFEntry flag = code->flags[i];

        fprintf(out, "%s", flagStrChar(flag));
        fprintf(out, "=");
        fprintFlagValue(out, flag);

        fprintf(out, (i == code->flagCount-1)?  ";" : ",");
    }

    if (code->data != NULL) fprintf(out, "%s", (char*)code->data);
    
    fprintf(out, escapeCode);
    fprintf(out, "\\");
}
