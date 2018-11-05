/**
 * latasia
 * Copyright (c) 2015 e7 <jackzxty@126.com>
 * */


#ifndef __LATASIA__RBT_TIMER_H__
#define __LATASIA__RBT_TIMER_H__


#include "adv_string.h"
#include "rbmap.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct lts_timer_node_s lts_timer_node_t;
struct lts_timer_node_s {
    void (*on_timeout)(lts_timer_node_t *); // 超时回调
    lts_rbmap_node_t mapnode;
};

static inline
void lts_timer_node_init(lts_timer_node_t *node,
                         uintptr_t timeout,
                         void (*func)(lts_timer_node_t *))
{
    node->on_timeout = func;
    lts_rbmap_node_init(&node->mapnode, timeout);
    return;
}

static inline
int lts_timer_node_empty(lts_timer_node_t *node)
{
    return lts_rbmap_node_empty(&node->mapnode);
}

typedef lts_rbmap_t lts_timer_t; // 定时器

extern int lts_timer_add(lts_timer_t *heap, lts_timer_node_t *node);

typedef enum {
    TIMEOUT_RESET = 1, // 重置
    TIMEOUT_EXTRA, // 加时
} lts_timer_modtype_t;
/**
 * 修改超时时间
 * timeout 函数内取绝对值
 */
extern int lts_timer_modify(lts_timer_t *heap,
                            lts_timer_node_t *node,
                            lts_timer_modtype_t modtype,
                            int64_t timeout);
extern int lts_timer_del(lts_timer_t *heap, lts_timer_node_t *node);
extern lts_timer_node_t *lts_timer_min(lts_timer_t *heap);


extern void lts_update_time(void);
extern void lts_time_string(lts_str_t *tm);
extern int64_t lts_current_time; // 当前时间
extern lts_timer_t lts_timer_heap; // 时间堆

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __LATASIA__RBT_TIMER_H__
