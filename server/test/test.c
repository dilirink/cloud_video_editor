#define CTEST_MAIN

#include "../functions.h"
#include "thirdparty/ctest.h"

CTEST(cfgRD, port) {
    if(cfg_reader() == -1)
        printf("error cfg_read\n");
    ASSERT_STR("3491", PORT);
}

CTEST(cfgRD, path) {
    if(cfg_reader() == -1)
        printf("error cfg_read\n");
    ASSERT_STR("milka/", path_index);
}

CTEST(httpReqType, POST) {
    char request[] = "POST / HTTP/1.1";
    ASSERT_EQUAL(1, http_request_type(request));
}

CTEST(httpReqType, GET) {
    char request[] = " GET / HTTP/1.1";
    ASSERT_EQUAL(2, http_request_type(request));
}

CTEST(httpReqType, ERR) {
    char request[] = "hello";
    ASSERT_EQUAL(3, http_request_type(request));
}

int main(int argc, const char *argv[]){
    return ctest_main(argc, argv);
}