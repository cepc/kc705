module recieve_adc_packet (
    input          CLK,     
//    input          CLK2M,    
    input          CLK2M_IN, 
    input          CLK2M_DIV,
//    input          CNVCLK,
    input          CNVCLK_IN,
    input          SR_OUT,
    input          RST,  
    //input          RST2M,
    input          RST2M_IN,
    input          SET_PARAM,
    input   [5:0]  ROW_START,
    input   [5:0]  ROW_END,
    input   [3:0]  COL_START,
    input   [3:0]  COL_END,
    input  [15:0]  DATA01,
    input  [15:0]  DATA02,
    input  [15:0]  DATA03,
    input  [15:0]  DATA04,
    input  [15:0]  DATA05,
    input  [15:0]  DATA06,
    input  [15:0]  DATA07,
    input  [15:0]  DATA08,
    input  [15:0]  DATA09,
    input  [15:0]  DATA10,
    input  [15:0]  DATA11,
    input  [15:0]  DATA12,
    input  [15:0]  DATA13,
    input  [15:0]  DATA14,
    input  [15:0]  DATA15,
    input  [15:0]  DATA16,
    output [31:0]  DATA_OUT,
    output         FRAME_END_FLAG,
    output         WR_EN 
);

(* MARK_DEBUG="true" *)wire RST2M;
(* MARK_DEBUG="true" *)wire CLK2M;
(* MARK_DEBUG="true" *)wire CNVCLK;
regenerate_ctrl_signal regen_ctrl_sig (
    .CLK_IN     (  CLK       ),
    .CLK2M_IN   (  CLK2M_IN  ),
    .RST_IN     (  RST       ),
    .CNVCLK_IN  (  CNVCLK_IN ),
    .NSTEP1     (  ROW_START ),  // Very temporary !
    .NSTEP2     (  ROW_END   ),  // Very temporary !
    .CLK2M_OUT  (  CLK2M     ),
    .RST2M_OUT  (  RST2M     ),
    .CNVCLK_OUT (  CNVCLK    )
);
 
wire [15:0] sample_data01, sample_data02, sample_data03, sample_data04;
(* mark_debug = "true" *) wire [15:0] sample_data05, sample_data06, sample_data07, sample_data08;
wire [15:0] sample_data09, sample_data10, sample_data11, sample_data12;
wire [15:0] sample_data13, sample_data14, sample_data15, sample_data16;

wire sample_sr_out01, sample_sr_out02, sample_sr_out03, sample_sr_out04; 
wire sample_sr_out05, sample_sr_out06, sample_sr_out07, sample_sr_out08; 
wire sample_sr_out09, sample_sr_out10, sample_sr_out11, sample_sr_out12; 
wire sample_sr_out13, sample_sr_out14, sample_sr_out15, sample_sr_out16; 

rec_sample_data adc_data_inst1( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out01 ), .DATA_IN( DATA01 ), .DATA_OUT( sample_data01 ) );
rec_sample_data adc_data_inst2( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out02 ), .DATA_IN( DATA02 ), .DATA_OUT( sample_data02 ) );
rec_sample_data adc_data_inst3( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out03 ), .DATA_IN( DATA03 ), .DATA_OUT( sample_data03 ) );
rec_sample_data adc_data_inst4( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out04 ), .DATA_IN( DATA04 ), .DATA_OUT( sample_data04 ) );
    
rec_sample_data adc_data_inst5( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out05 ), .DATA_IN( DATA05 ), .DATA_OUT( sample_data05 ) );
rec_sample_data adc_data_inst6( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out06 ), .DATA_IN( DATA06 ), .DATA_OUT( sample_data06 ) );
rec_sample_data adc_data_inst7( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out07 ), .DATA_IN( DATA07 ), .DATA_OUT( sample_data07 ) );
rec_sample_data adc_data_inst8( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out08 ), .DATA_IN( DATA08 ), .DATA_OUT( sample_data08 ) );
    
