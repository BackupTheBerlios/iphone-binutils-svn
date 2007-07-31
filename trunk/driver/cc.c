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
#include <syslog.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TODO_PREPROCESS             1
#define TODO_COMPILE_TO_BYTECODE    2
#define TODO_OPTIMIZE_BYTECODE      4 
#define TODO_TRANSLATE_BYTECODE     8
#define TODO_ASSEMBLE               16
#define TODO_LINK                   32

#define FOR_DRIVER                  0
#define FOR_COMPILER                1
#define FOR_LINKER                  2
#define FOR_LLC                     3 
#define FOR_IGNORE                  4

#define LANG_C                      0
#define LANG_CPP                    1
#define LANG_OBJC                   2 

struct arg_spec {
    char *name;
    int for_what;
    int takes_arg;
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
    int lang;

    struct input_file *next;
};

struct config_option {
    char *key;
    char *value;

    struct config_option *next;
};

struct temp_file {
    char *path;

    struct temp_file *next;
};

/* Keep these in asciibetical order - they will be bsearch()'d. */
struct arg_spec exact_arg_specs[] = {
    { "E",                      FOR_DRIVER,     0 },
    { "O0",                     FOR_COMPILER,   0 },
    { "ObjC",                   FOR_DRIVER,     0 },
    { "S",                      FOR_DRIVER,     0 },
    { "V",                      FOR_DRIVER,     0 },
    { "ansi",                   FOR_COMPILER,   0 },
    { "arch",                   FOR_IGNORE,     1 },
    { "c",                      FOR_DRIVER,     0 },
    { "dumpversion",            FOR_DRIVER,     0 },
    { "dynamic",                FOR_DRIVER,     0 },
    { "dynamiclib",             FOR_DRIVER,     0 },
    { "framework",              FOR_DRIVER,     1 },
    { "g",                      FOR_COMPILER,   0 },
    { "keep_private_externs",   FOR_LINKER,     0 },
    { "pedantic",               FOR_COMPILER,   0 },
    { "r",                      FOR_LINKER,     0 },
    { "v",                      FOR_DRIVER,     0 },
    { "w",                      FOR_COMPILER,   0 }
};

struct arg_spec partial_arg_specs[] = {
    { "D",                      FOR_COMPILER,   0 },
    { "I",                      FOR_COMPILER,   0 },
    { "L",                      FOR_LINKER,     0 },
    { "O",                      FOR_DRIVER,     0 },
    { "U",                      FOR_COMPILER,   0 },
    { "W",                      FOR_COMPILER,   0 },
    { "f",                      FOR_COMPILER,   0 },
    { "l",                      FOR_LINKER,     0 },
    { "o",                      FOR_DRIVER,     1 },
    { "relocation-model",       FOR_LLC,        0 },
    { "std",                    FOR_COMPILER,   0 }
};

char *prog_name;
int verbose_on = 0, make_dylib = 0;
struct input_file *input_files = NULL;
char *output_path = NULL;
struct config_option *config = NULL;
struct arg_list *llc_args;
struct temp_file *temp_files = NULL;

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

int compare_args_exactly(const void *i_key, const void *i_arg_spec)
{
    char *key;
    size_t key_len, spec_len;
    struct arg_spec *spec;

    key = (char *)i_key; spec = (struct arg_spec *)i_arg_spec;
    key_len = strlen(key); spec_len = strlen(spec->name);

    return strcmp(key, spec->name);
}

