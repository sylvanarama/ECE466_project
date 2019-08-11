#include "systemc.h"
#include "digit.h"
#include "datapath.h"
#include "modules.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

enum hs_fsm_state {WAIT_FOR_EN, EXECUTE, OUTPUT, FINISH};
enum ctrl_fsm_state {EXE_WAIT, EXE_LOAD, EXE_MULT, EXE_BR1, EXE_BR1_LT, EXE_BR2, EXE_BR2_LT, EXE_OUT};

SC_MODULE (dh_hw_mult)
{
  
  // Ports
  sc_in_clk clock;
  sc_in<bool> hw_mult_enable; 
  sc_in<NN_DIGIT> in_data_1;
  sc_in<NN_DIGIT> in_data_2;
  sc_out<NN_DIGIT> out_data_low;
  sc_out<NN_DIGIT> out_data_high;
  sc_out<bool> hw_mult_done;
  
  // Signals
  sc_signal<NN_DIGIT> a1_mux_sel;
  sc_signal<sc_logic> a0_mux_sel, t_mux_sel, u_mux_sel;
  sc_signal<sc_logic> b_rld, c_rld;
  sc_signal<sc_logic> a0_rld, a1_rld, t_rld, u_rld;
  sc_signal<sc_logic> LT1, LT2;
  
  sc_signal<NN_DIGIT> a0, a1, b, c;
  
  
  // Internal variables
  hs_fsm_state hs_state;
  ctrl_fsm_state exe_state;
  
  // Instances
  datapath DP;

  void process_hw_mult();
  
  SC_CTOR (dh_hw_mult): DP("DATAPATH")
  {   
      // Datapath connections
      DP.a0_mux_sel(a0_mux_sel);
      DP.a1_mux_sel(a1_mux_sel);
      DP.t_mux_sel(t_mux_sel);
      DP.u_mux_sel(u_mux_sel);
      
      DP.b_rld(b_rld);
      DP.c_rld(c_rld);   
      DP.a0_rld(a0_rld);
      DP.a1_rld(a1_rld);
      DP.t_rld(t_rld);
      DP.u_rld(u_rld);
      
      DP.B_IN(b);
      DP.C_IN(c);
      DP.A0_OUT(a0);
      DP.A1_OUT(a1);
      
      DP.LT1(LT1);
      DP.LT2(LT2);
      
      DP.clock(clock);
      
      hs_state = WAIT_FOR_EN;
      exe_state = EXE_WAIT;
    
      SC_CTHREAD (process_hw_mult, clock.pos());
      sensitive << hw_mult_enable;
  }
  
};

#endif /* end _DH_HW_MULT_H_ */
