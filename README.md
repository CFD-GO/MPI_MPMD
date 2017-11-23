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
Then, you can communicate within a single program through `MPMD.local` communicator, and with other programs with `MPMD["other program"].local` inter-communicator.

Additionaly, one can exclude some of the processes from the communication, which would be common in MPI programs with master-workers architecture.

You can use the library to combine programs both by running them simultaniously, and by dynamic spawning of processes:
![Scheme](https://raw.githubusercontent.com/CFD-GO/MPI_MPMD/images/scheme.svg?sanitize=true)

## Reference

### Initialization
You initialize the helper object with name of the program:
```c++
   MPMD.Init(MPI_COMM_WORLD, "my program");
```
The name is used to split the common world communicator into smaller communicators. You can use the name of the executable as the name:
```c++
   MPMD.Init(MPI_COMM_WORLD, argv[0]);
```
If your program has a master-workers architecture, you can create a subgrup, by excluding the master process (for instance 0):
```c++
   std::vector<int> excl;
   excl.push_back(0);
   MPMD.Init(MPI_COMM_WORLD, "my program with workers", excl);
```

### Communicators
After initialization, you can use:

- `MPMD.world` - the original `MPI_COMM_WORLD`
- `MPMD.local` - the communicator for the single program (the processes which declared the same `name`)
- `MPMD.work` - the communicator for the workers  (all processes excluding `excl`)

### Inter-communicators
You can communicate with other program (let's say `B`) with:

- `MPMD["B"].local` - inter-communicator with the whole `B` program
- `MPMD["B"].work` - inter-communicator with the worker processes of `B`

### Spawn
You can also dynamically spawn processes:
```c++
MPMD.Spawn("./slave", MPI_ARGV_NULL, N, MPI_INFO_NULL);
```
After you do, you can access inter-communicators the same way as above

### Running MPMD programs
The simplest way to run MPMD program with OpenMPI is:
```bash
mpirun -np 2 ./program_A : -np 3 ./program_B
```
