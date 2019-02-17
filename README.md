# CppParser

An easy, fast, and robust library to parse C/C++ source.

## Features
- It does not do pre-processing and also tries not to ignore comments.
- Developed from scratch and uses back-tracking yacc (BtYacc) to write C++ grammer.
- The result of parsing is the AST where elements of a file are arranged in a tree.
- Minimum dependency. Only external dependency is a [lexer](https://github.com/westes/flex) which is by default available on unix like platforms and can be easily get on Windows.

## Motivation ##
CppParser can be used to build tools that need parsing of C/C++ files.
I am using it to develop [cib](https://github.com/satya-das/cib/) which is a kind of source to source translator.

