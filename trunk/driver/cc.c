/* ----------------------------------------------------------------------------
 *   iphone-binutils: development tools for the Apple iPhone       07/16/2007
 *   Copyright (c) 2007 Patrick Walton <pcwalton@uchicago.edu> but freely
 *   redistributable under the terms of the GNU General Public License v2.
 *
 *   driver/cc.c: the interface to the iPhone build system 
 * ------------------------------------------------------------------------- */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    { "D",          FOR_COMPILER,   0,  1 },
    { "E",          FOR_DRIVER,     0,  0 },
    { "I",          FOR_COMPILER,   0,  1 },
    { "L",          FOR_LINKER,     0,  1 },
    { "O",          FOR_COMPILER,   0,  0 },
    { "O0",         FOR_COMPILER,   0,  0 },
    { "O1",         FOR_COMPILER,   0,  0 },
    { "O2",         FOR_COMPILER,   0,  0 },
    { "O3",         FOR_COMPILER,   0,  0 },
    { "Os",         FOR_COMPILER,   0,  0 },
    { "ObjC",       FOR_COMPILER,   0,  0 },
    { "S",          FOR_DRIVER,     0,  0 },
    { "U",          FOR_COMPILER,   0,  1 },
    { "V",          FOR_DRIVER,     0,  0 },
    { "W",          FOR_COMPILER,   0,  1 },
    { "ansi",       FOR_COMPILER,   0,  0 },
    { "arch",       FOR_IGNORE,     1,  0 },
    { "c",          FOR_DRIVER,     0,  0 },
    { "f",          FOR_COMPILER,   0,  1 },
    { "g",          FOR_COMPILER,   0,  0 },
    { "l",          FOR_LINKER,     0,  1 },
    { "o",          FOR_DRIVER,     1,  1 },
    { "pedantic",   FOR_COMPILER,   0,  0 },
    { "std",        FOR_COMPILER,   0,  1 },
    { "v",          FOR_DRIVER,     0,  0 },
    { "w",          FOR_COMPILER,   0,  0 }
};

char *prog_name;
int verbose_on = 0;
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
                else if (!strcmp(spec->name, "v"))
                    verbose_on = 1;
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

    /* by default, perform all stages of compilation */
    if (!*todo)
        *todo = TODO_PREPROCESS | TODO_COMPILE_TO_BYTECODE
            | TODO_TRANSLATE_BYTECODE | TODO_ASSEMBLE | TODO_LINK;
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
    char ch, *home, *key, *path, *val;
    int line_len;
    FILE *f;
    struct config_option *opt;

    if (!(home = getenv("HOME")))
        die("didn't find a $HOME environment variable");
    asprintf(&path, "%s/.arm-cc-specs", home);

    f = fopen(path, "r");
    if (!f)
        die("failed to open ~/.arm-cc-specs; see installation instructions");

    while (1) {
        line_len = 0;
        while ((ch = fgetc(f)) != '\n' && !feof(f))
            line_len++;

        if (feof(f))
            break;
        else
            fseek(f, -(line_len + 1), SEEK_CUR);
        
        key = (char *)malloc(line_len + 1);
        fread(key, line_len, 1, f);
        fgetc(f);   /* eat the trailing newline */
        key[line_len] = '\0';

        val = strchr(key, '=');
        if (!val) {
            free(key);
            continue;
        }

        *(val++) = '\0';

        opt = (struct config_option *)malloc(sizeof(struct config_option));
        opt->key = key;
        opt->value = val;
        opt->next = config;
        config = opt;
    }

    fclose(f);
}

void execute_step(char *prog_name_opt, char *flags_opt, struct arg_list *
    arg_list, int static_arg_count, ...)
{
    char **arg_array, *flags_line, **strh, *strp;
    int i, n_args = 1, term_status;
    va_list ap;
    struct arg *argp;

    flags_line = strdup(get_config_key(flags_opt, 1));
 
    strp = flags_line;
    while (*strp) {
        if (*strp == ' ')
            n_args++;
        strp++;
    }
    if (flags_line[0])
        n_args++;       /* get the first arg */

    if (arg_list) {
        argp = arg_list->first;
        while (argp) {
            n_args++;
            argp = argp->next;
        }
    }

    n_args += static_arg_count;
    
    arg_array = (char **)malloc(sizeof(char *) * (n_args + 1)); 
    arg_array[0] = get_config_key(prog_name_opt, 1); 

    strh = arg_array + 1; 

    if (flags_line[0])
        while ((*strh = strsep(&flags_line, " \t")) != NULL)
            if (**strh)
                strh++;

    if (arg_list) {
        argp = arg_list->first;
        while (argp) {
            *(strh++) = argp->data;
            argp = argp->next;
        }
    }

    va_start(ap, static_arg_count); 
    for (i = 0; i < static_arg_count; i++)
        *(strh++) = va_arg(ap, char *);
    va_end(ap);

    *strh = NULL;

    if (verbose_on) {
        for (i = 0; i < n_args; i++)
            fprintf(stderr, "%s ", arg_array[i]);
        fprintf(stderr, "\n");
    }

    if (fork()) {
        wait(&term_status);
        if (!(WIFEXITED(term_status) && WEXITSTATUS(term_status) == 0))
            exit(1);
    } else {
        execvp(arg_array[0], (char *const *)arg_array);
        exit(1);
    }
}

void prepare_outpath(unsigned int last_op, unsigned int this_op, char *suffix,
    char **outpath, char *template)
{
    if (last_op == this_op)
        *outpath = strdup(output_path);
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
            prepare_outpath(last_op, TODO_PREPROCESS, ".i", &outpath,
                template);
            execute_step("PREPROCESS", "CPPFLAGS", compiler_args, 3, "-o",
                outpath, inpath);

            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_COMPILE_TO_BYTECODE) {
            prepare_outpath(last_op, TODO_COMPILE_TO_BYTECODE, ".bc", &outpath,
                template);
            execute_step("LLVM_GCC", "CFLAGS", compiler_args, 4, "-c", "-o",
                outpath, inpath);
            
            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_TRANSLATE_BYTECODE) {
            prepare_outpath(last_op, TODO_TRANSLATE_BYTECODE, ".s", &outpath,
                template);
            execute_step("LLC", "LLCFLAGS", NULL, 3, "-o", outpath, inpath);
            
            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_ASSEMBLE) {
            prepare_outpath(last_op, TODO_ASSEMBLE, ".o", &outpath, template);
            execute_step("AS", "ASFLAGS", NULL, 3, "-o", outpath, inpath); 
            
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

            arg->next = linker_args->first;
            linker_args->first = arg;
        }

        in = in->next;
    }

    /* Run the linking step. */
    execute_step("LD", "LDFLAGS", linker_args, 2, "-o", output_path);
}

int main(int argc, char **argv)
{
    struct arg_list compiler_args, linker_args;
    unsigned int todo;

    prog_name = argv[0];

    read_config_file();
    gather_args(argc, argv, &todo, &compiler_args, &linker_args);
    perform_operations(todo, &compiler_args, &linker_args);

    return 0;
}

