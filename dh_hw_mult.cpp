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
	            // read inputs 
                b.write(in_data_1.read());
                c.write(in_data_2.read());
			}				
			else wait();
			    
			break;
		}
		
		case EXECUTE:
		{  					                            
            // default: all outputs at zero
            b_rld.write(SC_LOGIC_0); 
            c_rld.write(SC_LOGIC_0);
            a0_rld.write(SC_LOGIC_0);
            a1_rld.write(SC_LOGIC_0);

            switch(exe_state)
            {
                case EXE_LOAD:	
                b_rld.write(SC_LOGIC_1);
                c_rld.write(SC_LOGIC_1);
                exe_state = EXE_OUT;
                    break;
                  
                case EXE_OUT:
                a0_rld.write(SC_LOGIC_1);                
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

