HeatWave
========

HeatWave is a wavelet lifting library that served as a research tool to
establish the efficiency of wavelet transforms in images and videos on mobile
phones and other constrained devices. (The official research ended in 2006.)

Wavelet transforms can be performed on all 3 domains of a video, either on the
height and width plain (spatial), inbetween colour bands or along the time
axes (i.e. between frames).

The core of the library consists of a video, image, component (colour) and
lifting class, also an AVI reader. The lifting class (HeatWaveLift) implements
11 of the most common wavelet transforms, including that used in the JPEG 2000
algorithm. Most objects can be told not to *malloc()* any memory, but rather
to use an external memory buffer, this feature allowed us to use library
closely coupled to the on-board camera device of a testing devices.

## Building the Library

You will find project files for GNU Make, Microsoft Visual Studio 6 and
Microsoft eMbedded Visual C++ 4.04 under the _grp_ folder.

### Building with GNU Make

The following commands are all run in the _grp/make_ folder. To build the
library:

    $ make HeatLib.a  # build the library
    $ make            # build the library, the cli tools and the unit test

You might need to install 3rd party libraries first, the following should work
on Ubuntu and Debian based systems:

    $ sudo apt-get install libcppunit-dev libjasper-dev libmhash-dev libjpeg-dev

## Using the Command Line Tools

The command line tools are at best a bit unstable, and if possible they will
be cleaned up a bit, until such time please use at own risk. The option -h and
-H will details.
