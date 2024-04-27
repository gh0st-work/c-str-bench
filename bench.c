#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ITERS 10000

typedef struct { char* ptr; size_t len; } Str1;
typedef struct { char* ptr; char* ptr_end; } Str2;
    
char a_buf[(10+1 + 25+1 + 50+1 + 100+1 + 250+1 + 500+1)*ITERS*(2+2+2)] = {};
char* a_ptr = a_buf;

char* a_alloc(size_t size) {
    char* old_a_ptr = a_ptr;
    a_ptr += size;
    return old_a_ptr;
}



// --- 0 ----------------------------------------------------------------------
char* str_cat_clone_0_l(size_t str_1_len, char* str_1, size_t str_2_len, char* str_2) {
    size_t str_res_m_len = (str_1_len+str_2_len+1);
    char* str_res = a_alloc(str_res_m_len);
    memmove(str_res, str_1, str_1_len);
    memmove(str_res+str_1_len, str_2, str_2_len);
    str_res[str_1_len+str_2_len] = 0;
    return str_res;
}
char* str_cat_clone_0_nl(char* str_1, char* str_2) {
    size_t str_1_len = strlen(str_1);
    size_t str_2_len = strlen(str_2);
    size_t str_res_m_len = (str_1_len+str_2_len+1);
    char* str_res = a_alloc(str_res_m_len);
    memmove(str_res, str_1, str_1_len);
    memmove(str_res+str_1_len, str_2, str_2_len);
    str_res[str_1_len+str_2_len] = 0;
    return str_res;
}
void test_0_l(char** strs, size_t str_1_len, char* str_1, size_t str_2_len, char* str_2) {
    for (size_t i = 0; i < ITERS; ++i) {
        strs[i] = str_cat_clone_0_l(str_1_len, str_1, str_2_len, str_2);
    }
}
void test_0_nl(char** strs, char* str_1, char* str_2) {
    for (size_t i = 0; i < ITERS; ++i) {
        strs[i] = str_cat_clone_0_nl(str_1, str_2);
    }
}



// --- 1 ----------------------------------------------------------------------
Str1 str_cat_clone_1_nt(Str1 str_1, Str1 str_2) {
    size_t str_res_len = str_1.len+str_2.len+1;
    char* str_res_buf = a_alloc(str_res_len);
    Str1 str_res = (Str1){ .ptr = str_res_buf, .len = str_res_len-1 };
    memmove(str_res.ptr, str_1.ptr, str_1.len);
    memmove(str_res.ptr+str_1.len, str_2.ptr, str_2.len);
    str_res.ptr[str_1.len+str_2.len] = 0;
    return str_res;
}
Str1 str_cat_clone_1_nnt(Str1 str_1, Str1 str_2) {
    size_t str_res_len = str_1.len+str_2.len;
    char* str_res_buf = a_alloc(str_res_len);
    Str1 str_res = (Str1){ .ptr = str_res_buf, .len = str_res_len };
    memmove(str_res.ptr, str_1.ptr, str_1.len);
    memmove(str_res.ptr+str_1.len, str_2.ptr, str_2.len);
    return str_res;
}

void test_1_nt(Str1* strs, Str1 str_1, Str1 str_2) {
    for (size_t i = 0; i < ITERS; ++i) {
        strs[i] = str_cat_clone_1_nt(str_1, str_2);
    }
}
void test_1_nnt(Str1* strs, Str1 str_1, Str1 str_2) {
    for (size_t i = 0; i < ITERS; ++i) {
        strs[i] = str_cat_clone_1_nt(str_1, str_2);
    }
}