int compare_args_partially(const void *i_key, const void *i_arg_spec)
{
    char *key;
    size_t key_len, spec_len;
    struct arg_spec *spec;

    key = (char *)i_key; spec = (struct arg_spec *)i_arg_spec;
    key_len = strlen(key); spec_len = strlen(spec->name);

    if (key_len > spec_len)
        return strncmp(key, spec->name, spec_len);
    else 
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

void add_input_file(char *path, struct arg_list *compiler_args)
{
    char *ext;
    int lang = LANG_C;
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
                lang = (ext[2] == 'p' ? LANG_CPP : LANG_C);
                todo = TODO_PREPROCESS | TODO_COMPILE_TO_BYTECODE |
                    TODO_OPTIMIZE_BYTECODE | TODO_TRANSLATE_BYTECODE |
                    TODO_ASSEMBLE | TODO_LINK;
                break;
            case 'm':
                todo = TODO_PREPROCESS | TODO_COMPILE_TO_BYTECODE |
                    TODO_OPTIMIZE_BYTECODE | TODO_TRANSLATE_BYTECODE |
                    TODO_ASSEMBLE | TODO_LINK;
                lang = LANG_OBJC;
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
    in->lang = lang;
    
    in->next = input_files;
    input_files = in;
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

void gather_args(int argc, char **argv, unsigned int *todo, struct arg_list *
    compiler_args, struct arg_list *linker_args)
{
    char *val;
    int i;
    int want_optimization = 0;
    struct arg_list *this_arg_list;
    struct arg_spec *spec;

    *todo = 0;
    compiler_args->first = compiler_args->last = NULL;
    linker_args->first = linker_args->last = NULL;
    llc_args = (struct arg_list *)calloc(sizeof(struct arg_list), 1);

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            spec = (struct arg_spec *)bsearch(argv[i] + 1, exact_arg_specs,
                sizeof(exact_arg_specs) / sizeof(struct arg_spec), sizeof(
                struct arg_spec), compare_args_exactly);
            if (!spec) {
                spec = (struct arg_spec *)bsearch(argv[i] + 1,
                    partial_arg_specs, sizeof(partial_arg_specs) / sizeof(
                    struct arg_spec), sizeof(struct arg_spec),
                    compare_args_partially);
                if (!spec)
                    die("unknown argument '%s'", argv[i] + 1);
            }
           
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
                else if (!strcmp(spec->name, "dynamiclib"))
                    make_dylib = 1;
                else if (!strcmp(spec->name, "dynamic"))
                    make_dylib = 1;
                else if (!strcmp(spec->name, "framework")) {
                    create_arg(linker_args, get_config_key(
                        "LDFLAGS_FRAMEWORKSDIR", 1));
                    create_arg(linker_args, "-framework");
                    create_arg(linker_args, argv[++i]);
                } else if (!strcmp(spec->name, "dumpversion")) {
                    val = get_config_key("LLVM_GCC", 1);
                    execlp(val, val, "-dumpversion", NULL);
                } else if (!strcmp(spec->name, "ObjC")) {
                    create_arg(linker_args, "-ObjC");
                    create_arg(linker_args, "-lobjc");
                } else if (!strcmp(spec->name, "O")) {
                    create_arg(compiler_args, get_config_key(
                        "CFLAGS_COMPILEROPT", 1));
                    want_optimization = 1;
                }
            } else {
                switch (spec->for_what) {
                    case FOR_COMPILER:
                        this_arg_list = compiler_args;
                        break;
                    case FOR_LINKER:
                        this_arg_list = linker_args;
                        break;
                    case FOR_LLC:
                        this_arg_list = llc_args;
                }

                create_arg(this_arg_list, argv[i]);

                if (spec->takes_arg && i < argc - 1)
                    create_arg(this_arg_list, argv[++i]);
            }
        } else 
            add_input_file(argv[i], compiler_args); 
    } 

    /* by default, perform all stages of compilation except optimization */
    if (!*todo)
        *todo = TODO_PREPROCESS | TODO_COMPILE_TO_BYTECODE
            | TODO_TRANSLATE_BYTECODE | TODO_ASSEMBLE | TODO_LINK;

    if (want_optimization)
        *todo |= TODO_OPTIMIZE_BYTECODE;
}

char * get_full_path(char *prog_name)
{
    char *path, *dir, *test_path;

    if (prog_name[0] == '/' || prog_name[0] == '.')
        return strdup(prog_name);

    path = getenv("PATH");
    if (!path)
        return NULL;
    for (dir = strtok(path, ":"); dir != NULL; dir = strtok(NULL, ":")) {
        asprintf(&test_path, "%s/%s", dir, prog_name);
        if (!access(test_path, X_OK)) {
            return test_path;
        }
        free(test_path);
    }

    return NULL;
}

char * find_config_file_path(char * prog_name)
{
    char *home, *path, *cfg_dir;
    char resolved_path[PATH_MAX + 1];

    // Check user's config
    if (!(home = getenv("HOME")))
        die("didn't find a $HOME environment variable");
    asprintf(&path, "%s/.arm-cc-specs", home);
    if (!access(path, R_OK))
        return path;

    // Check general config
    path = get_full_path(prog_name);
    if (!path)
        return NULL;
    if (!realpath(path, resolved_path))
        return NULL;
    free(path);
    path = dirname(resolved_path);
    asprintf(&cfg_dir, "%s/../etc/arm-cc-specs", path);
    free(path);
    if (!realpath(cfg_dir, resolved_path))
        return NULL;
    free(cfg_dir);
    if (access(resolved_path, R_OK))
        return NULL;
    return strdup(resolved_path);
}

