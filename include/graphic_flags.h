#include <stddef.h>
#include <sys/types.h>

#define GraphicFlags \
    GFlag(ACTION,               action,             "a", "%c", char     ) \
    GFlag(QUIET,                quiet,              "q", "%u", u_int32_t) \
    GFlag(FORMAT,               format,             "f", "%u", u_int32_t) \
    GFlag(MEDIUM,               medium,             "t", "%c", char     ) \
    GFlag(WIDTH,                width,              "s", "%u", u_int32_t) \
    GFlag(HEIGHT,               height,             "v", "%u", u_int32_t) \
    GFlag(SIZE,                 size,               "S", "%u", u_int32_t) \
    GFlag(OFFSET,               offset,             "O", "%u", u_int32_t) \
    GFlag(IMAGE_ID,             imageId,            "i", "%u", u_int32_t) \
    GFlag(IMAGE_NUMBER,         imageNumber,        "I", "%u", u_int32_t) \
    GFlag(PLACEMENT_ID,         placementId,        "p", "%u", u_int32_t) \
    GFlag(COMPRESSION,          compression,        "o", "%c", char     ) \
    GFlag(MORE,                 more,               "m", "%u", u_int32_t) \
    GFlag(X,                    x,                  "x", "%u", u_int32_t) \
    GFlag(Y,                    y,                  "y", "%u", u_int32_t) \
    GFlag(W,                    w,                  "w", "%u", u_int32_t) \
    GFlag(H,                    h,                  "h", "%u", u_int32_t) \
    GFlag(X_OFFSET,             xOffset,            "X", "%u", u_int32_t) \
    GFlag(Y_OFFSET,             yOffset,            "Y", "%u", u_int32_t) \
    GFlag(COLUMNS,              columns,            "c", "%u", u_int32_t) \
    GFlag(ROWS,                 rows,               "r", "%u", u_int32_t) \
    GFlag(CURSOR,               cursor,             "C", "%u", u_int32_t) \
    GFlag(UNICODE,              unicode,            "U", "%u", u_int32_t) \
    GFlag(Z_INDEX,              zIndex,             "z", "%d", int32_t  ) \
    GFlag(PARENT_ID,            parentId,           "P", "%u", u_int32_t) \
    GFlag(PLACEMENT_PARENT_ID,  placementParentId,  "Q", "%u", u_int32_t) \
    GFlag(H_OFFSET,             hOffset,            "H", "%d", int32_t  ) \
    GFlag(V_OFFSET,             vOffset,            "V", "%d", int32_t  )

typedef enum {
#define GFlag(NAME, ...) NAME,
GraphicFlags
#undef GFlag
} GFLag;

typedef struct {
    GFLag flag;
    union {
    #define GFlag(NAME, name, f, q, type) type name;
    GraphicFlags
    #undef GFlag
    };
} GFEntry;
