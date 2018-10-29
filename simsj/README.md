# Implementation coded by Seb James

This implementation of the model is written in C++, writing out data
in text files (comma separated values). It has some python scripts for
data visualisation.

The code is written in a "procedural C++ style". I've used global
function definitions and few classes. I use some global variables
without apology. I chose this approach as the program is not large and
I consider it to be maintainable in this form.

The state space is represented by the type state_t (which is an
unsigned char). This contains sufficient bits for 4, 5 or 6 genes. The
genome space is represented by a fixed size array of genosect_t (which
is an unsigned int). There are N_Genes genosect_t variables in a full
genome, but not all bits of each genosect_t may be used. 2^(N_Genes-1)
bits are required in each genosect_t (That's 8 for N_Genes=4, 16 for
N_Genes=5 and 32 for N_Genes=6).

The code builds using cmake.

Program code is in the directory sim/. There are some CTests in
tests/. Results are written into data/. There are python scripts in
plot/ to visualise the results. factorial/ contains some code by Peter
Luschny for computing factorials. You should build in build/ or in a
directory whose name is the same as is returned by the hostname
command on your computer.

## Prerequisites:

cmake, a c++ compiler and the library mpir are required. On Ubuntu or Debian:

```bash
sudo apt install cmake build-essential
```
Ensures you have a compiler and cmake.

You will then need to obtain, build and install mpir from github:
git://github.com/wbhart/mpir.git (see also http://mpir.org)


I compiled mpir like this:
```bash
sudo apt install libtool autoconf yasm texinfo # Required only for the mpir build
git clone git://github.com/wbhart/mpir.git
cd mpir
autoreconf -is # I ignored a couple of warnings here
./configure --enable-cxx --prefix=/usr/local
make -j6 # or however many cores you have
sudo make install
sudo ldconfig # To ensure your system's dynamic linker can find the new libraries
```

## Compiling

You should create a sub-directory called build (or alternatively with
the name of your computer's hostname).

```bash
pushd build
cmake ..
make -j6 # or however many cores you have
popd
```

## Running

Now you can run the programs in build/sim/ or the scripts. (Add more
info here!).
