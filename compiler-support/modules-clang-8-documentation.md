---
description: 'https://clang.llvm.org/docs/Modules.html'
---

# Modules — Clang 8 documentation

## [Introduction](https://clang.llvm.org/docs/Modules.html#id9)

Most software is built using a number of software libraries, including libraries supplied by the platform, internal libraries built as part of the software itself to provide structure, and third-party libraries. For each library, one needs to access both its interface \(API\) and its implementation. In the C family of languages, the interface to a library is accessed by including the appropriate header files\(s\):  
大多数软件都是使用各种软件库构建的，包括由平台提供的库、作为软件本身的一部分进行构建的内部库，还有第三方库。对于每个库，都需要访问其接口（API）和实现。在C语言家族中，库的接口通过包含适当的头文件来访问：

```cpp
#include <SomeLib.h>
```

The implementation is handled separately by linking against the appropriate library. For example, by passing `-lSomeLib` to the linker.  
其具体实现是通过链接到合适的库来单独处理的。例如，传递`-lSomeLib`给链接器，链接到`SomeLib.h`中声明的实现。

Modules provide an alternative, simpler way to use software libraries that provides better compile-time scalability and eliminates many of the problems inherent to using the C preprocessor to access the API of a library.  
模块（Modules）提供了另一种更简便的方法来使用软件库，它提供了更好的编译时可伸缩性，并消除了使用C预处理器访问库API所固有的许多问题。

### [Problems with the current model](https://clang.llvm.org/docs/Modules.html#id10)

The `#include` mechanism provided by the C preprocessor is a very poor way to access the API of a library, for a number of reasons:  
由C预处理器提供的`#include`机制是访问库API的一种非常糟糕的方式，原因如下：

* **Compile-time scalability**: Each time a header is included, the compiler must preprocess and parse the text in that header and every header it includes, transitively. This process must be repeated for every translation unit in the application, which involves a huge amount of redundant work. In a project with _N_ translation units and _M_ headers included in each translation unit, the compiler is performing _M x N_ work even though most of the _M_ headers are shared among multiple translation units. C++ is particularly bad, because the compilation model for templates forces a huge amount of code into headers. **编译期可伸缩性**：每次包含头文件时，编译器必须对该头文件及其包含的每个头文件进行预处理和解析。应用程序中的每个编译单元都必须重复这个过程，这涉及到大量的冗余工作。在一个包含N个编译单元，每个编译单元包含M个头文件的项目中，编译器需要执行M x N个相关工作，尽管在多个编译单元之间共享了大多数的头文件。这在C++尤其糟糕，因为模板的编译模型需要将大量的代码强制放到头文件中。
* **Fragility**: `#include` directives are treated as textual inclusion by the preprocessor, and are therefore subject to any active macro definitions at the time of inclusion. If any of the active macro definitions happens to collide with a name in the library, it can break the library API or cause compilation failures in the library header itself. For an extreme example, `#define std "The C++ Standard"` and then include a standard library header: the result is a horrific cascade of failures in the C++ Standard Library’s implementation. More subtle real-world problems occur when the headers for two different libraries interact due to macro collisions, and users are forced to reorder `#include` directives or introduce `#undef` directives to break the \(unintended\) dependency. **脆弱性**：`#include`指令被预处理器视为文本包含，因此在包含时受任何活动宏定义的约束。如果这些宏定义碰巧与库中的名称发生冲突，它可能会破坏库API或导致库头文件中出现编译错误。举个极端的例子，`#define std "The C++ Standard"`，然后包含一个标准库头文件：结果是C++标准库实现中的一系列可怕的失败。当两个不同库的头文件由于宏冲突而互相影响时，会出现更微妙的实际问题，用户被迫重新调整`#include`的顺序或引入`#undef`指令，以打破（无意的）依赖关系。
* **Conventional workarounds**: C programmers have adopted a number of conventions to work around the fragility of the C preprocessor model. Include guards, for example, are required for the vast majority of headers to ensure that multiple inclusion doesn’t break the compile. Macro names are written with`LONG_PREFIXED_UPPERCASE_IDENTIFIERS` to avoid collisions, and some library/framework developers even use `__underscored` names in headers to avoid collisions with “normal” names that \(by convention\) shouldn’t even be macros. These conventions are a barrier to entry for developers coming from non-C languages, are boilerplate for more experienced developers, and make our headers far uglier than they should be. **传统的解决方案**：C程序员采用了许多惯用法来解决C预处理器模型的脆弱性问题。例如，大多数头文件都需要使用包含保护，以确保自身不会被包含多次引起编译错误。宏名采用`LONG_PREFIXED_UPPERCASE_IDENTIFIERS`规则来编写，以避免冲突，一些库/框架开发人员甚至在头中使用双下划线名称来避免与（按照惯例）甚至不应该是宏的“正常”名称发生冲突。
* **Tool confusion**: In a C-based language, it is hard to build tools that work well with software libraries, because the boundaries of the libraries are not clear. Which headers belong to a particular library, and in what order should those headers be included to guarantee that they compile correctly? Are the headers C, C++, Objective-C++, or one of the variants of these languages? What declarations in those headers are actually meant to be part of the API, and what declarations are present only because they had to be written as part of the header file? **工具的混乱**：在基于C的语言中，很难构建与软件库协同工作的工具，因为库的边界并不清楚。哪些头文件属于某个特定的库，以及这些头文件应该以何种顺序包含以确保它们正确编译？头文件是C、C++、Objective - C++还是这些语言的变体之一？这些头文件中的哪些声明实际上是API的一部分，以及哪些声明是仅因为必须作为头文件的一部分写入而出现的?

