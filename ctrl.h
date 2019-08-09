/******************************************************************
* ctrl.h - Controller for HW multiplier
*
* SystemC source
*
* Madeleine Townley
* Based on source from: Daler Rakhmatov 
******************************************************************/

#ifndef CONTROL_H
#define CONTROL_H
#include "systemc.h"

enum ctrl_state {LOAD, MULT, BR1, BR1_LT, BR2, BR2_LT, OUTPUT, END};  
    
///////////////////////////
// Controller
///////////////////////////
SC_MODULE(ctrl) {
    //
    // Ports
    //
    
    sc_out< sc_lv<2> > a1_mux_sel;
    sc_out<sc_logic> a0_mux_sel, t_mux_sel, u_mux_sel;
    sc_out<sc_logic> b_rld, c_rld;
    sc_out<sc_logic> a0_rld, a1_rld, t_rld, u_rld, tmp_rld;
	
    sc_in<sc_logic> reset;
    sc_in<sc_logic> LT1, LT2;
    sc_in_clk clock;
    
    //
    // Signals
    //
    sc_signal<ctrl_state> state, next_state;
    
    //
    // Functions
    //    
    void state_transition();
    void state_output();
    void state_reg();
   
    
    SC_CTOR(ctrl) { 
	SC_METHOD(state_transition);
	sensitive << state << LT1 << LT2;
	SC_METHOD(state_output);
	sensitive << state;	
	SC_CTHREAD(state_reg, clock.pos());
    }
};

#endif
