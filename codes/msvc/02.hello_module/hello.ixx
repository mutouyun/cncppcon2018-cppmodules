export module hello;

import std.core;
using namespace std;

export namespace hello {
    void say(const char* str) {
        cout << str << endl;
    }
}