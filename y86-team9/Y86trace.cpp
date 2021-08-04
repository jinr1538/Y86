#include <iostream>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <map>
#include <stdio.h>  // for printf
#include "Sim.h"
#include "Y86.h"
#include "PipeStage.h"
#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "WritebackStage.h"

#define	WORDSPERLINE	4
#define	WORDSIZE		8

#define MAXCYCLES		5000

using namespace std;


//-----------------------------------------------------------------
// regNames array -- to translate reg id to mnemonic
//-----------------------------------------------------------------
static const char *regNames[]{
	"%rax",
	"%rcx",
	"%rdx",
	"%rbx",
	"%rsp",
	"%rbp",
	"%rsi",
	"%rdi",
	"%r8",
	"%r9",
	"%r10",
	"%r11",
	"%r12",
	"%r13",
	"%r14",
	"----"
};
#define MAX_ICODE 11
#define MAX_ALU_OP 3
#define MAX_COND_OP 6
static const char *instName[] = {
	"halt",
	"nop",
	"cmovxx",
	"irmovq",
	"rmmovq",
	"mrmovq",
	"addq",
	"jxx",
	"call",
	"ret",
	"pushq",
	"popq",
	"<bad>"
};
static const char *jxx[] = {
	"jmp",
	"jle",
	"jl",
	"je",
	"jne",
	"jge",
	"jg",
	"<bad>"
};
static const char *aluOp[] = {
	"addq",
	"subq",
	"andq",
	"xorq",
	"<bad>"
};
static const char *cmovxx[] = {
	"rrmovq",
	"cmovle",
	"cmovl",
	"cmove",
	"cmovne",
	"cmovge",
	"cmovg",
	"<bad>"
};
static const char *status[] {
	"BUB",
	"AOK",
	"HLT",
	"ADR",
	"INS"
};
static  uint64_t initregs[NUM_REGISTERS]; // initial reg values
static  uint64_t initmem[MEMORY_SIZE];   // initial mem state (after load)

/*
static	uint64_t prevLine[WORDSPERLINE] = {0};
static	uint64_t currLine[WORDSPERLINE] = {0};
*/

static	bool	doTrace = false;

