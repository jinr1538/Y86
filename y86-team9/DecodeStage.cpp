

#include "Y86.h"
#include "DecodeStage.h"
#include "Forward.h"


void DecodeStage::reset(ExecuteStage *pexecute, ProgRegisters *pprog, Forward *frwrd)
{

  executeStage = pexecute;
  regs = pprog;
  forward = frwrd;
  stat.setInput(SBUB);
  icode.setInput(INOP);
  ifun.setInput(FNONE);
  rA.setInput(RNONE);
  rB.setInput(RNONE);
  valC.setInput(0);
  valP.setInput(0);

}

void DecodeStage::updateDRegister(uint64_t f_stat, uint64_t f_icode, uint64_t f_ifun, uint64_t f_rA, uint64_t f_rB, uint64_t f_valC, uint64_t f_valP)
{
  stat.setInput(f_stat);
  icode.setInput(f_icode);
  ifun.setInput(f_ifun);
  rA.setInput(f_rA);
  rB.setInput(f_rB);
  valC.setInput(f_valC);
  valP.setInput(f_valP);
}

void DecodeStage::clockP0()
{
  if(stageStall)
  {
    setStall(false);
    return;
  }

  if(stageBubble)
  {
    setBubble(false);
    updateDRegister(SBUB, INOP, 0, RNONE, RNONE, 0, 0);
    
  }
  stat.clock();
  icode.clock();
  ifun.clock();
  rA.clock();
  rB.clock();
  valC.clock();
  valP.clock();

  // Initialize internal signals to default values
  d_stat = stat.getState();
  d_icode = icode.getState();
  d_ifun = ifun.getState();
  d_valC = valC.getState();
  d_srcA = getSrcA();
  d_srcB = getSrcB();
  d_dstE = getDstE();
  d_dstM = getDstM();

}

void DecodeStage::clockP1()
{
  d_valA = selectFwdA();
  d_valB = forwardB();
  executeStage->updateERegister(d_stat, d_icode ,d_ifun, d_valC, d_valA, d_valB, d_dstE, d_dstM, d_srcA, d_srcB);
}

uint64_t DecodeStage::getSrcA()
{
  if(d_icode == IRRMOVQ || d_icode == IRMMOVQ || d_icode == IOPX || d_icode == IPUSHQ)
  {
    d_srcA = rA.getState();
  }
  else if(d_icode == IPOPQ || d_icode == IRET)
  {
    d_srcA = RSP;
  }
  else
  {
    d_srcA = RNONE;
  }
  return d_srcA;
}

uint64_t DecodeStage::getSrcB()
{
  if(d_icode == IOPX || d_icode == IRMMOVQ || d_icode == IMRMOVQ)
  {
    d_srcB = rB.getState();
  }
  else if(d_icode == IPUSHQ|| d_icode == IPOPQ || d_icode == IRET || d_icode == ICALL)
  {
    d_srcB = RSP;
  }
  else
  {
    d_srcB = RNONE;
  }
  return d_srcB;
}

uint64_t DecodeStage::getDstE()
{
  if(d_icode == IRRMOVQ || d_icode == IIRMOVQ || d_icode == IOPX)
  {
    d_dstE = rB.getState();
  }
  else if(d_icode == IPUSHQ|| d_icode == IPOPQ || d_icode == IRET || d_icode == ICALL)
  {
    d_dstE = RSP;
  }
  else
  {
    d_dstE = RNONE;
  }
  return d_dstE;
}

uint64_t DecodeStage::getDstM()
{
  if(d_icode == IMRMOVQ || d_icode == IPOPQ)
  {
    d_dstM = rA.getState();
  }
  else
  {
    d_dstM = RNONE;
  }
  return d_dstM;
}

uint64_t DecodeStage::selectFwdA()
{
  uint64_t W_dstE = forward->getW_dstE();
  uint64_t W_dstM = forward->getW_dstM();
  uint64_t M_dstE = forward->getM_dstE();
  uint64_t M_dstM = forward->getM_dstM();
  uint64_t E_dstE = forward->getE_dstE();

  if(d_icode == ICALL || d_icode == IJXX) //ICALL or IJump
  {
    return valP.getState();
  }
  else if(d_srcA == E_dstE)
  {
    return forward->getE_valE();
  }
  else if(d_srcA == M_dstM)               //SRCA == M_dstM
  {
    return forward->getM_valM();
  }
  else if(d_srcA == M_dstE)               //SRCA == M_dstE
  {
    return forward->getM_valE();
  }
  else if(d_srcA == W_dstM)               //SRCA == W_dstM
  {
    return forward->getW_valM();
  }
  else if(d_srcA == W_dstE)               //SRCA == W_dstE
  {
    return forward->getW_valE();
  }
  else
  {
    return regs->getReg(d_srcA);
  }
}

uint64_t DecodeStage::forwardB()
{
  uint64_t W_dstE = forward->getW_dstE();
  uint64_t W_dstM = forward->getW_dstM();
  uint64_t M_dstE = forward->getM_dstE();
  uint64_t M_dstM = forward->getM_dstM();
  uint64_t E_dstE = forward->getE_dstE();

  if(d_srcB == E_dstE)
  {
    return forward->getE_valE();
  }
  else if(d_srcB == M_dstM)
  {
    return forward->getM_valM();
  }
  else if(d_srcB == M_dstE)
  {
    return forward->getM_valE();
  }
  else if(d_srcB == W_dstM)
  {
    return forward->getW_valM();
  }
  else if(d_srcB == W_dstE)
  {
    return forward->getW_valE();
  }
  else
  {
    return regs->getReg(d_srcB);
  }
}
