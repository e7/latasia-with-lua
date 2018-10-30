#ifndef __LATASIA__PUBLIC_H__
#define __LATASIA__PUBLIC_H__

#include <stddef.h>

typedef struct lts_rlimit_s lts_rlimit_t;

// 进程资源限制
struct lts_rlimit_s {
    int cpu_total; // cpu总核数
    int cpu_online; // 可用cpu核数
    long open_max; // 能打开的最大文件数
    size_t sys_pagesize; // 内存页size
};

extern lts_rlimit_t lts_rlimit;
#endif // __LATASIA__PUBLIC_H__
