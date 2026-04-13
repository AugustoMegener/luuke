#define check(check, msg, code)     \
    if (!(check)) {                 \
        printf("Luuke: %s", msg);   \
        perror("Error: ");          \
        return code;                \
    }                               \


#define checked(type, name, value, ck, msg, code)	\
    type name = value;                              \
    check(ck, msg, code)                            \

#define withMAlloc(type, name, size, block) \
    do {                                    \
        type* name = malloc(size);          \
        block;                              \
        free(name);                         \
    } while(0)                              \


#define freeAfter(type, name, value, block) \
    do {                                    \
        type* name = value; 				\
        block;                              \
        free(name);                         \
    } while(0)                              \


#define withPipe(name, cmd, mode, block)                                                        \
    do {                                                                                        \
        checked(FILE*, name, popen(cmd, mode), name != NULL, "Can not create a new pipe", 0)    \
        block;                                                                                  \
        pclose(name);                                                                           \
    } while(0);


#define withFile(name, path, mode, block)                                               \
    do {                                                                                \
        checked(FILE*, name, fopen(path, mode), name != NULL, "Can not open file", 1); 	\
        block;                                                                          \
        fclose(name);                                                                   \
    } while(0);                                                                         \

#define Option(type) struct { bool hasValue; type value;}
