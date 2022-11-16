# Sequencer graphical user interface

![Node editor](./doc/nodeeditor.png)

Prototype of Sequencer Graphical User Interface.

## Installation on CODAC machines

The `sequencer-gui` application depends on `sup-dto`, `sequencer` and `sup-mvvm` libraries.

### Install sup-dto, sequencer

The `sup-dto` and `sequencer` are not yet packaged and should be build from the source

```
# install sup-dto
git clone https://git.iter.org/scm/coa/sup-dto.git
cd sup-dto; mvn clean install

# install sequencer
git clone https://git.iter.org/scm/coa/sequencer.git
cd sequencer; mvn clean install
```

### Install sup-mvvm library

```
# install latest gtest, libxml2
yum install codac-core-7.1-gtest-1.12.1-devel.x86_64 libxml2-devel.x86_64

# install qt5 
yum install qt5-qtbase-devel.x86_64 qt5-qtsvg-devel.x86_64 qt5-qttools-devel.x86_64 qt5-qtbase-gui.x86_64 adwaita-qt5.x86_64 mesa-libGL-devel.x86_64

# get sup-mvvm source
git clone https://git.iter.org/scm/coa/sup-mvvm.git

# compile
mkdir <build>; cd <build>
cmake <path-to-repo>
make -j4 && ctest
```

Please note, that thanks to CMake magic (the magic is located in $HOME/.cmake directory), installation is optional.
Without installation, libraries will be discoverable right from the build directory.

### Build sequencer-gui

```
# Get source code (develop branch)
git clone -b develop https://git.iter.org/scm/coa/sequencer-gui.git

# Compile
<build>mkdir build; cd build
cmake <path-to-repo>
make -j4 && ctest

# Run
<build>/bin/sequencer-gui
```

## Installation on custom Linux

It is possible to develop Sequencer GUI on custom Linux distributive. Tested on `Arch` Linux.

`sup-mvvm`, `sup-dto` and `sequencer` libraries should be compiled and installed before.

```
# We assume that the user wants to install all dependencies into <path-to-local>.

# Install sup-dto

# build sup-mvvm library
git clone https://git.iter.org/scm/coa/sup-mvvm.git
mkdir <build>; cd <build>
cmake <path-to-repo> && make -j4 && ctest

# build coa-compact library
git clone https://git.iter.org/scm/coa/coa-compact.git
mkdir <build>; cd <build>
cmake <source> && make -j4

# build Sequencer GUI
git clone -b develop https://git.iter.org/scm/coa/sequencer-gui.git
mkdir <build>; cd <build>
cmake <source> && make -j4 && ctest

# Run
<build>/bin/sequencer-gui
```
