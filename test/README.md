# Unit tests

This folder contains a small library with testing utilites, and 5 testing executables.

## testsequencer-gui-core

Testing `libsequencer-gui-core.so`

## testsequencer-gui-components

Testing `libsequencer-gui-components.so`

## testsequencer-gui-components-app

Testing `libsequencer-gui-components.so` (event loop is necessary)

## testsequencer-gui-components-views

Testing `libsequencer-gui-views.so`  (event loop is necessary)

## testsequencer-gui-components-integration

Testing everything (event loop is necessary, time consuming). These tests are not added 
to CTest discovery, so they are not executed by the IDE. However, they are executed via test.sh


