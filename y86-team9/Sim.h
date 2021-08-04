/*
    Sim.h
    This file includes typedefs and constants used by all classes
    
*/
#ifndef SIM_H
#define SIM_H

typedef unsigned char byte;

#define     NUMSTAGES       5
#define     MEMORY_SIZE     1024     // 64-bit words 


#define     NUM_REGISTERS   15       // 64-bit registers
#define     RAX             0
#define     RCX             1
#define     RDX             2
#define     RBX             3
#define     RSP             4
#define     RBP             5
#define     RSI             6
#define     RDI             7
#define     R8              8
#define     R9              9
#define     R10             10
#define     R11             11
#define     R12             12
#define     R13             13
#define     R14             14
#define     RNONE           15

// Flags Register (CC) bit number definitions
#define     OF              0
#define     SF              1
#define     ZF              2

/* Status codes for stat field of pipe registers */
#define		SBUB	0     // bubble in the pipeline (same effect as NOP)
#define 	SAOK    1   
#define 	SHLT    2
#define 	SADR    3
#define 	SINS    4

/* MAJOR OPCODES */
#define		IHALT		0
#define		INOP		1
#define		IRRMOVQ		2
#define		IIRMOVQ		3
#define		IRMMOVQ		4
#define		IMRMOVQ		5
#define		IOPX		6
#define		IJXX		7
#define		ICALL    	8
#define		IRET     	9
#define		IPUSHQ		10
#define		IPOPQ		11

/* Function Codes */
#define		FNONE	0  // used for "no function"--same as FADDQ
#define		FADDQ	0
#define		FSUBQ	1
#define		FANDQ	2
#define		FXORQ	3

/* Conditions used for Jxx and CMOVxx instructions */

#define		ALWAYS	0  // unconditional
#define		LEQ		1  // Less than or equal
#define		LT		2  // Less than
#define		EQ		3  // Equal (or zero)
#define		NEQ		4  // Not Equzl (or not zero)
#define		GTEQ	5  // Greater than or equal
#define		GT		6  // Greater than



#endif
