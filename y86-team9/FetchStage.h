/*
    File:   FetchStage.h
    Desc:   Declares FetchStage class and associated constants

*/
#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "DecodeStage.h"
#include "Tools.h"
#include "Forward.h"

class FetchStage : public PipeStage
{

    /* Register state */
    Register <uint64_t> predPC;    // Predicted PC value stored in F Register
    Register <uint64_t> rA;
    Register <uint64_t> rB;
    Register <uint64_t> valC;
    Register <uint64_t> valP;


    /* Pointers to Decode Stage and Memory object */
    DecodeStage *decodeStage;
    Memory 		*memory;
    Forward *forward;

    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */
    uint64_t f_PC;
    bool needsValC;
    bool needsRegs;
    bool instrValid;
    uint64_t f_stat;
    uint64_t f_icode;
    uint64_t f_ifun;
    uint64_t f_rA;
    uint64_t f_rB;
    uint64_t f_valC;
    uint64_t f_valP;
	int cycle = 0;

	/* Private methods - These are internal to the Fetch Stage */
    bool getByte0();
    bool checkNeedsRegs();
    bool checkNeedsValC();
    bool align();
    bool checkInstrValid();
    int getPCIncrement();
    void predictPC();
    uint64_t selectPC();

	public:
		void reset(DecodeStage *, Memory *, Forward *);

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();


};

#endif
