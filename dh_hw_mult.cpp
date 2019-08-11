#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include "modules.h"


void dh_hw_mult::process_hw_mult()
{

  for (;;) 
  {    
    switch (hs_state)
	  {
		case WAIT_FOR_EN:
		{
			if (hw_mult_enable.read() == true)
			{
				hs_state = EXECUTE;
				exe_state = EXE_LOAD;
			}				
			else wait();
			    
			break;
		}
		
		case EXECUTE:
		{  					                 
            // read inputs 
            b.write(in_data_1.read());
            c.write(in_data_2.read());
            
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

            switch(exe_state)
            {
                case EXE_LOAD:	
                b_rld.write(SC_LOGIC_1);
                c_rld.write(SC_LOGIC_1);
                //exe_state = EXE_MULT;
                exe_state = EXE_OUT;
                    break;
/*
                case EXE_MULT:
		        // bHigh = (NN_HALF_DIGIT)HIGH_HALF(b);
                // bLow = (NN_HALF_DIGIT)LOW_HALF(b);
                // cHigh = (NN_HALF_DIGIT)HIGH_HALF(c);
                // cLow = (NN_HALF_DIGIT)LOW_HALF(c);
                // a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
                // t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
                // u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
                // a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
                a0_rld.write(SC_LOGIC_1);
                a1_rld.write(SC_LOGIC_1);
                t_rld.write (SC_LOGIC_1);
                u_rld.write (SC_LOGIC_1);               
                exe_state = EXE_BR1;               
                    break;
                
                case EXE_BR1:
		        // if ((t += u) < u)
                t_mux_sel.write(SC_LOGIC_1);
                t_rld.write(SC_LOGIC_1);                    		        
                if(LT1.read() == SC_LOGIC_1) exe_state = EXE_BR1_LT;
                else exe_state = EXE_BR2;
                    break;

                case EXE_BR1_LT:
                // a[1] += TO_HIGH_HALF(1);
                a1_mux_sel.write(1);
                a1_rld.write(SC_LOGIC_1); 
                exe_state = EXE_BR2;
                    break;
                    
                case EXE_BR2:
                // u = TO_HIGH_HALF(t);
                // if ((a[0] += u) < u)
                //a1[1] = DP.a1.read();                
                u_mux_sel.write(SC_LOGIC_1); 
                u_rld.write (SC_LOGIC_1);
                a0_mux_sel.write(SC_LOGIC_1);
                a0_rld.write(SC_LOGIC_1);
                if(LT2.read() == SC_LOGIC_1) exe_state = EXE_BR2_LT;
                else exe_state = EXE_OUT;
                    break;

                case EXE_BR2_LT:
                // a[1]++;
                //a1_mux_sel.write(2);
                a1_rld.write(SC_LOGIC_1); 
                exe_state = EXE_OUT;
                    break;
  */                  
                case EXE_OUT:
                // a[1] += HIGH_HALF(t);             
                //a1[2] = DP.A1_OUT.read(); 
                a0_mux_sel.write(SC_LOGIC_1);
                a0_rld.write(SC_LOGIC_1);              
                a1_mux_sel.write(3);  
                a1_rld.write(SC_LOGIC_1);
                exe_state = EXE_WAIT;      
                    break;
                
                case EXE_WAIT:
                hs_state = OUTPUT;
                    break;
                    
                default:
                    break;
            } // switch exe_state
            
	        break;
        }

		case OUTPUT:
		{
			//out_data_low.write(a[0]);
			//out_data_high.write(a[1]);
			
			out_data_low.write(a0.read());
			out_data_high.write(a1.read());

			hw_mult_done.write(true);
			hs_state = FINISH;
			break;
		}

		case FINISH:
		{
			if (hw_mult_enable.read() == false)
			{
				hw_mult_done.write(false);
				hs_state = WAIT_FOR_EN;
			}					    
			break;
		}
		
		default:
		    break; 

	} // END switch
	wait();
  } // END for	  	  
} // END dh_hw_mult::process_hw_mult()

