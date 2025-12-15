Instructions for CODAC users
============================

Installation from RPM
---------------------

Sequencer Graphical User Interface can be installed with all dependencies using::

   yum install codac-core-7.3-sequencer-gui.x86_64

Running on CODAC
---------------------

Run one of the related applications from the command line::

   /opt/codac/bin/sequencer-gui
   /opt/codac/bin/sequencer-operation
   /opt/codac/bin/sup-pvmonitor
   /opt/codac/bin/anyvalue-editor

Manual compilation
---------------------

Install all prerequisites::

   yum install codac-core-7.3-gtest-1.12.1-devel.x86_64 libxml2-devel.x86_64
   yum install qt5-qtbase-devel.x86_64 qt5-qtsvg-devel.x86_64 qt5-qttools-devel.x86_64 qt5-qtbase-gui.x86_64 qt5-qtcharts-devel.x86_64 adwaita-qt5.x86_64 mesa-libGL-devel.x86_64

The CODAC Operation Applications repositories are located at `COA repositories <https://git.iter.org/scm/coa>`_.
Check out all packages from the list, compile and install them one-by-one, using the `mvn clean install` command::

   sup-utils
   sup-dto
   sup-di
   sup-protocol
   sup-epics
   sequencer
   sequencer-plugin-epics
   sup-auto-server
   sup-mvvm
   sup-gui-extra
   sup-gui-core
   sequencer-gui

