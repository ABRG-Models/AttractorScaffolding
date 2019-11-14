# Implementation of the model described in the paper 'Limit cycle dynamics can guide the evolution of gene regulatory networks towards point attractors'

## A quick introduction to the gene networks described in the paper

Note: The figures here (and their captions) are reproduced from the
paper under the terms of the Creative Commons 4.0 license and are
attributable to the authors of the paper.

The paper associated with this code repository considers a system of
Boolean gene networks. Consider a system of 5 genes, labelled a to e,
which interact with each other in a region of biological tissue.  Each
gene can be expressed (1) or not expressed (0). The instantaneous
state of all 5 genes can be represented as a 5 bit number with 32
possible values. Because the genes interact, the state of the system a
small time into the future is dependent on the current state. The
figure below illustrates; each gene receives inputs i to v from the
current time step. A table can be created, and randomly populated so
that each of the genes has its state a short time in the future
(i.e. the next timestep of a simulation) specified. The randomly
populated number, which is created by arranging all the coloured
columns in Fig. 1 into a line to give a 160 bit number, is what we
refer to as the 'genome' in the paper.

![Figure 1](/paper/figures/Fig1.jpg)

**Figure 1** Gene interaction network. A network of n=5 interacting
genes, shown labelled a-e, each with inputs labelled i-v. The truth
table determines the expression level of each gene in response to
each of the 2^n=32 possible patterns of gene expression. The coloured
elements thus constitute a 'genome' of N=n2^n=160 bits, which in this
case specifies a maximally fit network (f=1).

Referring to the example in Fig. 1, if all the genes are in the 'not
expressed' (0) state (look at the first row of the 'input' table),
then at the next time step, gene a (red) will be 'not expressing' or
0, gene b (yellow) will be 'expressing' or 1, and so on. If we have
some pre-specified starting state (For example, [00001]) and some
required target state (say [10101]), then we can compute the states
that the system will progress through and see if the intial state maps
to the target state. We regard as fully fit (fitness=1) a genome which
progresses from the initial state [00001] to the target state (in any
number of steps) and which then cycles back on itself - that is, the
state [10101] leads directly to [10101] as a 'stable point attractor'.

![Figure 2](/paper/figures/Fig2.jpg)

**Figure 2** The attractor landscape. The developmental dynamics of the
network that is specified by the genome in Fig. 1 reveals five
attractors (four point attractors and one with a limit cycle of
length two). Every possible gene expression pattern is represented
by one dot, and the transitions between states are represented by
arrows. Initial states [10000] and [00000] map to target states
[10101] and [01010] as point attractors. The blue path corresponds
to the development of the network in the anterior context and the
red path corresponds to the development of the network in the
posterior context.

The attractor landscape specified by the genome in the table in Fig. 1
is shown in Fig. 2. Each dot in the figure represents a single gene
expression state and indicates how the system develops over
time. Here, the initial state [10000] follows a path through 5
intermediate states to finally end up on the stable point attractor
[10101], which cycles back on itself. The same genome also takes a
starting state of [00000] to the state [01010].

Up to this point, I've referred to 5 genes interacting within a
'single context'; meaning that the genes are in the same spatial
location.  This work relates the genes a-e to a specific set of genes
which are known to be important in the arealization of the mammalian
neocortex. These genes (Fgf8, Pax6, Emx2, Sp8 and Coup-tf1) produce
specific patterns of expression; Fgf8 is initially expressed at the
anterior part of the cortical plate, and initiates a cascade of
interactions which result in anterior to posterior *or* posterior to
anterior gradients of expression for each of the 5 genes. We divided
space up into only two compartments, anterior and posterior and asked
how easy is it to find the genome which takes the anterior state
[10000] to the anterior target [10101] and the posterior initial state
[00000] to the target [01010]. The anterior and posterior target
states represent a spatial gradient in expression for all 5 genes. By
random search, this is difficult. Approximately 1 in every 60000
genomes achieves this, where we define the desired, stable target
states as having fitness 1, and anything else fitness 0.

