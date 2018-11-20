#include <stdlib.h>

#include "latasia.h"

static char demo_html_header[] = "HTTP/1.1 200 OK\r\n\
Server: nginx/1.10.3\r\n\
Date: Tue, 20 Nov 2018 03:05:30 GMT\r\n\
Content-Type: text/html\r\n\
Content-Length: 556\r\n\
Last-Modified: Fri, 03 Mar 2017 22:03:47 GMT\r\n\
Connection: keep-alive\r\n\
ETag: \"58b9e843-264\"\r\n\
Accept-Ranges: bytes\r\n\r\n";

static char demo_html_body[] = "\
<!DOCTYPE html>\
<html>\
<head>\
<title>Welcome to nginx!</title>\
<style>\
body {\
width: 35em;margin: 0 auto;\
font-family: Tahoma, Verdana, Arial, sans-serif;\
}\
</style>\
</head>\
<body>\
<h1>Welcome to nginx!</h1>\
<p>If you see this page, the nginx web server is successfully installed and\
working. Further configuration is required.</p>\
<p>For online documentation and support please refer to\
<a href=\"http://nginx.org/\">nginx.org</a>.<br/>\
Commercial support is available at\
<a href=\"http://nginx.com/\">nginx.com</a>.</p>\
<p><em>Thank you for using nginx.</em></p>\
</body>\
</html>";

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
    //lts_buffer_t *p = s->conn->rbuf;

    //s->conn->rbuf = s->conn->sbuf;
    //s->conn->sbuf = p;

    lts_buffer_clear(s->conn->rbuf);
    lts_buffer_append(s->conn->sbuf, demo_html_header, sizeof(demo_html_header));
    lts_buffer_append(s->conn->sbuf, demo_html_body, sizeof(demo_html_body));
    lts_soft_event(s, LTS_SOFT_WRITE);
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
