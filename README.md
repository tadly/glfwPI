glfwPI - PictureIt test project
================================
[![Build Status](https://travis-ci.org/Tadly/glfwPI.svg?branch=master)](https://travis-ci.org/Tadly/glfwPI)

Just a test project I use to work on [PictureIt](https://github.com/linuxwhatelse/PictureIt)

## Requirements
If you're on a typical linux installation (with xorg and pulseaudio)  
you should be just fine.

If you'r trying to build this project on a minimal linux system,  
you might have to install some libs first though.

The following **Ubuntu** package names may vary depending on your distribution:
* libsndfile1-dev
* libpulse-dev
* libgl1-mesa-dev
* libglu1-mesa-dev
* libx11-dev
* libxcursor-dev
* libxi-dev
* libxinerama-dev
* libxrandr-dev
* libxxf86vm-dev

## Build instructions
Here, have some Copy&Paste help :)
```bash
git clone https://github.com/Tadly/glfwPI
cd glfwPI

mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=./install ../

make install
```
After that, you should have a `bin/glfwPI` executable

## Usage
Super easy (the command-line would tell you the same)
```bash
bin/glfwPI img-directory [-b spectrum-bar-count] [-a audio-file]
```
[Check here](http://www.mega-nerd.com/libsndfile/) for a list of supported audio formats
(I tested \*.wav only though)

On startup *glfwPI* will print some keyboard-shortcuts you
can use to mess around with it :)
