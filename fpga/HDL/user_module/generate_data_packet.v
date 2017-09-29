module generate_data_packet (
    input          CLK,          
    input          RST,          
    output [31:0]  DATA_OUT 
);

// Counter (Header + 24words + footer) = 26 clocks 
reg [7:0] cnt = 0;
always @( posedge CLK )
begin
    if ( RST )
        cnt <= 8'h0;
    else if ( cnt == 8'd25 )
        cnt <= 8'h0;
    else
        cnt <= cnt + 1'h1;
end

// Generate Data Set
//reg  [31:0] data = 32'b0;
//(* keep = "true" *) reg  [15:0] tst_data_row1  = 16'h0000;
//(* keep = "true" *) reg  [15:0] tst_data_row2  = 16'h0000;
//always @( posedge CLK )
//begin
//    if ( cnt == 8'd0 )  begin   // header
//        data <= 32'haaaaaaaa;   
//        tst_data_row1 <= 16'h0000;
//        tst_data_row2 <= 16'h0001;
//    end    
//    else if ( cnt == 8'd25 )   // footer
//        data <= 32'hf0f0f0f0;
//    else begin
//        data <= { tst_data_row2, tst_data_row1 };
//        tst_data_row1 <= tst_data_row1 + 16'h0002;
//        tst_data_row2 <= tst_data_row2 + 16'h0002;
//    end
//end

(* keep = "true" *) reg  [31:0] data = 32'h0;
always @( posedge CLK )
begin
    case ( cnt )
        8'd0 :  data <= 32'haaaaaaaa;
        8'd1 :  data <= 32'h00010000;
        8'd2 :  data <= 32'h00030002;        
        8'd3 :  data <= 32'h00050004;
        8'd4 :  data <= 32'h00070006;
        8'd5 :  data <= 32'h00090008;
        8'd6 :  data <= 32'h000b000a;
        8'd7 :  data <= 32'h000d000c;          
        8'd8 :  data <= 32'h000f000e;
        8'd9 :  data <= 32'h00110010;
        8'd10 : data <= 32'h00130012;
        8'd11 : data <= 32'h00150014;          
        8'd12 : data <= 32'h00170016;
        8'd13 : data <= 32'h00190018;
        8'd14 : data <= 32'h001b001a;
        8'd15 : data <= 32'h001d001c;          
        8'd16 : data <= 32'h001f001e;
        8'd17 : data <= 32'h00210020;
        8'd18 : data <= 32'h00230022;
        8'd19 : data <= 32'h00250024;      
        8'd20 : data <= 32'h00270026;
        8'd21 : data <= 32'h00290028;
        8'd22 : data <= 32'h002b002a;
        8'd23 : data <= 32'h002d002c;          
        8'd24 : data <= 32'h002f002e;
        8'd25 : data <= 32'hf0f0f0f0;     
        default : data <= 32'h0;
    endcase
end

assign DATA_OUT = data;   

endmodule
