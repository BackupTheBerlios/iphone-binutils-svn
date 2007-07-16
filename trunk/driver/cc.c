#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_PREPROCESS             1
#define TODO_COMPILE_TO_BYTECODE    2
#define TODO_TRANSLATE_BYTECODE     4
#define TODO_ASSEMBLE               8
#define TODO_LINK                   16

#define FOR_DRIVER                  0
#define FOR_COMPILER                1
#define FOR_LINKER                  2
#define FOR_IGNORE                  3

struct arg_spec {
    char *name;
    int for_what;
    int takes_arg;
    int partial_match_ok;
};

struct arg {
    char *data;
    struct arg *next;
};

struct arg_list {
    struct arg *first;
    struct arg *last;
};

struct input_file {
    char *path;
    unsigned int todo;

    struct input_file *next;
};

struct config_option {
    char *key;
    char *value;

    struct config_option *next;
};

/* Keep this in asciibetical order, because it'll be bsearch()'d. */
struct arg_spec arg_specs[] = {
    { "ansi",       FOR_COMPILER,   0,  0 },
    { "arch",       FOR_IGNORE,     1,  0 },
    { "c",          FOR_DRIVER,     0,  0 },
    { "f",          FOR_COMPILER,   0,  1 },
    { "g",          FOR_COMPILER,   0,  0 },
    { "l",          FOR_LINKER,     0,  1 },
    { "o",          FOR_DRIVER,     1,  1 },
    { "pedantic",   FOR_COMPILER,   0,  0 },
    { "std",        FOR_COMPILER,   0,  1 },
    { "w",          FOR_COMPILER,   0,  0 },
    { "D",          FOR_COMPILER,   0,  1 },
    { "E",          FOR_DRIVER,     0,  0 },
    { "I",          FOR_COMPILER,   0,  1 },
    { "L",          FOR_LINKER,     0,  1 },
    { "O",          FOR_COMPILER,   0,  0 },
    { "O1",         FOR_COMPILER,   0,  0 },
    { "O2",         FOR_COMPILER,   0,  0 },
    { "O3",         FOR_COMPILER,   0,  0 },
    { "Os",         FOR_COMPILER,   0,  0 },
    { "ObjC",       FOR_COMPILER,   0,  0 },
    { "S",          FOR_DRIVER,     0,  0 },
    { "U",          FOR_COMPILER,   0,  1 },
    { "V",          FOR_DRIVER,     0,  0 },
    { "W",          FOR_COMPILER,   0,  1 }
};

struct input_file *input_files = NULL;
char *output_path = "a.out";
struct config_option *config = NULL;

void create_arg(struct arg_list *arg_list, char *data)
{
    struct arg *arg;

    arg = (struct arg *)malloc(sizeof(struct arg));
    arg->data = data;

    if (arg_list->last)
        arg_list->last->next = arg;
    else
        arg_list->first = arg;

    arg_list->last = arg;
    arg->next = NULL;
}

int compare_args(const void *i_key, const void *i_arg_spec)
{
    char *key;
    size_t len;
    struct arg_spec *spec;

    key = (char *)i_key; spec = (struct arg_spec *)i_arg_spec;

    if (spec->partial_match_ok) {
        len = strlen(key);
        if (len > strlen(spec->name))
            return -1;
        return strncmp(key, spec->name, len);
    }
 
    return strcmp(key, spec->name);
}

void die(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr, "%s: ", prog_name);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");

    exit(1);
}

void add_input_file(char *path)
{
    char *ext;
    size_t len;
    unsigned int todo;
    struct input_file *in;

    /* Determine the set of operations we're willing to perform on this file.
     * It'll be AND'd with the operations we were actually told to perform to
     * get the final set of operations to do. */
    ext = strrchr(path, '.');
    if (!ext)
        todo = TODO_LINK;
    else
        switch (ext[1]) {
            case 'c':
            case 'C':
                todo = TODO_PREPROCESS | TODO_COMPILE_TO_BYTECODE |
                    TODO_TRANSLATE_BYTECODE | TODO_ASSEMBLE | TODO_LINK;
                break;
            case 's':
            case 'S':
                todo = TODO_PREPROCESS | TODO_ASSEMBLE | TODO_LINK;
                break;
            case 'o':
            default:
                todo = TODO_LINK;
        }

    in = (struct input_file *)malloc(sizeof(struct input_file));
    in->path = path;
    in->todo = todo;
    
    in->next = input_files;
    input_files = in;
}

