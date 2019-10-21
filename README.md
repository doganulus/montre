# montre

A timed regular expression matcher

## Usage
```sh
montre [OPTIONS ...] PATTERN [FILE]
```
## Options
```sh
-b, --offline              Offline Mode (Batch)
-e, --expr=PATTERN         Use PATTERN for matching
-f, --filename=FILE        Use FILE for matching
-i, --online               Online Mode (Incremental)
-h, --help                 Display this information
-o, --output=FILE          Place the output into FILE (Default: stdout)

    --output-type=TYPE     Supported types are <end|zone>
    --syntax-help          Display the syntax of timed regular expressions
    --version              Version Information
```

## Syntax of Timed Regular Expressions

```sh
Atom = p                   (Propositional Symbol)
     : !Atom               (Negation)
     : Atom1 && Atom2      (Conjunction)
     : Atom1 || Atom2      (Disjunction)

Expr = Atom                (Atomic Expression)
     : <:Atom              (Begin-anchored)
     : Atom:>              (End-anchored)
     : <:Atom:>            (BeginEnd-anchored)
     : Expr1 ; Expr2       (Concatenation)
     : Expr1 | Expr2       (Union)
     : Expr1 & Expr2       (Intersection)
     : Expr1 % (i,j)       (Time Restriction)
     : Expr1 *             (Zero-or-more Repetition)
     : Expr1 +             (One-or-more Repetition)
     : (Expr)              (Grouping)
```
# Installation

Ubuntu LTS 14.04 64-bit is the recommended platform. 

## Requirements

  * GNU C/C++ and the corresponding libraries.
  * Pure Programming Language:
  	* The GNU multiprecision library, or some compatible replacement such as MPIR.
  	* The GNU multiprecision floating point library.
  	* LLVM (3.5 or less. It is known that LLVM 3.6+ is incompatible.)
  	* More info: https://bitbucket.org/purelang/pure-lang/wiki/GettingStarted
  * pkg-config

## Instructions

If you have problems with permissions don't forget to prefix with `sudo`

1. Clone the Montre repository:

  ```sh
  git clone https://github.com/doganulus/montre
  cd montre
  ```

2. Build and install Montre:

  ```sh
  make && make install
  ```

  It will install Montre with the default prefix /usr/local. You can modify Makefile if you would like to install to a custom location.

## Running Montre in Docker

Docker provides an alternative (and easier) way for people that want to run Montre without needing to install all dependencies. If Docker is not installed on your machine, you can install using [these instructions](https://docs.docker.com/install/linux/docker-ce/ubuntu/#extra-steps-for-aufs). Note that you may need `sudo` for `docker` commands below. Many thanks to [@mibarg](https://github.com/mibarg) for preparing the `Dockerfile`.

1. Pull the Montre docker image.
  ```sh
  docker pull doganulus/montre:latest
  ```

2. Start a command prompt inside the container.
  ```sh
  docker run -it doganulus/montre:latest /bin/bash
  ```

3. Do some timed pattern matching using `montre`!
  ```sh
  montre '(p;q)%(3,4)' montre/examples/my_sym_beh.txt
  ```
  
# Tutorial

We have used Montre to find all sprints performed by a player in a real soccer match. See the tutorial [Finding Sprints](https://github.com/doganulus/montre/blob/master/demo/sprints.ipynb).

# Documentation

  * Timed Pattern Matching. Dogan Ulus, Thomas Ferrere, Eugene Asarin and Oded Maler. FORMATS'14.
  * Online Timed Pattern Matching using Derivatives. Dogan Ulus, Thomas Ferrere, Eugene Asarin and Oded Maler. TACAS'16.
  * Montre: A Tool for Monitoring Timed Regular Expressions. Dogan Ulus. CAV'17.
  * Pattern Matching with Time: Theory and Applications. Dogan Ulus. PhD Thesis. 2018.

