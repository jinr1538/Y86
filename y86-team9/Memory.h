/*
    File:       Memory.h
    Desc:       Declares memory class and defines interface to access.

    Author:
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>  // Let's us use fixed-width types uintXX_t

#include "Sim.h"

class Memory
{
	// Private member data
		uint64_t    	mem[MEMORY_SIZE];
		bool        	memError;

	// Private member functions
		void        	store	(uint64_t waddr, uint64_t val);
		uint64_t    	fetch	(uint64_t waddr);

	// Public interface
    public:
        Memory();															// Constructor
        unsigned char   getByte	(uint64_t byteAddress); 					// gets byte from spec'd byte address
        void     putByte	(uint64_t byteAddress, uint8_t value);  	// writes byte to spec'd byte address
        uint64_t        getWord	(uint64_t byteAddress); 					// gets word from spec'd byte address
        void            putWord	(uint64_t byteAddress, uint64_t wordValue); // writes word to spec'd byte address
        void            reset	(void); 									// clears memory to all zero, memError=false
		    bool			isError(void){return memError;}						// returns true if memory error
        //friend void Y86Loader::storeByte(string inputByte, int byteAddress);

};


#endif
