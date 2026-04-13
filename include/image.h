#ifndef IMAGE_H
#define IMAGE_H

#include <stdbool.h>
#include "util.h"
#include <sys/types.h>

#define imagePixelsSize(size) (sizeof(RGBA) * (size).width * (size).height) 
#define imageSize(size) (sizeof(u_int32_t) * 2) + (imagePixelsSize(size))


typedef struct {
    u_int8_t red, green, blue, alpha;
} RGBA;

RGBA rgb(u_int8_t red, u_int8_t green, u_int8_t blue);
RGBA rgba(u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t alpha);

typedef struct {
    u_int32_t width, height;    
} Size;

Size size(u_int32_t width, u_int32_t height);

typedef struct {
    Size size;
    RGBA* pixels;
} PixelMap;

typedef Option(PixelMap) PixelMapOption;

PixelMap pixelMapOf(Size size, RGBA* pixels);
PixelMap newPixelMap(u_int32_t width, u_int32_t height);

RGBA getMapPixel(PixelMap* img, u_int32_t x, u_int32_t y);
void setMapPixel(PixelMap* img, u_int32_t x, u_int32_t y, RGBA px);

PixelMapOption loadPNG(const char* path);

#define mixMode(NAME, Name, name) NAME
typedef enum {
    AllMixModes
} MixMode;
#undef mixMode

RGBA* mergePixels(MixMode mode, Size size, float topOpacity, RGBA* top, float downOpacity, RGBA* down);

typedef struct {
    MixMode mode;
    double opacity;
    RGBA* pixels;
} Layer;

Layer layerOf(RGBA* pixels, double opacity, MixMode mode);


typedef struct {
    u_long id;
    Size size;
    size_t format;
    u_int8_t* data; 
} ImageB64;

ImageB64 toB64(PixelMap data);   

ImageB64 toPngB64(PixelMap data);   

#endif
