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

// Adds a constant to input A
SC_MODULE(add_const) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> A; 
    sc_out<NN_DIGIT> OUT;
    
    NN_DIGIT k;

    void add_const_proc()
    {
        OUT.write(A.read() + k);
    }
    
    SC_HAS_PROCESS(add_const);
    
    add_const(sc_module_name nm, NN_DIGIT add_val)
    :sc_module(nm)
    {
        SC_METHOD(add_const_proc);
        sensitive << A;
        k = add_val;
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

// Bitwise AND with a constant value
SC_MODULE(and_const) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> A; 
    sc_out<NN_DIGIT> OUT;
    
    NN_DIGIT k;

    void and_const_proc()
    {
        OUT.write(A.read() & k);
    }
    
    SC_HAS_PROCESS(and_const);
    
    and_const(sc_module_name nm, NN_DIGIT and_val)
    :sc_module(nm)
    {
        SC_METHOD(and_const_proc);
        sensitive << A;
        k = and_val;
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
// mux4 - 4:1 multiplexer
////////////////////////
SC_MODULE(mux4) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> sel;
    sc_in<NN_DIGIT> A; 
    sc_in<NN_DIGIT> B;	
    sc_in<NN_DIGIT> C;
    sc_in<NN_DIGIT> D;
    sc_out<NN_DIGIT> OUT;

    void mux4_proc() 
    {   
        if (sel.read() == 0) 
            OUT.write(A.read());
        else if (sel.read() == 1) 
            OUT.write(B.read());
        else if (sel.read() == 2) 
            OUT.write(C.read());
        else 
            OUT.write(D.read());
    }
    
    SC_CTOR(mux4) {
        SC_METHOD(mux4_proc);
        sensitive << sel << A << B << C << D;
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

////////////////////////
// Shifter: Shift 16 bits left or right
////////////////////////
SC_MODULE(shiftL) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> IN; 
    sc_out<NN_DIGIT> OUT;

    void shiftL_proc()
    {
        OUT.write(IN.read() << 16);          
    }
    
    SC_CTOR(shiftL) {
        SC_METHOD(shiftL_proc);
        sensitive << IN;
    }
};

SC_MODULE(shiftR) {
    //
    // Ports
    //
    sc_in<NN_DIGIT> IN; 
    sc_out<NN_DIGIT> OUT;

    void shiftR_proc()
    {
        OUT.write(IN.read() >> 16);          
    }
    
    SC_CTOR(shiftR) {
        SC_METHOD(shiftR_proc);
        sensitive << IN;
    }
};


#endif
