#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include "modules.h"


void dh_hw_mult::process_hw_mult()
{
  //printf("-- HW MULT --\n");

    //NN_DIGIT a[2], b, c, t, u;
    //NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;
    NN_DIGIT a0[2], a1[4], tn[2], un[2];

  for (;;) 
  {    
    switch (hs_state)
	  {
		case WAIT_FOR_EN:
		{
			//printf("FSM: wait\n");
			if (hw_mult_enable.read() == true)
			{
				hs_state = EXECUTE;
				exe_state = EXE_LOAD;
			}
				
			else
			    wait();
			    
			break;
		}
		
		case EXECUTE:
		{  					
			/*
			b = in_data_1.read();
			c = in_data_2.read();

			// Original code from NN_DigitMult()...		
			bHigh = (NN_HALF_DIGIT)HIGH_HALF(b);
			bLow = (NN_HALF_DIGIT)LOW_HALF(b);
			cHigh = (NN_HALF_DIGIT)HIGH_HALF(c);
			cLow = (NN_HALF_DIGIT)LOW_HALF(c);
			cout << "bH: " << std::hex << bHigh << ", bL: " << bLow << std::dec << endl;
			cout << "cH: " << std::hex << cHigh << ", cL: " << cLow << std::dec << endl;
            
			a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
			t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
			u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
			a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
			cout << "a0(1): " << std::hex << a[0] << ", a1(1): " << a[1] << std::dec << endl;
			cout << "t(1): " << std::hex << t << ", u(1): " << u << std::dec << endl;

			if ((t += u) < u) a[1] += TO_HIGH_HALF(1);
			u = TO_HIGH_HALF(t);
			cout << "t(2): " << std::hex << t << std::dec << endl;
			cout << "a1(2): " << std::hex << a[1] << std::dec << endl;
			cout << "u(2): " << std::hex << u << std::dec << endl;

			if ((a[0] += u) < u) a[1]++;
			cout << "a0(2): " << std::hex << a[0] << std::dec << endl;
			cout << "a1(3): " << std::hex << a[1] << std::dec << endl;
			a[1] += HIGH_HALF(t);
			cout << "a1(4): " << std::hex << a[1] << std::dec << endl;
			cout << endl;
            */
            
            
            
            
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
                // b = in_data_1.read();
                // c = in_data_2.read();
                //printf("EXE: LOAD\n");
                b_rld.write(SC_LOGIC_1);
                c_rld.write(SC_LOGIC_1);
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
                //printf("EXE: MULT\n");
                a0_rld.write(SC_LOGIC_1);
                a1_rld.write(SC_LOGIC_1);
                t_rld.write (SC_LOGIC_1);
                u_rld.write (SC_LOGIC_1);               
                exe_state = EXE_BR1;                   
                    break;
                
                case EXE_BR1:
		        // f ((t += u) < u)
	            //printf("EXE: BR1\n");
                t_mux_sel.write(SC_LOGIC_1);
                t_rld.write(SC_LOGIC_1);                    		        
                if(LT1.read() == SC_LOGIC_1) exe_state = EXE_BR1_LT;
                else exe_state = EXE_BR2;
                    break;

                case EXE_BR1_LT:
                // a[1] += TO_HIGH_HALF(1);
                //printf("EXE: BR1_LT\n");
                a1_mux_sel.write(1);
                a1_rld.write(SC_LOGIC_1); 
                exe_state = EXE_BR2;
                    break;
                    
                case EXE_BR2:
                // u = TO_HIGH_HALF(t);
                // if ((a[0] += u) < u)
                //printf("EXE: BR2\n");
                u_mux_sel.write(SC_LOGIC_1); 
                u_rld.write (SC_LOGIC_1);
                a0_mux_sel.write(SC_LOGIC_1);
                a0_rld.write(SC_LOGIC_1);
                if(LT2.read() == SC_LOGIC_1) exe_state = EXE_BR2_LT;
                else exe_state = EXE_OUT;
                    break;

                case EXE_BR2_LT:
                // a[1]++;
                //printf("EXE: BR2_LT\n");
                a1_mux_sel.write(2);
                a1_rld.write(SC_LOGIC_1); 
                exe_state = EXE_OUT;
                    break;
                    
                case EXE_OUT:
                // a[1] += HIGH_HALF(t);
                //printf("EXE: OUTPUT\n");
                a1_mux_sel.write(3);  
                a1_rld.write(SC_LOGIC_1);
                exe_state = EXE_WAIT;         
                    break;
                
                case EXE_WAIT:
                //printf("EXE: WAIT\n");
                hs_state = OUTPUT;
                    break;
                    
                default:
                    break;
            } // switch exe_state
            
            hs_state = OUTPUT;
	        break;
        }

		case OUTPUT:
		{
			//printf("FSM: output\n");
			// Write outputs
			//out_data_low.write(a[0]);
			//out_data_high.write(a[1]);
			
			//out_data_low.write(DP.A0_OUT.read());
			//out_data_high.write(DP.A1_OUT.read());

			hw_mult_done.write(true);
			hs_state = FINISH;
			break;
		}

		case FINISH:
		{
			//printf("FSM: finish\n");
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
  } // END for	  	  
} // END dh_hw_mult::process_hw_mult()

