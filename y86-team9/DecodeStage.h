/*
    File:   DecodeStage.h
    Desc:   Declares DecodeStage class and associated constants

*/
#ifndef DECODESTAGE_H
#define DECODESTAGE_H

#include "Y86.h"
#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "ExecuteStage.h"
#include "Forward.h"



class DecodeStage : public PipeStage
{

    /* Register state */

    Register <uint64_t> rA;
    Register <uint64_t> rB;
    Register <uint64_t> valC;
    Register <uint64_t> valP;

    /* Pointers to Execute Stage and Memory object */
    ExecuteStage *executeStage;
    ProgRegisters *regs;
    Forward *forward;




    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */

    uint64_t d_stat;
    uint64_t d_icode;
    uint64_t d_ifun;
    uint64_t d_valC;
    uint64_t d_valA;
    uint64_t d_valB;
    uint64_t d_dstM;
    uint64_t d_dstE;
    uint64_t d_srcA;
    uint64_t d_srcB;

	/* Private methods - These are internal to the Decode Stage */

	public:
		void reset(ExecuteStage *, ProgRegisters *, Forward *);
    void updateDRegister(uint64_t f_stat, uint64_t f_icode, uint64_t f_ifun,
                         uint64_t f_rA, uint64_t f_rB, uint64_t f_valC, uint64_t f_valP);

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();

    uint64_t getSrcA();
    uint64_t getSrcB();
    uint64_t getDstE();
    uint64_t getDstM();
    uint64_t selectFwdA();
    uint64_t forwardB();


};

#endif
