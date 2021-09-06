# linux_syspub
封装Linux C++内核操作，包含多线程、多进程、网络通信等操作类封装

## Code Style

[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

### How to use VSCode to format

1. `Ctrl + ,` to call out configuration window.
2. search *Clang_format_fallback Style*
3. use this configuration: { BasedOnStyle: Google, UseTab: Never, IndentWidth: 2, TabWidth: 2, AllowShortIfStatementsOnASingleLine: false, ColumnLimit: 0, AccessModifierOffset: -2, SortIncludes: false, }
4. search *Format On Save* and choose it.
