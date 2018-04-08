module state_control (
    input          CLK,
    input          RST,
    input          RST_PULSE,
    input          SET,
    input          SET_PULSE,
    input          START,
    input          START_PULSE,
    input          STOP,
    input          STOP_PULSE,
    input          FIFO_EMPTY,
    input          FIFO_PROG_FULL,
    input          FIFO_FULL,
    input          FRAME_END,
    input          BUFFER_SWITCH,
    input          TRIG_PULSE,
    output   [2:0] RUN_STATUS,
    output   [2:0] FIFO_STATUS,
    output         FIFO_WR_EN,
    output         RST_SIG,       
    output         RST_SIG_PULSE 
);


// Moor Type State Machine ::  the output should be a function of the present state only !  //
localparam INIT    = 3'b000;
localparam IDLE    = 3'b001;
localparam STARTUP = 3'b010;
localparam WAIT    = 3'b011;
localparam BUSY    = 3'b100;

localparam SPARE1  = 3'b101;
localparam SPARE2  = 3'b110;
localparam ERROR   = 3'b111;

wire system_ready;

reg [2:0] current_status = INIT;

always @( posedge CLK )
begin
    if ( RST )
        current_status <= INIT;
    else begin
        case ( current_status )
            INIT:
                begin
                    if ( SET_PULSE )
                        current_status <= IDLE;
                    else if ( START_PULSE )       // If DAQ start without any setting. 
                        current_status <= STARTUP;   
                    else
                        current_status <= INIT;
                end
               
            IDLE: 
                begin
                    if ( START_PULSE )
                        current_status <= STARTUP;
                    else
                        current_status <= IDLE;
                end
                
            STARTUP:
                begin
                    if ( system_ready )
                        current_status <= WAIT;
                    else
                        current_status <= STARTUP;
                end
                        
            WAIT:
                begin
                    if ( TRIG_PULSE )                // This part should be modified : a trigger arival does not equal BUSY !
                        current_status <= BUSY;
                    else if ( STOP_PULSE )  
                        current_status <= IDLE;
                    else
                        current_status <= WAIT;
                end
           
            BUSY:
                begin
                    if ( BUFFER_SWITCH )
                        current_status <= WAIT;
                    else if ( STOP_PULSE )  
                        current_status <= IDLE;
                    else
                        current_status <= BUSY;
                end
                
            default: current_status <= IDLE;        // move to "IDLE" status
//            default: current_status <= ERROR;        // Error status. 
                
        endcase
    end
end



// STARTUP Counter -- Waiting 4000 clocks 
reg [15:0] startup_cnt  = 16'h0;
always @( posedge CLK )
begin
    if ( RST )
        startup_cnt <= 16'h0;
    else if ( current_status == STARTUP ) 
        startup_cnt <= startup_cnt + 16'h1;
    else
        startup_cnt <= 16'h0;
end

assign system_ready = ( startup_cnt >= 16'd4000 );


//******************   Generate "RST"   *******************//
reg rst_pulse;
reg rst_command;
always @( posedge CLK )
begin
    rst_pulse <= ( RST_PULSE || START_PULSE || STOP_PULSE) ;
    rst_command <= ( RST || START || STOP ) ;
end

assign RST_SIG = rst_command;
assign RST_SIG_PULSE = rst_pulse;

//****************   FIFO Write Enable Control   ****************//
(* mark_debug = "true" *) reg fifo_wr_enable = 1'b0;
always @( posedge CLK )
begin
    if ( current_status == WAIT ) begin
        if ( FIFO_PROG_FULL )     
            fifo_wr_enable <= 1'b0;    // When FIFO is "almost" FULL. 
        else
            fifo_wr_enable <= 1'b1;
    end
    else
        fifo_wr_enable <= 1'b0;
end

(* mark_debug = "true" *) reg wr_en = 1'b0;
always @( posedge CLK )
begin
    if ( current_status == WAIT ) begin
        if ( FRAME_END )              
                wr_en <= fifo_wr_enable;   // FIFO write enable signal is changed after the end of current frame
        else
                wr_en <= wr_en;
    end
    else
            wr_en <= 1'b0;
end
    
assign FIFO_WR_EN = wr_en;


//****************   Current Status   ****************//

assign RUN_STATUS = current_status;


//****************   FIFO Status   ****************//

assign FIFO_STATUS = { FIFO_FULL, FIFO_PROG_FULL, FIFO_EMPTY };


endmodule

