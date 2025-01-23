# Unit tests

This folder contains a small library with testing utilites, and 5 testing executables.

## testoac-tree-gui-core

Testing `liboac-tree-gui-core.so`

## testoac-tree-gui-components

Testing `liboac-tree-gui-components.so`

## testoac-tree-gui-components-app

Testing `liboac-tree-gui-components.so` (event loop is necessary)

## testoac-tree-gui-components-views

Testing `liboac-tree-gui-views.so`  (event loop is necessary)

## testoac-tree-gui-integration

Testing everything (event loop is necessary, time consuming). These tests are not added 
to CTest discovery, so they are not executed by the IDE. However, they are executed via test.sh


