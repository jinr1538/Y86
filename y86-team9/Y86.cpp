/*
    Name:   Y86.cpp
    Desc:   Starting implementation of Y86 class
    Author: efb
    Date:   1/6/2016
*/
#include <cstdint>
#include "Y86.h"


/*-------------------------------------------------------------------------
  Name:  Y86()
  Desc:  Constructor
---------------------------------------------------------------------------*/
Y86::Y86()
{
    reset();
}
/*-------------------------------------------------------------------------
  reset - resets Y86 object to initial state.

---------------------------------------------------------------------------*/
void Y86::reset()
{
	memory.reset();
	regs.reset();
  stage[0] = &fetchStage;
  stage[1] = &decodeStage;
  stage[2] = &executeStage;
  stage[3] = &memoryStage;
  stage[4] = &writebackStage;
  fetchStage.reset(&decodeStage, &memory, &forward);
  decodeStage.reset(&executeStage, &regs, &forward);
  executeStage.reset(&memoryStage, &regs, &forward);
  memoryStage.reset(&writebackStage, &memory, &forward);
  writebackStage.reset(&regs, &forward);
  cycles = 0;
}

void  Y86::clock()
{
	regs.clock();
  for(int i = 0; i <= 4; i++)
  {
    stage[i]->clockP0();
  }

  for(int i = 0; i <= 4; i++)
  {
    stage[i]->clockP1();
  }

  pipeControl();
  if(F_stall)
  {
    stage[0]->setStall(true);
  }

  if(D_stall)
  {
    stage[1]->setStall(true);
  }

  if(W_stall)
  {
    stage[4]->setStall(true);
  }

  if(D_bubble)
  {
    stage[1]->setBubble(true);
  }
  if(E_bubble)
  {
    stage[2]->setBubble(true);
  }
  if(M_bubble)
  {
    stage[3]->setBubble(true);
  }
  cycle++;
}

uint64_t Y86::getStat()
{
  uint64_t stat = writebackStage.getStat();
  if(stat == SAOK || stat == SBUB)
  {
    return 0;
  }
  else
  {
    return stat;
  }
}

void Y86::pipeControl()
{
  F_stall = false;
  D_stall = false;
  W_stall = false;
  D_bubble = false;
  E_bubble = false;
  M_bubble = false;
  //stage[0] = fetch
  //stage[1] = decode
  //stage[2] = execute
  //stage[3] = memory
  //stage[4] = writeback
  //
  //FetchStage Stall and Bubble

  //F stall
  if((stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) && (executeStage.getDstM() == decodeStage.getSrcA() || executeStage.getDstM() == decodeStage.getSrcB())
    || (stage[1]->getIcode() == IRET || stage[2]->getIcode() == IRET || stage[3]->getIcode() == IRET))
    {
      F_stall = true;
    }
    else F_stall = false;

    //DecodeStage Stall and Bubble
//D stall
    if((stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) && (executeStage.getDstM() == decodeStage.getSrcA() || executeStage.getDstM() == decodeStage.getSrcB()))
      {
        D_stall = true;
      }
      else
      {
        D_stall = false;
      }
//D bubble
      if((stage[2]->getIcode() == IJXX && !(executeStage.getCnd())) || (!((stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) &&
      (executeStage.getDstM() == decodeStage.getSrcA() || executeStage.getDstM() == decodeStage.getSrcB()))) && (stage[1]->getIcode() == IRET || stage[2]->getIcode() == IRET || stage[3]->getIcode() == IRET))
      {
        D_bubble = true;
      }
      else
      {
        D_bubble = false;
      }

    //ExecuteStage Stall and Bubble

//E bubble
    if((stage[2]->getIcode() == IJXX && !executeStage.getCnd()) || (stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) && (executeStage.getDstM() == decodeStage.getSrcA() || executeStage.getDstM() == decodeStage.getSrcA()))
    {
      E_bubble = true;
    }
    else
    {
      E_bubble = false;
    }
    //MemoryStage Stall and Bubble

//M bubble
    if((stage[3]->getStat() == SADR || stage[3]->getStat() == SINS || stage[3]->getStat() == SHLT) || (stage[4]->getStat() == SADR || stage[4]->getStat() == SINS || stage[4]->getStat() == SHLT))
    {
      M_bubble = true;
    }
    else
    {
      M_bubble = false;
    }

    ////WritebackStage Stall and Bubble
//W stall
    if((stage[4]->getStat() == SADR || stage[4]->getStat() == SINS || stage[4]->getStat() == SHLT))
    {
      W_stall = true;
    }
    else
    {
      W_stall = false;
    }


}
