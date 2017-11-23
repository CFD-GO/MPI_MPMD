all: master slave bystander

test : master_single_test slave_single_test bystander_single_test master_two_test slave_two_test bystander_two_test master_slave_one_two_test

%_single_test : %
	./$<

%_two_test : %
	mpirun -np 2 ./$<

master_slave_one_two_test : master slave
	mpirun -np 1 ./master : -np 2 ./slave

master.o : master.cpp ../MPMD.hpp

slave : master
	ln -s $< $@

bystander : master
	ln -s $< $@
	

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