//---------------------------------------------------------------
// Local (static) functions
//---------------------------------------------------------------
/*
static void dumpLine(uint64_t *line, uint64_t address);
static bool isEqual(uint64_t *line1, uint64_t *line2);
static void copyLine(uint64_t *dst, uint64_t *src);
*/
static const char *getInst(uint64_t code, uint64_t fun);
static const char *getInst(uint64_t);
/*--------------------------------------------------------------------------------------------------
    WritebackStage trace function - calls stage trace functions
----------------------------------------------------------------------------------------------------
*/
void WritebackStage::trace()
{
	if(!traceEnabled)
		return;
	const char *wstat = status[stat.getState()];
	const char *inst = getInst(icode.getState(),ifun.getState());
	
	// cout << " W: stat: " << wstat << ", inst: " << inst    
						// << ", valE: 0x" << hex << (long)valE.getState()
						// << ", valM: 0x" << hex << (long)valM.getState()
						// << ", dstE: " << regNames[dstE.getState()] << ", dstM: " << regNames[dstM.getState()]	
						// << endl << endl;	
	printf("W: instr = %s, valE = 0x%lx, valM = 0x%lx, dstE = %s, dstM = %s, Stat = %s\n",
		inst,valE.getState(),valM.getState(),
		regNames[dstE.getState()],regNames[dstM.getState()],wstat);

	  
						
}
/*--------------------------------------------------------------------------------------------------
    MemoryStage trace function - calls stage trace functions
----------------------------------------------------------------------------------------------------
*/
void MemoryStage::trace()
{
	if(!traceEnabled)
		return;
	const char *mstat = status[stat.getState()];
	const char *inst = getInst(icode.getState(), ifun.getState());
	
	// cout << " M: stat: " << mstat << ", inst: " << inst << ", Cnd: " << cnd.getState() << endl
						// << " M: valE: 0x" << hex << (long)valE.getState()
						// << ", valA: 0x" << hex << (long)valA.getState()
						// << ", dstE: " << regNames[dstE.getState()] << ", dstM: " << regNames[dstM.getState()]
						// << endl;
	printf("M: instr = %s, Cnd = %d, valE = 0x%lx, valA = 0x%lx\n   dstE = %s, dstM = %s, Stat = %s\n",
		inst, cnd.getState(),valE.getState(),valA.getState(),
		regNames[dstE.getState()],regNames[dstM.getState()], mstat);
						
}
/*--------------------------------------------------------------------------------------------------
    ExecuteStage trace function - calls stage trace functions
----------------------------------------------------------------------------------------------------
*/
void ExecuteStage::trace()
{
	if(!traceEnabled)
		return;
	const char *estat = status[stat.getState()];
	uint64_t code = icode.getState();
	uint64_t fun = ifun.getState();
	const char *inst = getInst(code,fun);
	
	// cout << " E: stat: " << estat << ", inst: " << inst   
						// << ", valC: 0x" << hex << (long)valC.getState()
						// << ", valA: 0x" << hex << (long)valA.getState()
						// << ", valB: 0x" << hex << (long)valB.getState() << endl
						// << " E: srcA: " << regNames[srcA.getState()] << ", srcB: " << regNames[srcB.getState()]
						// << ", dstE: " << regNames[dstE.getState()] << ", dstM: " << regNames[dstM.getState()]	
						// << endl;

	printf("E: instr = %s, valC = 0x%lx, valA = 0x%lx, valB = 0x%lx\n   srcA = %s, srcB = %s, dstE = %s, dstM = %s, Stat = %s\n",
		inst, valC.getState(), valA.getState(), valB.getState(),
		regNames[srcA.getState()], regNames[srcB.getState()],
		regNames[dstE.getState()], regNames[dstM.getState()], estat);
		
						
					
}
/*--------------------------------------------------------------------------------------------------
    DecodeStage trace function - calls stage trace functions
----------------------------------------------------------------------------------------------------
*/
void DecodeStage::trace()
{
	if(!traceEnabled)
		return;
	// icode:ifun, rA, rB, valC, valP
	const char *dstat = status[stat.getState()];
	uint64_t code = icode.getState();
	uint64_t fun = ifun.getState();
	uint64_t regA = rA.getState();
	uint64_t regB = rB.getState();
	const char *inst = getInst(code,fun);
	
	// cout << " D: stat: " << dstat << ", inst: " << inst << ", rA: " << regNames[regA] << ", rB: " << regNames[regB] 
						// << ", valC: 0x" << hex << (long)valC.getState()
						// << ", valP: 0x" << hex << (long)valP.getState()
						// << endl;
						
	printf("D: instr = %s, rA = %s, rB = %s, valC = 0x%lx, valP = 0x%lx, Stat = %s\n",
		inst, regNames[regA], regNames[regB], valC.getState(), valP.getState(), dstat);
	
						
}
/*--------------------------------------------------------------------------------------------------
    FetchStage trace function - calls stage trace functions
----------------------------------------------------------------------------------------------------
*/
void FetchStage::trace()
{
	if(!traceEnabled)
		return;
//	cout << " F: predPC = 0x" << setw(4) << setfill('0') << hex << (long)predPC.getState() << endl;
	printf("F: predPC = 0x%lx\n", predPC.getState());
	
}
/*--------------------------------------------------------------------------------------------------
    setTrace - enables trace options for each stage.
	           Trace (display) output is enabled for any stage
			   whose corresponding trace parameter is set to 'true'
----------------------------------------------------------------------------------------------------
*/
void Y86::setTrace(bool f, bool d, bool e, bool m, bool w)
{
	
    // Save initial states of registers and memory
	for(int i = 0; i < NUM_REGISTERS; i++)
		initregs[i] = regs.getReg(i);
	for(int i = 0; i < MEMORY_SIZE; i++)
		initmem[i] = memory.getWord((uint64_t)(i*8));
	
	// Determine if any trace flags set
	doTrace = f || d || e || m || w;
	
	fetchStage.setTrace(f);
	decodeStage.setTrace(d);	
	executeStage.setTrace(e);	
	memoryStage.setTrace(m);	
	writebackStage.setTrace(w);
}
/*--------------------------------------------------------------------------------------------------
    Main trace function - calls stage trace functions
----------------------------------------------------------------------------------------------------
*/
void Y86::trace()
{
		
	if(doTrace) {
		printf("\nCycle %ld. CC=%s, Stat=%s\n", (long)cycles, (getFlagsString()).c_str(), status[SAOK]);
		fetchStage.trace();
		decodeStage.trace();
		executeStage.trace();
		memoryStage.trace();
		writebackStage.trace();
	}
	cycles++;
	if(cycles > MAXCYCLES){
		cout << "Maximum cycles exceeded--possible infinite loop. Cycles = " << MAXCYCLES << endl;
		exit(1);
	}
		
}