rec_sample_data adc_data_inst9( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out09 ), .DATA_IN( DATA09 ), .DATA_OUT( sample_data09 ) );
rec_sample_data adc_data_inst10( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out10 ), .DATA_IN( DATA10 ), .DATA_OUT( sample_data10 ) );
rec_sample_data adc_data_inst11( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out11 ), .DATA_IN( DATA11 ), .DATA_OUT( sample_data11 ) );
rec_sample_data adc_data_inst12( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out12 ), .DATA_IN( DATA12 ), .DATA_OUT( sample_data12 ) );
    
rec_sample_data adc_data_inst13( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out13 ), .DATA_IN( DATA13 ), .DATA_OUT( sample_data13 ) );
rec_sample_data adc_data_inst14( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out14 ), .DATA_IN( DATA14 ), .DATA_OUT( sample_data14 ) );
rec_sample_data adc_data_inst15( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out15 ), .DATA_IN( DATA15 ), .DATA_OUT( sample_data15 ) );
rec_sample_data adc_data_inst16( .CLK( CLK2M ), .CNVCLK( CNVCLK ), .RST( RST ), .SR_OUT_IN( SR_OUT ), 
    .SR_OUT_OUT( sample_sr_out16 ), .DATA_IN( DATA16 ), .DATA_OUT( sample_data16 ) );
    
    
// Net for Dummy Data Set !
wire  [15:0] dummy_data_header, dummy_data_footer;
wire  [15:0] dummy_data01, dummy_data02, dummy_data03, dummy_data04;
wire  [15:0] dummy_data05, dummy_data06, dummy_data07, dummy_data08;
wire  [15:0] dummy_data09, dummy_data10, dummy_data11, dummy_data12;
wire  [15:0] dummy_data13, dummy_data14, dummy_data15, dummy_data16;
    
wire  dummy_sr_out_header, dummy_sr_out_footer;
wire  dummy_sr_out01, dummy_sr_out02, dummy_sr_out03, dummy_sr_out04;
wire  dummy_sr_out05, dummy_sr_out06, dummy_sr_out07, dummy_sr_out08;
wire  dummy_sr_out09, dummy_sr_out10, dummy_sr_out11, dummy_sr_out12;
wire  dummy_sr_out13, dummy_sr_out14, dummy_sr_out15, dummy_sr_out16;
    
// Generate Dummy Data -- Header & Footer
gen_simple_data   dummy_data_inst0( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data_header ), .SR_OUT( dummy_sr_out_header ) );  
gen_simple_data   dummy_data_inst17( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data_footer ), .SR_OUT( dummy_sr_out_footer ) );     
    
// Generate Dummy Data -- Signal (16 ch)
gen_simple_data   dummy_data_inst1( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data01 ), .SR_OUT( dummy_sr_out01 ) );
gen_simple_data   dummy_data_inst2( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data02 ), .SR_OUT( dummy_sr_out02 ) );
gen_simple_data   dummy_data_inst3( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data03 ), .SR_OUT( dummy_sr_out03 ) );
gen_simple_data   dummy_data_inst4( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data04 ), .SR_OUT( dummy_sr_out04 ) );
     
gen_simple_data   dummy_data_inst5( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data05 ), .SR_OUT( dummy_sr_out05 ) );
gen_simple_data   dummy_data_inst6( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data06 ), .SR_OUT( dummy_sr_out06 ) );
gen_simple_data   dummy_data_inst7( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data07 ), .SR_OUT( dummy_sr_out07 ) );
gen_simple_data   dummy_data_inst8( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data08 ), .SR_OUT( dummy_sr_out08 ) );
    
gen_simple_data   dummy_data_inst9( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data09 ), .SR_OUT( dummy_sr_out09 ) );
gen_simple_data   dummy_data_inst10( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data10 ), .SR_OUT( dummy_sr_out10 ) );
gen_simple_data   dummy_data_inst11( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data11 ), .SR_OUT( dummy_sr_out11 ) );
gen_simple_data   dummy_data_inst12( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data12 ), .SR_OUT( dummy_sr_out12 ) );
    
