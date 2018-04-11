module state_control (
    input          CLK,
    input          RST,
    input          RST_PULSE,
    input          INIT,
    input          INIT_PULSE,
    input          START,
    input          START_PULSE,
    input          STOP,
    input          STOP_PULSE,
    input          FRAME_END,
    output   [1:0] STATE,
    output         FIFO_WR_EN,
    output         RST_SIG,       
    output         RST_SIG_PULSE 
);

(* mark_debug = "true" *) reg [1:0] run_status = 2'b00;

// This should be replaced with State Machine. For the moment, just use if simple sentences.
always @( posedge CLK )
begin
    if ( START_PULSE ) 
        run_status <= 2'b10;  // Running
    else if ( STOP_PULSE )
        run_status <= 2'b01;  // Idle
    else if ( INIT_PULSE )
        run_status <= 2'b01;  // Set -> Idle
    else if ( RST_PULSE )
        run_status <= 2'b01;
    else
        run_status <= run_status;
end

// Generate "RST"
reg rst_pulse;
reg rst_command;
always @( posedge CLK )
begin
    rst_pulse <= ( RST_PULSE || START_PULSE || STOP_PULSE) ;
    rst_command <= ( RST || START || STOP ) ;
end

assign RST_SIG = rst_command;
assign RST_SIG_PULSE = rst_pulse;


// Additional Veto after "Start" 
reg [15:0] veto_cnt = 16'h0;
(* mark_debug = "true" *) reg fifo_write_veto_r = 1'b1;
always @( posedge CLK )
begin
    if( START ) begin
        fifo_write_veto_r <= 1'b1;
        veto_cnt <= 16'h0;
    end
    else if ( veto_cnt < 16'd4000 ) begin  // Wait 4000 clock to relase the write veto
        fifo_write_veto_r <= 1'b1;
        veto_cnt <= veto_cnt + 1'h1;
    end
    else if (veto_cnt == 16'd4000 ) begin  
        fifo_write_veto_r <= 1'b0;
        veto_cnt <= veto_cnt + 1'h1;
    end
    else begin
        fifo_write_veto_r <= fifo_write_veto_r;
        veto_cnt <= veto_cnt;
    end
end

// Additional Veto after "Stop" 
//always @( posedge CLK )
//begin
//    if( STOP ) 
//        fifo_write_veto_r <= 1'b1;
//end


(* mark_debug = "true" *) reg fifo_wr_enable = 1'b0;
always @( posedge CLK )
begin
    case ( run_status ) 
        2'b10   :   
        begin
            if( fifo_write_veto_r == 1'b0 )
                fifo_wr_enable <= 1'b1;
            else
                fifo_wr_enable <= 1'b0;
        end
        2'b01   :   fifo_wr_enable <= 1'b0;
        default: fifo_wr_enable <= 1'b0;
    endcase
end


(* mark_debug = "true" *) reg wr_en = 1'b0;
always @( posedge CLK )
begin
    if ( run_status == 2'b10 )
        if ( FRAME_END ) 
            wr_en <= fifo_wr_enable;
        else
            wr_en <= wr_en;
    else
        wr_en <= fifo_wr_enable;
end

assign FIFO_WR_EN = wr_en;
assign STATE = run_status;


endmodule

