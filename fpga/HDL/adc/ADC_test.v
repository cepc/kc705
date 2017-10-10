`timescale 1ns / 1ps

//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:05:09 07/01/2017 
// Design Name: 
// Module Name:    ADC_test 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module ADC_test(
	input				sys_clk_adc	,// #100MHz USR CLK
	input				rst_n		,
	
	output				CNV			,
	output				SCK			,	
	
	output		[ 6:0]	CH_en		,
	output				start		,
	output				LE			,
	output				ADC_rst 	,
	output		[15:0]	ADC_DO		,
	output	reg			ADC_SCK		,
	output	reg	[ 7:0]	frame_count	,
	output		[ 9:0]	data_count	,
	output		[15:0]	ADC_FIFO_OUT
    );
	
	//=======================================================
	// SCK  _/ˉˉ\__/--\__/--\__/--\__/--\__/--\__/--\__/--\_
	// CNV _|-----|______________________________________|-----
	// SDO=======< D15>< D14>< D13>< D12>< D11>< D10>....
	// CLKO  _/ˉˉ\__/--\__/--\__/--\__/--\__/--\__/--\__/--\_
	// =======================================================

	//*****************************************************
	//1.generate 10MHz clk 
	//* for ADC clk 
	//*****************************************************	  
	
//	parameter CNT_NUM_1 = 50; 	//100M /10M=10
//	parameter CNT_NUM_2 = 25 ;	//
	
	parameter CNT_NUM_1 = 50 - 1; 	//100M /10M=10 --> Modified by R.K. 2017/08/17
    parameter CNT_NUM_2 = 25 - 1 ;    //

    (* keep = "true" *) wire clk_50M;
	reg	clk_50M_r;	
	always @(posedge sys_clk_adc or negedge rst_n) 
	begin
		if(!rst_n) 
			clk_50M_r <= 1'b0;
		else
			clk_50M_r <= ~clk_50M_r;
	end
	
	assign clk_50M = clk_50M_r;
	
	always @(posedge sys_clk_adc or negedge rst_n) 
	begin
		if(!rst_n) 
			ADC_SCK <= 1'b1;
		else if(CNV==0)
			ADC_SCK <= clk_50M;
		else
			ADC_SCK <= 1'b1;
	end	
	
	reg[7:0] clk_cnt;
	(* keep = "true" *) wire clk_2M;
	always @(posedge sys_clk_adc or negedge rst_n) 
	begin
		if(!rst_n) 
			clk_cnt <= 8'b0;
		else if(clk_cnt == CNT_NUM_1)
			clk_cnt <= 8'b0;
		else
			clk_cnt <= clk_cnt + 1'b1;		
	end

    reg clk_2M_r;
	always @(posedge sys_clk_adc or negedge rst_n)
	begin
		if(!rst_n)
			clk_2M_r <= 1'b0;
		else if(clk_cnt == CNT_NUM_2)
			clk_2M_r <= 1'b1;
		else if(clk_cnt == CNT_NUM_1) 
			clk_2M_r <= 1'b0;
		else;
	end

	assign clk_2M = clk_2M_r;
	
	//*****************************************************
	//2.generate data source 
	//* for ADC data source simulate 
	//*****************************************************	
    reg [15:0]	raw_data;
	always @(posedge clk_2M or negedge rst_n)
	begin
		if(!rst_n)
			raw_data <= 16'h0;
		else if(raw_data == 16'hFFFF)
			raw_data <= 16'h0;
		else 
			raw_data <= raw_data + 1'b1;
	end
	
	
	//*****************************************************
	//3.generate adc Start Input CNV
	//* for ADC data  Conversion Start Input
	//*****************************************************	
	reg 		cnv_reg;
	reg	[11:0]	cnv_cnt;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			cnv_cnt <= 12'h0;
	    else if(cnv_cnt == 12'h18)	//50MHz/2MHz=25  --> Modified by R.K. 2017/08/17
//		else if(cnv_cnt == 12'h19)	//50MHz/2MHz=25
			cnv_cnt <= 12'h0;
		else 
			cnv_cnt <= cnv_cnt + 1'b1;
	end
	
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			cnv_reg <= 12'h0;
		else if(cnv_cnt == 12'h1||cnv_cnt == 12'h2)
			cnv_reg <= 1'b1;
		else 
			cnv_reg <= 1'b0;
	end
	
	assign	CNV = cnv_reg;
	
	reg	CNV_r1,CNV_r2;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)	begin
			CNV_r1 <= 1'b0;
			CNV_r2 <= 1'b0;
		end
		else begin
			CNV_r1 <= cnv_reg;
			CNV_r2 <= CNV_r1;	
		end
	end
	
	// assign	CNV_TRIG = CNV_r1&&!CNV_r2;  -- ?? 2017/08/16
    wire	CNV_TRIG = CNV_r1&&!CNV_r2; // Modified by R.K.
	
	reg	[15:0]	raw_data_r;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			raw_data_r <= 16'h0;
		else if(CNV)
			raw_data_r <= raw_data;
		else 
			raw_data_r <= raw_data_r;
	end
	

	//*****************************************************
	//3.generate data source 
	//* for ADC data source simulate 
	//*****************************************************		
	reg	[7:0]	data_cnt;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			data_cnt <= 8'h0;
		else if(CNV==1'b0)	
			data_cnt <= data_cnt+1;
		else 
			data_cnt <= 8'h0;
	end
	
	reg[15:0] ADC_SDO;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
		begin
			ADC_SDO <= 16'h0;
		end
		else if(CNV==1'b0)
		begin
			case(data_cnt)
				'd1: ADC_SDO[15] <= raw_data_r[15];
				'd2: ADC_SDO[14] <= raw_data_r[14];
				'd3: ADC_SDO[13] <= raw_data_r[13];
				'd4: ADC_SDO[12] <= raw_data_r[12];
				'd5: ADC_SDO[11] <= raw_data_r[11];
				'd6: ADC_SDO[10] <= raw_data_r[10];
				'd7: ADC_SDO[ 9] <= raw_data_r[ 9];
				'd8: ADC_SDO[ 8] <= raw_data_r[ 8]; 
				'd9: ADC_SDO[ 7] <= raw_data_r[ 7];
				'd10: ADC_SDO[6] <= raw_data_r[ 6]; 			
				'd11: ADC_SDO[5] <= raw_data_r[ 5];
				'd12: ADC_SDO[4] <= raw_data_r[ 4]; 			
				'd13: ADC_SDO[3] <= raw_data_r[ 3];
				'd14: ADC_SDO[2] <= raw_data_r[ 2]; 				
				'd15: ADC_SDO[1] <= raw_data_r[ 1];
				'd16: ADC_SDO[0] <= raw_data_r[ 0]; //逐位存�?�数�?
			endcase
		end
		else if(CNV==1'b1)
			ADC_SDO <=ADC_SDO;
		else;
	end
	
	reg[15:0] ADC_data;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			ADC_data <= 16'h0;
		else if(CNV==1'b1)
			ADC_data <= ADC_SDO;
		else if(CNV==1'b0)
			ADC_data <= ADC_data;
	end

	assign	ADC_DO = ADC_data;
	
	reg	[15:0] ADC_SDO_cnt;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			ADC_SDO_cnt <= 16'h0;
		else if(CNV_TRIG)begin
			if(ADC_SDO_cnt=='d48)
			ADC_SDO_cnt <= 16'h0;
			else
			ADC_SDO_cnt <= ADC_SDO_cnt + 1'b1;		
		end
		else;
	end

	reg	SR_OUT;		
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			SR_OUT <= 1'b0;
		else if(ADC_SDO_cnt=='d48)
			SR_OUT <= 1'b1;
		else
			SR_OUT <= 1'b0;
	end	
	
	//reg	[7:0]	frame_count;
	always @(posedge clk_50M or negedge rst_n)
	begin
		if(!rst_n)
			frame_count <= 8'b0;
		else if(SR_OUT==1'b1&&CNV_TRIG)
			frame_count <= frame_count+ 1'b1;
		else
			frame_count <= frame_count;
	end	

	wire	prog_full,prog_empty;
		
	fifo ADC_OUT (
	  .clk			(clk_2M			), // input clk
	  .rst			(!rst_n			), // input rst
	  .din			(ADC_DO			), // input [15 : 0] din
	  .wr_en		(!prog_full		), // input wr_en
	  .rd_en		(!prog_empty	), // input rd_en
	  .dout			(ADC_FIFO_OUT	), // output [15 : 0] dout
	  .data_count	(data_count		), // output [9 : 0] data_count
	  .prog_full	(prog_full		), // output prog_full
	  .prog_empty	(prog_empty		)  // output prog_empty
	);
	
endmodule
