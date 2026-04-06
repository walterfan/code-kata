# What is kata

Kata 中文应该称为"套路", 练习武术的秘诀就是掌握各种套路, 先要采百家之长, 熟知江湖上各大小门派的套路, 才能融会贯通, 达到无招胜有招的境界

Dave Thomas - "The Pragmatic Programmer "一书的作者, 提出了 Code Kata 这个想法, Dave 还在他的网站上收集了一小的练习项目(http://codekata.com/)。

作为一个专业程序员，我还是希望练习一些工作中经常能用得上的套路，例如一些文件修改，图片切割，网络收发的小套路, 故而在此做一些套路的整理和收集

## Structure

This repository is organized as a collection of git submodules, each covering a different language or topic:

| Submodule | Language / Topic | Repository |
|-----------|-----------------|------------|
| [kata-cpp](./kata-cpp) | C++ | https://github.com/walterfan/kata-cpp |
| [kata-java](./kata-java) | Java | https://github.com/walterfan/kata-java |
| [kata-python](./kata-python) | Python | https://github.com/walterfan/kata-python |
| [kata-go](./kata-go) | Go | https://github.com/walterfan/kata-go |
| [kata-rust](./kata-rust) | Rust | https://github.com/walterfan/kata-rust |
| [kata-web](./kata-web) | JS / TS / HTML / CSS | https://github.com/walterfan/kata-web |
| [kata-app](./kata-app) | Application | https://github.com/walterfan/kata-app |

The `example/` folder contains legacy code (cpp_kata, go_kata, java_kata, js_kata, python_kata) that has been migrated into the individual submodule repositories above.

## Getting Started

Clone this repository with all submodules:

```bash
git clone --recurse-submodules git@github.com:walterfan/code-kata.git
```

If you already cloned without submodules, initialize them with:

```bash
git submodule update --init --recursive
```

To pull the latest changes for all submodules:

```bash
git submodule update --remote --merge
```
