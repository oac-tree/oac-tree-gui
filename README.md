# oac-tree Graphical User Interface

## Main application

- <install_dir>/bin/sequencer-gui

![Explorer view](./doc/explorer-view.jpg)

Explorer view

![Composer view](./doc/nodeeditor-view.jpg)

Composer view (NodeEditor and InstructionTree panels)

![Composer view](./doc/composer-view.jpg)

Composer view (InstructionTree and Workspace panels)

![Operation View](./doc/operation-view.jpg)

Operation view (Real-time instruction tree with breakpoints and variable table)

Other applications:

- <install_dir>/bin/sequencer-operation
- <install_dir>/bin/sup-pvmonitor
- <install_dir>/bin/anyvalue-editor

## The source

- [Codac Operation Applications (COA)](https://git.iter.org/scm/coa)
- [oac-tree collection](https://github.com/orgs/oac-tree/repositories)

## Dependencies

- C++-17
- CMake >= 3.14
- gtest >= 1.12
- libxml2
- Qt6 (base, tools, charts, svg)
- oak-tree and plugins

*Note:* Qt5 is also possible with `-DCOA_USE_QT6=OFF` flag

**oak-tree and plugins**

- sup-utils
- sup-dto
- sup-di
- sup-protocol
- sup-epics
- sequencer
- sequencer-plugin-epics
- sup-auto-server
- sup-mvvm
- sup-gui-extra
- sup-gui-core
- sequencer-gui

## Installation on custom Linux

    We are currently working on a convenient way of installing oac-tree related packages all at once
    using the `vcpkg` dependency manager. While it is not yet ready, one has to install oac-tree packages
    one by one using the standard ``CMake/make/make install`` chain.

All oak-tree packages are ordinary CMake packages and they can be compiled and installed on any Linux
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
thanks to CMake magic (the magic is located in $HOME/.cmake directory). Use `-DCOA_EXPORT_BUILD_TREE=ON` 
during CMake configuration phase.

```bash
git clone https://git.iter.org/scm/coa/sup-utils.git
mkdir <build>; cd <build>
cmake -DCOA_EXPORT_BUILD_TREE=ON <path-to-repo>
make -j4; ctest
```

## Installation on CODAC

### Installation from RPMs

```bash
yum install codac-core-7.2-sequencer-gui.x86_64
```

## Run on CODAC

- /opt/codac/bin/sequencer-gui
- /opt/codac/bin/sequencer-operation
- /opt/codac/bin/sup-pvmonitor
- /opt/codac/bin/anyvalue-editor

## Manual build on CODAC using Maven

```bash
# prerequisites
yum install codac-core-7.2-gtest-1.12.1-devel.x86_64 libxml2-devel.x86_64
yum install qt5-qtbase-devel.x86_64 qt5-qtsvg-devel.x86_64 qt5-qttools-devel.x86_64 qt5-qtbase-gui.x86_64 qt5-qtcharts-devel.x86_64 adwaita-qt5.x86_64 mesa-libGL-devel.x86_64
```

Checkout packages from the list above from [COA git](https://git.iter.org/projects/COA/repos/sup-utils/browse) and install them one-by-one using `mvn clean install`
