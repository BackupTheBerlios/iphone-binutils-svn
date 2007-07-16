#include <stdio.h>

#define TODO_PREPROCESS             1
#define TODO_COMPILE_TO_BYTECODE    2
#define TODO_TRANSLATE_BYTECODE     4
#define TODO_ASSEMBLE               8
#define TODO_LINK                   16

struct arg {
    char *data;
    struct arg *next;
};

void gather_args(unsigned int *todo, struct arg **compiler_args, struct arg **
    linker_args)
{
     
}

int main()
{
    struct arg *compiler_args, *linker_args;
    unsigned int todo;

    gather_args(&todo, &compiler_args, &linker_args);

    return 0;
}

