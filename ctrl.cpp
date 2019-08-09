/******************************************************************
* ctrl.cpp - HW multiplier controller functions
*
* SystemC source
*
* Madeleine Townley
* Based on source from: Daler Rakhmatov
******************************************************************/
#include <systemc.h>
#include "ctrl.h"
#include "modules.h"

void ctrl::state_reg() {
  while(1) {
    if (reset.read() == SC_LOGIC_1) state.write(LOAD);
    else state.write(next_state.read());
    wait();
  }
}

void ctrl::state_transition() {

  // default: self-loop
  next_state.write(state.read());
  
  switch (state.read()) {

    case LOAD:
        next_state.write(MULT);
        break;
        
    case MULT:
        next_state.write(BR1);
        break;
	
    case BR1:
        if(LT1.read() == SC_LOGIC_1) next_state.write(BR1_LT);
	    else next_state.write(BR2);
        break;

    case BR1_LT:
        next_state.write(BR2);
        break;
        
    case BR2:
        if(LT2.read() == SC_LOGIC_1) next_state.write(BR2_LT);
	    else if(GTE.read() == SC_LOGIC_1) next_state.write(DONE);
        break;

    case BR2_LT:
        next_state.write(DONE);
        break;
        
    case OUTPUT:
        next_state.write(END);
        break;
    
    case END:
        next_state.write(END);
        break;
        
    default:
        break;
  }    
}


void ctrl::state_output() {
  // default: all outputs at zero
  a0_mux_sel.write(SC_LOGIC_0); 
  a1_mux_sel.write(0);
  t_mux_sel.write(SC_LOGIC_0); 
  u_mux_sel.write(SC_LOGIC_0);
  
  b_rld.write(SC_LOGIC_0); 
  c_rld.write(SC_LOGIC_0);
  a0_rld.write(SC_LOGIC_0);
  a1_rld.write(SC_LOGIC_0);
  t_rld.write(SC_LOGIC_0);
  u_rld.write(SC_LOGIC_0);
  
  switch (state.read()) {

    case LOAD:
    b_rld.write(SC_LOGIC_1);
    c_rld.write(SC_LOGIC_1);	    
        break;

    case MULT:
    a0_rld.write(SC_LOGIC_1);
    a1_rld.write(SC_LOGIC_1);
    t_rld.write (SC_LOGIC_1);
    u_rld.write (SC_LOGIC_1);
        break;

    case BR1:
    t_mux_sel.write(SC_LOGIC_1);
    t_rld.write(SC_LOGIC_1);
        break;
    
    case BR1_LT:
    a1_mux_sel.write(1);
    a1_rld.write(SC_LOGIC_1);  
        break;
    
    case BR2:
    u_mux_sel.write(SC_LOGIC_1); 
    u_rld.write (SC_LOGIC_1);
    a0_mux_sel.write(SC_LOGIC_1);
    a0_rld.write(SC_LOGIC_1);
        break;
    
    case BR2_LT:
    a1_mux_sel.write(2);
    a1_rld.write(SC_LOGIC_1);  
        break;
    
    case OUTPUT:
    a1_mux_sel.write(3);  
    a1_rld.write(SC_LOGIC_1);
        break;
    
    case END:
        break;
	
    default:
        break;
  }    
}
