export module hello;
import std.core;

export namespace hello {
    inline void say_hello() {
        std::cout << "hello world!" << std::endl;
    }
}