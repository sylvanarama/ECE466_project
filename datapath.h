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

///////////////////////////
// Data Path
///////////////////////////
SC_MODULE(datapath) {
    //
    // Ports
    //
    sc_in< sc_lv<2> > a1_mux_sel;
    sc_in<sc_logic> a0_mux_sel, t_mux_sel, u_mux_sel;
    sc_in<sc_logic> b_rld, c_rld;
    sc_in<sc_logic> a0_rld, a1_rld, t_rld, u_rld;
    
    sc_out<sc_logic> C1, C2; // comparator outputs
    
    sc_in<NN_DIGIT> B_IN, C_IN;
    sc_out<NN_DIGIT> A0_OUT, A1_OUT;

	
    sc_in<sc_logic> reset;
    sc_in_clk clock;

    //
    // Signals
    //
    sc_signal<NN_DIGIT> b, c, a0, a0_n, a1, a1_n, t, t_n, u, u_n; // variables
    sc_signal<NN_DIGIT> S1, S2, S3, S4;     // shifter output
    sc_signal<NN_DIGIT> N1, N2, N3, N4, N5; // AND gate outputs
    sc_signal<NN_DIGIT> M1, M2, M3, M4;     // multiplier outputs
    sc_signal<NN_DIGIT> P1, P2, P3, P4, P5; // adder outputs
    //sc_signal<NN_DIGIT> C1, C2;             // comparator outputs
    sc_signal<NN_DIGIT> MUX1, MUX2;        // mux outputs
        
    //
    // Instances
    //
    reg     a0reg, a1reg, breg, creg, treg, ureg;    
    mux4    a1mux;
    mux2    a0mux, tmux, umux, mux1 mux2; 

    shift    shift1, shift2, shift3, shift4;
    and2     n1, n2, n3, n4, n5; 
    mult     mult1, mult2, mult3,mult4;
    add      add1, add2, add3, add4, add5;
    comp     comp1, comp2;
    
    
    SC_CTOR(datapath):
    a0reg("a0reg"), a1reg("a1reg"), breg("breg"), creg("creg"),
    treg("treg"), ureg("ureg"), 
    
    a1mux("a1mux"), a0mux("a0mux"), tmux("tmux"), umux("umux"), 
    mux1("mux1"), mux2("mux2"),	
	
	shift1("shift1"), shift2("shift2"), shift3("shift3"), shift4("shift4"),
	n1("n1"), n2("n2"), n3("n3"), n4("n4"), n5("n5"),
	mult1("mult1"), mult2("mult2"), mult3("mult3"), mult4("mult4"),
	add1("add1"), add2("add2"), add3("add3"), add4("add4"), add5("add5"),
	comp1("comp1"), comp2("comp2"){ 
 	//
	// Interconnections
	//
        a0reg.clock(clock);
        a0reg.reset(reset);
        a0reg.load(a0_rld);
        a0reg.IN(a0_n);
        a0reg.OUT(a0);  
        
        a1reg.clock(clock);
        a1reg.reset(reset);
        a1reg.load(a1_rld);
        a1reg.IN(a1_n);
        a1reg.OUT(a1);                  
        
        breg.clock(clock);
        breg.reset(reset);
        breg.load(b_rld);
        breg.IN(B_IN);
        breg.OUT(b);  
        
        creg.clock(clock);
        creg.reset(reset);
        creg.load(c_rld);
        creg.IN(C_IN);
        creg.OUT(c); 
        
        treg.clock(clock);
        treg.reset(reset);
        treg.load(t_rld);
        treg.IN(t_n);
        treg.OUT(t);   
        
        ureg.clock(clock);
        ureg.reset(reset);
        ureg.load(u_rld);
        ureg.IN(u_n);
        ureg.OUT(u);   
        
        a0mux.A(M2);
        a0mux.B(P3);
        a0mux.OUT(a0_n);
        a0mux.sel(a0_mux_sel);
        
        a1mux.A(M1);
        a1mux.B(MUX1);
        a1mux.C(MUX2);
        a1mux.D(P5);
        a1mux.OUT(a1_n);
        a1mux.sel(a1_mux_sel);
            
        tmux.A(M3);
        tmux.B(P1);
        tmux.OUT(t_n);
        tmux.sel(t_mux_sel);
        
        umux.A(M4);
        umux.B(S3);
        umux.OUT(u_n);
        umux.sel(u_mux_sel);
        
        mux1.A(M1);
        mux1.B(P2);
        mux1.OUT(MUX1);
        mux1.sel(LT1);
        
        mux2.A(MUX1);
        mux2.B(P4);
        mux2.OUT(MUX2);
        mux2.sel(LT2);
            
        shift1.IN(b);
        shift1.OUT(S1);
        shift1.LR(SC_LOGIC_1);
        
        shift2.IN(c);
        shift2.OUT(S2);
        shift2.LR(SC_LOGIC_1);
        
        shift1.IN(P1);
        shift1.OUT(S3);
        shift1.LR(SC_LOGIC_0);
        
        shift1.IN(P1);
        shift1.OUT(S4);
        shift1.LR(SC_LOGIC_1);

        n1.A(MAX_NN_HALF_DIGIT);
        n1.B(S1);
        n1.OUT(N1);
        
        n2.A(MAX_NN_HALF_DIGIT);
        n2.B(b);
        n2.OUT(N2);
        
        n3.A(MAX_NN_HALF_DIGIT);
        n3.B(S2);
        n3.OUT(N3);
        
        n4.A(MAX_NN_HALF_DIGIT);
        n4.B(c);
        n4.OUT(N4);
        
        n5.A(MAX_NN_HALF_DIGIT);
        n5.B(S4);
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

        add2.A(0x10000);
        add2.B(M1);
        add2.OUT(P2);

        add3.A(M2);
        add3.B(S3);
        add3.OUT(P3);

        add4.A(0x1);
        add4.B(MUX1);
        add4OUT(P4);

        add5.A(MUX2);
        add5.B(N5);
        add5.OUT(P5);
		
        comp1.A(P1);
        comp1.B(M4);
        comp1.LT(LT1);
        
        comp2.A(P3);
        comp2.B(S3);
        comp2.LT(LT2);
	
    }
};

#endif