void read_config_file(char * prog_name)
{
    char ch, *key, *path, *val;
    int line_len;
    FILE *f;
    struct config_option *opt;

    path = find_config_file_path(prog_name);
    if (!path)
        die("Failed to locate the arm-cc-specs config file; see installation "
            "instructions");

    f = fopen(path, "r");
    if (!f)
        die("failed to open arm-cc-specs; see installation instructions");

    free(path);

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
        while ((*strh = strsep(&flags_line, " \t")) != NULL) {
            if (**strh)
                strh++;

            if (flags_line && flags_line[0] == '%') {
                strsep(&flags_line, " \t");
                break;
            }
        }

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

    if (flags_line && flags_line[0])
        while ((*strh = strsep(&flags_line, " \t")) != NULL) {
            if (**strh)
                strh++;
        }

    if (verbose_on) {
        for (i = 0; i < n_args; i++) {
            fprintf(stderr, "%s ", arg_array[i]);
        }
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

void assign_default_output_path(unsigned int last_op, char *i_infile)
{
    char *infile, *inptr, *strp;

    if (last_op == TODO_LINK)
        output_path = "a.out";
    else {
        infile = strdup(i_infile);
        if ((strp = strrchr(infile, '.')))
            *strp = '\0';   /* chop off the extension, if present */

        /* Remove any path specification. */
        if ((inptr = strrchr(infile, '/')))
            inptr++;
        else
            inptr = infile; 

        switch (last_op) {
            case TODO_PREPROCESS:
                output_path = "/dev/fd/1";  /* FIXME: make this portable */
                break;
            case TODO_COMPILE_TO_BYTECODE:
                asprintf(&output_path, "%s.bc", inptr);
                break;
            case TODO_TRANSLATE_BYTECODE:
                asprintf(&output_path, "%s.s", inptr);
                break;
            case TODO_ASSEMBLE:
                asprintf(&output_path, "%s.o", inptr);
        }

        free(infile);
    }
}

void prepare_outpath(unsigned int last_op, unsigned int this_op, char *suffix,
    char **outpath, char *template)
{
    struct temp_file *tf;

    if (last_op == this_op)
        *outpath = strdup(output_path);
    else {
        asprintf(outpath, "%s%s", template, suffix); 

        tf = (struct temp_file *)malloc(sizeof(struct temp_file));
        tf->path = strdup(*outpath);
        tf->next = temp_files;
        temp_files = tf; 
    }
}

void perform_operations(unsigned int req_todo, struct arg_list *compiler_args,
    struct arg_list *linker_args)
{
    char *inpath, *outpath, *template, *tmp;
    int need_default_output_path;
    struct arg *arg;
    struct input_file *in;
    unsigned int last_op, n, todo;

    need_default_output_path = (output_path == NULL);

    in = input_files;
    while (in) {
        todo = (req_todo & (in->todo));

        n = todo; last_op = 1;
        if (!todo)
            break;
        while (n != 1) {
            n = (n >> 1);
            last_op = (last_op << 1);
        }
    
        template = strdup("/tmp/XXXXXX");
        mktemp(template);

        inpath = strdup(in->path);
        outpath = inpath;

        if (need_default_output_path)
            assign_default_output_path(last_op, inpath); 

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

            switch (in->lang) {
                case LANG_C:
                    execute_step("LLVM_GCC", "CFLAGS", compiler_args, 4, "-c",
                        "-o", outpath, inpath);
                    break;
                case LANG_CPP:
                    execute_step("LLVM_GCCC", "CCFLAGS", compiler_args, 4,
                        "-c", "-o", outpath, inpath);
                    break;
                case LANG_OBJC:
                    execute_step("LLVM_GCC", "CFLAGS_OBJC", compiler_args, 4,
                        "-c", "-o", outpath, inpath);
            }
                    
            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_OPTIMIZE_BYTECODE) {
            prepare_outpath(last_op, TODO_OPTIMIZE_BYTECODE, ".opt.bc",
                &outpath, template);

            asprintf(&tmp, "-o=%s", outpath);
            execute_step("OPT", "OPTFLAGS", NULL, 2, tmp, inpath);
            free(tmp);

            free(inpath);
            inpath = outpath;
        }

        if (todo & TODO_TRANSLATE_BYTECODE) {
            prepare_outpath(last_op, TODO_TRANSLATE_BYTECODE, ".s", &outpath,
                template);
            execute_step("LLC", make_dylib ? "LLCFLAGS_DYLIB" : "LLCFLAGS",
                llc_args, 3, "-o", outpath, inpath);
            
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
    if (req_todo & TODO_LINK)
        execute_step("LD", make_dylib ? "LDFLAGS_DYLIB" : "LDFLAGS",
            linker_args, 2, "-o", output_path);
}

void clean_up_temporary_files()
{
    struct temp_file *tf;

    tf = temp_files;
    while (tf) {
        if (verbose_on)
            fprintf(stderr, "rm %s\n", tf->path);

        unlink(tf->path); 

        tf = tf->next;
    }

    /* no need to free since we're exiting anyway */
}

int main(int argc, char **argv)
{
    struct arg_list compiler_args, linker_args;
    unsigned int todo;

#if 0
    int i;
    for (i = 0; i < argc; i++)
        syslog(LOG_ERR, "got arg: %s\n", argv[i]);
#endif

    prog_name = argv[0];

    read_config_file(prog_name);
    gather_args(argc, argv, &todo, &compiler_args, &linker_args);
    perform_operations(todo, &compiler_args, &linker_args);
    clean_up_temporary_files();

    return 0;
}

