all: master slave bystander

master.o : master.cpp ../MPMD.hpp

slave : master
	ln -s $< $@

bystander : master
	ln -s $< $@
	

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