void Y86::dumpProcessorRegisters()
{
//    printf("Stopped in %d steps at PC = 0x%llx.  Status '%s', CC %s\n",
//	   cycles-4, , stat_name(e), cc_name(s->cc));
	cout << "Processor State:" << endl;
	cout << "Status '" << status[writebackStage.getStat()] << "', CC " << getFlagsString() << endl << endl;;
	
}
void Y86::dumpProgramRegisters()
{
/*
	cout << "Registers:" << endl;
	cout << "%rax:" << setw(16) << setfill('0') << hex << regs.getReg(RAX) << setfill(' ') << ' ';
	cout << "%rcx:" << setw(16) << setfill('0') << hex << regs.getReg(RCX) << setfill(' ') << ' ';
	cout << "%rdx:" << setw(16) << setfill('0') << hex << regs.getReg(RDX) << setfill(' ') << ' ';
	cout << "%rbx:" << setw(16) << setfill('0') << hex << regs.getReg(RBX) << setfill(' ');
	cout << endl;

	cout << "%rsp:" << setw(16) << setfill('0') << hex << regs.getReg(RSP) << setfill(' ') << ' ';
	cout << "%rbp:" << setw(16) << setfill('0') << hex << regs.getReg(RBP) << setfill(' ') << ' ';
	cout << "%rsi:" << setw(16) << setfill('0') << hex << regs.getReg(RSI) << setfill(' ') << ' ';
	cout << "%rdi:" << setw(16) << setfill('0') << hex << regs.getReg(RDI) << setfill(' ');
	cout << endl;

	cout << " %r8:" << setw(16) << setfill('0') << hex << regs.getReg(R8) << setfill(' ') << ' ';
	cout << " %r9:" << setw(16) << setfill('0') << hex << regs.getReg(R9) << setfill(' ') << ' ';
	cout << "%r10:" << setw(16) << setfill('0') << hex << regs.getReg(R10) << setfill(' ') << ' ';
	cout << "%r11:" << setw(16) << setfill('0') << hex << regs.getReg(R11) << setfill(' ');
	cout << endl;

	cout << "%r12:" << setw(16) << setfill('0') << hex << regs.getReg(R12) << setfill(' ') << " ";
	cout << "%r13:" << setw(16) << setfill('0') << hex << regs.getReg(R13) << setfill(' ') << " ";
	cout << "%r14:" << setw(16) << setfill('0') << hex << regs.getReg(R14) << setfill(' ');
	cout << endl << endl;
*/
	printf("Changes to registers:\n");
	for(int i = 0; i < NUM_REGISTERS; i++){
		if (initregs[i] != regs.getReg(i)){
			printf("%s:\t0x%.16lx\t0x%.16lx\n",
			regNames[i], (long)initregs[i], (long)regs.getReg(i));
		}
	}
	printf("\n");
	
}
const char *getInst(uint64_t icode)
{
	if(icode > MAX_ICODE)
		icode = MAX_ICODE+1;
	return instName[icode];
}
const char *getInst(uint64_t code, uint64_t fun)
{
	if(code == 2){
		if(fun > MAX_COND_OP)
			fun = MAX_COND_OP+1;
		return cmovxx[fun];
	}
	else if(code == 6){
		if(fun > MAX_ALU_OP)
			fun = MAX_ALU_OP+1;
		return aluOp[fun];
	}
	else if(code == 7){
		if(fun > MAX_COND_OP)
			fun = MAX_COND_OP+1;
		return jxx[fun];
	}
	else {
		
		return getInst(code);
	}
}
string Y86::getFlagsString()
{
	stringstream ss;
//	ss << "CC: ZF=" << regs.getCC(ZF) << " SF=" << regs.getCC(SF) << " OF=" << regs.getCC(OF);
	ss << "Z=" << regs.getCC(ZF) << " S=" << regs.getCC(SF) << " O=" << regs.getCC(OF);
	return ss.str();
}
void Y86::dumpMemory()
{
	printf("Changes to memory:\n");
	for(int i = 0; i < MEMORY_SIZE; i++){
		uint64_t mval = memory.getWord(i*8);
		if(initmem[i] != mval)
			printf("0x%.4lx:\t0x%.16lx\t0x%.16lx\n", (long)(i*8), (long)initmem[i], (long)mval);
	}
}
/*
void Y86::dumpMemory()
{
	int address = 0;
	int memsize = MEMORY_SIZE*WORDSIZE;
	int linesize = WORDSPERLINE*WORDSIZE;
	bool star = false;
	
	cout << "Memory:" << endl;
	getLine(prevLine, address);
	dumpLine(prevLine, address);
	for(address = linesize; address < memsize; address+=linesize){
		getLine(currLine, address);
		if(isEqual(currLine,prevLine)){
			if(!star){
				cout << "*" << endl;  //star on prev line, then newline
				star = true;
			}
		}
		else {
			cout << endl;              // just newline
			dumpLine(currLine,address);// write newline
			star = false;
		}
		copyLine(prevLine,currLine);
	}
	cout << endl << endl;
}
void Y86::getLine(uint64_t *line, uint64_t address)
{
	for(int i = 0; i < WORDSPERLINE; i++){
		line[i] = memory.getWord(address);
		address += WORDSIZE;
	}
}
static void dumpLine(uint64_t *line, uint64_t address)
{
	cout << "0x" << setfill('0') << setw(4) << hex << (int)address << ":";
	for(int i = 0; i < WORDSPERLINE; i++)
		cout << " " << setw(16) << hex << setfill('0') << (unsigned long)line[i];
}
static bool isEqual(uint64_t *line1, uint64_t *line2)
{
	for(int i = 0; i < WORDSPERLINE; i++){
		if(line1[i] != line2[i])
			return false;
	}
	return true;
}

static void copyLine(uint64_t *dst, uint64_t *src)
{
	for(int i = 0; i < WORDSPERLINE; i++) dst[i] = src[i];
}
*/