// --- 2 ----------------------------------------------------------------------
Str2 str_cat_clone_2_nt(Str2 str_1, Str2 str_2) {
    size_t str_1_len = str_1.ptr_end-str_1.ptr;
    size_t str_2_len = str_2.ptr_end-str_2.ptr;
    size_t str_res_len = str_1_len+str_2_len+1;
    char* str_res_buf = a_alloc(str_res_len);
    Str2 str_res = (Str2){ .ptr = str_res_buf, .ptr_end = str_res_buf+str_res_len-1 };
    memmove(str_res.ptr, str_1.ptr, str_1_len);
    memmove(str_res.ptr+str_1_len, str_2.ptr, str_2_len);
    str_res.ptr[str_res_len-1] = 0;
    return str_res;
}
Str2 str_cat_clone_2_nnt(Str2 str_1, Str2 str_2) {
    size_t str_1_len = str_1.ptr_end-str_1.ptr;
    size_t str_2_len = str_2.ptr_end-str_2.ptr;
    size_t str_res_len = str_1_len+str_2_len;
    char* str_res_buf = a_alloc(str_res_len);
    Str2 str_res = (Str2){ .ptr = str_res_buf, .ptr_end = str_res_buf+str_res_len };
    memmove(str_res.ptr, str_1.ptr, str_1_len);
    memmove(str_res.ptr+str_1_len, str_2.ptr, str_2_len);
    return str_res;
}

void test_2_nt(Str2* strs, Str2 str_1, Str2 str_2) {
    for (size_t i = 0; i < ITERS; ++i) {
        strs[i] = str_cat_clone_2_nt(str_1, str_2);
    }
}
void test_2_nnt(Str2* strs, Str2 str_1, Str2 str_2) {
    for (size_t i = 0; i < ITERS; ++i) {
        strs[i] = str_cat_clone_2_nnt(str_1, str_2);
    }
}



// --- Run --------------------------------------------------------------------
#define clck(prefix, fn_call) ({ \
    clock_t start = clock(); \
    fn_call; \
    clock_t end = clock(); \
    double diff = (end-start)/(double)CLOCKS_PER_SEC; \
    printf("%s%f\n", prefix, diff); \
    diff; \
})

#define validate(prefix, postfix, real_var, len) \
    if (memcmp(prefix[0]postfix, real_var, len) != 0 || memcmp(prefix[rand() % ITERS]postfix, real_var, len) != 0 || memcmp(prefix[ITERS-1]postfix, real_var, len) != 0 ) { fprintf(stderr, "mismatch\n"); free_cat_strs(); return 1; }


