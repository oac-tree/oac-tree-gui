# Sequencer Graphical User Interface

![Node editor](./doc/nodeeditor.png)

Prototype of Sequencer Graphical User Interface.

## Installation on CODAC machines from nightly RPMs

To be written.

## Installation on CODAC machines (manual)

The `sequencer-gui` application depends on a number of Codac Operation Application libraries.

### Install prerequisites

```bash
yum install codac-core-7.1-gtest-1.12.1-devel.x86_64 libxml2-devel.x86_64
yum install qt5-qtbase-devel.x86_64 qt5-qtsvg-devel.x86_64 qt5-qttools-devel.x86_64 qt5-qtbase-gui.x86_64 adwaita-qt5.x86_64 mesa-libGL-devel.x86_64
```

### Install COA packages

Checkout packages from [COA git](https://git.iter.org/projects/COA/repos/sup-utils/browse
) and install them one-by-one using `mvn clean install`

- sup-utils
- sup-dto
- sup-di
- sup-protocol
- sup-epics
- sequencer
- sequencer-plugin-epics
- sup-mvvm
- sup-gui-extra
- sup-gui-core
- sequencer-gui

### Run

/opt/codac/bin/sequencer-gui

Other applications:

/opt/codac/bin/sequencer-operation
/opt/codac/bin/sup-pvmonitor
/opt/codac/bin/anyvalue-editor

## Installation on custom Linux

All COA packages are ordinary CMake packages and they can be compiled and installed on any Linux
of user choice like any other CMake project. Here we show the installation of `sup-utils` package. 
All others have to be installed in a similar way in the order given by the list above.
We assume that the user wants to install all dependencies into `<path-to-local-install>` (e.g. /opt/local).

```bash
git clone https://git.iter.org/scm/coa/sup-utils.git
mkdir <build>; cd <build>
cmake -DCMAKE_PREFIX_PATH=<path-to-local-install> -DCMAKE_INSTALL_PREFIX=<path-to-local-install> <path-to-repo>
make -j4; ctest; make install
```

It might be needed to add `<path-to-local-install>/lib` to `LD_LIBRARY_PATH`.

## Alternative installation on custom Linux

This method doesn't require installation, all packages will be used from their `<build>` directories
thanks to CMake magic (the magic is located in $HOME/.cmake directory). Use `-DDCOA_EXPORT_BUILD_TREE=OFF` 
during CMake configuration phase.

```bash
git clone https://git.iter.org/scm/coa/sup-utils.git
mkdir <build>; cd <build>
cmake -DCOA_EXPORT_BUILD_TREE=ON <path-to-repo>
make -j4; ctest
```

