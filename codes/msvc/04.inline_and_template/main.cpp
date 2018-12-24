import hello;
import say;
import foo;
import std.core;

int main() {
    hello::say_hello();
    say{}.hello<sizeof(say)>();
    std::cout << foo<int>{}.hello() << std::endl;
    return 0;
}