export module foo;

export {
    /* gcc would ICE with class template */
    /*template <typename>
    struct foo {
        constexpr const char* hello(void) const {
            return "hello typename!";
        }
    };*/

    // template <>
    // struct foo<int> {
    //     constexpr const char* hello(void) const {
    //         return "hello typename int!";
    //     }
    // };
}