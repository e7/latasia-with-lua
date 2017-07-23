/**
 * latasia
 * Copyright (c) 2015 e7 <jackzxty@126.com>
 * */


#include "socket.h"
#include "adv_string.h"

#define __THIS_FILE__       "src/socket.c"


int lts_accept_disabled;
lts_pool_t *lts_sockcache_pool; // socket缓存专用内存池
dlist_t lts_sock_list; // socket缓存列表
size_t lts_sockcache_n;
size_t lts_sock_inuse_n;
lts_socket_t **lts_listen_array; // 监听socket数组
dlist_t lts_watch_list;
dlist_t lts_post_list;
dlist_t lts_task_list = DLIST_NODE(lts_task_list); // 请求列表


char *lts_inet_ntoa(struct in_addr in)
{
    static char rslt[16];

    union {
        uint8_t u8[4];
        uint32_t u32;
    } uin;

    uin.u32 = in.s_addr;
    memset(rslt, 0, sizeof(rslt)); // 清空
    strcat(rslt, lts_uint322cstr(uin.u8[0])); // 保证不越界可使用该函数
    strcat(rslt, "."); // 保证不越界可使用该函数
    strcat(rslt, lts_uint322cstr(uin.u8[1])); // 保证不越界可使用该函数
    strcat(rslt, "."); // 保证不越界可使用该函数
    strcat(rslt, lts_uint322cstr(uin.u8[2])); // 保证不越界可使用该函数
    strcat(rslt, "."); // 保证不越界可使用该函数
    strcat(rslt, lts_uint322cstr(uin.u8[3])); // 保证不越界可使用该函数

    return rslt;
}

// 创建socket缓存
void lts_create_sockcache(size_t nconns)
{
    lts_socket_t *sock_cache;

    lts_sock_inuse_n = 0;
    lts_sockcache_n = nconns;
    dlist_init(&lts_sock_list);

    lts_sockcache_pool = lts_create_pool(
            (lts_sockcache_n + 1) * sizeof(lts_socket_t)
    );
    sock_cache = (lts_socket_t *)(
        lts_palloc(lts_sockcache_pool, lts_sockcache_n * sizeof(lts_socket_t))
    );
    ASSERT(NULL != sock_cache);
    for (int i = 0; i < lts_sockcache_n; ++i) {
        dlist_add_tail(&lts_sock_list, &sock_cache[i].dlnode);
    }
}

// 销毁socket缓存
void lts_destroy_sockcache(void)
{
    lts_destroy_pool(lts_sockcache_pool);
}


#ifndef HAVE_FUNCTION_ACCEPT4
int lts_accept4(int sockfd, struct sockaddr *addr,
                socklen_t *addrlen, int flags)
{
    int fd;

    fd = accept(sockfd, addr, addrlen);
    if (-1 == fd) {
        return -1;
    }
    (void)lts_set_nonblock(fd);

    return fd;
}
#endif
