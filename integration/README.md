
ABOUT
=====

sshdrop-trayicon provides GTK-based desktop-integration of sshdrop.
It monitors the SSHDROP_CLIPBOARD_OUTPUT_DIR (configurable via
~/.sshdrop/config) for changes in files therein. If files are changed,
whic hhappns when new data is "dropped", a popup is displayed, showing 
the sender of the data und some lines of preview. It is then possible
to copy the data into the X11 primary selection ( better known as clipboard) by
clicking "Copy to Clipboard".



PREREQUISITES
=============

To compile sshdrop-trayicon you need the following tools and libraries:

Libs:

* libgtk-2.0 (2.24.10 or higher)
* libnotify 

Tools:

* cmake
* make


Under Debian, those packages my be installed using the followign commandline:

~~~~shell
apt-get install libgtk-2.0-dev libnotify-dev cmake build-essential
~~~~
   
   
COMPILING & INSTALLATION
========================

~~~~shell
mkdir build
cd build
cmake ..
make
sudo make install
~~~~


After sucessful compilation, start sshdrop-trayicon using the following commandline:

~~~~shell
$ sshdrop-trayicon /path/to/SSHDROP_CLIPBOARD_OUTPUT_DIR  (as defined id .sshdrop/config) &
~~~~