gen_simple_data   dummy_data_inst13( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data13 ), .SR_OUT( dummy_sr_out13 ) );
gen_simple_data   dummy_data_inst14( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data14 ), .SR_OUT( dummy_sr_out14 ) );
gen_simple_data   dummy_data_inst15( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data15 ), .SR_OUT( dummy_sr_out15 ) );
gen_simple_data   dummy_data_inst16( .CLK ( CLK2M ), .RST ( RST2M ), .DATA_OUT( dummy_data16 ), .SR_OUT( dummy_sr_out16 ) );
    
    
wire  [15:0] d_header, d_footer;
(* mark_debug = "true" *) wire  [15:0] d01, d02, d03, d04;
(* mark_debug = "true" *) wire  [15:0] d05, d06, d07, d08;
wire  [15:0] d09, d10, d11, d12, d13, d14, d15, d16;
    
wire   [5:0] a_header, a_footer;
wire   [5:0] a01, a02, a03, a04, a05, a06, a07, a08; 
wire   [5:0] a09, a10, a11, a12, a13, a14, a15, a16;
    
wire         wren_header, wren_footer;
wire         wren01, wren02, wren03, wren04, wren05, wren06, wren07, wren08; 
wire         wren09, wren10, wren11, wren12, wren13, wren14, wren15, wren16;
    
wire        flag_header, flag_footer;
(* mark_debug = "true" *) wire        flag01, flag02, flag03, flag04, flag05, flag06, flag07, flag08;
(* mark_debug = "true" *) wire        flag09, flag10, flag11, flag12, flag13, flag14, flag15, flag16;
    
reg   [1:0] data_type_header = 2'b01;
reg   [1:0] data_type_signal = 2'b10;
reg   [1:0] data_type_footer = 2'b11;
            
            
// DATA PATH -- Header/Footer 
set_data  set_data_inst0(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_header ), .DATA_IN( dummy_data_header ), 
    .SR_OUT_IN( dummy_sr_out_header ), .FIRST_ROW_FLAG( flag_header ),  .DATA_OUT( d_header ), .MEM_ADDR( a_header ), .MEM_WREN( wren_header ) ); 
set_data  set_data_inst17(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_footer ), .DATA_IN( dummy_data_footer ), 
    .SR_OUT_IN( dummy_sr_out_footer ), .FIRST_ROW_FLAG( flag_footer ),  .DATA_OUT( d_footer ), .MEM_ADDR( a_footer ), .MEM_WREN( wren_footer ) ); 
                  
           
// DATA PATH -- Signal (16 ch)
//set_data  set_data_inst01(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data01 ), 
//    .SR_OUT_IN( sample_sr_out01 ), .FIRST_ROW_FLAG( flag01 ),  .DATA_OUT( d01 ), .MEM_ADDR( a01 ), .MEM_WREN( wren01 ) ); 
//set_data  set_data_inst02(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data02 ), 
//    .SR_OUT_IN( sample_sr_out02 ), .FIRST_ROW_FLAG( flag02 ),  .DATA_OUT( d02 ), .MEM_ADDR( a02 ), .MEM_WREN( wren02 ) );
//set_data  set_data_inst03(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data03 ), 
//    .SR_OUT_IN( sample_sr_out03 ), .FIRST_ROW_FLAG( flag03 ),  .DATA_OUT( d03 ), .MEM_ADDR( a03 ), .MEM_WREN( wren03 ) );    
//set_data  set_data_inst04(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data04 ), 
//    .SR_OUT_IN( sample_sr_out04 ), .FIRST_ROW_FLAG( flag04 ),  .DATA_OUT( d04 ), .MEM_ADDR( a04 ), .MEM_WREN( wren04 ) ); 
        
set_data  set_data_inst05(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data05 ), 
    .SR_OUT_IN( sample_sr_out05 ), .FIRST_ROW_FLAG( flag05 ),  .DATA_OUT( d05 ), .MEM_ADDR( a05 ), .MEM_WREN( wren05 ) ); 
set_data  set_data_inst06(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data06 ), 
    .SR_OUT_IN( sample_sr_out06 ), .FIRST_ROW_FLAG( flag06 ),  .DATA_OUT( d06 ), .MEM_ADDR( a06 ), .MEM_WREN( wren06 ) );
