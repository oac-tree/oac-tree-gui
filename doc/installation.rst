Installation
============

We are currently working on a convenient way of installing oac-tree related packages all at once
using the `vcpkg` dependency manager. While it is not yet ready, one has to install oac-tree packages
one by one using the standard ``CMake/make/make install`` chain.

Source
------

* `oac-tree repository collections <https://github.com/orgs/oac-tree/repositories>`_
* `Codac Operation Applications <https://git.iter.org/scm/coa>`_

Prerequisites for oac-tree GUI
------------------------------

* C++-17
* CMake >= 3.14
* gtest >= 1.12
* libxml2
* oac-tree and plugins
* Qt6 (qt6-base, qt6-tools, qt6-charts, qt6-svg)

Qt5 build is also possible with ``-DCOA_USE_QT6=OFF`` flag.

Install COA packages
--------------------

Checkout and install packages from `oac-tree repository collections <https://github.com/orgs/oac-tree/repositories>`_

* `sup-utils <https://github.com/oac-tree/sup-utils>`_
* `sup-dto <https://github.com/oac-tree/sup-dto>`_
* `sup-di <https://github.com/oac-tree/sup-di>`_
* `sup-protocol <https://github.com/oac-tree/sup-protocol>`_
* `sup-epics <https://github.com/oac-tree/sup-epics>`_
* `oac-tree <https://github.com/oac-tree/oac-tree>`_
* `oac-tree-epics <https://github.com/oac-tree/oac-tree-epics>`_
* `oac-tree-server <https://github.com/oac-tree/oac-tree-server>`_
* `sup-mvvm <https://github.com/oac-tree/sup-mvvm>`_
* `sup-gui-extra <https://github.com/oac-tree/sup-gui-extra>`_
* `sup-gui-core <https://github.com/oac-tree/sup-gui-core>`_
* `oac-tree-gui <https://github.com/oac-tree/oac-tree-gui>`_

Installation on custom Linux
-----------------------------

All COA packages are ordinary CMake packages and they can be compiled and installed on any Linux
of user choice like any other CMake project. Here we show the installation of `sup-utils` package.
All others have to be installed in a similar way in the order given by the list above.
We assume that the user wants to install all dependencies into `<path-to-local-install>` (e.g. /opt/local)::

   git clone https://github.com/oac-tree/sup-utils.git
   mkdir <build>; cd <build>
   cmake -DCMAKE_PREFIX_PATH=<path-to-local-install> -DCMAKE_INSTALL_PREFIX=<path-to-local-install> <path-to-repo>
   make -j4; ctest; make install

It might be needed to add `<path-to-local-install>/lib` to `LD_LIBRARY_PATH`.

Alternative installation on custom Linux
-----------------------------------------

This method doesn't require installation, all packages will be used from their `<build>` directories
thanks to CMake magic (the magic is located in $HOME/.cmake directory). Use `-DCOA_EXPORT_BUILD_TREE=ON`
during CMake configuration phase::

   git clone https://github.com/oac-tree/sup-utils.git
   mkdir <build>; cd <build>
   cmake -DCOA_EXPORT_BUILD_TREE=ON <path-to-repo>
   make -j4; ctest

