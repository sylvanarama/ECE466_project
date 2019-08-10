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

void ctrl::state_reg() 
{
    printf("CL: STATE REG\n");
    while(1) 
    {
        // wait while handshaking FSM not in EXECUTE state
        if(state.read() == WAIT) 
            state.write(WAIT);   
        else state.write(next_state.read());  
        wait();
    }
}

void ctrl::state_transition() {
  // printf("CL: STATE TRANSITION\n");
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
	    else next_state.write(WRITE);
        break;

    case BR2_LT:
        next_state.write(WRITE);
        break;
        
    case WRITE:
        next_state.write(WAIT);      
        break;
    
    case WAIT:
        next_state.write(LOAD);
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
    printf("CL: LOAD\n");
    b_rld.write(SC_LOGIC_1);
    c_rld.write(SC_LOGIC_1);	    
        break;

    case MULT:
    printf("CL: MULT\n");
    a0_rld.write(SC_LOGIC_1);
    a1_rld.write(SC_LOGIC_1);
    t_rld.write (SC_LOGIC_1);
    u_rld.write (SC_LOGIC_1);
        break;

    case BR1:
    printf("CL: BR1\n");
    t_mux_sel.write(SC_LOGIC_1);
    t_rld.write(SC_LOGIC_1);
        break;
    
    case BR1_LT:
    printf("CL: BR1_LT\n");
    a1_mux_sel.write(1);
    a1_rld.write(SC_LOGIC_1);  
        break;
    
    case BR2:
    printf("CL: BR2\n");
    u_mux_sel.write(SC_LOGIC_1); 
    u_rld.write (SC_LOGIC_1);
    a0_mux_sel.write(SC_LOGIC_1);
    a0_rld.write(SC_LOGIC_1);
        break;
    
    case BR2_LT:
    printf("CL: BR2_LT\n");
    a1_mux_sel.write(2);
    a1_rld.write(SC_LOGIC_1);  
        break;
    
    case WRITE:
    printf("CL: OUTPUT\n");
    a1_mux_sel.write(3);  
    a1_rld.write(SC_LOGIC_1);
        break;
    
    case WAIT:
    printf("CL: WAIT\n");
        break;
	
    default:
        break;
  }    
}
