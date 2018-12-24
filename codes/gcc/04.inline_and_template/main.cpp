import hello;
import say;
import foo;

#include <iostream>

int main() {
    hello::say_hello();
    /*say{}.*/say_hello(/*sizeof(say)*/123);
    // std::cout << foo<int>{}.hello() << std::endl;
    return 0;
}