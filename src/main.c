#include "usagic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>

#ifdef _WIN32
#ifndef S_ISREG
#define S_ISREG(mode) (((mode) & _S_IFMT) == _S_IFREG)
#endif
#endif

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

static void usage(FILE *stream) {
    fprintf(stream,
        "UsagiC %s — native C11 transpiler\n"
        "Usage:\n"
        "  usagic transpile <file.usagic> [-o output.c] [--force]\n"
        "  usagic build <file.usagic> [-o executable] [--cc gcc|clang|cl] [--force]\n"
        "  usagic --version\n"
        "Default output: <file.usagic.c> and a native executable.\n"
        "Custom -o paths are protected from overwrites unless --force is set.\n",
        USAGIC_VERSION);
}

static int ends_with(const char *s, const char *suffix) {
    size_t a = strlen(s), b = strlen(suffix);
    return a >= b && strcmp(s + a - b, suffix) == 0;
}

static char *path_with_extension(const char *base, const char *extension) {
    size_t a = strlen(base), b = strlen(extension);
    if (a > SIZE_MAX - b - 1) return NULL;
    char *path = malloc(a + b + 1);
    if (!path) return NULL;
    memcpy(path, base, a);
    memcpy(path + a, extension, b + 1);
    return path;
}

static char *exe_name(const char *filename) {
    size_t len = strlen(filename) - strlen(".usagic");
#ifdef _WIN32
    const char *extension = ".exe";
#else
    const char *extension = "";
#endif
    size_t add = strlen(extension);
    char *result = malloc(len + add + 1);
    if (!result) return NULL;
    memcpy(result, filename, len);
    memcpy(result + len, extension, add + 1);
    return result;
}

static int read_source(const char *path, char **data, size_t *size) {
    FILE *file = fopen(path, "rb");
    if (!file) { perror(path); return 0; }
    if (fseek(file, 0, SEEK_END) != 0) { fclose(file); return 0; }
    long length = ftell(file);
    if (length < 0 || length > 16L * 1024L * 1024L) {
        fprintf(stderr, "usagic: input must be 16 MiB or smaller\n");
        fclose(file); return 0;
    }
    rewind(file);
    *size = (size_t)length;
    *data = malloc(*size + 1);
    if (!*data) { fclose(file); return 0; }
    if (fread(*data, 1, *size, file) != *size) {
        fprintf(stderr, "usagic: could not read %s\n", path);
        free(*data); *data = NULL; fclose(file); return 0;
    }
    (*data)[*size] = '\0';
    fclose(file);
    if (memchr(*data, '\0', *size)) {
        fprintf(stderr, "usagic: NUL byte in source file\n");
        free(*data); *data = NULL; return 0;
    }
    return 1;
}

static int write_source(const char *path, const char *source) {
    FILE *file = fopen(path, "wb");
    if (!file) { perror(path); return 0; }
    size_t size = strlen(source);
    int ok = fwrite(source, 1, size, file) == size;
    if (fclose(file) != 0) ok = 0;
    if (!ok) { fprintf(stderr, "usagic: write failed: %s\n", path); return 0; }
    return 1;
}

static int is_msvc(const char *cc) {
    const char *part = cc;
    for (const char *p = cc; *p; ++p) if (*p == '/' || *p == '\\') part = p + 1;
    char buf[16]; size_t i = 0;
    for (; part[i] && i + 1 < sizeof buf; ++i)
        buf[i] = (char)tolower((unsigned char)part[i]);
    buf[i] = '\0';
    return strcmp(buf, "cl") == 0 || strcmp(buf, "cl.exe") == 0 ||
           strcmp(buf, "msvc") == 0;
}

static int run_compiler(const char *cc, const char *source, const char *exe) {
    char *msvc_output = NULL;
    const char *compiler = strcmp(cc, "msvc") == 0 ? "cl" : cc;
    const char *gnu_argv[] = {compiler, "-std=c11", "-O2", "-Wall", "-Wextra",
                              "-x", "c", source, "-o", exe, NULL};
    const char *msvc_argv[] = {compiler, "/nologo", "/std:c11", "/utf-8",
                              "/W4", "/O2", "/TC", source, NULL, NULL};
    const char *const *argv = gnu_argv;
    if (is_msvc(cc)) {
        msvc_output = path_with_extension("/Fe:", exe);
        if (!msvc_output) { fprintf(stderr, "usagic: out of memory\n"); return 1; }
        msvc_argv[8] = msvc_output;
        argv = msvc_argv;
    }
#ifdef _WIN32
    intptr_t status = _spawnvp(_P_WAIT, compiler, argv);
    if (status == -1) { perror(compiler); free(msvc_output); return 1; }
    free(msvc_output);
    return (int)status;
#else
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); free(msvc_output); return 1; }
    if (pid == 0) {
        execvp(compiler, (char *const *)argv);
        perror(compiler);
        _exit(127);
    }
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid"); free(msvc_output); return 1;
    }
    free(msvc_output);
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    fprintf(stderr, "usagic: compiler terminated by signal\n");
    return 1;
