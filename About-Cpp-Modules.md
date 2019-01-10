# About C++ Modules
 
## 1. 为什么我们需要C++ Modules？

实际上，关于C++ Modules的讨论由来已久。  
 
多年以来，C++的各个独立模块/编译单元之间通讯的手段，一直沿用C语言的头文件机制。这套机制简陋且低效，再加之C++中模板和`inline`的大量使用，`#include`已经显得有些不堪重负了。  
 
头文件的问题主要有如下几点：  
 
 * 脆弱的文本展开
 * 内部细节的意外导出
 * 大量的重复处理（N x M）导致了低下的编译效率
 * 无法保证编译单元的一致性（ODR）
 * 对开发工具很不友好
 
一个经典的例子是`#include <Windows.h>`引发的莫名其妙的编译错误：  

```c++
#include <iostream>
#include <limits>

#include <Windows.h>

int main() {
    std::cout << std::numeric_limits<int>::max() << std::endl;
    /*
        warning C4003: not enough actual parameters for macro 'max'
        error C2589: '(' : illegal token on right side of '::'
        error C2143: syntax error : missing ')' before '::'
        error C2059: syntax error : ')'
    */
    return 0;
}
```
 
