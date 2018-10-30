#include <stdlib.h>

#include "latasia.h"

static int init_echo_module(lts_module_t *module)
{
    return 0;
}


static void exit_echo_module(lts_module_t *module)
{
}


static void mod_on_connected(lts_socket_t *s)
{
}


static void mod_on_received(lts_socket_t *s)
{
    lts_buffer_t *p = s->conn->rbuf;

    s->conn->rbuf = s->conn->sbuf;
    s->conn->sbuf = p;
}


static void mod_on_sent(lts_socket_t *s)
{
}


static void mod_on_closing(lts_socket_t *s)
{
}


static lts_app_module_itfc_t echo_itfc = {
    &mod_on_connected,
    &mod_on_received,
    &mod_on_sent,
    &mod_on_closing,
};

lts_module_t lts_app_echo_module = {
    lts_string("lts_app_echo_module"),
    LTS_APP_MODULE,
    &echo_itfc,
    NULL,
    NULL,
    // interfaces
    NULL,
    &init_echo_module,
    &exit_echo_module,
    NULL,
};
