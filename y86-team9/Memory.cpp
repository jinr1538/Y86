/*
    Name:   Memory.cpp
    Desc:   Implementation of the Memory class
    Auth:
    Date:
*/
#include <iostream>
#include <iomanip>
#include <cstdint>
#include "Memory.h"
#include "Tools.h"



/*-------------------------------------------------------------------------
  Constructor:  Memory
  Description:  Store 64-bit value to a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
Memory::Memory()
{
    reset();
}
/*-------------------------------------------------------------------------
  Function:     store
  Description:  Store 64-bit value to a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
void   Memory::store	(uint64_t waddr, uint64_t val) // takes word address
{
  if(waddr >= MEMORY_SIZE || waddr < 0)
  {
    memError = true;
    return;
  }
  else
  {
    memError = false;
    mem[waddr] = val;
  }
}
/*-------------------------------------------------------------------------
  Function:     fetch
  Description:  Fetch a 64-bit value from a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
uint64_t  Memory::fetch	(uint64_t waddr) // takes word address
{
  if(waddr >= MEMORY_SIZE || waddr < 0)
  {
    memError = true;
    return 0;
  }
  else
  {
    memError = false;
    uint64_t word = mem[waddr];
    return word;
  }
}
/*--------------------------------------------------------------------
   Function:   getByte

   Description:  Returns a byte from the specified byte address
                 of Y86 memory array.
                 If address is out of range the error status is set.
--------------------------------------------------------------------*/
unsigned char   Memory::getByte	(uint64_t byteAddress) // takes byte address
{
  if(byteAddress < 0 || byteAddress > MEMORY_SIZE*8)
  {
    memError = true;
    return 0;
  }
  else
  {
     memError = false;
	   unsigned char result = 0;
     uint64_t wordv = fetch(byteAddress/8);
     unsigned byte = byteAddress % 8;
     result = Tools::getByteNumber(byte, wordv);
     return result;
  }
}
/*--------------------------------------------------------------------
   Function:     putByte

   Description:  Write a single byte into the Y86 simulated memory
                 at the byte address specified by 'address'.
                 If address is out of range the error status is set.
--------------------------------------------------------------------*/
void  Memory::putByte(uint64_t byteAddress, uint8_t value) // takes byte address
{
  if(byteAddress < 0 || byteAddress > (MEMORY_SIZE*8))
  {
    memError = true;
  }
  else
  {
    memError = false;

    uint64_t addrval = fetch(byteAddress/8);
    addrval = Tools::putByteNumber((byteAddress % 8), value, addrval);
    store((byteAddress/8), addrval);
  }
}
/*--------------------------------------------------------------------
   Function:   getWord

   Description:  Returns word starting from the specified byte address.
                 No part of the word must lie outside memory range.
--------------------------------------------------------------------*/
uint64_t   Memory::getWord	(uint64_t byteAddress)
{
 //use getByte to get all 8 bytes of word and then buildword to assemble and return
 if(byteAddress < 0 || byteAddress > (MEMORY_SIZE*8))
 {
   memError = true;
   return 0;
 }
 else
 {
  memError = false;
  unsigned char byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8;
  byte1 = getByte(byteAddress);
  byteAddress += 1;
  byte2 = getByte(byteAddress);
  byteAddress += 1;
  byte3 = getByte(byteAddress);
  byteAddress += 1;
  byte4 = getByte(byteAddress);
  byteAddress += 1;
  byte5 = getByte(byteAddress);
  byteAddress += 1;
  byte6 = getByte(byteAddress);
  byteAddress += 1;
  byte7 = getByte(byteAddress);
  byteAddress += 1;
  byte8 = getByte(byteAddress);
  uint64_t word = Tools::buildWord(byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8);
  return word;
  }
}
/*----------------------------------------------------------------------------------------------
   Function:   putWord

   Description:  Writes a word to memory starting at the specified byte address.
                 No part of the word must lie outside memory range. If there is
				 a memory error (out of range) the memory should not be changed.
------------------------------------------------------------------------------------------------*/
void Memory::putWord	(uint64_t byteAddress, uint64_t wordValue)
{
  //use putBytes to assemble it back into memory
  if(byteAddress < 0 || byteAddress > (MEMORY_SIZE*8))
  {
    memError = true;
  }
  else
  {
    memError = false;
    uint8_t wordByte;
    for(int i = 0; i < 8; i++, byteAddress++)
    {
      wordByte = Tools::getByteNumber(i, wordValue);
      putByte(byteAddress, wordByte);
    }
  }
}
/*--------------------------------------------------------------------
   Function:   reset

   Description:  Clears memory to all zero. Clears error status.

--------------------------------------------------------------------*/
void  Memory::reset	(void) // clears memory to all zero
{
	memError = false;
  for(int i = 0; i < MEMORY_SIZE; i++)
  {
    store(i, 0);
  }
}
