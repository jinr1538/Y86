/*
    File:   Y86.h
    Desc:   Y86 constants 
    
    Author: efb
*/

#ifndef REGISTER_H
#define REGISTER_H

template <class T> class Register {
        T input;
        T state;

    public:
        Register(){state = 0; input = 0;}
        void  reset(void){state = 0;input = 0;}
        void	reset(T val){state = val; input = val;}
        void	clock(void){state = input;}
        void	setInput(T next){input = next;}
        T 		getInput(){return input;}
        T 		getState(){return state;}
};        

#endif