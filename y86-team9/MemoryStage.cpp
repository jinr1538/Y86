#include "Y86.h"
#include "MemoryStage.h"
#include "Forward.h"

void MemoryStage::reset(WritebackStage *pwriteback, Memory *pmem, Forward *frwrd)
{
  writebackStage = pwriteback;
  memory = pmem;
  forward = frwrd;
  stat.setInput(SBUB);
  icode.setInput(INOP);
  ifun.setInput(FNONE);
  cnd.setInput(false);
  valE.setInput(0);
  valA.setInput(0);
  dstE.setInput(RNONE);
  dstM.setInput(RNONE);
}

void MemoryStage::updateMRegister(uint64_t e_stat, uint64_t e_icode, uint64_t e_ifun, bool e_cnd, uint64_t e_valE, uint64_t e_valA, uint64_t e_dstE, uint64_t e_dstM)
{
  stat.setInput(e_stat);
  icode.setInput(e_icode);
  ifun.setInput(e_ifun);
  cnd.setInput(e_cnd);
  valA.setInput(e_valA);
  valE.setInput(e_valE);
  dstE.setInput(e_dstE);
  dstM.setInput(e_dstM);
}

void MemoryStage::clockP0()
{
  if(stageBubble)
  {
    setBubble(false);
    updateMRegister(SBUB, INOP, 0, false, 0, 0, RNONE, RNONE);

  }


  stat.clock();
  icode.clock();
  ifun.clock();
  cnd.clock();
  valE.clock();
  valA.clock();
  dstE.clock();
  dstM.clock();

  m_stat = stat.getState();
  m_icode = icode.getState();
  m_ifun = ifun.getState();
  m_cnd = cnd.getState();
  m_valE = valE.getState();
  m_valM = 0;
  m_dstE = dstE.getState();
  m_dstM = dstM.getState();
  mem_read = false;
  mem_write = false;

  //forward->setM_valE(valE.getState());

///////////////////////////////////////////////////////////////PROBLEMS////////////////////////////
  memControl();
  memaddy = getAddress();

  if(Tools::isNegative(memaddy) || memaddy > 1024*8)
  {
    m_stat = SADR;
  }
  else
  {
    if(mem_read)
    {
      m_valM = memory->getWord(memaddy);

    }

    if(mem_write)
    {
      memory->putWord(valE.getState(), valA.getState());
    }
  }

  forward->setM_dstE(m_dstE);
  forward->setM_dstM(m_dstM);
  forward->setM_valE(m_valE);

  if(m_stat == SADR)
  {
    m_valM = 0;
  }
if(forward->getW_stat() == SBUB)
{
  forward->setW_stat(SAOK);
}


  forward->setM_Cnd(m_cnd);
  forward->setM_icode(m_icode);
  forward->setM_valA(valA.getState());
  forward->setM_stat(m_stat);
  forward->setM_valM(m_valM);
}

void MemoryStage::clockP1()
{
  writebackStage->updateWRegister(m_stat, m_icode , m_ifun, m_valE, m_valM, m_dstE, m_dstM);
}

//if its mem read then it sets valM to memory @ mem addr
//if its mem write then it doesnt return anything it writes to memory at valE (with valA)

void MemoryStage::memControl()
{
  if(m_icode == IRMMOVQ || m_icode == IPUSHQ || m_icode == ICALL)
  {
    mem_write = true;
    mem_read = false;
  }
  else if(m_icode == IMRMOVQ || m_icode == IPOPQ || m_icode == IRET)
  {
    mem_read = true;
    mem_write = false;
  }
  else
  {
    mem_read = false;
    mem_write = false;
  }

  setStatus();
}

uint64_t MemoryStage::getAddress()
{
  uint64_t result = 0;
  if(m_icode == IRMMOVQ || m_icode == IPUSHQ || m_icode == ICALL || m_icode == IMRMOVQ)
  {
    result = m_valE;
  }
  else if(m_icode == IPOPQ || m_icode == IRET)
  {
    result = valA.getState();
  }
  else
  {
    result = 0;
  }
  return result;
}

void MemoryStage::setStatus() {
    if (memory->isError()) m_stat = SADR;
}
