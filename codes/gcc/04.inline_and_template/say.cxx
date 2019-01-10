module;
#include <cstdio>
export module say;

/* gcc would ICE with this class */
export/* class say {
public:*/
    // template <int N>
    void say_hello(int N) {
        std::printf("hello, say class: %d\n", N);
    }
// };
