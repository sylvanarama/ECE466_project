/******************************************************************
* datapath.h - Datapath for HW multiplier
*
* SystemC source
*
* Madeleine Townley
* Based on source from: Josh Pfrimmer & Daler Rakhmatov 
******************************************************************/

#ifndef DATAPATH_H
#define DATAPATH_H
#include "systemc.h"
#include "modules.h"
#include "digit.h"

///////////////////////////
// Data Path
///////////////////////////
SC_MODULE(datapath) {
    //
    // Ports
    //
    sc_in<sc_logic> b_rld, c_rld;
    sc_in<sc_logic> a0_rld, a1_rld;      
    sc_in<NN_DIGIT> B_IN, C_IN;
    
    sc_out<NN_DIGIT> A0_OUT, A1_OUT;
    sc_in_clk clock;

    //
    // Signals
    //
    sc_signal<NN_DIGIT> b, c;               // inputs
    sc_signal<NN_DIGIT> S1, S2, S3, S4;     // shifter output
    sc_signal<NN_DIGIT> N1, N2, N3, N4, N5; // AND gate outputs
    sc_signal<NN_DIGIT> M1, M2, M3, M4;     // multiplier outputs
    sc_signal<NN_DIGIT> P1, P2, a0, P4, a1; // adder outputs 
    sc_signal<NN_DIGIT> MUX1, MUX2;         // mux outputs   
    sc_signal<sc_logic> LT1, LT2;           // comparator outputs 
    sc_signal<NN_DIGIT> k1, k2, k3;
        
    //
    // Instances
    //
    reg     a0reg, a1reg, breg, creg;    
    mux2    muxbr1, muxbr2; 

    shift     shift1, shift2, shift3, shift4;
    and2      n1, n2, n3, n4, n5; 
    mult      mult1, mult2, mult3, mult4;
    add       add1, add2, add3, add4, add5;
    comp      comp1, comp2;
    
    SC_CTOR(datapath):
    a0reg("a0reg"), a1reg("a1reg"), breg("breg"), creg("creg"),  
    muxbr1("mux1"), muxbr2("mux2"),	
	shift1("shift1", SC_LOGIC_1, 16), shift2("shift2", SC_LOGIC_1, 16), 
	shift3("shift3", SC_LOGIC_0, 16), shift4("shift4", SC_LOGIC_1, 16),
	n1("n1"), n2("n2"), n3("n3"), n4("n4"), n5("n5"),
	mult1("mult1"), mult2("mult2"), mult3("mult3"), mult4("mult4"),
	add1("add1"), add2("add2"), add3("add3"), add4("add4"), add5("add5"),
	comp1("comp1"), comp2("comp2"){ 
	
 	//
	// Interconnections
	//
        a0reg.clock(clock);
        a0reg.load(a0_rld);
        a0reg.IN(a0);
        a0reg.OUT(A0_OUT);  
        
        a1reg.clock(clock);
        a1reg.load(a1_rld);
        a1reg.IN(a1);
        a1reg.OUT(A1_OUT);                  
        
        breg.clock(clock);
        breg.load(b_rld);
        breg.IN(B_IN);
        breg.OUT(b);  
        
        creg.clock(clock);
        creg.load(c_rld);
        creg.IN(C_IN);
        creg.OUT(c);                  
        
        muxbr1.A(M1);
        muxbr1.B(P2);
        muxbr1.OUT(MUX1);
        muxbr1.sel(LT1);
        
        muxbr2.A(MUX1);
        muxbr2.B(P4);
        muxbr2.OUT(MUX2);
        muxbr2.sel(LT2);
            
        shift1.IN(b);
        shift1.OUT(S1);
        
        shift2.IN(c);
        shift2.OUT(S2);
        
        shift3.IN(P1);
        shift3.OUT(S3);

        shift4.IN(P1);
        shift4.OUT(S4);

        n1.A(S1);
        n1.B(k1);
        n1.OUT(N1);
        
        n2.A(b);
        n2.B(k1);
        n2.OUT(N2);
        
        n3.A(S2);
        n3.B(k1);
        n3.OUT(N3);
        
        n4.A(c);
        n4.B(k1);
        n4.OUT(N4);
        
        n5.A(S4);
        n5.B(k1);
        n5.OUT(N5);
        
        mult1.A(N1);
        mult1.B(N3);
        mult1.OUT(M1);
        
        mult2.A(N2);
        mult2.B(N4);
        mult2.OUT(M2);
        
        mult3.A(N3);
        mult3.B(N2);
        mult3.OUT(M3);      
        
        mult4.A(N4);
        mult4.B(N1);
        mult4.OUT(M4);      
        
        add1.A(M3);
        add1.B(M4);
        add1.OUT(P1);

        add2.A(M1);
        add2.B(k2);
        add2.OUT(P2);

        add3.A(M2);
        add3.B(S3);
        add3.OUT(a0);

        add4.A(MUX1);
        add4.B(k3);
        add4.OUT(P4);

        add5.A(MUX2);
        add5.B(N5);
        add5.OUT(a1);
		
        comp1.A(P1);
        comp1.B(M4);
        comp1.LT(LT1);
        
        comp2.A(a0);
        comp2.B(S3);
        comp2.LT(LT2);
        
        k1.write(MAX_NN_HALF_DIGIT);
        k2.write(0x10000);
        k3.write(0x1);
	
    }
};

#endif
