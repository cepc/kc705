create_clock -period 10.000 -name sys_clk [get_pins -match_style ucf */pcieclk_ibuf/O]

set_false_path -to [get_pins -match_style ucf */pipe_clock/pclk_i1_bufgctrl.pclk_i1/S0]
set_false_path -to [get_pins -match_style ucf */pipe_clock/pclk_i1_bufgctrl.pclk_i1/S1]

set_property DONT_TOUCH true [get_cells -of [get_nets -of [get_pins -match_style ucf */pipe_clock/pclk_i1_bufgctrl.pclk_i1/S0]]]
set_property DONT_TOUCH true [get_cells -hier -filter name=~*/pipe_clock/pclk_sel*]

set_false_path -from [get_ports PCIE_PERST_B_LS]

set_property LOC IBUFDS_GTE2_X0Y1 [get_cells xillybus_ins/pcieclk_ibuf]

set_property PACKAGE_PIN G25 [get_ports PCIE_PERST_B_LS]
set_property IOSTANDARD LVCMOS25 [get_ports PCIE_PERST_B_LS]
set_property PULLUP true [get_ports PCIE_PERST_B_LS]

set_property PACKAGE_PIN AB8 [get_ports {GPIO_LED[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {GPIO_LED[0]}]
set_property PACKAGE_PIN AA8 [get_ports {GPIO_LED[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {GPIO_LED[1]}]
set_property PACKAGE_PIN AC9 [get_ports {GPIO_LED[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {GPIO_LED[2]}]
set_property PACKAGE_PIN AB9 [get_ports {GPIO_LED[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {GPIO_LED[3]}]






