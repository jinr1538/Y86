CXX=g++
CXXFLAGS = -Wall -g -std=c++11

# Put object files in next line
OBJS = Sim.o Tools.o Memory.o ProgRegisters.o Y86.o Y86Loader.o Y86trace.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o

# The OBJTEST macro is same as 'OBJS' above except replace Sim.o with testSim.o
OBJTEST = testSim.o Tools.o Memory.o ProgRegisters.o Y86.o

#complete the following targets with appropriate dependencies and commands
yess:	$(OBJS)
						$(CXX) $(CXXFLAGS) $(OBJS) -o yess

#ytest:  $(OBJTEST)
#					$(CXX) $(CXXFLAGS) $(OBJTEST) -o ytest

Sim.o: Sim.cpp Sim.h

Tools.o: Tools.cpp Tools.h
#your object file targets below here...
Memory.o: Memory.cpp Memory.h

ProgRegisters.o: ProgRegisters.cpp ProgRegisters.h

Y86.o: Y86.cpp Y86.h

Y86Loader.o: Y86Loader.cpp Y86.h

Y86trace.o: Y86trace.cpp Y86.h

FetchStage.o: FetchStage.cpp FetchStage.h

DecodeStage.o: DecodeStage.cpp DecodeStage.h

ExecuteStage.o: ExecuteStage.cpp ExecuteStage.h

MemoryStage.o: MemoryStage.cpp MemoryStage.h

WritebackStage.o: WritebackStage.cpp WritebackStage.h


# clean target
# Remeber, you don't want to delete your testSim.o file
# The following target preserves testSim.o
clean:
#	cp testSim.o testSim-tmp
	rm *.o yess
#	rm *.o ytest
#	cp testSim-tmp testSim.o
