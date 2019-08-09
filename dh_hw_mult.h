#include "systemc.h"
#include "digit.h"
#include "datapath.h"
#include "ctrl.h"
#include "modules.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

SC_MODULE (dh_hw_mult)
{
  
  // Ports
  sc_in_clk clock;
  sc_in<sc_logic> reset;
  sc_in<bool> hw_mult_enable; 
  sc_in<NN_DIGIT> in_data_1;
  sc_in<NN_DIGIT> in_data_2;
  sc_out<NN_DIGIT> out_data_low;
  sc_out<NN_DIGIT> out_data_high;
  sc_out<bool> hw_mult_done;
  
  // Signals
  sc_signal< <NN_DIGIT> a1_mux_sel;
  sc_signal<sc_logic> a0_mux_sel, t_mux_sel, u_mux_sel;
  sc_signal<sc_logic> b_rld, c_rld;
  sc_signal<sc_logic> a0_rld, a1_rld, t_rld, u_rld;
  sc_signal<sc_logic> LT1, LT2;
  
  // Instances
  datapath DP;
  ctrl CL;

  void process_hw_mult();
  
  SC_CTOR (dh_hw_mult): DP("DATAPATH"), CL("CONTROL")
  {   
      // Interconnections
      
      // Datapath
      DP.a0_mux_sel(a0_mux_sel);
      DP.a1_mux_sel(a1_mux_sel);
      DP.t_mux_sel(t_mux_sel);
      DP.u_mux_sel(u_mux_sel);
      
      DP.b_rld(d_rld);
      DP.c_rld(c_rld);   
      DP.a0_rld(a0_rld);
      DP.a1_rld(a1_rld);
      DP.t_rld(t_rld);
      DP.u_rld(u_rld);
      
      DP.B_IN(in_data_1);
      DP.C_IN(in_data_2);
      DP.A0_OUT(out_data_low);
      DP.A1_OUT(out_data_high);
      
      DP.LT1(LT1);
      DP.LT2(LT2);
      
      DP.reset(reset);
      DP.clock(clock);
      
      // Controller
      CL.a0_mux_sel(a0_mux_sel);
      CL.a1_mux_sel(a1_mux_sel);
      CL.t_mux_sel(t_mux_sel);
      CL.u_mux_sel(u_mux_sel);
      
      CL.b_rld(d_rld);
      CL.c_rld(c_rld);   
      CL.a0_rld(a0_rld);
      CL.a1_rld(a1_rld);
      CL.t_rld(t_rld);
      CL.u_rld(u_rld);
      
      CL.LT1(LT1);
      CL.LT2(LT2);
      
      CL.reset(reset);
      CL.clock(clock);
    
      SC_CTHREAD (process_hw_mult, clock.pos());
      sensitive << hw_mult_enable;
  }
  
};

#endif /* end _DH_HW_MULT_H_ */
