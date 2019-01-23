all: master slave with_workers worker_spawn

test_check : TEST=@./.run.test
test_check : test
test_save : TEST=@./.run.test save
test_save : test

MPIRUN=mpirun --mca mpi_yield_when_idle 1

test : all
	$(TEST) ./master
	$(TEST) ./slave
	$(TEST) ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./master
	$(TEST) $(MPIRUN) -np 2 ./slave
	$(TEST) $(MPIRUN) -np 2 ./with_workers
	$(TEST) ./master 1 ./slave
	$(TEST) ./master 2 ./slave
	$(TEST) ./master 1 ./with_workers
	$(TEST) ./master 2 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./master 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./master 2 ./slave
	$(TEST) $(MPIRUN) -np 2 ./master 1 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./master 2 ./with_workers
	$(TEST) $(MPIRUN) -np 1 ./master : -np 1 ./slave
	$(TEST) $(MPIRUN) -np 1 ./master : -np 2 ./slave
	$(TEST) $(MPIRUN) -np 2 ./master : -np 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./master : -np 2 ./slave
	$(TEST) $(MPIRUN) -np 1 ./master : -np 1 ./with_workers
	$(TEST) $(MPIRUN) -np 1 ./master : -np 2 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./master : -np 1 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./master : -np 2 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./master : -np 2 ./slave : -np 2 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./with_workers : -np 1 ./master 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./with_workers : -np 1 ./master 2 ./slave
	$(TEST) $(MPIRUN) -np 2 ./with_workers : -np 2 ./master 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./with_workers : -np 2 ./master 2 ./slave
	$(TEST) ./with_workers
	$(TEST) ./with_workers 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./with_workers 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./with_workers 1 ./worker_spawn
	$(TEST) $(MPIRUN) -np 2 ./with_workers 2 ./slave
	$(TEST) $(MPIRUN) -np 2 ./with_workers 2 ./worker_spawn
	$(TEST) $(MPIRUN) -np 2 ./with_workers 3 ./worker_spawn
	$(TEST) $(MPIRUN) -np 3 ./with_workers 1 ./slave
	$(TEST) $(MPIRUN) -np 3 ./with_workers 1 ./worker_spawn
	$(TEST) $(MPIRUN) -np 3 ./with_workers 2 ./slave
	$(TEST) $(MPIRUN) -np 3 ./with_workers 2 ./worker_spawn
	$(TEST) $(MPIRUN) -np 3 ./with_workers 3 ./worker_spawn
	$(TEST) ./worker_spawn
	$(TEST) ./worker_spawn 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./worker_spawn 1 ./slave
	$(TEST) $(MPIRUN) -np 2 ./worker_spawn 1 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./worker_spawn 2 ./slave
	$(TEST) $(MPIRUN) -np 2 ./worker_spawn 2 ./with_workers
	$(TEST) $(MPIRUN) -np 2 ./worker_spawn 3 ./with_workers
	$(TEST) $(MPIRUN) -np 3 ./worker_spawn 1 ./slave
	$(TEST) $(MPIRUN) -np 3 ./worker_spawn 1 ./with_workers
	$(TEST) $(MPIRUN) -np 3 ./worker_spawn 2 ./slave
	$(TEST) $(MPIRUN) -np 3 ./worker_spawn 2 ./with_workers
	$(TEST) $(MPIRUN) -np 3 ./worker_spawn 3 ./with_workers

master.o : master.cpp ../MPMD.hpp
slave.o : slave.cpp ../MPMD.hpp
with_workers.o : with_workers.cpp ../MPMD.hpp
worker_spawn.o : worker_spawn.cpp ../MPMD.hpp

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