The paper describes a mechanism by which an evolutionary search of the
genome space can be sped up compared with a random search. It does
this by allowing genomes which do not fulfil the required target
states perfectly to nevertheless have fitness > 0 as long as they
express the desired gene expressions for some proportion of
developmental time. By this mechanism, a search of the genome which
flips a few bits on each evolutionary generation can find 'paths up to
the peaks in the fitness landscape' and accelerates this simple, 2
context (anterior and posterior) system by 70 times. Furthermore, we
show that this speed-up is maintained even as the number of spatial
contexts increases. If we divide space up into a larger number of
compartments, the difficulty of the random search increases
exponentially, but the effectiveness of attractor scaffolding also
increases exponentially!

## The code

This implementation of the Boolean network model is written in C++,
writing out data in text files (comma separated values) into the
directory data/. It has some python scripts for data visualisation in
the subdirectory plot/ and scripts to run multiple instances of the
model in the subdirectory scripts/.

The code is flexible enough to specify 3, 4, 5 or 6 genes for the 'k=n'
network in which every gene receives input from every other gene,
including itself. It can also compute an n=k-1 network for 7
genes. The number of genes, as well as the choice of k=n-1 or k=n and
some other features are set at compile time.

The code builds using cmake. There is a cpp file for every program,
though some programs are compiled into several binaries, with
different #defines resulting in different program behaviour. Consult
the **sim(_supp)/CMakeLists.txt** file for details of the compilation.

The state space is represented by the type state_t (which is an
unsigned char). This contains sufficient bits for 4, 5 or 6 genes. The
genome space is represented by a fixed size array of genosect_t (which
is set to either an unsigned int or an unsigned long long int at
compile time). There are N_Genes genosect_t variables in a full
genome, but not all bits of each genosect_t may be used. For 'k=n',
2^(N_Genes) bits are required in each genosect_t (That's 16 for
N_Genes=4, 32 for N_Genes=5 and 64 for N_Genes=6).

The main, reported program code is in the directory
sim/. Supplementary analysis code is in sim_supp/. There are some
CTests in tests/. Results are written into data/. There are python
scripts in plot/ to visualise the results. factorial/ contains some
code by Peter Luschny for computing factorials. You should build in
build/.

## Prerequisites:

cmake, a c++ compiler and the libraries mpir and jsoncpp are required. On Ubuntu or Debian:

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

I compiled jsoncpp like this:
```sh
git clone https://github.com/open-source-parsers/jsoncpp.git
cd jsoncpp
mkdir build
cd build
cmake ..
make
sudo make install
```

## Compiling

You should create a sub-directory called build to compile
AttractorScaffolding.

```bash
mkdir -p build && pushd build
cmake .. # alternatively use `CXX=icpc cmake ..` or `CXX=clang++ cmake ..` to compile with an optimised compiler
make -j6 # or however many cores you have
popd
```

## Running

See the sim/README.md for details about all the programs that are
compiled in build/sim/

To reproduce the essential result of this work, first run:

```
cd ./scripts/
./runevolve_c2.sh
```

This will generate data files in data/. These can then be plotted with
plot/main/fig4.py.

```
cd plot/main
python fig4.py
```

The result of exhaustively searching the n=3 gene system can be
reproduced by running:

```
./build/sim/proprandom3
```

## Directories

### sim

Simulation programs and header files.

### sim_supp

Supplementary simulation programs.

### standalone

The standalone implementation, evolve.cpp.

### scripts

Some bash scripts to run the programs in **sim/** and **sim_supp/**.

### data

A directory into which the results of simulations are written.

### plot

Scripts to plot results from data found in **data/**.

### combinatorics

Contains a document analysing the probability of fit states as a
function of the ending limit cycle size.

### factorial

Contains factorial code from Peter Luschny and some test programs
using this library.

### tests

A few unit testing programs.
