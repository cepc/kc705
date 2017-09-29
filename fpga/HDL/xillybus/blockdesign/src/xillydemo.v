module xillydemo
  (
   input  PCIE_PERST_B_LS,
   input  PCIE_REFCLK_N,
   input  PCIE_REFCLK_P,
   input [7:0] PCIE_RX_N,
   input [7:0] PCIE_RX_P,
   output [3:0] GPIO_LED,
   output [7:0] PCIE_TX_N,
   output [7:0] PCIE_TX_P
   );

   blockdesign blockdesign_ins
     (
      .PCIE_PERST_B_LS(PCIE_PERST_B_LS),
      .PCIE_REFCLK_N(PCIE_REFCLK_N),
      .PCIE_REFCLK_P(PCIE_REFCLK_P),
      .PCIE_RX_N(PCIE_RX_N),
      .PCIE_RX_P(PCIE_RX_P),
      .GPIO_LED(GPIO_LED),
      .PCIE_TX_N(PCIE_TX_N),
      .PCIE_TX_P(PCIE_TX_P)
      );   
endmodule
