Y86-64 Simulator

Lab 7 Summary

-- Adds Forward class
-- Updates each stage class to add pointer to Forward object
-- Updates reset methods of each stage class to recive Forward pointer
-- Updates WritebackStage to write W_dstE and W_valE during clockP0
-- Updates FetchStage to fetch and split all instructions into decode register fields
-- Updates DecodeStage to add methods for determining d_valA and d_valB from selectFwdA and fowardB
   using Forward values.
-- Updates ExecuteStage to implement irmovq, addq, subq, xorq, andq
-- adds additional test files
-- updates run.py script

