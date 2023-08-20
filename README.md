# Tetris 2.0 
Recreating Tetris with ImGui but with words. Where the aim is the blocks are cleared for a word found on the board.

 ### 🚧 WIP

 - [x] Build a clone of original tetris
 - [ ] Inbuilt parsing and detecting words
 - [ ] Improved visuals

## Technical Details
This version of the App framework uses [GLFW](https://www.glfw.org/) and [OpenGL](https://www.opengl.org/) as backend.
As a testing framework [Catch2](https://github.com/catchorg/Catch2) is included.
As buildsystem [meson](https://mesonbuild.com) is used.

## Setup
### Running on a standard linux (e.g. Ubuntu)
The following packages are needed to run build and run the application:
```
apt-get install -y bash libglfw3 libglfw3-dev xorg-dev libxkbcommon-dev meson clang git cmake libssl-dev cmake

apt-get install --yes --no-install-recommends wget build-essential libcurl4 && \
    wget https://curl.se/download/curl-7.81.0.tar.gz && \
    tar -xvf curl-7.81.0.tar.gz && cd curl-7.81.0 && \
    ./configure --with-openssl && make && make install
```
Additionally, it is necessary to clone Dear ImGui and ImPlot:
```
    git clone --depth 1 --branch v1.88 https://github.com/ocornut/imgui.git /imgui
    git clone --depth 1 --branch v0.14 https://github.com/epezent/implot.git /implot
```

Useful commands for building and running the application.

```
# Setup the build directory
meson setup builddir --native-file=native.build

# Compile the application
ninja -C builddir src/app/tetris/tetris

# Run the application
./builddir/src/app/tetris/tetris

# Run all tests
ninja -C builddir test
meson test -C builddir

```


