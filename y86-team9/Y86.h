/*
    File:   Y86.h
    Desc:   Y86 constants

    Author: efb
*/

#ifndef Y86_H
#define Y86_H

#include <iostream>
#include <cstdint>
#include <string>
#include "Memory.h"
#include "ProgRegisters.h"
#include "PipeStage.h"
#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "WritebackStage.h"



//class PipeStage;  // Forward declare because of PipeStage reference array

class Y86 {

    Memory              memory;
    ProgRegisters       regs;
    Forward             forward;

    FetchStage fetchStage;
    DecodeStage decodeStage;
    ExecuteStage executeStage;
    MemoryStage memoryStage;
    WritebackStage writebackStage;

    PipeStage *stage[5];  // an array of 5 PipeStage pointers

    int cycles;
	int cycle = 0;	// cycle count

    bool F_stall;
    bool D_stall;
    bool W_stall;
    bool D_bubble;
    bool E_bubble;
    bool M_bubble;



    /* Private member functions */
	bool 		readFile		(std::ifstream& infile);
	int 		writeMemory		(std::string inst, uint64_t address);
  void 		getLine			(uint64_t *, uint64_t);

	std::string getFlagsString	(void); // in Y86trace.cpp

    /* Public member functions */
    public:
        Y86();
    void reset();
		void clock();
		bool load(char *fname);  // takes name of a file as a C string
		void dumpMemory();
		void dumpProgramRegisters();
		void dumpProcessorRegisters();
    void storeByte(std::string inputByte, int a);
    void storeData(std::string inputLine, int dataBytes);
    bool readFile(std::string fileName);
    void pipeControl();

    Memory& getMemory(){return memory;}
    ProgRegisters& getProgRegisters(){return regs;}

    void setTrace(bool f, bool d, bool e, bool m, bool w);
    void trace(void);
    uint64_t getStat();
    int getCycles(){return cycles;};
};

#endif