#endif
}

static int get_stat(const char *path, struct stat *out) {
    return stat(path, out) == 0;
}
static int same_file(const char *a, const char *b) {
    if (!strcmp(a, b)) return 1;
#ifdef _WIN32
    /* MSVC _stat does not expose a reliable per-file inode number. */
    char full_a[_MAX_PATH], full_b[_MAX_PATH];
    if (!_fullpath(full_a, a, sizeof full_a) ||
        !_fullpath(full_b, b, sizeof full_b)) return 0;
    return _stricmp(full_a, full_b) == 0;
#else
    struct stat sa, sb;
    if (!get_stat(a, &sa) || !get_stat(b, &sb)) return 0;
    return sa.st_dev == sb.st_dev && sa.st_ino == sb.st_ino;
#endif
}
/* Recognize and safely regenerate previously generated C source files. */
static int is_generated_c(const char *path) {
    static const char marker[] = "/* Generated by UsagiC ";
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    char buf[sizeof marker];
    size_t n = fread(buf, 1, sizeof marker - 1, f);
    fclose(f);
    return n == sizeof marker - 1 &&
           memcmp(buf, marker, sizeof marker - 1) == 0;
}
static int output_allowed(const char *path, int force,
                          int auto_generated_c, int default_exe) {
    struct stat info;
    if (!get_stat(path, &info)) {
        if (errno == ENOENT) return 1;
        fprintf(stderr, "usagic: cannot inspect output %s: %s\n",
                path, strerror(errno));
        return 0;
    }
    if (!S_ISREG(info.st_mode)) {
        fprintf(stderr, "usagic: output is not a regular file: %s\n", path);
        return 0;
    }
    if (force || (auto_generated_c && is_generated_c(path)) || default_exe)
        return 1;
    fprintf(stderr, "usagic: refusing to overwrite existing file %s (use --force)\n", path);
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        printf("UsagiC %s\n", USAGIC_VERSION);
        return 0;
    }
    if (argc == 1 || (argc == 2 &&
        (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "help") == 0))) {
        usage(stdout); return 0;
    }
    if (argc < 3 || (strcmp(argv[1], "transpile") != 0 && strcmp(argv[1], "build") != 0)) {
        usage(stderr); return 2;
    }
    int build = strcmp(argv[1], "build") == 0;
    const char *filename = argv[2], *output = NULL, *cc = "gcc";
    int force = 0;
    if (!ends_with(filename, ".usagic")) {
        fprintf(stderr, "usagic: input must end in .usagic\n"); return 2;
    }
    for (int i = 3; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc && !output) {
            output = argv[++i];
        } else if (strcmp(argv[i], "--cc") == 0 && i + 1 < argc && build) {
            cc = argv[++i];
        } else if (strcmp(argv[i], "--force") == 0) {
            force = 1;
        } else {
            fprintf(stderr, "usagic: unknown or incomplete argument: %s\n", argv[i]);
            return 2;
        }
    }
    char *generated = path_with_extension(filename, ".c");
    char *default_exe = build ? exe_name(filename) : NULL;
    const char *source_path = build ? generated : (output ? output : generated);
    const char *binary_path = output ? output : default_exe;
    if (!generated || (build && !default_exe)) {
        fprintf(stderr, "usagic: out of memory\n");
        free(generated); free(default_exe); return 1;
    }
    if (same_file(filename, source_path) ||
        (build && (same_file(filename, binary_path) || same_file(source_path, binary_path)))) {
        fprintf(stderr, "usagic: output must not overwrite input or generated C\n");
        free(generated); free(default_exe); return 2;
    }
    if (!output_allowed(source_path, force, source_path == generated, 0) ||
        (build && !output_allowed(binary_path, force, 0, !output))) {
        free(generated); free(default_exe); return 2;
    }
    char *input = NULL;
    size_t input_size = 0;
    if (!read_source(filename, &input, &input_size)) {
        free(generated); free(default_exe); return 1;
    }
    char error[512];
    char *c_source = usagic_transpile(input, input_size, filename, error, sizeof error);
    free(input);
    if (!c_source) {
        fprintf(stderr, "usagic: %s\n", error);
        free(generated); free(default_exe); return 1;
    }
    int ok = write_source(source_path, c_source);
    free(c_source);
    if (!ok) { free(generated); free(default_exe); return 1; }
    printf("UsagiC: %s -> %s\n", filename, source_path);
    int code = 0;
    if (build) {
        code = run_compiler(cc, source_path, binary_path);
        if (code == 0) printf("UsagiC: built %s\n", binary_path);
        else fprintf(stderr, "usagic: C compilation failed (exit %d)\n", code);
    }
    free(generated); free(default_exe);
    return code;
}