set_data  set_data_inst07(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data07 ), 
    .SR_OUT_IN( sample_sr_out07 ), .FIRST_ROW_FLAG( flag07 ),  .DATA_OUT( d07 ), .MEM_ADDR( a07 ), .MEM_WREN( wren07 ) );    
set_data  set_data_inst08(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( sample_data08 ), 
    .SR_OUT_IN( sample_sr_out08 ), .FIRST_ROW_FLAG( flag08 ),  .DATA_OUT( d08 ), .MEM_ADDR( a08 ), .MEM_WREN( wren08 ) ); 
            
set_data  set_data_inst01(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data01 ), 
    .SR_OUT_IN( dummy_sr_out01 ), .FIRST_ROW_FLAG( flag01 ),  .DATA_OUT( d01 ), .MEM_ADDR( a01 ), .MEM_WREN( wren01 ) ); 
set_data  set_data_inst02(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data02 ), 
    .SR_OUT_IN( dummy_sr_out02 ), .FIRST_ROW_FLAG( flag02 ),  .DATA_OUT( d02 ), .MEM_ADDR( a02 ), .MEM_WREN( wren02 ) );
set_data  set_data_inst03(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data03 ), 
    .SR_OUT_IN( dummy_sr_out03 ), .FIRST_ROW_FLAG( flag03 ),  .DATA_OUT( d03 ), .MEM_ADDR( a03 ), .MEM_WREN( wren03 ) );    
set_data  set_data_inst04(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data04 ), 
    .SR_OUT_IN( dummy_sr_out04 ), .FIRST_ROW_FLAG( flag04 ),  .DATA_OUT( d04 ), .MEM_ADDR( a04 ), .MEM_WREN( wren04 ) ); 
    
//set_data  set_data_inst05(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data05 ), 
//    .SR_OUT_IN( dummy_sr_out05 ), .FIRST_ROW_FLAG( flag05 ),  .DATA_OUT( d05 ), .MEM_ADDR( a05 ), .MEM_WREN( wren05 ) ); 
//set_data  set_data_inst06(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data06 ), 
//    .SR_OUT_IN( dummy_sr_out06 ), .FIRST_ROW_FLAG( flag06 ),  .DATA_OUT( d06 ), .MEM_ADDR( a06 ), .MEM_WREN( wren06 ) );
//set_data  set_data_inst07(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data07 ), 
//    .SR_OUT_IN( dummy_sr_out07 ), .FIRST_ROW_FLAG( flag07 ),  .DATA_OUT( d07 ), .MEM_ADDR( a07 ), .MEM_WREN( wren07 ) );    
//set_data  set_data_inst08(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data08 ), 
//    .SR_OUT_IN( dummy_sr_out08 ), .FIRST_ROW_FLAG( flag08 ),  .DATA_OUT( d08 ), .MEM_ADDR( a08 ), .MEM_WREN( wren08 ) ); 
          
set_data  set_data_inst09(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data09 ), 
    .SR_OUT_IN( dummy_sr_out09 ), .FIRST_ROW_FLAG( flag09 ),  .DATA_OUT( d09 ), .MEM_ADDR( a09 ), .MEM_WREN( wren09 ) ); 
set_data  set_data_inst10(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data10 ), 
    .SR_OUT_IN( dummy_sr_out10 ), .FIRST_ROW_FLAG( flag10 ),  .DATA_OUT( d10 ), .MEM_ADDR( a10 ), .MEM_WREN( wren10 ) );
set_data  set_data_inst11(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data11 ), 
    .SR_OUT_IN( dummy_sr_out11 ), .FIRST_ROW_FLAG( flag11 ),  .DATA_OUT( d11 ), .MEM_ADDR( a11 ), .MEM_WREN( wren11 ) );    
set_data  set_data_inst12(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data12 ), 
    .SR_OUT_IN( dummy_sr_out12 ), .FIRST_ROW_FLAG( flag12 ),  .DATA_OUT( d12 ), .MEM_ADDR( a12 ), .MEM_WREN( wren12 ) ); 
                
set_data  set_data_inst13(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data13 ), 
    .SR_OUT_IN( dummy_sr_out13 ), .FIRST_ROW_FLAG( flag13 ),  .DATA_OUT( d13 ), .MEM_ADDR( a13 ), .MEM_WREN( wren13 ) ); 
