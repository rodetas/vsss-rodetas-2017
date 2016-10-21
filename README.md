# Rodetas

> The Rodetas team is composed of students from Federal University of Ouro Preto (UFOP), especially students graduating in Control and Automation Engineering and Computer Science. We represent the university in nacional and internacional robotic competitions at the category IEEE VSSS (Very Small Size Soccer), in addition we have already been south american champions.

> The game is based on 2 teams against each other, with 3 robots each of them which act with the support of mathematical models and with no direct human interference during the match. The next tutorial explains how to install the software used by the team during the match. Our main dependencies are: OpenCV, SFML and TGUI

## How to install Rodetas Software

You can install using our [installer script](https://www.dropbox.com/s/0nr1ewfklxafvwa/configure.sh?dl=0) or using the step by step tutorial below.

### Updating repositories:
```sh
$ sudo apt-get update
```

### Installing dependencies:
```sh
$ sudo apt-get install git g++ uvcdynctrl build-essential cmake qt5-default libvtk6-dev zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libjasper-dev libopenexr-dev libgdal-dev libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev yasm libopencore-amrnb-dev libopencore-amrwb-dev libv4l-dev libxine2-dev libtbb-dev libeigen3-dev arduino-mk freeglut3-dev
```

### Installing OpenCV 3.0:
For first you have to download the source code version 3.0 [here](https://codeload.github.com/opencv/opencv/zip/3.0.0).
Then, lets install:

Go inside your downloaded path and type:
```sh
$ mkdir build && cd build
$ cmake -DWITH_QT=ON -DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON DBUILD_EXAMPLES=ON -D WITH_IPP=OFF ..
$ make -j4
$ sudo make install
$ sudo ldconfig
```
### Installing SFML 2.3.2
Now, there are some differences between installation on Ubuntu and Debian.
#### Debian Users:
Download the source code [here](http://mirror1.sfml-dev.org/files/SFML-2.3.2-linux-gcc-64-bit.tar.gz). Unzip it and go inside the directory.
```sh
$ sudo apt-get install libpthread-stubs0-dev libgl1-mesa-dev libx11-dev libxrandr-dev libfreetype6-dev libglew1.5-dev libsndfile1-dev libopenal-dev sudo libxcb-image0 libxcb-image0-dev libudev-dev libgudev-1.0-0
$ cmake .
$ make -j4
$ sudo make install
$ sudo ldconfig
```

#### Ubuntu Users:
```sh
$ sudo apt-get install libsfml-dev
```

### Installing TGUI 0.7
Also there are differences between distros.
#### Debian Users:
Download the source code [here](https://codeload.github.com/texus/TGUI/zip/0.7.1). Unzip it and cd into the directory.
```sh
$ cmake .
$ sudo make install
$ sudo ldconfig
```
#### Ubuntu Users:
Just type:
```sh
$ sudo add-apt-repository ppa:texus/tgui
$ sudo apt-get update
$ sudo apt-get install libtgui-dev
```

### Sublime Text 3
If you do not have a text editor you can download the latest build release [here](http://www.sublimetext.com/3). To install you are able to open using the Ubuntu Central Program or install via terminal.
```
$ sudo dpkg -i sublime-text_build-3065_amd64.deb
```