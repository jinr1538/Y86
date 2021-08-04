/*
    Utility functions (e.g., for bit/byte manipulations) used in Y86 simulator.
    
    Author: Zach Stermon	   
    version:  1.3  
*/
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstdint>
#include "Tools.h"

namespace Tools {
    
/*-----------------------------------------------------------------------------------------------
    getBits
	Extracts bits 'low' to 'high' (inclusive) from 'source' and returns the result as
	an 64-bit integer. 
	Example:
	
	getBits(12,15,0x0123456789abcdef);
	
	would return the value:
	
	0x00000000000000c;
	
	(Bits 12-15 of the 64-bit word, counting from the right correspond to hex digit 'c' in the
	source word. Note that any number of bits from 1 to 64 could be specified depending on the values
	of 'low' and 'high'.
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t getBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));
    
  uint64_t bitshiftLeft;
	   bitshiftLeft = 63-high;
  uint64_t bitshiftRight;
           bitshiftRight = bitshiftLeft + low;

  uint64_t newSource; 
           newSource = source << bitshiftLeft;
           newSource >>= bitshiftRight;        
  return newSource;
}
/*-----------------------------------------------------------------------------------------------
    setBits
	Sets (to 1) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t setBits(unsigned low, unsigned high, uint64_t source)
{
	assert(high < 64 && (low <= high));    
	uint64_t exampleNum = 0xffffffffffffffff >> low;
	exampleNum <<= (low + (63 - high));
	exampleNum >>= 63 - high;
	uint64_t final = source | exampleNum;
	return final;
}
/*-----------------------------------------------------------------------------------------------
    clearBits
	
	Clears (to 0) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t clearBits(unsigned low, unsigned high, uint64_t source)
{
	assert (high < 64 && (low <= high));
	uint64_t exampleNum = 0xffffffffffffffff >> low;
	exampleNum <<= (low + (63-high));
	exampleNum >>= (63 - high);
	exampleNum = ~exampleNum;
	uint64_t final;
	final = source & exampleNum;
	return final;
}

/*-----------------------------------------------------------------------------------------------
    assignOneBit
	Sets the bit number 'bitNum' to the binary value (0 or 1) specified by 'bitVal' in
	the source word and returns the resulting word.
	
	'bitNum' must be in the range 0 to 63 (inclusive) and 'bitVal' must be 0 or 1.
------------------------------------------------------------------------------------------------*/
uint64_t assignOneBit(unsigned bitNum, unsigned bitVal, uint64_t source)
{
	assert(bitNum >= 0 && bitNum <= 63);
	assert(bitVal == 0 || bitVal == 1);
	if (bitVal) 
	{
		return setBits(bitNum, bitNum, source);
	}
	else
	{
		return clearBits(bitNum, bitNum, source);
	}
}

/*-----------------------------------------------------------------------------------------------
    getByteNumber
	Returns the specified byte number from the source word.
	
	Example:
	getByteNumber(3,0x0011223344556677);
	
	will return the single byte value:
	
	0x44
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint8_t getByteNumber(unsigned byteNum, uint64_t source)
{
	assert (byteNum < 8 && byteNum >= 0);
	uint8_t finalRes = 0; 
	finalRes = getBits((byteNum*8),(byteNum*8+7),source);
    return finalRes;
}
/*-----------------------------------------------------------------------------------------------
    putByteNumber
	Returns the source word after replacing the specified byte (byteNum) with a given 
	byte value (byteVal).
	
	Example:
	putByteNumber(3,0x00,0x0011223344556677);
	
	will return the value:
	
	0x0011223300556677
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint64_t putByteNumber(unsigned byteNum, uint8_t byteVal, uint64_t source)
{
    uint64_t exampleNum = 0xff; 
	exampleNum <<= byteNum * 8;
	source &= ~exampleNum;
	exampleNum = (uint64_t)byteVal << byteNum * 8;
	return exampleNum | source;
}
/*-----------------------------------------------------------------------------------------------
    buildWord
	
	Returns a 64 bit word consisting of the 8 parameter bytes, where b0 is the least significant
	and b7 is the most significant byte.
------------------------------------------------------------------------------------------------*/
uint64_t buildWord(unsigned char b0, unsigned char b1,unsigned char b2, unsigned char b3,
                          unsigned char b4, unsigned char b5,unsigned char b6, unsigned char b7)
{
    uint64_t final = 0;
	
	final += (uint64_t)b7;
	final <<= 8;
	final += (uint64_t)b6;
	final <<= 8;
	final += (uint64_t)b5;
	final <<= 8;
	final += (uint64_t)b4;
	final <<= 8;
	final += (uint64_t)b3;
	final <<= 8;
	final += (uint64_t)b2;
	final <<= 8;
	final += (uint64_t)b1;
	final <<= 8;
	final += (uint64_t)b0;
	 
    return final;                     
}
/*-----------------------------------------------------------------------------------------------
    isNegative

	Returns true if the input parameter is negative when interpreted as a signed value.
------------------------------------------------------------------------------------------------*/
bool isNegative(uint64_t source)
{
      uint64_t sourceNew;
               sourceNew = source >> 63;
	if (sourceNew == 1)
	{
		return true;
	}
	else return false;
}
/*-----------------------------------------------------------------------------------------------
    expandBits
	
	Reads the source and converts it to a character string consisting of 72 bytes--71 characters
	followed by a '\0' character (string terminator).
	Example usage: 
		char buf[72];
		expandBits(0xAABBCCDDEE001122UL,buf);

		Will cause the following string to be written to the buf[] array (not including quotes):
	
	"10101010 10111011 11001100 11011101 11101110 00000000 00010001 00100010"
	
	Note the space after every group of 8 digits except the last. The last byte of
	the string is the '\0' byte, that is not printable but used to indicate the end
	of a C string.
------------------------------------------------------------------------------------------------*/
void expandBits(uint64_t source, char *bits)
{
	assert(bits > 0);
	
	uint64_t offset;
	int count = 63;
	uint64_t workNum = (uint64_t)1 << 63;
	
	for (int i = 0; i <= 70; i++)
	{
		if(i==8 || i==17 || i==26 || i==35 || i == 44 || i ==53 || i == 62)
		{
			bits[i] = ' ';
		}
		else
		{
			offset = source & workNum;
			bits[i] = (char)((offset >> count) + 48);
			workNum >>= 1;
			count--;
		}
	}
	bits[71] = '\0';
}
/*-----------------------------------------------------------------------------------------------
    clearBuffer
	Treats pbuf as a pointer to an array of 'size' bytes and clears them all to zero.
------------------------------------------------------------------------------------------------*/
void clearBuffer(char * pbuf, int size)
{
	for(int i =0;i<size;i++)
	{
		pbuf[i]=0;
	}

}

} // end namespace Tools
