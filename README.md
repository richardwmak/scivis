# Compilation instructions

This code was compiled and tested on a VM running Xubuntu 18.10. Required software:

* `make`
* `g++`
* `libx11-dev`
* `pkg-config`
* `libfltk1.3-dev`
* `libxrender-dev`
* `libxcursor-dev`
* `libxft-dev`
* `libxinerama-dev`

i.e. on Ubuntu or Debian, execute `sudo apt install make g++ libx11-dev pkg-config libfltk1.3-dev libxrender-dev libxcursor-dev libxft-dev libxinerama-dev`

In the root directory, execute `make run`.

# Controls

* Click and drag to apply a force.
* When streamlines are enabled and the grid is not, hold control and click to set seed points.
* Left and right arrow rotate the camera left and right.
* Up and down arrow zoom the camera in and out.
* A and D move the camera laterally left and right.
* R resets the camera view.

In order to draw streamlines using the grid as seeds, the 'streamline grid' option needs to be toggled on. 

# Other

Based on the skeleton code provided in the course. Other code written by Richard Mak (richardwmak@gmail.com)

This software is based in part on the work of the FLTK project (http://www.fltk.org) and FFTW (http://www.fftw.org)
