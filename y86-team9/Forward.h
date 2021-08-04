#ifndef FORWARD_H
#define FORWARD_H

#include <iostream>
#include <cstdint>
#include <string>
#include "Memory.h"
#include "ProgRegisters.h"
#include "PipeStage.h"
#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "WritebackStage.h"

class Forward
{
    uint64_t W_icode;
    uint64_t M_icode;
    uint64_t M_valA;
    uint64_t M_Cnd;
    uint64_t W_dstE;
    uint64_t W_valE;
    uint64_t W_dstM;
    uint64_t W_valM;
    uint64_t M_dstM;
    uint64_t M_valM;
    uint64_t M_dstE;
    uint64_t M_valE;
    uint64_t E_dstE;
    uint64_t E_valE;
    uint64_t M_stat;
    uint64_t W_stat;


    public:

      uint64_t getM_stat() {return M_stat;}
      uint64_t getW_stat() {return W_stat;}
      uint64_t getE_dstE() {return E_dstE;}
      uint64_t getE_valE() {return E_valE;}
      uint64_t getW_dstE() {return W_dstE;}
      uint64_t getW_valE() {return W_valE;}
      uint64_t getW_dstM() {return W_dstM;}
      uint64_t getW_valM() {return W_valM;}
      uint64_t getM_dstE() {return M_dstE;}
      uint64_t getM_valE() {return M_valE;}
      uint64_t getM_dstM() {return M_dstM;}
      uint64_t getM_valM() {return M_valM;}
      uint64_t getM_valA() {return M_valA;}
      uint64_t getM_icode() {return M_icode;}
      uint64_t getW_icode() {return W_icode;}
      uint64_t getM_Cnd() {return M_Cnd;}



      void setM_stat(uint64_t m_stat) {M_stat = m_stat;}
      void setW_stat(uint64_t w_stat) {W_stat = w_stat;}
      void setE_dstE(uint64_t e_dstE) {E_dstE = e_dstE;}
      void setE_valE(uint64_t e_valE) {E_valE = e_valE;}
      void setW_dstE(uint64_t w_dstE) {W_dstE = w_dstE;}
      void setW_valE(uint64_t w_valE) {W_valE = w_valE;}
      void setW_dstM(uint64_t w_dstM) {W_dstM = w_dstM;}
      void setW_valM(uint64_t w_valM) {W_valM = w_valM;}
      void setM_dstE(uint64_t m_dstE) {M_dstE = m_dstE;}
      void setM_valE(uint64_t m_valE) {M_valE = m_valE;}
      void setM_dstM(uint64_t m_dstM) {M_dstM = m_dstM;}
      void setM_valM(uint64_t m_valM) {M_valM = m_valM;}
      void setM_icode(uint64_t m_icode) {M_icode = m_icode;}
      void setW_icode(uint64_t w_icode) {W_icode = w_icode;}
      void setM_Cnd(uint64_t m_Cnd) {M_Cnd = m_Cnd;}
      void setM_valA(uint64_t m_valA) {M_valA = m_valA;}
};


#endif
