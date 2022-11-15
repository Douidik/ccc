# The ANSI-C Cursed Compiler
> ANSI-C compiler written in C++20 supporting basic C code.
## Work in progress 🔬:
- ### Lexical analysis
	- 🟢 Regular expression implementation 🔤
	- 🔴 Lexer engine and tokenization 🗃️
- ### Parser:
	- 🔴 Abstract system tree, the node structure 🌳
	- 🔴 Parser engine, tokens to AST 🧠
- ### Compiler
	- 🔴 Code generation from AST 🧬
		- 🔴 x86_64 asm 🐘
		- 🔴 ARM asm 🐝
		- 🔴 6502 asm 🦖
## Build it from home 🏡:
> Using Linux, C++20 compiler, CMake and Ninja 
```
git clone https://github.com/Douidik/ccc.git
cd ccc/
mkdir build && cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=<RELEASE/DEBUG>
ninja
bin/ccc-cmd -h
```
