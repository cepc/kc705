`timescale 1ns/1ps
/* `include "kc705.v" */

module test_bench_kc705;

    localparam STEP_PCIe = 8; 
    localparam STEP_FPGA = 5; 

    reg SYSTEM_CLOCK_P;
    reg SYSTEM_CLOCK_N; 
    reg PCIE_PERST_B_LS;
    reg PCIE_REFCLK_N;
    reg PCIE_REFCLK_P;
    reg [7:0] PCIE_RX_N, PCIE_RX_P;

    wire [3:0] GPIO_LED;
    wire [7:0] PCIE_TX_N, PCIE_TX_P;
    
    kc705 kc705_ins(
        .SYSTEM_CLOCK_P    (  SYSTEM_CLOCK_P ),
        .SYSTEM_CLOCK_N    (  SYSTEM_CLOCK_N ),
        .PCIE_PERST_B_LS   ( PCIE_PERST_B_LS ),
        .PCIE_REFCLK_N     (   PCIE_REFCLK_N ),
        .PCIE_REFCLK_P     (   PCIE_REFCLK_P ),
        .PCIE_RX_N         (       PCIE_RX_N ),
        .PCIE_RX_P         (       PCIE_RX_P ),
        .GPIO_LED          (        GPIO_LED ), 
        .PCIE_TX_N         (       PCIE_TX_N ),
        .PCIE_TX_P         (       PCIE_TX_P )
    );

    
    always begin
      PCIE_REFCLK_P = 0; PCIE_REFCLK_N = 1; #(STEP_PCIe/2);
      PCIE_REFCLK_P = 1; PCIE_REFCLK_N = 0; #(STEP_PCIe/2);
    end
    
    
    always begin
      //SYSTEM_CLOCK_P = 0; SYSTEM_CLOCK_N = 1; #(STEP_FPGA/2);
      //SYSTEM_CLOCK_P = 1; SYSTEM_CLOCK_N = 0; #(STEP_FPGA/2);
      SYSTEM_CLOCK_P = 0; SYSTEM_CLOCK_N = 1; #(2.500);
      SYSTEM_CLOCK_P = 1; SYSTEM_CLOCK_N = 0; #(2.500);
    end
    

endmodule

