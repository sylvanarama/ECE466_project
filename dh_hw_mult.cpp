#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include "modules.h"


void dh_hw_mult::process_hw_mult()
{
  //printf("-- HW MULT --\n");

    //NN_DIGIT a[2], b, c, t, u;
    //NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;
    //NN_DIGIT a0[2], a1[4], tn[2], un[2];
    //int print = 0;

  for (;;) 
  {    
    switch (hs_state)
	  {
		case WAIT_FOR_EN:
		{
			printf("FSM: wait\n");
			if (hw_mult_enable.read() == true)
			{
				hs_state = EXECUTE;
				exe_state = EXE_LOAD;
				//print  = 0;
			}
				
			else
			    wait();
			    
			break;
		}
		
		case EXECUTE:
		{  					                 
            //printf("FSM: execute\n");
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
                b.write(in_data_1.read());
                c.write(in_data_2.read());
                printf("EXE: LOAD\n");
                b_rld.write(SC_LOGIC_1);
                c_rld.write(SC_LOGIC_1);
                printf("b: 0x%08x\n", DP.b.read());
                printf("c: 0x%08x\n", DP.c.read());
                exe_state = EXE_MULT;
                    break;

                case EXE_MULT:
		        // bHigh = (NN_HALF_DIGIT)HIGH_HALF(b);
                // bLow = (NN_HALF_DIGIT)LOW_HALF(b);
                // cHigh = (NN_HALF_DIGIT)HIGH_HALF(c);
                // cLow = (NN_HALF_DIGIT)LOW_HALF(c);

                // a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
                // t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
                // u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
                // a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
                printf("EXE: MULT\n");
                a0_rld.write(SC_LOGIC_1);
                a1_rld.write(SC_LOGIC_1);
                t_rld.write (SC_LOGIC_1);
                u_rld.write (SC_LOGIC_1);               
                exe_state = EXE_BR1;  
                //a0[0] = DP.a0.read();
                //a1[0] = DP.a1.read();
                //tn[0] = DP.t.read();
                //un[0] = DP.u.read();               
                    break;
                
                case EXE_BR1:
		        // if ((t += u) < u)
	            printf("EXE: BR1\n");
                t_mux_sel.write(SC_LOGIC_1);
                t_rld.write(SC_LOGIC_1);                    		        
                if(LT1.read() == SC_LOGIC_1) exe_state = EXE_BR1_LT;
                else exe_state = EXE_BR2;
                //tn[1] = DP.t.read();
                    break;

                case EXE_BR1_LT:
                // a[1] += TO_HIGH_HALF(1);
                printf("EXE: BR1_LT\n");
                a1_mux_sel.write(1);
                a1_rld.write(SC_LOGIC_1); 
                exe_state = EXE_BR2;
                    break;
                    
                case EXE_BR2:
                // u = TO_HIGH_HALF(t);
                // if ((a[0] += u) < u)
                printf("EXE: BR2\n");
                //a1[1] = DP.a1.read();
                
                u_mux_sel.write(SC_LOGIC_1); 
                u_rld.write (SC_LOGIC_1);
                a0_mux_sel.write(SC_LOGIC_1);
                a0_rld.write(SC_LOGIC_1);
                if(LT2.read() == SC_LOGIC_1) exe_state = EXE_BR2_LT;
                else exe_state = EXE_OUT;
                //un[1] = DP.u.read();  
                //a0[1] = DP.A0_OUT.read();
                    break;

                case EXE_BR2_LT:
                // a[1]++;
                printf("EXE: BR2_LT\n");
                //a1_mux_sel.write(2);
                a1_rld.write(SC_LOGIC_1); 
                exe_state = EXE_OUT;
                    break;
                    
                case EXE_OUT:
                // a[1] += HIGH_HALF(t);
                printf("EXE: OUTPUT\n");                
                //a1[2] = DP.A1_OUT.read();
                
                a1_mux_sel.write(3);  
                a1_rld.write(SC_LOGIC_1);
                exe_state = EXE_WAIT; 
                //a1[3] = DP.A1_OUT.read();        
                    break;
                
                case EXE_WAIT:
                printf("EXE: WAIT\n");
                hs_state = OUTPUT;
                    break;
                    
                default:
                    break;
            } // switch exe_state
            
	        break;
        }

		case OUTPUT:
		{
			printf("FSM: output\n");
			// Write outputs
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
			printf("FSM: finish\n");
			/*
			if(print == 0)
			{
    			cout << "HW MULT RESULTS:" << endl;
    			printf("t:  0x%08x   0x%08x\n", tn[0], tn[1]);
    			printf("u:  0x%08x   0x%08x\n", un[0], un[1]);
    			printf("a0: 0x%08x   0x%08x\n", a0[0], a0[1]);
    			printf("a1: 0x%08x   0x%08x   0x%08x   0x%08x\n", a1[0], a1[1], a1[2], a1[3]);
    			print = 1; 			
			}
			*/
			if (hw_mult_enable.read() == false)
			{
				hw_mult_done.write(false);
				hs_state = WAIT_FOR_EN;
			}		
			else
			    wait();
			    
			break;
		}
		
		default:
		    break; 

	} // END switch
	wait();
  } // END for	  	  
} // END dh_hw_mult::process_hw_mult()

