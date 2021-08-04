#include "Y86.h"
#include "ExecuteStage.h"
#include "Forward.h"
#include "ProgRegisters.h"


void ExecuteStage::reset(MemoryStage *pmemory, ProgRegisters *preg, Forward *frwrd)
{
  memoryStage = pmemory;
  forward = frwrd;
  regs = preg;
  stat.setInput(SBUB);
  icode.setInput(INOP);
  ifun.setInput(FADDQ);
  valC.setInput(0);
  valA.setInput(0);
  dstE.setInput(RNONE);
  dstM.setInput(RNONE);
  srcA.setInput(RNONE);
  srcB.setInput(RNONE);
}

void ExecuteStage::updateERegister(uint64_t d_stat, uint64_t d_icode, uint64_t d_ifun, uint64_t d_valC, uint64_t d_valA, uint64_t d_valB, uint64_t d_dstE, uint64_t d_dstM, uint64_t d_srcA, uint64_t d_srcB)
{
  stat.setInput(d_stat);
  icode.setInput(d_icode);
  ifun.setInput(d_ifun);
  valC.setInput(d_valC);
  valA.setInput(d_valA);
  valB.setInput(d_valB);
  dstE.setInput(d_dstE);
  dstM.setInput(d_dstM);
  srcA.setInput(d_srcA);
  srcB.setInput(d_srcB);
}

void ExecuteStage::clockP0()
{

  if(stageBubble)
  {
    setBubble(false);
    updateERegister(SBUB, INOP, 0, 0, 0, 0, RNONE, RNONE, RNONE, RNONE);

  }

  stat.clock();
  icode.clock();
  ifun.clock();
  valA.clock();
  valB.clock();
  srcA.clock();
  srcB.clock();
  valC.clock();
  dstE.clock();
  dstM.clock();

  e_stat = stat.getState();
  e_icode = icode.getState();
  e_ifun = ifun.getState();
  e_cnd = false;
  e_valE = 0;
  e_valA = valA.getState();
  e_valC = valC.getState();
  e_valB = valB.getState();
  e_dstE = dstE.getState();
  e_dstM = dstM.getState();

  if(e_icode == IRRMOVQ)
  {
    e_valE = e_valA;
  }

  if(e_icode == IPUSHQ || e_icode == IPOPQ || e_icode == IMRMOVQ || e_icode == IRMMOVQ || e_icode == ICALL || e_icode == IRET)
  {
    uint64_t aluA = getALUA();
    uint64_t aluB = getALUB();
    e_valE = aluA+aluB;
  }

  if(e_icode == IOPX)
  {
    uint64_t aluA = getALUA();
    uint64_t aluB = getALUB();
    uint64_t aluF = getALUFunction();


    if(aluF == FADDQ)
    {
      e_valE = aluA+aluB;
      bool negValE = Tools::isNegative(e_valE);
      bool negAluA = Tools::isNegative(aluA);
      bool negAluB = Tools::isNegative(aluB);

      if(e_valE == 0)
      {
        updateZF = true;
      }
      else updateZF = false;

      if(negValE && !negAluA && !negAluB)
      {
        updateOF = true;
      }
      else if(!negValE && negAluA && negAluB)
      {
        updateOF = true;
      }
      else
      {
        updateOF = false;
      }

      if(negValE)
      {
        updateSF = true;
      }
      else
      {
        updateSF = false;
      }

    }

    if(aluF == FSUBQ)
    {
      e_valE = aluB-aluA;
      bool negValE = Tools::isNegative(e_valE);
      bool negAluA = Tools::isNegative(aluA);
      bool negAluB = Tools::isNegative(aluB);

      if(e_valE == 0)
      {
        updateZF = true;
      }
      else updateZF = false;

      if(negValE && negAluA && !negAluB)
      {
        updateOF = true;
      }
      else if(!negValE && !negAluA && negAluB)
      {
        updateOF = true;
      }
      else
      {
        updateOF = false;
      }

      if(negValE)
      {
        updateSF = true;
      }
      else
      {
        updateSF = false;
      }
    }

    if(aluF == FANDQ)
    {
      e_valE = aluA&aluB;
      bool negValE = Tools::isNegative(e_valE);
      bool negAluA = Tools::isNegative(aluA);
      bool negAluB = Tools::isNegative(aluB);

      if(e_valE == 0)
      {
        updateZF = true;
      }
      else updateZF = false;

      if(negValE)
      {
        updateSF = true;
      }
      else
      {
        updateSF = false;
      }
    }

    if(aluF == FXORQ)
    {
      e_valE = aluA ^ aluB;

      bool negValE = Tools::isNegative(e_valE);
      bool negAluA = Tools::isNegative(aluA);
      bool negAluB = Tools::isNegative(aluB);

      if(e_valE == 0)
      {
        updateZF = true;
      }
      else updateZF = false;

      if(negValE)
      {
        updateSF = true;
      }
      else
      {
        updateSF = false;
      }
    }

  }

  if(e_icode == IIRMOVQ)
  {
     e_valE = e_valC;
  }
///////////////////Maybe getCnd and selectDstE need to be in clockp1
  e_cnd = getCnd();
  e_dstE = selectDstE();
  forward->setE_valE(e_valE);
  forward->setE_dstE(e_dstE);

}

