Instructions for GitHub users
============================

The current recommended way of installing `oac-tree` GUI is by compiling it from the source as a part of
`oac-tree bundle <https://github.com/oac-tree>`_::

   git clone --recurse-submodules https://github.com/oac-tree/oac-tree-bundle.git
   mkdir <BUILD_DIR>
   cd <BUILD_DIR>
   cmake -DCMAKE_INSTALL_PREFIX=<INSTALL_PREFIX> <SOURCE_DIR>
   make -j24
   # note that install step is intentionally skipped
   source <INSTALL_PREFIX>/bin/oac-tree-path.sh

Please see detailed `oac-tree bundle installation instructions <https://github.com/oac-tree#installation-from-source>`_


