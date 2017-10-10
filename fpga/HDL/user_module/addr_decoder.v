module addr_decoder (
    input CLK,
    input [4:0] ADDR,
    output RST
);
 
reg reset;
reg reset_flag_on;

always @ ( posedge CLK)
begin
    if ( ADDR == 5'b01000 ) 
    begin
        if ( reset_flag_on != 1 ) 
        begin
            reset <= 1;
            reset_flag_on <= 1;
        end
        else begin
            reset <= 0;
            reset_flag_on <= 1;
        end
   end                    
   else 
   begin
        reset <= 0;
        reset_flag_on <= 0;
   end
end

assign RST = reset;

endmodule