void ExecuteStage::clockP1()
{
  if(!setCC())
  {
    memoryStage -> updateMRegister(SBUB, INOP, 0, 0, 0, 0, RNONE, RNONE);
	cycle++;
    return;
  }

  if(isSetCC())
  {
    regs->setCC(ZF, updateZF);
    regs->setCC(SF, updateSF);
    regs->setCC(OF, updateOF);
  }

  memoryStage->updateMRegister(e_stat, e_icode , e_ifun, e_cnd, e_valE, e_valA, e_dstE, e_dstM);
  cycle++;
}

bool ExecuteStage::getCnd()
{
  uint64_t OFf = regs->getCC(OF);
  uint64_t SFf = regs->getCC(SF);
  uint64_t ZFf = regs->getCC(ZF);
  if(e_ifun == 0)
  {
    e_cnd = true;
  }
  if(e_ifun == 1)
  {
    e_cnd = ((SFf ^ OFf) | ZFf);
  }
  if(e_ifun == 2)
  {
    e_cnd = (SFf ^ OFf);
  }
  if(e_ifun == 3)
  {
    e_cnd = ZFf;
  }
  if(e_ifun == 4)
  {
    e_cnd = (!ZFf);
  }
  if(e_ifun == 5)
  {
    e_cnd = (!(SFf ^ OFf));
  }
  if(e_ifun == 6)
  {
    e_cnd = (!(SFf ^ OFf) & !ZFf);
  }

  return e_cnd;
}



uint64_t ExecuteStage::getALUA()
{
  if(e_icode == IRRMOVQ || e_icode == IOPX)
  {
    return e_valA;
  }

  if(e_icode == IIRMOVQ || e_icode == IRMMOVQ || e_icode == IMRMOVQ)
  {
    return e_valC;
  }
  else if(e_icode == ICALL || e_icode == IPUSHQ)
  {
    return -8;
  }
  else if(e_icode == IRET || e_icode == IPOPQ)
  {
    return 8;
  }
}

uint64_t ExecuteStage::getALUB()
{
  if(e_icode == IRMMOVQ || e_icode == IMRMOVQ || e_icode == IOPX || e_icode == ICALL || e_icode == IPUSHQ || e_icode == IRET || e_icode == IPOPQ)
  {
    return e_valB;
  }
  if(e_icode == IRRMOVQ || e_icode == IIRMOVQ)
  {
    return 0;
  }
}

bool ExecuteStage::isSetCC()
{
  if(e_icode == IOPX) return true;
  else return false;
}

bool ExecuteStage::setCC()
{
  uint64_t W_stat = forward->getW_stat();
  uint64_t M_stat = forward->getM_stat();
  if(M_stat == SADR || M_stat == SINS || M_stat == SHLT)
  {
    return false;
  }
  if(W_stat == SADR || W_stat == SINS || W_stat == SHLT)
  {
    return false;
  }

  return true;
}

uint64_t ExecuteStage::getALUFunction()
{
  if(e_icode == IOPX) return e_ifun;
  else return FADDQ;
}

uint64_t ExecuteStage::selectDstE()
{
  if(e_icode == IRRMOVQ && !e_cnd)
  {
    return RNONE;
  }
  else return e_dstE;
}
