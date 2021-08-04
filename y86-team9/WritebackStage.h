/*
    File:   WritebackStage.h
    Desc:   Declares WritebackStage class and associated constants

*/
#ifndef WritebackSTAGE_H
#define WritebackSTAGE_H

#include "Y86.h"
#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "FetchStage.h"
#include "Forward.h"

class WritebackStage : public PipeStage
{
    /* Register state */

    Register <uint64_t> valE;
    Register <uint64_t> valM;
    Register <uint64_t> dstE;
    Register <uint64_t> dstM;

    uint64_t w_dstE;
    uint64_t w_dstM;
    uint64_t w_valE;
    uint64_t w_valM;
    /* Pointers to ProgRegisters object */
    ProgRegisters *regs;
    Forward *forward;


    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */
    //uint64_t W_valE;
    //uint64_t W_valM;


	public:
		void reset(ProgRegisters *, Forward *);
    void updateWRegister(uint64_t m_stat, uint64_t m_icode , uint64_t m_ifun, uint64_t m_valE, uint64_t m_valM, uint64_t m_dstE, uint64_t m_dstM);

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();

};

#endif
