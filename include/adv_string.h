/**
 * latasia
 * Copyright (c) 2015 e7 <jackzxty@126.com>
 * */


#ifndef __LATASIA__ADV_STRING_H__
#define __LATASIA__ADV_STRING_H__


#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// start of charmap {{
typedef union {
    uint8_t bytes[32];
    uint32_t ints[8];
} charmap_t;

// 统计整数中比特位为1的个数
static inline
int count_bit_1(uint32_t x)
{
    x = (x & 0x55555555U) + ((x >> 1) & 0x55555555U);
    x = (x & 0x33333333U) + ((x >> 2) & 0x33333333U);
    x = (x & 0x0F0F0F0FU) + ((x >> 4) & 0x0F0F0F0FU);
    x = (x & 0x00FF00FFU) + ((x >> 8) & 0x00FF00FFU);
    x = (x & 0x0000FFFFU) + ((x >> 16) & 0x0000FFFFU);

    return x;
}

static inline
void charmap_clean(charmap_t *cm)
{
    (void)memset(cm->bytes, 0, sizeof(cm->bytes));
}

static inline
void charmap_set(charmap_t *cm, uint8_t c)
{
    cm->bytes[c / 8] |= (1U << (c % 8));
}

static inline
int charmap_isset(charmap_t *cm, uint8_t c)
{
    return (cm->bytes[c / 8] & (1U << (c % 8)));
}

static inline
int charmap_count(charmap_t *cm)
{
    int count = 0;

    for (int i = 0; i < ARRAY_COUNT(cm->ints); ++i) {
        count += count_bit_1(cm->ints[i]);
    }

    return count;
}
// }} end of charmap


// 数字转C字符串，非线程安全 {{
extern char *lts_uint322cstr(uint32_t u32);
// }} 数字转C字符串，非线程安全


// =================================== lts_str_t
typedef struct {
    uint8_t *data;
    ssize_t len;
} lts_str_t;


// 用数组初始化
#define lts_string(str)         {(uint8_t *)(str), sizeof(str) - 1,}
#define lts_null_string         {NULL, 0,}

// 空字符串
extern lts_str_t lts_empty_string;

static inline void lts_str_init(lts_str_t *str, uint8_t *data, ssize_t len)
{
    str->data = data;
    str->len = len;
}

static inline void lts_str_share(lts_str_t *dst, lts_str_t const *src)
{
    dst->data = src->data;
    dst->len = src->len;
}

static inline ssize_t i64_width(int64_t x)
{
    ssize_t rslt = ((x < 0) ? 1 : 0);

    do {
        ++rslt;
        x /= 10;
    } while (x);

    return rslt;
}


// 去除字符串前后空格
extern void lts_str_trim(lts_str_t *str);

// 反转字符串
extern void lts_str_reverse(lts_str_t *src);

// 移除子串
extern void lts_str_hollow(lts_str_t *src, ssize_t start, ssize_t len);

// 字符过滤
extern ssize_t lts_str_filter(lts_str_t *src, uint8_t c);
extern ssize_t lts_str_filter_multi(lts_str_t *src, uint8_t *c, ssize_t len);

// 字符串比较
extern int lts_str_compare(lts_str_t *a, lts_str_t *b);

// 子串搜索
extern int lts_str_find(lts_str_t *text, lts_str_t *pattern, int offset);

// int64转字符串
extern void lts_i642str(lts_str_t *str, int64_t x, int check);
extern int64_t lts_str2i64(const lts_str_t *str);

// print
extern void lts_str_println(FILE *stream, lts_str_t *s);

#ifdef __cplusplus
}
#endif // __cplusplus


#ifdef ADV_STRING_ENHANCE
#include "mem_pool.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
// 带pool的函数返回的字符串都是有结束符的，可直接使用

// 字符串克隆
extern lts_str_t *lts_str_clone(lts_str_t *s, lts_pool_t *pool);

// 格式化
extern lts_str_t *lts_str_sprintf(lts_pool_t *pool, char const *format, ...);

// 分割字符串，返回字符串数组，NULL结束
extern lts_str_t **lts_str_split(lts_str_t *src, uint8_t c, lts_pool_t *pool);

// base64编解码
extern lts_str_t *lts_str_base64_en(char const *src, lts_pool_t *pool);
extern lts_str_t *lts_str_base64_en2(lts_str_t *src, lts_pool_t *pool);
extern lts_str_t *lts_str_base64_de(char const *src, lts_pool_t *pool);
extern lts_str_t *lts_str_base64_de2(lts_str_t *src, lts_pool_t *pool);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ADV_STRING_ENHANCE

#endif // __LATASIA__ADV_STRING_H__
