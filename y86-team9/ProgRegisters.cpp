/*
    Name:       ProgRegisters.cpp

    Desc:       Implements functions for the program register file.

    Author:

    Date:
*/
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include "ProgRegisters.h"
#include "Tools.h"


/*-------------------------------------------------------------------------------------------------
    ProgRegisters constructor
--------------------------------------------------------------------------------------------------*/
ProgRegisters::ProgRegisters()
{
    reset();
}

/*-------------------------------------------------------------------------------------------------
    clock  - calls clock method of each register and also the CC register
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: clock()
{
  reg[0].clock();
  reg[1].clock();
  reg[2].clock();
  reg[3].clock();
  reg[4].clock();
  reg[5].clock();
  reg[6].clock();
  reg[7].clock();
  reg[8].clock();
  reg[9].clock();
  reg[10].clock();
  reg[11].clock();
  reg[12].clock();
  reg[13].clock();
  reg[14].clock();
  CC.clock();
}

/*-------------------------------------------------------------------------------------------------
    setReg - sets the INPUT of the specified register
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: setReg(unsigned regNum, uint64_t rval)
{
  assert((regNum <= 14 || regNum >= 0));
if(regNum != 15)
{
  reg[regNum].setInput(rval);
}
}
/*-------------------------------------------------------------------------------------------------
    getReg - returns STATE of specified register
--------------------------------------------------------------------------------------------------*/
uint64_t ProgRegisters:: getReg(unsigned regNum)
{
  assert((regNum <= 15 || regNum >= 0));
	if(regNum==15)
  {
    return 0;
  }

  return reg[regNum].getState();
}
/*-------------------------------------------------------------------------------------------------
    setCC - Sets the INPUT of the specified CC bit to the specified value (0 or 1)
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: setCC(unsigned bitNumber, unsigned val)
{
  assert((bitNumber <= 2 || bitNumber >= 0));

  uint64_t source = CC.getInput();
  uint64_t result = Tools::assignOneBit(bitNumber, val, source);
  CC.setInput(result);

}

/*-------------------------------------------------------------------------------------------------
    getCC - returns the STATE of the specified CC bit
--------------------------------------------------------------------------------------------------*/
unsigned ProgRegisters:: getCC(unsigned bitNumber)
{
  assert((bitNumber <= 2 || bitNumber >= 0));

  unsigned result = 0;
  result = Tools::getBits(bitNumber, bitNumber, CC.getState());
  return result;
}

/*-------------------------------------------------------------------------------------------------
    reset - resets registers and CC flags
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: reset(void)
{
	reg[0].reset();
  reg[1].reset();
  reg[2].reset();
  reg[3].reset();
  reg[4].reset();
  reg[5].reset();
  reg[6].reset();
  reg[7].reset();
  reg[8].reset();
  reg[9].reset();
  reg[10].reset();
  reg[11].reset();
  reg[12].reset();
  reg[13].reset();
  reg[14].reset();
  CC.reset(4);
}
