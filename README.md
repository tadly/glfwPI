glfwPI - PictureIt test project
================================
[![Build Status](https://travis-ci.org/Tadly/glfwPI.svg?branch=pi_refactor)](https://travis-ci.org/Tadly/glfwPI)

Just a test project I use to work on [PictureIt](https://github.com/linuxwhatelse/PictureIt)


## Requirements
If you're on a typical linux installation (with xorg and pulseaudio) you should be just fine.


## Build instructions
Here, have some Copy&Paste help :)
```bash
git clone https://github.com/Tadly/glfwPI
cd glfwPI

mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=. ../

make install
```

## Usage
```bash
bin/glfwPI [img-directory]
```
