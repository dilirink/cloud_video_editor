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

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
}