set_data  set_data_inst14(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data14 ), 
    .SR_OUT_IN( dummy_sr_out14 ), .FIRST_ROW_FLAG( flag14 ),  .DATA_OUT( d14 ), .MEM_ADDR( a14 ), .MEM_WREN( wren14 ) );
set_data  set_data_inst15(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data15 ), 
    .SR_OUT_IN( dummy_sr_out15 ), .FIRST_ROW_FLAG( flag15 ),  .DATA_OUT( d15 ), .MEM_ADDR( a15 ), .MEM_WREN( wren15 ) );    
set_data  set_data_inst16(  .CLK( CLK2M ), .RST( RST2M ), .CH_ID( data_type_signal ), .DATA_IN( dummy_data16 ), 
    .SR_OUT_IN( dummy_sr_out16 ), .FIRST_ROW_FLAG( flag16 ),  .DATA_OUT( d16 ), .MEM_ADDR( a16 ), .MEM_WREN( wren16 ) ); 
                                           
    
    
    
// Data Storage Buffer (TDP Memory :: 16bit/48depth )
wire  [15:0] d_header_out, d_footer_out;
(* mark_debug = "true" *) wire  [15:0] d01_out, d02_out, d03_out, d04_out;
(* mark_debug = "true" *) wire  [15:0] d05_out, d06_out, d07_out, d08_out;
wire  [15:0] d09_out, d10_out, d11_out, d12_out, d13_out, d14_out, d15_out, d16_out;
    
wire   [5:0] mem_addr_read;
wire   [5:0] mem_addr_header, mem_addr_footer;
wire   [5:0] mem_addr01, mem_addr02, mem_addr03, mem_addr04, mem_addr05, mem_addr06, mem_addr07, mem_addr08;
wire   [5:0] mem_addr09, mem_addr10, mem_addr11, mem_addr12, mem_addr13, mem_addr14, mem_addr15, mem_addr16;
    
assign mem_addr_header = mem_addr_read;
assign mem_addr_footer = mem_addr_read;
    
assign mem_addr01 = mem_addr_read;
assign mem_addr02 = mem_addr_read;
assign mem_addr03 = mem_addr_read;
assign mem_addr04 = mem_addr_read;
assign mem_addr05 = mem_addr_read;
assign mem_addr06 = mem_addr_read;
assign mem_addr07 = mem_addr_read;
assign mem_addr08 = mem_addr_read;
    
assign mem_addr09 = mem_addr_read;
assign mem_addr10 = mem_addr_read;
assign mem_addr11 = mem_addr_read;
assign mem_addr12 = mem_addr_read;
assign mem_addr13 = mem_addr_read;
assign mem_addr14 = mem_addr_read;
assign mem_addr15 = mem_addr_read;
assign mem_addr16 = mem_addr_read;
    
    
reg [15:0] dinb_header, dinb_footer;
reg [15:0] dinb01, dinb02, dinb03, dinb04, dinb05, dinb06, dinb07, dinb08;
reg [15:0] dinb09, dinb10, dinb11, dinb12, dinb13, dinb14, dinb15, dinb16;
    
reg web_header, web_footer;
reg web01, web02, web03, web04, web05, web06, web07, web08;
reg web09, web10, web11, web12, web13, web14, web15, web16;
    
always @( posedge CLK )
begin
    dinb_header <= 16'h0;   dinb_footer <= 16'h0;
    dinb01 <= 16'h0;  dinb02 <= 16'h0;  dinb03 <= 16'h0;  dinb04 <= 16'h0;
    dinb05 <= 16'h0;  dinb06 <= 16'h0;  dinb07 <= 16'h0;  dinb08 <= 16'h0;
    dinb09 <= 16'h0;  dinb10 <= 16'h0;  dinb11 <= 16'h0;  dinb12 <= 16'h0;
    dinb13 <= 16'h0;  dinb14 <= 16'h0;  dinb15 <= 16'h0;  dinb16 <= 16'h0;
    
    web_header <= 1'h0;  web_footer <= 1'h0;
    web01 <= 16'h0;  web02 <= 16'h0;  web03 <= 16'h0;  web04 <= 16'h0;
    web05 <= 16'h0;  web06 <= 16'h0;  web07 <= 16'h0;  web08 <= 16'h0;
    web09 <= 16'h0;  web10 <= 16'h0;  web11 <= 16'h0;  web12 <= 16'h0;
    web13 <= 16'h0;  web14 <= 16'h0;  web15 <= 16'h0;  web16 <= 16'h0;
