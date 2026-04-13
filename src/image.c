#include <stdbool.h>

#include "b64.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "mix_mode.h"

#define imagePixelsSize(size) (((size_t)sizeof(RGBA)) * (size).width * (size).height) 
#define imageSize(size) (sizeof(u_int32_t) * 2) + (imagePixelsSize(size))


typedef struct {
    u_int8_t red, green, blue, alpha;
} RGBA;

RGBA rgb(u_int8_t red, u_int8_t green, u_int8_t blue) {
    return (RGBA) { red, green, blue, 255 };
}

RGBA rgba(u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t alpha) {
    return (RGBA) { red, green, blue, alpha };
}


typedef struct {
    u_int32_t width, height;	
} Size;

Size size(u_int32_t width, u_int32_t height) {
    return (Size) { width, height };
}

typedef struct {
    Size size;
    RGBA* pixels;
} PixelMap;

typedef Option(PixelMap) PixelMapOption;

PixelMap pixelMapOf(Size size, RGBA* pixels) {
    PixelMap img;
    img.size = size;
    img.pixels = pixels;

    return img;
}

PixelMap newPixelMap(u_int32_t width, u_int32_t height) {
    PixelMap img;
    img.size = size(width, height);
    img.pixels = malloc(imagePixelsSize(img.size));

    return img;
}

RGBA getMapPixel(PixelMap* img, u_int32_t x, u_int32_t y) {
    return img->pixels[y * img->size.width + x];
}

void setMapPixel(PixelMap* img, u_int32_t x, u_int32_t y, RGBA px) {
    img->pixels[y * img->size.width + x] = px;
}



PixelMapOption loadPNG(const char* path) {
    int channels;

    Size size;

    RGBA* pxs = (RGBA*)stbi_load(path, (int*)&size.width, (int*)&size.height, &channels, 4);

    if (!pxs) {
        free(pxs);
        return (PixelMapOption) { false };
    }

    return (PixelMapOption) { true, (PixelMap) { size, pxs } };
}


#define mixMode(NAME, Name, name) NAME,
typedef enum {
    AllMixModes
} MixMode;
#undef mixMode


RGBA normalPixelMix(RGBA top, RGBA down) {
    return (RGBA) {
        (top.red * top.alpha + down.red * (255 - top.alpha)) / 255,
        (top.green * top.alpha + down.green * (255 - top.alpha)) / 255,
        (top.blue * top.alpha + down.blue * (255 - top.alpha)) / 255,
        top.alpha + (down.alpha * (255 - top.alpha)) / 255
    };	
}

RGBA* mergePixels(MixMode mode, Size size, float topOpacity, RGBA* top, float downOpacity, RGBA* down) {
    RGBA (*mixer)(RGBA, RGBA);

    switch (mode) {
#define mixMode(NAME, Name, name) case NAME: mixer = name##PixelMix; break;
        AllMixModes
#undef mixMode
        default: return NULL; break;
    }

    RGBA* pixels = malloc(imagePixelsSize(size));

    for (u_int32_t i = 0; i < (size.width * size.height); i++) {
        RGBA topColor = top[i], downColor = down[i];
        topColor.alpha *= topOpacity;
        downColor.alpha *= downOpacity;

        pixels[i] = mixer(topColor, downColor);
    }

    return pixels;
}

typedef struct {
    MixMode mode;
    double opacity;
    RGBA* pixels;
} Layer;

Layer layerOf(RGBA* pixels, double opacity, MixMode mode) {
    return (Layer) { mode, opacity, pixels };
}

typedef struct {
    u_long id;
    Size size;
    size_t format;
    u_int8_t* data; 
} ImageB64;

static u_long nextImageId = 0;

ImageB64 toB64(PixelMap data) {
    return (ImageB64) { nextImageId++, data.size, 32, encodeToB64(imagePixelsSize(data.size), (u_int8_t*)data.pixels) };
}   


ImageB64 toPngB64(PixelMap data) {
    int outLen;
    int w = (int)data.size.width;
    int h = (int)data.size.height;
    
    u_int8_t* pngData = stbi_write_png_to_mem((u_int8_t*)data.pixels, 0, w, h, 4, &outLen);
    u_int8_t* b64 = encodeToB64(outLen, pngData);
    
    free(pngData);

    return (ImageB64) { nextImageId++, data.size, 100, b64};
}


