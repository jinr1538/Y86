/*
    File:   ExecuteStage.h
    Desc:   Declares ExecuteStage class and associated constants

*/
#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H

#include "Y86.h"
#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "WritebackStage.h"
#include "Forward.h"

class MemoryStage : public PipeStage
{

    /* Register state */

    Register <bool> cnd;
    Register <uint64_t> valE;
    Register <uint64_t> valA;
    Register <uint64_t> dstE;
    Register <uint64_t> dstM;


    /* Pointers to Writeback Stage and Memory object */
    WritebackStage *writebackStage;
    Memory 		*memory;
    Forward *forward;


    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */

    uint64_t m_stat;
    uint64_t m_icode;
    uint64_t m_ifun;
    uint64_t m_valE;
    uint64_t m_valM;
    uint64_t m_dstE;
    uint64_t m_dstM;
    uint64_t memaddy;
    bool m_cnd;
    bool mem_read;
    bool mem_write;

	public:
		void reset(WritebackStage *, Memory *, Forward *);
    void updateMRegister(uint64_t e_stat, uint64_t e_icode , uint64_t e_ifun, bool e_cnd, uint64_t e_valE, uint64_t e_valA, uint64_t e_dstE, uint64_t e_dstM);
    void memControl();
    void setStatus();
    uint64_t getAddress();

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();

};

#endif
