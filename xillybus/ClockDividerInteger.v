//*******************************************************//
//           Random Integer Clock Divider                //
//*******************************************************//
// Function ：
//   * Divide the input clock by the factor F_DIV
//   * The input clock is AHz
//   * The output clock is (A/F_DIV)Hz
// Parameters : 
//   * Input: F_DIV, the divider parameters, F_DIV<2^(F_DIV_WIDTH)
//   * Input: F_DIV_WIDTH, the bit numbers of the F_DIV
//   * Input: clock_i, the input original clock
//   * Input: reset, the reset signal
//   * output : clock_o, the output clock after being divided
// Description :
//   * If F_DIV is even, the duty of clock_o is 50%
//   * If F_DIV is odd, the duty of clock_o is determined by the duty of
//     clock_i and the F_DIV. When the duty of clock_i is 50%, the duty
//     of clock_o is also 50%.
//   * To change the divider factor, change F_DIV and F_DIV_WIDTH
//--------------------------------------------------------
// Timing diagram of the three times clock divider :
//               1    2   3    4    5   6
// clock_i     |---|__|---|__|---|__|---|__|---|__|---|__|
// clock_o_p   |____|--------------|____|---------------| 
// clock_o_n   ---|____|---------------|____|-----------
// clk_o       |______|-----------|_______|----------|
// 
//*******************************************************//

module  ClockDividerInteger(clock_o, clock_i, reset, F_DIV);

// Input and output
output clock_o     ; // Output clock
input	 clock_i     ; // Input clock
input  reset       ; // Input reset
input  F_DIV       ; // Input divider factor

parameter F_DIV_WIDTH = 32; // Input divider factor's bit number 

// internal register
reg [(F_DIV_WIDTH-1) : 0] count_p   ; // Counter of the posedege of clock_i
reg [(F_DIV_WIDTH-1) : 0] count_n   ; // Counter of the negedege of clock_i
reg	                      clock_o_p ; // Triggered clock of the posedege of clock_i
reg                       clock_o_n ; // Triggered clock of the negedege of clock_i

// Clock divider for the posedge of clock_i
always @(posedge clock_i or posedge reset)
  begin
    if (reset)
      begin
        count_p   <= 0;
        clock_o_p <= 1'b0;
      end
    else
      begin
        if (count_p == ((F_DIV>>1)-1)) 
          clock_o_p <= ~clock_o_p;
        else if (count_p == (F_DIV-1))
          begin
            clock_o_p <= ~clock_o_p;
            count_p <= 0;
          end
        else
          count_p <= count_p + 1;
      end
  end

// Clock divider for the negedge of clock_i
always @(negedge clock_i or posedge reset)
  begin
    if (reset)
      begin
        count_n   <= 0;
        clock_o_n <= 1'b0;
      end
    else
      begin
        if (count_n == ((F_DIV>>1)-1))
          clock_o_n <= ~clock_o_n;
        else if (count_n == (F_DIV-1))
          begin
            clock_o_n <= ~clock_o_n;
            count_n <= 0;
          end
        else
          count_n <= count_n + 1;
      end
  end

// Output clock
assign clock_o = (F_DIV%2) ? (clock_o_p&clock_o_n) : clock_o_p;

endmodule
