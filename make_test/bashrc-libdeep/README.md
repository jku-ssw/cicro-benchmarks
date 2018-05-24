# libdeep: C/C++ library for deep learning

<img src="https://github.com/bashrc/libdeep/blob/master/img/logo.png?raw=true" width=200/>

This is a C library which can be used in deep learning applications.  What differentiates libdeep from the numerous other deep learning systems out there is that it's small and that trained networks can be exported as *completely standalone C or Python programs* which can either be used via the commandline or within an Arduino IDE for creating robotics or IoT applications.

A Python API for libdeep can be found at https://github.com/bashrc/libdeep-python

<img src="https://github.com/bashrc/libdeep/blob/master/img/training.jpg?raw=true" width=640/>

Installation
============

On Debian based systems:

```bash
sudo apt-get install build-essential doxygen xdot
```

If you want to be able to visualize call graphs for development or debugging purposes then you will need to install the [egypt](http://www.gson.org/egypt) script.

On Arch based systems:

``` bash
sudo pacman -S gcc doxygen egypt xdot
```

To build from source:

```bash
make
sudo make install
```

This creates the library and installs it into /usr/local

Unit Tests
==========

You can run the unit tests to check that the system is working as expected:

```bash
cd unittests
make
./tests
```

Or to check for any memory leaks:

```bash
valgrind --leak-check=full ./tests
```

Source Documentation
====================

To generate source code documentation make sure that you have doxygen installed and then run the generatedocs.sh script.  A subdirectory called docs will be created within which html and latex formated documentation can be found.  For general usage information you can also see the manpage.

```bash
man libdeep
```

Text or Numeric
===============

Inputs within training data sets can include both text and/or numeric data. For example, within the XOR training data you can have something like:

``` text
0.0,0.0,0.0
0.0,1.0,1.0
1.0,0.0,1.0
1.0,1.0,0.0
```

or alternatively, using whatever text representations you prefer:

``` text
zero,zero,0.0
zero,one,1.0
one,zero,1.0
one,one,0.0
```

Showing the call graph
======================

<img src="https://github.com/bashrc/libdeep/blob/master/img/callgraph.jpg?raw=true" width=640/>

If you want to visualize the call graph for a particular source file for debugging or development purposes:

``` bash
SOURCEFILE=deeplearn.c make graph
```

And you can change the *SOURCEFILE* value to whatever file you're interested in.

Examples
========

<img src="https://github.com/bashrc/libdeep/blob/master/img/learned_features.jpg?raw=true" width=640/>

There are also some example programs within the examples directory. Reading the examples is the best way to learn how to use this library within your own code. Examples are:

 * Face recognition with a deep convolutional network
 * Automatic feature learning and image reconstruction from features
 * Determining whether a cancer is malignant or benign
 * Assessing wine quality from ingredients
 * Predicting concrete quality from ingredients

<img src="https://github.com/bashrc/libdeep/blob/master/img/weight_magnitude.jpg?raw=true" width=640/>

Using trained neural nets in your system
========================================

You can export trained neural nets either as a C program or a Python program. These programs are completely independent and can be used either as commands or integrated into a larger software application. This makes it easy to use the resulting neural net without needing to link to libdeep. See the source code in the examples directory for how to use the export function. If you include the word "sketch" or "arduino" within the filename to be exported to then it will appear as Arduino compatible C suitable for use within an Arduino IDE rather than as standard C.

In your training program:

``` C
deeplearn_export(&learner, "export.c");
```

Then when training is done:

``` bash
gcc -o export_c export.c -lm
./export_c [first input] [second input]...
```

Portability
===========

Although this software was primarily written to run on Linux-based systems it's pretty much just vanilla C99 standard code and so it should be easily portable to other platforms, such as Microsoft Windows and Mac systems. The independent random number generator should mean that results are consistent across different compilers and platforms.

Packaging
=========

To build packages for Debian (deb) see https://github.com/bashrc/libdeep-debian
