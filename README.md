# CodingAssistant-Clang #
***

## Description ##
This tool can be used to check those non-syntax flaws in the code. It is created to check your projects or code fragments expecting to find those parts which conflict with advices in the following books:
>[1]Bjarne Stroustrup. **The C++ Programming Language 4th Edition**.
>
>[2]Bjarne Stroustrup. **The C++ Programming Language 3rd Edition**.
>
>[3]Herb Sutter ,Andrei Alexandrescu . **C++ Coding Standards 101 Rules ,GuideLines, and Best Practices**.
>
>[4]Scott Meyers . **Effective C++: 55 Specific Ways to Improve Your Programs and Designs,3rd Edition**.

Our tool is based on the clang-3.5, the latest version of [clang](http://clang.llvm.org/). And we implement this tool using [LibTooling and LibASTMatchers](http://clang.llvm.org/docs/LibASTMatchersTutorial.html) provided by clang. We also find [this tutuorial](https://github.com/loarabia/Clang-tutorial) very helpful. 

***

## Installation ##

>sudo apt-get install clang-3.5


>sudo apt-get install libclang-3.5-dev


>cd src

>make

We found it sometimes may fail to build because of the llvm version, so you can revise the LLVMCONFIG in the src/makefile manually. In a word, to build the project successfully, you need the right **llvm-config**(may be /usr/lib/llvm-3.5/bin/llvm-config) command.

***

## Usage ##
It's very east to use.

>./utility sourcefile1, sourcefile2 -- [options]

**Attention, '--'is needed**

You can see the ![command](https://github.com/jitara/CodingAssistant-Clang/blob/master/command.png) picture and ![rule001 of the result](https://github.com/jitara/CodingAssistant-Clang/blob/master/rule001.png) picture here.

Also by writing your own shell script, you can easily use this tool on a project.


***

## Why we do this ##
This tool can be used to do some research works. It will also be helpful in the daily work if you want to avoid some dangerous flaws which may take you very long time to debug(such as "naked" new without using RAII). The most exciting thing we think is that based on this tool you can produce you own tailor-made tool easily.  

And this tool is far from perfect sitution. We build this tool in a very easy to learn and understand structure. We provide **UnitTest** directory in which you can manipulate any single feature which you are interested in. And we also provide detail **README** documention in both **UnitTest** and **src**. They will be very helpful.
