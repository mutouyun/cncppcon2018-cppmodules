export module say;
import std.core;

export class say {
public:
    template <int N>
    void hello() {
        std::cout << "hello, say class: " << N << std::endl;
    }
};