void gather_args(int argc, char **argv, unsigned int *todo, struct arg_list *
    compiler_args, struct arg_list *linker_args)
{
    int i;
    struct arg_list *this_arg_list;
    struct arg_spec *spec;

    *todo = 0;
    compiler_args->first = compiler_args->last = NULL;
    linker_args->first = linker_args->last = NULL;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            spec = (struct arg_spec *)bsearch(argv[i] + 1, arg_specs,
                sizeof(arg_specs) / sizeof(struct arg_spec), sizeof(struct
                arg_spec), compare_args);
            if (!spec)
                die("unknown argument '%s'", argv[i] + 1);
           
            if (spec->for_what == FOR_DRIVER) {
                if (!strcmp(spec->name, "E"))
                    *todo = TODO_PREPROCESS;
                else if (!strcmp(spec->name, "S"))
                    *todo = TODO_PREPROCESS | TODO_COMPILE_TO_BYTECODE |
                        TODO_TRANSLATE_BYTECODE;
                else if (!strcmp(spec->name, "c"))
                    *todo = TODO_PREPROCESS | TODO_COMPILE_TO_BYTECODE |
                        TODO_TRANSLATE_BYTECODE | TODO_ASSEMBLE;
                else if (!strcmp(spec->name, "o") && i < argc - 1)
                    output_path = argv[++i];
                break;
            } else {
                switch (spec->for_what) {
                    case FOR_COMPILER:
                        this_arg_list = compiler_args;
                        break;
                    case FOR_LINKER:
                        this_arg_list = linker_args;
                }

                create_arg(this_arg_list, argv[i]);

                if (spec->takes_arg && i < argc - 1)
                    create_arg(this_arg_list, argv[++i]);
            }
        } else
            add_input_file(argv[i]); 
    } 
}

char *get_config_key(char *key, int die_on_error)
{
    struct config_option *opt;

    /* linear search... but who really cares? */
    opt = config;
    while (opt) {
        if (!strcmp(key, opt->key))
            return opt->value;
        opt = opt->next;
    }

    if (die_on_error)
        die("required configuration setting '%s' is not present", key);

    return NULL;
}

void read_config_file()
{
    char *home, *path;
    FILE *f;
    struct config_option *opt;

    if (!(home = getenv("HOME")))
        die("didn't find a $HOME environment variable");
    asprintf(&path, "%s/.arm-cc-specs", home);

    f = fopen(path, "r");
    if (!f)
        die("failed to open ~/.arm-cc-specs; see installation instructions");

    while (!feof(f)) {
        opt = (struct config_option *)malloc(sizeof(struct config_option));
        opt->key = (char *)malloc(256);
        opt->value = (char *)malloc(256);
        if (fscanf(path, "%255[A-Za-z_]=%255s\n", &(opt->name), &(opt->value))
            < 2)
            free(opt);
        else {
            opt->next = config;
            config = opt;
        }
    }

    fclose(f);        
}

void execute_step(char *prog_name_opt, char *flags_opt, struct arg *arg_list,
    ...)
{
}

void prepare_outpath(unsigned int last_op, unsigned int this_op, char *suffix,
    char **outpath, char *template)
{
    if (last_op == this_op)
        *outpath = strdup(*output_path);
    else
        asprintf(outpath, "%s%s", template, suffix); 
}

void perform_operations(unsigned int req_todo, struct arg_list *compiler_args,
    struct arg_list *linker_args)
{
    char *inpath, *outpath, *template;
    struct arg *arg;
    struct input_file *in;
    unsigned int last_op, n, todo;

    in = input_files;
    while (in) {
        todo = (req_todo & (in->todo));

        n = todo; last_op = 1;
        while (n != 1) {
            n = (n >> 1);
            last_op = (last_op << 1);
        }
    
        template = strdup("/tmp/XXXXXX");
        mktemp(template);

        inpath = strdup(in->path);

        if (todo & TODO_PREPROCESS) {
            prepare_outpath(last_op, TODO_PREPROCESS, ".cxx", &outpath,
                template);
            execute_step("PREPROCESS", "CFLAGS", compiler_args, "-o", outpath,
                inpath, NULL);

            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_COMPILE_TO_BYTECODE) {
            prepare_outpath(last_op, TODO_COMPILE_TO_BYTECODE, ".bc", &outpath,
                template);
            execute_step("LLVM_GCC", "CFLAGS", compiler_args, "-c", "-o",
                outpath, inpath, NULL);
            
            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_TRANSLATE_BYTECODE) {
            prepare_outpath(last_op, TODO_TRANSLATE_BYTECODE, ".s", &outpath,
                template);
            execute_step("LLC", "LLCFLAGS", NULL, "-o", outpath, inpath, NULL);
            
            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_ASSEMBLE) {
            prepare_outpath(last_op, TODO_ASSEMBLE, ".o", &outpath, template);
            execute_step("AS", "ASFLAGS", NULL, "-o", outpath, inpath, NULL); 
            
            free(inpath);
            inpath = outpath;
        }
         
        free(template);

        /* Change the input file's name to that of the object file so that the
         * we will know where to find it during the linking step. */
        if (todo & TODO_LINK) 
            in->path = outpath;

        in = in->next;
    }

    /* Build up the linking step args. */
    in = input_files;
    while (in) {
        todo = (req_todo & (in->todo));
       
        if (todo & TODO_LINK) {
            arg = (struct arg *)malloc(sizeof(struct arg));
            arg->data = in->path;

            arg->next = linker_args;
            linker_args = arg;
        }

        in = in->next;
    }

    /* Run the linking step. */
    execute_step("LD", "LDFLAGS", linker_args, "-o", output_path, NULL);
}

int main(int argc, char **argv)
{
    struct arg_list compiler_args, linker_args;
    unsigned int todo;

    read_config_file();
    gather_args(argc, argv, &todo, &compiler_args, &linker_args);
    perform_operations(todo, &compiler_args, &linker_args);

    return 0;
}

