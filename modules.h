/******************************************************************
* modules.h - Modules for HW multiplication simulation
*
* SystemC source
*
* Modified by :Madeleine Townley
* Created by: Josh Pfrimmer & Daler Rakhmatov
******************************************************************/
#ifndef MODULES_H
#define MODULES_H
#include "systemc.h"
#include "digit.h"


////////////////////////
// add - adds number B to number A
////////////////////////
SC_MODULE(add) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> A; 
    sc_in<NN_DIGIT> B;	
    sc_out<NN_DIGIT> OUT;

    void add_proc()
    {
        OUT.write(A.read() + B.read());
    }
    
    SC_CTOR(add) {
        SC_METHOD(add_proc);
        sensitive << A << B;
    }
};

////////////////////////
// and2 - takes the bitwise AND of A and B
////////////////////////
SC_MODULE(and2) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> A; 
    sc_in<NN_DIGIT> B;	
    sc_out<NN_DIGIT> OUT;

    void and2_proc()
    {
        OUT.write(A.read() & B.read());
    }
    
    SC_CTOR(and2) {
        SC_METHOD(and2_proc);
        sensitive << A << B;
    }
};

////////////////////////
// mult - multiplies two NN_DIGITs
////////////////////////
SC_MODULE(mult) {

  sc_in<NN_DIGIT> A,B;
  sc_out<NN_DIGIT> OUT;
  
  void mult_proc() {  
    OUT.write( A.read() * B.read() );
  }

  SC_CTOR(mult) {
    SC_METHOD(mult_proc);
    sensitive << A << B;
  }

};

////////////////////////
// mux2 - 2:1 multiplexer
////////////////////////
SC_MODULE(mux2) {
    //
    // Ports
    //
    sc_in<sc_logic> sel;
    sc_in<NN_DIGIT> A; 
    sc_in<NN_DIGIT> B;	
    sc_out<NN_DIGIT> OUT;

    void mux2_proc() 
    {   
        if (sel.read() == SC_LOGIC_1) 
            OUT.write(B.read());
        else 
            OUT.write(A.read());
    }
    
    SC_CTOR(mux2) {
        SC_METHOD(mux2_proc);
        sensitive << sel << A << B;
    }
};

////////////////////////
// reg - NN_DIGIT register with reset and load
////////////////////////
SC_MODULE(reg) {
    //
    // Ports
    //
    //sc_in<sc_logic> load, reset;
    sc_in<sc_logic> load;
    sc_in_clk clock;
    sc_in<NN_DIGIT> IN; 
    sc_out<NN_DIGIT> OUT;	

    void reg_proc() 
    {  
        while (1) {
            //if (reset.read() == SC_LOGIC_1) OUT.write(0);
            //else if (load.read() == SC_LOGIC_1) OUT.write(IN.read());
            if (load.read() == SC_LOGIC_1) OUT.write(IN.read());
	        wait();
        }
    }
    
    SC_CTOR(reg) {
        SC_CTHREAD(reg_proc, clock.pos());
    }
};


////////////////////////
// comp - less-than comparator
////////////////////////
SC_MODULE(comp) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> A, B; 
    //sc_out<sc_logic> EQ, GT, LT;
    sc_out<sc_logic> LT;

    void comp_proc() 
    {
        
        // Default
        //EQ.write(SC_LOGIC_0);
        //GT.write(SC_LOGIC_0);
        LT.write(SC_LOGIC_0);
        
        //if (A.read() == B.read()) EQ.write(SC_LOGIC_1);
        //if (A.read() > B.read()) GT.write(SC_LOGIC_1);
        if (A.read() < B.read()) LT.write(SC_LOGIC_1);
        
    }
    
    SC_CTOR(comp) {
        SC_METHOD(comp_proc);
        sensitive << A << B;
    }
};


// Shift a value by K bits left or right
// k:  # bits to shift
// LR: 0 for <<, 1 for >>
SC_MODULE(shift) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> IN; 
    sc_out<NN_DIGIT> OUT;
    
    sc_logic LR;
    NN_DIGIT k;

    void shift_proc()
    {
        if(LR == SC_LOGIC_1)
            OUT.write(IN.read() >> k);
        else
            OUT.write(IN.read() << k);
    }
    
    SC_HAS_PROCESS(shift);
    
    shift(sc_module_name nm, sc_logic dir, NN_DIGIT num_bits)
    :sc_module(nm)
    {
        SC_METHOD(shift_proc);
        sensitive << IN;
        k = num_bits;
        LR = dir;
    }
};


#endif
