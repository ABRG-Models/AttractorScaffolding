# Implementation coded by Seb James

This code builds using cmake.

Program code is in the directory sim/. There are some CTests in
tests/. Results are written into data/. There are python scripts in
plot/ to visualise the results. factorial/ contains some code by Peter
Luschny for computing factorials. You should build in build/ or in a
directory whose name is the same as is returned by the hostname
command on your computer.

## Prerequisites:

On Ubuntu or Debian:

```
sudo apt install cmake build-essential
```

You will then need to obtain, build and install mpir from github:
git://github.com/wbhart/mpir.git (see also http://mpir.org)

## Compiling

You should create a sub-directory called build (or alternatively with
the name of your computer's hostname).

```
pushd build
cmake ..
make -j6 # or however many cores you have
popd
```

## Running

Now you can run the programs in build/sim/ or the scripts. (Add more
info here!).