### [Semantic import](https://clang.llvm.org/docs/Modules.html#id11)

Modules improve access to the API of software libraries by replacing the textual preprocessor inclusion model with a more robust, more efficient semantic model. From the user’s perspective, the code looks only slightly different, because one uses an `import` declaration rather than a `#include` preprocessor directive:  
模块通过将**文本预处理器包含模型**替换为更健壮、更有效的**语义模型**来改进对软件库API的访问。从用户的角度来看，代码看起来只是略有不同，因为使用import声明而不是\#include预处理器指令：

```cpp
import std.io; // pseudo-code; see below for syntax discussion
```

However, this module import behaves quite differently from the corresponding `#include <stdio.h>`: when the compiler sees the module import above, it loads a binary representation of the `std.io` module and makes its API available to the application directly. Preprocessor definitions that precede the import declaration have no impact on the API provided by `std.io`, because the module itself was compiled as a separate, standalone module. Additionally, any linker flags required to use the `std.io` module will automatically be provided when the module is imported [\[1\]](https://clang.llvm.org/docs/Modules.html#id5). This semantic import model addresses many of the problems of the preprocessor inclusion model:  
但是，这个模块导入的行为与对应的`#include <stdio.h>`有很大不同：当编译器看到上面的模块导入时，它加载`std.io`模块的二进制表达形式（binary representation），并将其API直接提供给应用程序。导入声明之前的预处理器定义对`std.io`提供的API没有影响，因为模块本身被编译为独立的模块。此外，在导入模块时，将自动提供使用`std.io`模块所需的任何linker flags。该语义导入模型解决了预处理器包含模型的许多问题：

* **Compile-time scalability**: The `std.io` module is only compiled once, and importing the module into a translation unit is a constant-time operation \(independent of module system\). Thus, the API of each software library is only parsed once, reducing the _M x N_ compilation problem to an _M + N_ problem.
* **Fragility**: Each module is parsed as a standalone entity, so it has a consistent preprocessor environment. This completely eliminates the need for `__underscored`names and similarly defensive tricks. Moreover, the current preprocessor definitions when an import declaration is encountered are ignored, so one software library can not affect how another software library is compiled, eliminating include-order dependencies.
* **Tool confusion**: Modules describe the API of software libraries, and tools can reason about and present a module as a representation of that API. Because modules can only be built standalone, tools can rely on the module definition to ensure that they get the complete API for the library. Moreover, modules can specify which languages they work with, so, e.g., one can not accidentally attempt to load a C++ module into a C program.

### [Problems modules do not solve](https://clang.llvm.org/docs/Modules.html#id12)

Many programming languages have a module or package system, and because of the variety of features provided by these languages it is important to define what modules do _not_ do. In particular, all of the following are considered out-of-scope for modules:

* **Rewrite the world’s code**: It is not realistic to require applications or software libraries to make drastic or non-backward-compatible changes, nor is it feasible to completely eliminate headers. Modules must interoperate with existing software libraries and allow a gradual transition.
* **Versioning**: Modules have no notion of version information. Programmers must still rely on the existing versioning mechanisms of the underlying language \(if any exist\) to version software libraries.
* **Namespaces**: Unlike in some languages, modules do not imply any notion of namespaces. Thus, a struct declared in one module will still conflict with a struct of the same name declared in a different module, just as they would if declared in two different headers. This aspect is important for backward compatibility, because \(for example\) the mangled names of entities in software libraries must not change when introducing modules.
* **Binary distribution of modules**: Headers \(particularly C++ headers\) expose the full complexity of the language. Maintaining a stable binary module format across architectures, compiler versions, and compiler vendors is technically infeasible.

## [Using Modules](https://clang.llvm.org/docs/Modules.html#id13)

To enable modules, pass the command-line flag `-fmodules`. This will make any modules-enabled software libraries available as modules as well as introducing any modules-specific syntax. Additional [command-line parameters](https://clang.llvm.org/docs/Modules.html#command-line-parameters) are described in a separate section later.

### [Objective-C Import declaration](https://clang.llvm.org/docs/Modules.html#id14)

Objective-C provides syntax for importing a module via an _@import declaration_, which imports the named module:

```objectivec
@import std;
```

The `@import` declaration above imports the entire contents of the `std` module \(which would contain, e.g., the entire C or C++ standard library\) and make its API available within the current translation unit. To import only part of a module, one may use dot syntax to specific a particular submodule, e.g.,

```objectivec
@import std.io;
```

Redundant import declarations are ignored, and one is free to import modules at any point within the translation unit, so long as the import declaration is at global scope.

At present, there is no C or C++ syntax for import declarations. Clang will track the modules proposal in the C++ committee. See the section [Includes as imports](https://clang.llvm.org/docs/Modules.html#includes-as-imports) to see how modules get imported today.

### [Includes as imports](https://clang.llvm.org/docs/Modules.html#id15)

The primary user-level feature of modules is the import operation, which provides access to the API of software libraries. However, today’s programs make extensive use of `#include`, and it is unrealistic to assume that all of this code will change overnight. Instead, modules automatically translate `#include` directives into the corresponding module import. For example, the include directive  
模块的主要用户级特性是导入操作，它提供了对软件库API的访问。然而，今天的程序广泛使用了`#include`，假定所有这些代码一夜之间就会改变是不现实的。相反，模块自动将`#include`指令转换为相应的模块导入。例如：

```cpp
#include <stdio.h>
```

will be automatically mapped to an import of the module `std.io`. Even with specific `import` syntax in the language, this particular feature is important for both adoption and backward compatibility: automatic translation of `#include` to `import` allows an application to get the benefits of modules \(for all modules-enabled libraries\) without any changes to the application itself. Thus, users can easily use modules with one compiler while falling back to the preprocessor-inclusion mechanism with other compilers.  
将自动映射到模块`std.io`的导入。即使使用语言中的特定`import`语法，这个特殊的特性对于接受度和向后兼容都很重要：`#include`到`import`的自动转换允许应用程序获得模块的好处（对于所有支持模块的库），而无需对应用程序本身进行任何更改。因此，用户可以轻松地在一个编译器中使用模块，而在其他编译器中使用预处理包含机制。

{% hint style="info" %}
 **Note**

The automatic mapping of `#include` to `import` also solves an implementation problem: importing a module with a definition of some entity \(say, a `struct Point`\) and then parsing a header containing another definition of `struct Point` would cause a redefinition error, even if it is the same `struct Point`. By mapping `#include` to `import`, the compiler can guarantee that it always sees just the already-parsed definition from the module.
{% endhint %}

While building a module, `#include_next` is also supported, with one caveat. The usual behavior of `#include_next` is to search for the specified filename in the list of include paths, starting from the path _after_ the one in which the current file was found. Because files listed in module maps are not found through include paths, a different strategy is used for `#include_next` directives in such files: the list of include paths is searched for the specified header name, to find the first include path that would refer to the current file. `#include_next` is interpreted as if the current file had been found in that path. If this search finds a file named by a module map, the `#include_next` directive is translated into an import, just like for a `#include` directive.\`\`

### [Module maps](https://clang.llvm.org/docs/Modules.html#id16)

The crucial link between modules and headers is described by a _module map_, which describes how a collection of existing headers maps on to the \(logical\) structure of a module. For example, one could imagine a module `std` covering the C standard library. Each of the C standard library headers \(`<stdio.h>`, `<stdlib.h>`, `<math.h>`, etc.\) would contribute to the `std` module, by placing their respective APIs into the corresponding submodule \(`std.io`, `std.lib`, `std.math`, etc.\). Having a list of the headers that are part of the `std` module allows the compiler to build the `std` module as a standalone entity, and having the mapping from header names to \(sub\)modules allows the automatic translation of `#include` directives to module imports.

Module maps are specified as separate files \(each named `module.modulemap`\) alongside the headers they describe, which allows them to be added to existing software libraries without having to change the library headers themselves \(in most cases [\[2\]](https://clang.llvm.org/docs/Modules.html#id6)\). The actual [Module map language](https://clang.llvm.org/docs/Modules.html#module-map-language) is described in a later section.

{% hint style="info" %}
**Note**

To actually see any benefits from modules, one first has to introduce module maps for the underlying C standard library and the libraries and headers on which it depends. The section [Modularizing a Platform](https://clang.llvm.org/docs/Modules.html#modularizing-a-platform) describes the steps one must take to write these module maps.
{% endhint %}

One can use module maps without modules to check the integrity of the use of header files. To do this, use the `-fimplicit-module-maps` option instead of the `-fmodules`option, or use `-fmodule-map-file=` option to explicitly specify the module map files to load.

### [Compilation model](https://clang.llvm.org/docs/Modules.html#id17)

The binary representation of modules is automatically generated by the compiler on an as-needed basis. When a module is imported \(e.g., by an `#include` of one of the module’s headers\), the compiler will spawn a second instance of itself [\[3\]](https://clang.llvm.org/docs/Modules.html#id7), with a fresh preprocessing context [\[4\]](https://clang.llvm.org/docs/Modules.html#id8), to parse just the headers in that module. The resulting Abstract Syntax Tree \(AST\) is then persisted into the binary representation of the module that is then loaded into translation unit where the module import was encountered.

The binary representation of modules is persisted in the _module cache_. Imports of a module will first query the module cache and, if a binary representation of the required module is already available, will load that representation directly. Thus, a module’s headers will only be parsed once per language configuration, rather than once per translation unit that uses the module.

Modules maintain references to each of the headers that were part of the module build. If any of those headers changes, or if any of the modules on which a module depends change, then the module will be \(automatically\) recompiled. The process should never require any user intervention.

### [Command-line parameters](https://clang.llvm.org/docs/Modules.html#id18)

`-fmodules`

Enable the modules feature.

## [Module Semantics](https://clang.llvm.org/docs/Modules.html#id19)

Modules are modeled as if each submodule were a separate translation unit, and a module import makes names from the other translation unit visible. Each submodule starts with a new preprocessor state and an empty translation unit.

{% hint style="info" %}
**Note**

This behavior is currently only approximated when building a module with submodules. Entities within a submodule that has already been built are visible when building later submodules in that module. This can lead to fragile modules that depend on the build order used for the submodules of the module, and should not be relied upon. This behavior is subject to change.
{% endhint %}

As an example, in C, this implies that if two structs are defined in different submodules with the same name, those two types are distinct types \(but may be _compatible_ types if their definitions match\). In C++, two structs defined with the same name in different submodules are the _same_ type, and must be equivalent under C++’s One Definition Rule.

{% hint style="info" %}
**Note**

Clang currently only performs minimal checking for violations of the One Definition Rule.
{% endhint %}

If any submodule of a module is imported into any part of a program, the entire top-level module is considered to be part of the program. As a consequence of this, Clang may diagnose conflicts between an entity declared in an unimported submodule and an entity declared in the current translation unit, and Clang may inline or devirtualize based on knowledge from unimported submodules.

### [Macros](https://clang.llvm.org/docs/Modules.html#id20)

The C and C++ preprocessor assumes that the input text is a single linear buffer, but with modules this is not the case. It is possible to import two modules that have conflicting definitions for a macro \(or where one `#define`s a macro and the other `#undef`ines it\). The rules for handling macro definitions in the presence of modules are as follows:  
C和C++预处理器假定输入文本是单个线性缓冲区，但是对于模块则不是这样。可以导入两个对宏定义有冲突的模块（或者其中一个定义某个宏，然后另一个取消它的定义）。在模块存在时处理宏定义的规则如下：

* Each definition and undefinition of a macro is considered to be a distinct entity. 一个宏的每个定义和解定义（undefinition）都被认为是一个不同的实体
* Such entities are _visible_ if they are from the current submodule or translation unit, or if they were exported from a submodule that has been imported.
* A `#define X` or `#undef X` directive _overrides_ all definitions of `X` that are visible at the point of the directive.
* A `#define` or `#undef` directive is _active_ if it is visible and no visible directive overrides it.
* A set of macro directives is _consistent_ if it consists of only `#undef` directives, or if all `#define` directives in the set define the macro name to the same sequence of tokens \(following the usual rules for macro redefinitions\).
* If a macro name is used and the set of active directives is not consistent, the program is ill-formed. Otherwise, the \(unique\) meaning of the macro name is used.

For example, suppose:

* `<stdio.h>` defines a macro `getc` \(and exports its `#define`\)
* `<cstdio>` imports the `<stdio.h>` module and undefines the macro \(and exports its `#undef`\)

The `#undef` overrides the `#define`, and a source file that imports both modules _in any order_ will not see `getc` defined as a macro.

