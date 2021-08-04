/*
    Name:   PipeStage.h

    Desc:   Abstract base class that represents a Y86 pipeline stage. This is
            subclassed by each stage class where pure virtual methods are implemented.

    Author: efb
*/

#ifndef PIPESTAGE_H
#define PIPESTAGE_H

#include "Sim.h"
#include "Register.h"


class Y86;  // forward reference to allow Y86 declaration

class PipeStage {

    protected:  // visible to subclasses
        Register<uint64_t>    stat;     // unused in Fetch stage
        Register<uint64_t>    icode;    // unused in Fetch stage
        Register <uint64_t> 	ifun;     // ALU function code
        bool					        traceEnabled; // used for output tracing
        bool stageStall;
        bool stageBubble;

    public:
        void setTrace(bool enable){traceEnabled = enable;}
        uint64_t getStat(){return stat.getState();}
        uint64_t getIcode(){return icode.getState();}
        void setStall(bool input){stageStall = input;}
        void setBubble(bool input){stageBubble = input;}

        // Pure virtual--must be implemented in subclasses
        virtual void clockP0() = 0;
        virtual void clockP1() = 0;
        virtual void trace() = 0; // This is implemented in Y86trace.cpp
                                  // for each subclass of PipeStage


};

#endif
