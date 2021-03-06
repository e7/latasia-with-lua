/**
 * latasia
 * Copyright (c) 2015 e7 <jackzxty@126.com>
 * */


#include <execinfo.h>

#include "latasia.h"
#include "vsignal.h"
#include "logger.h"

#define __THIS_FILE__       "src/vsignal.c"


static void dump_stack_frames(void)
{
    int sfsz;
    void *sfs[32];
    char **str_sfs;

    sfsz = backtrace(sfs, ARRAY_COUNT(sfs));
    str_sfs = backtrace_symbols(sfs, sfsz);
    if (str_sfs) {
        for (int i = 0; i < sfsz; ++i) {
            (void)lts_write_logger(
                &lts_file_logger, LTS_LOG_EMERGE, "%s\n", str_sfs[i]
            );
        }
        free(str_sfs);
    }

    return;
}


static void sig_int_handler(int s)
{
    lts_signals_mask |= LTS_MASK_SIGEXIT;

    return;
}

static void sig_term_handler(int s)
{
    lts_signals_mask |= LTS_MASK_SIGEXIT;

    return;
}

static void sig_chld_handler(int s)
{
    lts_signals_mask |= LTS_MASK_SIGCHLD;

    return;
}

static void sig_pipe_handler(int s)
{
    lts_signals_mask |= LTS_MASK_SIGPIPE;
    dump_stack_frames();

    return;
}

static void sig_alrm_handler(int s)
{
    lts_signals_mask |= LTS_MASK_SIGALRM;

    return;
}

static void sig_segv_handler(int s)
{
    dump_stack_frames();
    return;
}


static lts_signal_t lts_signals_master[] = {
    {SIGINT, "SIGINT", &sig_int_handler},
    {SIGTERM, "SIGTERM", &sig_term_handler},
    {SIGCHLD, "SIGCHLD", &sig_chld_handler},
    {SIGPIPE, "SIGPIPE", &sig_pipe_handler},
    {SIGALRM, "SIGALRM", SIG_IGN},
    {SIGSEGV, "SIGSEGV", &sig_segv_handler},
    {0, NULL, NULL},
};
static lts_signal_t lts_signals_slave[] = {
    {SIGINT, "SIGINT", SIG_IGN},
    {SIGTERM, "SIGTERM", SIG_IGN},
    {SIGCHLD, "SIGCHLD", &sig_chld_handler},
    {SIGPIPE, "SIGPIPE", &sig_pipe_handler},
    {SIGALRM, "SIGALRM", &sig_alrm_handler},
    {0, NULL, NULL},
};


int lts_init_sigactions(int role)
{
    struct sigaction sa;
    lts_signal_t *sig_array;

    sa.sa_flags = 0;
    // 信号处理过程中屏蔽所有信号
    if (-1 == sigfillset(&sa.sa_mask)) {
        return -1;
    }

    if (LTS_MASTER == role) {
        sig_array = lts_signals_master;
    } else {
        sig_array = lts_signals_slave;
    }
    for (int i = 0; sig_array[i].signo; ++i) {
        // 子进程忽略所有信号
        sa.sa_handler = sig_array[i].handler;
        if (-1 == sigaction(sig_array[i].signo, &sa, NULL)) {
            return -1;
        }
    }

    return 0;
}
