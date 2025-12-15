Custom installation
===================

**These instructions are intended for developers.**

All COA packages are ordinary CMake packages and they can be compiled and installed on any Linux
distribution of the user's choice, like any other CMake project. 

Prerequisites for oac-tree GUI
------------------------------

* C++-17
* CMake >= 3.14
* gtest >= 1.12
* libxml2
* oac-tree and plugins
* Qt6 (qt6-base, qt6-tools, qt6-charts, qt6-svg)

A Qt5 build is also possible with the ``-DCOA_USE_QT6=OFF`` flag.

Install COA packages
--------------------

Check out and install packages from `oac-tree repository collections <https://github.com/orgs/oac-tree/repositories>`_

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

Packages have to be installed one-by-one using the standard CMake chain in the order given by the list above.
The example below is provided for the `sup-utils` package.
This assumes that the user wants to install all dependencies into `<path-to-local-install>` (e.g. /opt/local)::

   git clone https://github.com/oac-tree/sup-utils.git
   mkdir <build>; cd <build>
   cmake -DCMAKE_PREFIX_PATH=<path-to-local-install> -DCMAKE_INSTALL_PREFIX=<path-to-local-install> <path-to-repo>
   make -j4; ctest; make install

You might need to add `<path-to-local-install>/lib` to `LD_LIBRARY_PATH`.

Alternative installation method
-----------------------------------------

This method doesn't require installing the packages; all packages will be used from their
`<build>` directories thanks to CMake's build tree export feature (the configuration is located in
the $HOME/.cmake directory). 
Use `-DCOA_EXPORT_BUILD_TREE=ON` during the CMake configuration
phase::

   git clone https://github.com/oac-tree/sup-utils.git
   mkdir <build>; cd <build>
   cmake -DCOA_EXPORT_BUILD_TREE=ON <path-to-repo>
   make -j4; ctest

