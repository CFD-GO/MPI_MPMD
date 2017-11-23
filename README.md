[![Build Status](https://travis-ci.org/CFD-GO/MPI_MPMD.svg?branch=master)](https://travis-ci.org/CFD-GO/MPI_MPMD)[![codecov](https://codecov.io/gh/CFD-GO/MPI_MPMD/branch/master/graph/badge.svg)](https://codecov.io/gh/CFD-GO/MPI_MPMD)

# Multiple Program Multiple Data programming with MPI

This is a simple C++ class to help to easily make programs for MPMD running.
It is contained in a single `.hpp` file for ease of use.

## Usage

The class can be used as a global static, or just as a local variable in
main. Either way, you need to only initialize it after you initialize MPI:

```c++
   MPMDHelper MPMD;

   MPI_Init(&argc, &argv);

   MPMD.Init(MPI_COMM_WORLD, "my program");
```
