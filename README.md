# montre

A timed regular expression matcher

# Linux

Ubuntu LTS 14.04 64-bit is the recommended platform.

## Requirements

  * GNU C/C++ and the corresponding libraries.
  * GNU make
  * Pure Programming Language:
  	* The GNU multiprecision library, or some compatible replacement such as MPIR.
  	* The GNU multiprecision floating point library.
  	* LLVM (3.5 or less. It is known that LLVM 3.6+ is incompatible.)
  	* More info: https://bitbucket.org/purelang/pure-lang/wiki/GettingStarted

## Instructions

If you have problems with permissions don't forget to prefix with `sudo`

1. Clone the Montre repository:

  ```sh
  git clone https://github.com/doganulus/montre
  cd atom
  ```

2. Build and install Montre:

  ```sh
  make && make install
  ```

  It will install Montre with default prefix /usr/local. You can modify Makefile if you would like to install a custom location.

## Documentation

  * Timed Pattern Matching.
  * Online Timed Pattern Matching using Derivatives.
  * Montre: A Tool for Monitoring Timed Regular Expressions.