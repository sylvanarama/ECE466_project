#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"

#define WAIT 1
#define EXECUTE 2
#define OUTPUT 3
#define FINISH 4

void dh_hw_mult::process_hw_mult()
{
	
  NN_DIGIT a[2], b, c, t, u;
  NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;
  NN_DIGIT state = WAIT;

  for (;;) {  
	
	  switch (state)
	  {
		case WAIT:
		{
			if (hw_mult_enable.read() == true)
				state = EXECUTE;
			break;
		}
		
		case EXECUTE:
		{
			// Read inputs	
			b = in_data_1.read();
			c = in_data_2.read();

			// Original code from NN_DigitMult()...		
			bHigh = (NN_HALF_DIGIT)HIGH_HALF(b);
			bLow = (NN_HALF_DIGIT)LOW_HALF(b);
			cHigh = (NN_HALF_DIGIT)HIGH_HALF(c);
			cLow = (NN_HALF_DIGIT)LOW_HALF(c);

			a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
			t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
			u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
			a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;

			if ((t += u) < u) a[1] += TO_HIGH_HALF(1);
			u = TO_HIGH_HALF(t);

			if ((a[0] += u) < u) a[1]++;
			a[1] += HIGH_HALF(t);

			state = OUTPUT;
			break;
		}

		case OUTPUT:
		{
			// Write outputs
			out_data_low.write(a[0]);
			out_data_high.write(a[1]);

			hw_mult_done.write(true);
			state = FINISH;
			break;
		}

		case FINISH:
		{
			if (hw_mult_enable.read() == false)
			{
				hw_mult_done.write(false);
				state = WAIT;
				break;
			}
		} 

	} // END switch
  } // END for
	  	  
} // END dh_hw_mult::process_hw_mult()

