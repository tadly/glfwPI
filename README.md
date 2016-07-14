glfwPI - PictureIt test project
================================
[![Build Status](https://travis-ci.org/Tadly/glfwPI.svg?branch=master)](https://travis-ci.org/Tadly/glfwPI)

Just a test project I use to work on [PictureIt](https://github.com/linuxwhatelse/PictureIt)

## Build instructions
Here, have some Copy&Paste help :)
```bash
git clone https://github.com/Tadly/glfwPI
cd glfwPI
git submodule update --init

mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=./install ../

make install
```
After that, you should have a `bin/glfwPI` executable

## Usage
Super easy (the command-line would tell you the same)
```bash
bin/glfwPI </path/to/dir/with/images>
```

On startup *glfwPI* will print some keyboard-shortcuts you
can use to mess around with it :)