int main() {
    srand(time(NULL));

    FILE* file = fopen("cat_me.txt", "rb");
    if (file == NULL) { fprintf(stderr, "Failed to read `cat_me.txt`: `fopen` failed\n"); return 1; }
    fseek(file, 0, SEEK_END);
    long file_len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* file_str = malloc(file_len+ 1);
    if (fread(file_str, file_len, 1, file) != 1) { fprintf(stderr, "Failed to read `cat_me.txt`: `fread` failed\n"); fclose(file); return 1; }
    fclose(file);
    file_str[file_len] = 0;

    size_t strs_len = 6;
    size_t cat_strs_len = strs_len*2;
    char** cat_strs = malloc(sizeof(char*)*cat_strs_len);
#define free_cat_strs() for (size_t i = 0; i < cat_strs_len; ++i) free(cat_strs[i]); free(cat_strs);
    size_t cat_strs_i = 0;
    for (char* ch = file_str; ch < file_str+file_len && cat_strs_i < cat_strs_len; ) {
        if (*ch == '\n' || *ch == '\r') continue;
        char* nl_ch = strchr(ch, '\n');
        if (nl_ch == NULL) { fprintf(stderr, "Failed to read line in `cat_me.txt`: new line not found\n"); free_cat_strs(); free(file_str); return 1; }
        size_t len = nl_ch - ch;
        char* cat_str = malloc(len+1);
        if (cat_str == NULL) { fprintf(stderr, "Failed to read line in `cat_me.txt`: malloc returned NULL\n"); free_cat_strs(); free(file_str); return 1; }
        memmove(cat_str, ch, len);
        cat_str[len] = 0;
        // printf("%lu: %s|\n", cat_strs_i, cat_str);
        cat_strs[cat_strs_i++] = cat_str;
        ch = nl_ch+1;
    }
    free(file_str);

#define decl0(prefix, strs_i, valid_str) \
    char* prefix##_1 = cat_strs[strs_i*2]; \
    char* prefix##_2 = cat_strs[strs_i*2+1]; \
    size_t prefix##_1_len = strlen(prefix##_1); \
    size_t prefix##_2_len = strlen(prefix##_2); \
    char prefix##_res[] = valid_str; \
    size_t prefix##_res_len = strlen(prefix##_res);

    // 8+2 = 10
    decl0(str0_10, 0, "value = 10");
    // 7+18 = 25
    decl0(str0_25, 1, "ERROR: ../lsp/rpc.lua:734");
    // 35+15 = 50
    decl0(str0_50, 2, "Quisque id orci et lorem volutpat. In est leo vel.");
    // 45+55 = 100
    decl0(str0_100, 3, "Aliquam id ultrices libero. Curabitur proin. Quisque maximus feugiat risus, sit amet venenatis nisi.");
    // 99+151 = 250
    decl0(str0_250, 4, "Pellentesque eget turpis ipsum. Vivamus ut posuere felis. Donec lobortis sem mauris, vel eleifend. Praesent eu tempus turpis. Donec leo lorem, malesuada ut fermentum at, euismod quis mi. Proin bibendum risus quis tristique efficitur. Vestibulum cras.");
    // 368+132 = 500
    decl0(str0_500, 5, "Maecenas aliquam elit erat, et finibus magna eleifend vitae. Aliquam sit amet velit at felis vestibulum sodales dapibus et nisi. Curabitur sollicitudin sodales placerat. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse pulvinar lectus bibendum est egestas, ac sagittis ex varius. Ut rhoncus cursus aliquam. Pellentesque habitant morbi dapibus nam. Aliquam ac accumsan felis. Pellentesque et augue eu sapien interdum suscipit. Sed quis massa sodales, egestas tellus sed erat curae.");

    char* strs0_nl_10[ITERS];
    char* strs0_nl_25[ITERS];
    char* strs0_nl_50[ITERS];
    char* strs0_nl_100[ITERS];
    char* strs0_nl_250[ITERS];
    char* strs0_nl_500[ITERS];
    clck("str0:nl:10:     ", test_0_nl(strs0_nl_10, str0_10_1, str0_10_2));
    validate(strs0_nl_10, , str0_10_res, str0_10_res_len);
    clck("str0:nl:25:     ", test_0_nl(strs0_nl_25, str0_25_1, str0_25_2));
    validate(strs0_nl_25, , str0_25_res, str0_25_res_len);
    clck("str0:nl:50:     ", test_0_nl(strs0_nl_50, str0_50_1, str0_50_2));
    validate(strs0_nl_50, , str0_50_res, str0_50_res_len);
    clck("str0:nl:100:    ", test_0_nl(strs0_nl_100, str0_100_1, str0_100_2));
    validate(strs0_nl_100, , str0_100_res, str0_100_res_len);
    clck("str0:nl:250:    ", test_0_nl(strs0_nl_250, str0_250_1, str0_250_2));
    validate(strs0_nl_250, , str0_250_res, str0_250_res_len);
    clck("str0:nl:500:    ", test_0_nl(strs0_nl_500, str0_500_1, str0_500_2));
    validate(strs0_nl_500, , str0_500_res, str0_500_res_len);
    char* strs0_l_10[ITERS];
    char* strs0_l_25[ITERS];
    char* strs0_l_50[ITERS];
    char* strs0_l_100[ITERS];
    char* strs0_l_250[ITERS];
    char* strs0_l_500[ITERS];
    clck("str0:l:10:      ", test_0_l(strs0_l_10, str0_10_1_len, str0_10_1, str0_10_2_len, str0_10_2));
    validate(strs0_l_10, , str0_10_res, str0_10_res_len);
    clck("str0:l:25:      ", test_0_l(strs0_l_25, str0_25_1_len, str0_25_1, str0_25_2_len, str0_25_2));
    validate(strs0_l_25, , str0_25_res, str0_25_res_len);
    clck("str0:l:50:      ", test_0_l(strs0_l_50, str0_50_1_len, str0_50_1, str0_50_2_len, str0_50_2));
    validate(strs0_l_50, , str0_50_res, str0_50_res_len);
    clck("str0:l:100:     ", test_0_l(strs0_l_100, str0_100_1_len, str0_100_1, str0_100_2_len, str0_100_2));
    validate(strs0_l_100, , str0_100_res, str0_100_res_len);
    clck("str0:l:250:     ", test_0_l(strs0_l_250, str0_250_1_len, str0_250_1, str0_250_2_len, str0_250_2));
    validate(strs0_l_250, , str0_250_res, str0_250_res_len);
    clck("str0:l:500:     ", test_0_l(strs0_l_500, str0_500_1_len, str0_500_1, str0_500_2_len, str0_500_2));
    validate(strs0_l_500, , str0_500_res, str0_500_res_len);

#define decl1(num) \
    Str1 str1_##num##_1 = (Str1){ .ptr = str0_##num##_1, .len = str0_##num##_1_len }; \
    Str1 str1_##num##_2 = (Str1){ .ptr = str0_##num##_2, .len = str0_##num##_2_len };

    decl1(10); decl1(25); decl1(50); decl1(100); decl1(250); decl1(500);

    Str1 strs1_nt_10[ITERS];
    Str1 strs1_nt_25[ITERS];
    Str1 strs1_nt_50[ITERS];
    Str1 strs1_nt_100[ITERS];
    Str1 strs1_nt_250[ITERS];
    Str1 strs1_nt_500[ITERS];
    clck("str1:nt:10:     ", test_1_nt(strs1_nt_10, str1_10_1, str1_10_2));
    validate(strs1_nt_10, .ptr, str0_10_res, str0_10_res_len);
    clck("str1:nt:25:     ", test_1_nt(strs1_nt_25, str1_25_1, str1_25_2));
    validate(strs1_nt_25, .ptr, str0_25_res, str0_25_res_len);
    clck("str1:nt:50:     ", test_1_nt(strs1_nt_50, str1_50_1, str1_50_2));
    validate(strs1_nt_50, .ptr, str0_50_res, str0_50_res_len);
    clck("str1:nt:100:    ", test_1_nt(strs1_nt_100, str1_100_1, str1_100_2));
    validate(strs1_nt_100, .ptr, str0_100_res, str0_100_res_len);
    clck("str1:nt:250:    ", test_1_nt(strs1_nt_250, str1_250_1, str1_250_2));
    validate(strs1_nt_250, .ptr, str0_250_res, str0_250_res_len);
    clck("str1:nt:500:    ", test_1_nt(strs1_nt_500, str1_500_1, str1_500_2));
    validate(strs1_nt_500, .ptr, str0_500_res, str0_500_res_len);
    Str1 strs1_nnt_10[ITERS];
    Str1 strs1_nnt_25[ITERS];
    Str1 strs1_nnt_50[ITERS];
    Str1 strs1_nnt_100[ITERS];
    Str1 strs1_nnt_250[ITERS];
    Str1 strs1_nnt_500[ITERS];
    clck("str1:nnt:10:    ", test_1_nnt(strs1_nnt_10, str1_10_1, str1_10_2));
    validate(strs1_nnt_10, .ptr, str0_10_res, str0_10_res_len);
    clck("str1:nnt:25:    ", test_1_nnt(strs1_nnt_25, str1_25_1, str1_25_2));
    validate(strs1_nnt_25, .ptr, str0_25_res, str0_25_res_len);
    clck("str1:nnt:50:    ", test_1_nnt(strs1_nnt_50, str1_50_1, str1_50_2));
    validate(strs1_nnt_50, .ptr, str0_50_res, str0_50_res_len);
    clck("str1:nnt:100:   ", test_1_nnt(strs1_nnt_100, str1_100_1, str1_100_2));
    validate(strs1_nnt_100, .ptr, str0_100_res, str0_100_res_len);
    clck("str1:nnt:250:   ", test_1_nnt(strs1_nnt_250, str1_250_1, str1_250_2));
    validate(strs1_nnt_250, .ptr, str0_250_res, str0_250_res_len);
    clck("str1:nnt:500:   ", test_1_nnt(strs1_nnt_500, str1_500_1, str1_500_2));
    validate(strs1_nnt_500, .ptr, str0_500_res, str0_500_res_len);

#define decl2(num) \
    Str2 str2_##num##_1 = (Str2){ .ptr = str0_##num##_1, .ptr_end = str0_##num##_1+str0_##num##_1_len }; \
    Str2 str2_##num##_2 = (Str2){ .ptr = str0_##num##_2, .ptr_end = str0_##num##_2+str0_##num##_2_len };

    decl2(10); decl2(25); decl2(50); decl2(100); decl2(250); decl2(500);

    Str2 strs2_nt_10[ITERS];
    Str2 strs2_nt_25[ITERS];
    Str2 strs2_nt_50[ITERS];
    Str2 strs2_nt_100[ITERS];
    Str2 strs2_nt_250[ITERS];
    Str2 strs2_nt_500[ITERS];
    clck("str2:nt:10:     ", test_2_nt(strs2_nt_10, str2_10_1, str2_10_2));
    validate(strs2_nt_10, .ptr, str0_10_res, str0_10_res_len);
    clck("str2:nt:25:     ", test_2_nt(strs2_nt_25, str2_25_1, str2_25_2));
    validate(strs2_nt_25, .ptr, str0_25_res, str0_25_res_len);
    clck("str2:nt:50:     ", test_2_nt(strs2_nt_50, str2_50_1, str2_50_2));
    validate(strs2_nt_50, .ptr, str0_50_res, str0_50_res_len);
    clck("str2:nt:100:    ", test_2_nt(strs2_nt_100, str2_100_1, str2_100_2));
    validate(strs2_nt_100, .ptr, str0_100_res, str0_100_res_len);
    clck("str2:nt:250:    ", test_2_nt(strs2_nt_250, str2_250_1, str2_250_2));
    validate(strs2_nt_250, .ptr, str0_250_res, str0_250_res_len);
    clck("str2:nt:500:    ", test_2_nt(strs2_nt_500, str2_500_1, str2_500_2));
    validate(strs2_nt_500, .ptr, str0_500_res, str0_500_res_len);
    Str2 strs2_nnt_10[ITERS];
    Str2 strs2_nnt_25[ITERS];
    Str2 strs2_nnt_50[ITERS];
    Str2 strs2_nnt_100[ITERS];
    Str2 strs2_nnt_250[ITERS];
    Str2 strs2_nnt_500[ITERS];
    clck("str2:nnt:10:    ", test_2_nnt(strs2_nnt_10, str2_10_1, str2_10_2));
    validate(strs2_nnt_10, .ptr, str0_10_res, str0_10_res_len);
    clck("str2:nnt:25:    ", test_2_nnt(strs2_nnt_25, str2_25_1, str2_25_2));
    validate(strs2_nnt_25, .ptr, str0_25_res, str0_25_res_len);
    clck("str2:nnt:50:    ", test_2_nnt(strs2_nnt_50, str2_50_1, str2_50_2));
    validate(strs2_nnt_50, .ptr, str0_50_res, str0_50_res_len);
    clck("str2:nnt:100:   ", test_2_nnt(strs2_nnt_100, str2_100_1, str2_100_2));
    validate(strs2_nnt_100, .ptr, str0_100_res, str0_100_res_len);
    clck("str2:nnt:250:   ", test_2_nnt(strs2_nnt_250, str2_250_1, str2_250_2));
    validate(strs2_nnt_250, .ptr, str0_250_res, str0_250_res_len);
    clck("str2:nnt:500:   ", test_2_nnt(strs2_nnt_500, str2_500_1, str2_500_2));
    validate(strs2_nnt_500, .ptr, str0_500_res, str0_500_res_len);

    free_cat_strs();
}

