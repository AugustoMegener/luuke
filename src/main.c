#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <getopt.h>

#include "util.h"
#include "image.h"
#include "kgp.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char** argv) {

    check(optind < argc, "No file path passed", 1);

    char* filePath = argv[optind];

    PixelMapOption pngImageOption = loadPNG(filePath);
    
    check(pngImageOption.hasValue, "Can not load image", 1);

    PixelMap pngImage = pngImageOption.value;
    Size size = pngImage.size;

    PixelMap checkerImage = newPixelMap(size.width, size.height); 

    for (u_int32_t x = 0; x < size.width; x++) {
        for (u_int32_t y = 0; y < size.height; y++) {
            if ((x/10 + y/10)%2) setMapPixel(&checkerImage, x, y, rgb(0, 0, 0));
            else setMapPixel(&checkerImage, x, y, rgb(255, 255, 255));
        }
    }

    RGBA* imagePixels = mergePixels(0, size, 0.5, checkerImage.pixels, 1, pngImage.pixels);
    free(checkerImage.pixels);
    free(pngImage.pixels);

    char shm_name[64];
    sprintf(shm_name, "/luuke-img-%d.rgba", getpid());

    shm_unlink(shm_name);


    


    GCode* code = shmGCode(shm_name, 64, size);
    putFlag(code, actionFlag('c'));
    putFlag(code, imageIdFlag(1));

    fprintGCode(stdout, code); 
    free(code);

    GCode* wcode = newGCode();
    putFlag(wcode, actionFlag('p'));
    putFlag(wcode, imageIdFlag(1));

    fprintGCode(stdout, wcode); 
    free(wcode);

    //shm_unlink(shm_name);
    free(imagePixels);

    return 0;
}
