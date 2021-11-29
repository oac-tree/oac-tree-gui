# Sequencer graphical user interface

## Installation on CODAC machines

### Install cpp-mvvm library

The `sequencer-gui` application depends on Qt, `cpp-mvvm` and `sequencer` libraries.

```
#install qt5 
yum install codac-core-7.0-qt5.x86_64

# get cpp-mvvm source
git clone https://git.iter.org/scm/coa/cpp-mvvm.git

# compile
cd <source>; mkdir build; cd build
PATH=/opt/codac/qt5/bin:$PATH cmake ../
make -j4
ctest
```

It is perfectly fine not to install the library after the build. Thanks to CMake magic it can be discovered and used right from the `build` directory.

### Install sequencer-gui

```
# Get source code (develop branch)
git clone -b develop https://git.iter.org/scm/coa/sequencer-gui.git

# Compile
cd <source>; mkdir build; cd build
PATH=/opt/codac/qt5/bin:$PATH cmake ../
make -j4
ctest

# Run
bin/sequencer-gui
```
