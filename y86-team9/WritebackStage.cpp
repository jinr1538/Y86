#include "Y86.h"
#include "WritebackStage.h"
#include "Forward.h"

void WritebackStage::reset(ProgRegisters *pregs, Forward *frwrd)
{
  regs = pregs;
  forward = frwrd;
  stat.setInput(SBUB);
  icode.setInput(INOP);
  ifun.setInput(FNONE);
  valE.setInput(0);
  valM.setInput(0);
  dstE.setInput(RNONE);
  dstM.setInput(RNONE);
}

void WritebackStage::updateWRegister(uint64_t m_stat, uint64_t m_icode , uint64_t m_ifun, uint64_t m_valE, uint64_t m_valM, uint64_t m_dstE, uint64_t m_dstM)
{
  stat.setInput(m_stat);
  icode.setInput(m_icode);
  ifun.setInput(m_ifun);
  valM.setInput(m_valM);
  valE.setInput(m_valE);
  dstE.setInput(m_dstE);
  dstM.setInput(m_dstM);
}

void WritebackStage::clockP0()
{

  if(stageStall)
  {
    setStall(false);
    return;
  }

  stat.clock();
  icode.clock();
  ifun.clock();
  valE.clock();
  valM.clock();
  dstE.clock();
  dstM.clock();

  w_dstE = dstE.getState();
  w_dstM = dstM.getState();
  w_valE = valE.getState();
  w_valM = valM.getState();

  //if(icode.getState() == IRRMOVQ || icode.getState() == IIRMOVQ || icode.getState() == IOPX)
  //{
    forward->setW_dstE(w_dstE);
    forward->setW_dstM(w_dstM);
    forward->setW_valE(w_valE);
    forward->setW_valM(w_valM);
    forward->setW_stat(stat.getState());
    forward->setW_icode(icode.getState());
  //}

  //if(icode.getState() == IMRMOVQ)
//  {


//  }

}

void WritebackStage::clockP1()
{
  //if(icode.getState() == IRRMOVQ || icode.getState() == IIRMOVQ || icode.getState() == IOPX)
  //{
    regs->setReg(w_dstE, w_valE);
  //}
//  if(icode.getState() == IMRMOVQ)
  //{
    regs->setReg(w_dstM, w_valM);
//  }

}
