module;
#include <cstdio>
export module hello;

export namespace hello {
    /* gcc wilwouldl ICE with inline */
    /*inline*/ void say_hello() {
        std::printf("hello world!\n");
    }
}