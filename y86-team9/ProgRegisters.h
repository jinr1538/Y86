/*
    File:       ProgRegisters.h
    Desc:       Used to model program registers. 
    
    Author:     efb
*/

#ifndef PROGREGISTERS_H
#define PROGREGISTERS_H

#include <cstdint>
#include "Sim.h"
#include "Register.h"   // Template for registers

class ProgRegisters
{
		Register<uint64_t>    	reg[NUM_REGISTERS];
		Register <uint64_t>   	CC;        

    public:
	            ProgRegisters();
        void        clock		();
        void        setReg      (unsigned regNum, uint64_t rval); 
        uint64_t    getReg      (unsigned regNum);
        void        setCC       (unsigned bitNum, unsigned val);
        unsigned    getCC       (unsigned bitNum);
        void        reset	    (void); 
};


#endif
