#include <stdlib.h>
#include <sys/types.h>


static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

u_int8_t* encodeToB64(size_t lenght, const u_int8_t* data) {
    size_t dataSize = 4 * ((lenght + 2) / 3);
        u_int8_t* b64 = malloc(sizeof(u_int8_t) * (dataSize + 1));

    size_t i = 0, j = 0;
    while (i < lenght) {
        u_int32_t a = i < lenght ? data[i++] : 0;
        u_int32_t b = i < lenght ? data[i++] : 0;
        u_int32_t c = i < lenght ? data[i++] : 0;
        u_int32_t t = (a << 16) + (b << 8) + c;

        b64[j++] = b64_table[(t >> 18) & 0x3f];
        b64[j++] = b64_table[(t >> 12) & 0x3f];
        b64[j++] = b64_table[(t >> 6) & 0x3f];
        b64[j++] = b64_table[t & 0x3f];
    }

    for (size_t pad = 0; pad < (3 - lenght % 3) % 3; pad++)
        b64[dataSize - 1 - pad] = '=';

    b64[dataSize] = '\0';
    return b64;
}
