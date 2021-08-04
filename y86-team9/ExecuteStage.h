/*
    File:   ExecuteStage.h
    Desc:   Declares ExecuteStage class and associated constants

*/
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H

#include "Y86.h"
#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "MemoryStage.h"
#include "Forward.h"
#include "ProgRegisters.h"

class ExecuteStage : public PipeStage
{

    /* Register state */

    Register <uint64_t> valC;
    Register <uint64_t> valA;
    Register <uint64_t> valB;
    Register <uint64_t> dstE;
    Register <uint64_t> dstM;
    Register <uint64_t> srcA;
    Register <uint64_t> srcB;

    /* Pointers to Execute Stage and Memory object */
    MemoryStage *memoryStage;
    ProgRegisters *regs;
    Forward *forward;


    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */

    uint64_t e_stat;
    uint64_t e_icode;
    uint64_t e_ifun;
    bool e_cnd;
    bool updateSF;
    bool updateOF;
    bool updateZF;
    uint64_t e_valE;
    uint64_t e_valA;
    uint64_t e_valC;
    uint64_t e_valB;
    uint64_t e_dstE;
    uint64_t e_dstM;
	int cycle;

	public:
		void reset(MemoryStage *, ProgRegisters *, Forward *);
    void updateERegister(uint64_t d_stat, uint64_t d_icode, uint64_t d_ifun, uint64_t d_valC, uint64_t d_valA, uint64_t d_valB, uint64_t d_dstE, uint64_t d_dstM, uint64_t d_srcA, uint64_t d_srcB);
    uint64_t getDstM(){return dstM.getState();}
		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
    uint64_t getALUFunction();
    uint64_t getALUA();
    uint64_t getALUB();
    uint64_t selectDstE();
    bool isSetCC();
    bool setCC();
    bool getCnd();
};

#endif
