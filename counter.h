//-----------------------------------------------------
// This is my second Systemc Example
// Design Name : first_counter
// File Name : first_counter.cpp
// Function : This is a 4 bit up-counter with
// Synchronous active high reset and
// with active high enable signal
//-----------------------------------------------------
#include "systemc.h"

SC_MODULE (counter) {
  sc_in_clk   clock ;      // Clock input of the design
  sc_in<bool> sig1;
  sc_in<bool> sig2;

  //------------Local Variables Here---------------------
  int	count;

  //------------Code Starts Here-------------------------
  // Below function implements actual counter logic
  void incr_count () {
    count++;
    printf("Cycle %d: EN = %d, DONE = %d\n", count, sig1.read(), sig2.read());
  } // End of function incr_count

  // Constructor for the counter
  // Since this counter is a positive edge trigged one,
  // We trigger the below block with respect to positive
  // edge of the clock and also when ever reset changes state
  SC_CTOR(counter) {
    // Edge and level sensitive
    SC_METHOD(incr_count);
    sensitive << clock.pos();
    count = 0;
  } // End of Constructor

}; // End of Module counter
