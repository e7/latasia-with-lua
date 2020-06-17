/**
 * latasia
 * Copyright (c) 2015 e7 <jackzxty@126.com>
 * */


#include <sys/time.h>

#include "rbt_timer.h"

#define __THIS_FILE__       "src/rbt_timer.c"


int lts_timer_add(lts_timer_t *heap, lts_timer_node_t *node)
{
    return lts_rbmap_add(heap, &node->mapnode);
}

/**
 * 修改超时时间
 */
int lts_timer_modify(lts_timer_t *heap,
                     lts_timer_node_t *node,
                     lts_timer_modtype_t modtype,
                     int64_t timeout)
{
    // 删除旧结点，已存在其它节点则忽略
    (void)lts_rbmap_safe_del(heap, &node->mapnode);

    if (TIMEOUT_RESET == modtype) {
        node->mapnode.key = (uintptr_t)ABS_INTEGER(timeout);
    } else if (TIMEOUT_EXTRA == modtype) {
        node->mapnode.key += (uintptr_t)ABS_INTEGER(timeout);
    } else {
        ASSERT(FALSE);
    }

    return lts_rbmap_add(heap, &node->mapnode);
}


int lts_timer_del(lts_timer_t *heap, lts_timer_node_t *node)
{
    return (NULL != lts_rbmap_safe_del(heap, &node->mapnode));
}


lts_timer_node_t *lts_timer_min(lts_timer_t *heap)
{
    lts_rbmap_node_t *node = lts_rbmap_min(heap);

    if (node) {
        return CONTAINER_OF(lts_rbmap_min(heap), lts_timer_node_t, mapnode);
    } else {
        return NULL;
    }
}



void lts_update_time(void)
{
    lts_timeval_t current;

    (void)gettimeofday(&current, NULL);
    lts_current_time = current.tv_sec * 10 + current.tv_usec / 1000 / 100;

    return;
}


void lts_time_string(lts_str_t *tm)
{
    static uint8_t tmstr[32];

    memset(tmstr, 0, sizeof(tmstr));
    lts_str_init(tm, tmstr, sizeof(tmstr));
    lts_i642str(tm, lts_current_time, FALSE);
}


int64_t lts_current_time; // 当前时间
lts_timer_t lts_timer_heap; // 时间堆
