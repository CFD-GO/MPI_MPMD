all: master slave with_workers

test : all
	./master
	./slave
	./with_workers
	mpirun -np 2 ./master
	mpirun -np 2 ./slave
	mpirun -np 2 ./with_workers
	./master 1 ./slave
	./master 2 ./slave
	./master 1 ./with_workers
	./master 2 ./with_workers
	mpirun -np 2 ./master 1 ./slave
	mpirun -np 2 ./master 2 ./slave
	mpirun -np 2 ./master 1 ./with_workers
	mpirun -np 2 ./master 2 ./with_workers
	mpirun -np 1 ./master : -np 1 ./slave
	mpirun -np 1 ./master : -np 2 ./slave
	mpirun -np 2 ./master : -np 1 ./slave
	mpirun -np 2 ./master : -np 2 ./slave
	mpirun -np 1 ./master : -np 1 ./with_workers
	mpirun -np 1 ./master : -np 2 ./with_workers
	mpirun -np 2 ./master : -np 1 ./with_workers
	mpirun -np 2 ./master : -np 2 ./with_workers
	mpirun -np 2 ./master : -np 2 ./slave : -np 2 ./with_workers
	mpirun -np 2 ./with_workers : -np 1 ./master 1 ./slave	
	mpirun -np 2 ./with_workers : -np 1 ./master 2 ./slave	
	mpirun -np 2 ./with_workers : -np 2 ./master 1 ./slave	
	mpirun -np 2 ./with_workers : -np 2 ./master 2 ./slave	


master.o : master.cpp ../MPMD.hpp
slave.o : slave.cpp ../MPMD.hpp
with_workers.o : with_workers.cpp ../MPMD.hpp
worker_spawn.o : worker_spawn.cpp ../MPMD.hpp

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