end
    
    
coregen_buffer_mem16 buffer_mem16_inst0 (
    .clka( CLK2M ), .addra( a_header ), .dina( d_header ), .wea( wren_header ), .doutb( d_header_out), .clkb( CLK ), .addrb( mem_addr_header ), .dinb( dinb_header ), .web( web_header ) );
coregen_buffer_mem16 buffer_mem16_inst17 (
    .clka( CLK2M ), .addra( a_footer ), .dina( d_footer ), .wea( wren_footer ), .doutb( d_footer_out), .clkb( CLK ), .addrb( mem_addr_footer ), .dinb( dinb_footer ), .web( web_footer ) );
      
coregen_buffer_mem16 buffer_mem16_inst1 (
    .clka( CLK2M ), .addra( a01 ), .dina( d01 ), .wea( wren01 ), .doutb( d01_out), .clkb( CLK ), .addrb( mem_addr01 ), .dinb( dinb01 ), .web( web01 ) );
coregen_buffer_mem16 buffer_mem16_inst2 (
    .clka( CLK2M ), .addra( a02 ), .dina( d02 ), .wea( wren02 ), .doutb( d02_out), .clkb( CLK ), .addrb( mem_addr02 ), .dinb( dinb02 ), .web( web02 )  );
coregen_buffer_mem16 buffer_mem16_inst3 (
    .clka( CLK2M ), .addra( a03 ), .dina( d03 ), .wea( wren03 ), .doutb( d03_out), .clkb( CLK ), .addrb( mem_addr03 ), .dinb( dinb03 ), .web( web03 )  );
coregen_buffer_mem16 buffer_mem16_inst4 (
    .clka( CLK2M ), .addra( a04 ), .dina( d04 ), .wea( wren04 ), .doutb( d04_out), .clkb( CLK ), .addrb( mem_addr04 ), .dinb( dinb04 ), .web( web04 )  );
coregen_buffer_mem16 buffer_mem16_inst5 (
    .clka( CLK2M ), .addra( a05 ), .dina( d05 ), .wea( wren05 ), .doutb( d05_out), .clkb( CLK ), .addrb( mem_addr05 ), .dinb( dinb05 ), .web( web05 )  );
coregen_buffer_mem16 buffer_mem16_inst6 (
    .clka( CLK2M ), .addra( a06 ), .dina( d06 ), .wea( wren06 ), .doutb( d06_out), .clkb( CLK ), .addrb( mem_addr06 ), .dinb( dinb06 ), .web( web06 )  );
coregen_buffer_mem16 buffer_mem16_inst7 (
    .clka( CLK2M ), .addra( a07 ), .dina( d07 ), .wea( wren07 ), .doutb( d07_out), .clkb( CLK ), .addrb( mem_addr07 ), .dinb( dinb07 ), .web( web07 )  );
coregen_buffer_mem16 buffer_mem16_inst8 (
    .clka( CLK2M ), .addra( a08 ), .dina( d08 ), .wea( wren08 ), .doutb( d08_out), .clkb( CLK ), .addrb( mem_addr08 ), .dinb( dinb08 ), .web( web08 )  );
      
coregen_buffer_mem16 buffer_mem16_inst9 (
    .clka( CLK2M ), .addra( a09 ), .dina( d09 ), .wea( wren09 ), .doutb( d09_out), .clkb( CLK ), .addrb( mem_addr09 ), .dinb( dinb09 ), .web( web09 )  );
coregen_buffer_mem16 buffer_mem16_inst10 (
    .clka( CLK2M ), .addra( a10 ), .dina( d10 ), .wea( wren10 ), .doutb( d10_out), .clkb( CLK ), .addrb( mem_addr10 ), .dinb( dinb10 ), .web( web10 )  );
coregen_buffer_mem16 buffer_mem16_inst11 (
    .clka( CLK2M ), .addra( a11 ), .dina( d11 ), .wea( wren11 ), .doutb( d11_out), .clkb( CLK ), .addrb( mem_addr11 ), .dinb( dinb11 ), .web( web11 )  );
coregen_buffer_mem16 buffer_mem16_inst12 (
    .clka( CLK2M ), .addra( a12 ), .dina( d12 ), .wea( wren12 ), .doutb( d12_out), .clkb( CLK ), .addrb( mem_addr12 ), .dinb( dinb12 ), .web( web12 )  );
coregen_buffer_mem16 buffer_mem16_inst13 (
    .clka( CLK2M ), .addra( a13 ), .dina( d13 ), .wea( wren13 ), .doutb( d13_out), .clkb( CLK ), .addrb( mem_addr13 ), .dinb( dinb13 ), .web( web13 )  );
coregen_buffer_mem16 buffer_mem16_inst14 (
    .clka( CLK2M ), .addra( a14 ), .dina( d14 ), .wea( wren14 ), .doutb( d14_out), .clkb( CLK ), .addrb( mem_addr14 ), .dinb( dinb14 ), .web( web14 )  );
coregen_buffer_mem16 buffer_mem16_inst15 (
    .clka( CLK2M ), .addra( a15 ), .dina( d15 ), .wea( wren15 ), .doutb( d15_out), .clkb( CLK ), .addrb( mem_addr15 ), .dinb( dinb15 ), .web( web15 )  );
coregen_buffer_mem16 buffer_mem16_inst16 (
    .clka( CLK2M ), .addra( a16 ), .dina( d16 ), .wea( wren16 ), .doutb( d16_out), .clkb( CLK ), .addrb( mem_addr16 ), .dinb( dinb16 ), .web( web16 )  );
    
    
   

// Control System 
wire [31:0] sel_data_out;
wire        sel_fifo_wren;
wire SET = 1'b0;
wire        frame_end;
//data_selector #( .ROW_START(6'd0), .ROW_END(6'd47), .COLUMN_START(4'd0), .COLUMN_END(4'd15) )
data_selector data_selector_inst ( 
    .CLK            (  CLK           ), 
    .RST            (  RST           ), 
    .ROW_START      (  ROW_START     ),
    .ROW_END        (  ROW_END       ),
    .COL_START      (  COL_START     ),
    .COL_END        (  COL_END       ),
    .SET_PARAM      (  SET           ),    // No SET !!
//    .SET_PARAM      (  SET_PARAM     ),
    .DATA_H         (  d_header_out  ),  
    .DATA_F         (  d_footer_out  ),  
    .DATA01         (  d01_out       ), 
    .DATA02         (  d02_out       ), 
    .DATA03         (  d03_out       ), 
    .DATA04         (  d04_out       ),    
    .DATA05         (  d05_out       ), 
    .DATA06         (  d06_out       ), 
    .DATA07         (  d07_out       ), 
    .DATA08         (  d08_out       ),
    .DATA09         (  d09_out       ), 
    .DATA10         (  d10_out       ), 
    .DATA11         (  d11_out       ), 
    .DATA12         (  d12_out       ),
    .DATA13         (  d13_out       ), 
    .DATA14         (  d14_out       ), 
    .DATA15         (  d15_out       ), 
    .DATA16         (  d16_out       ),
//    .MEM_RD_FLAG    (  flag01        ),
    .MEM_RD_FLAG    (  flag05        ),    // Change Memory Read Timing Flag to 05 ! 02/07/2018 night
    .MEM_ADDR_OUT   (  mem_addr_read ), 
    .DATA_OUT       (  sel_data_out  ),
    .FRAME_END_FLAG (  frame_end     ),
    .FIFO_WR_EN     (  sel_fifo_wren )
);
    
assign DATA_OUT  = sel_data_out;
assign WR_EN     = sel_fifo_wren;
assign FRAME_END_FLAG = frame_end;


endmodule


    