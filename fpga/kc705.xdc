#set_false_path -from [get_clocks -of_objects [get_pins xillydemo_ins/xillybus_ins/pipe_clock/pipe_clock/mmcm_i/CLKOUT2]] -to [get_clocks -of_objects [get_pins sysclk_ins/inst/mmcm_adv_inst/CLKOUT0]]
#create_clock -period 5.000 [get_ports SYSCLK_P]

set_property IOSTANDARD LVDS [get_ports SYSCLK_P]

set_property PACKAGE_PIN AD11 [get_ports SYSCLK_N]
set_property IOSTANDARD LVDS [get_ports SYSCLK_N]

create_clock -period 500.000 -name clk2M [get_nets clk2M]
#create_clock -period 500.000 -name clk_2M [get_nets ADC_test_ins/clk_2M]
#create_clock -period 20.000 -name clk_50M [get_nets ADC_test_ins/clk_50M]
create_clock -period 500.000 -name clk_2M-1 [get_nets ADC_test_ins1/clk_2M]
create_clock -period 500.000 -name clk_2M-2 [get_nets ADC_test_ins2/clk_2M]
create_clock -period 20.000 -name clk_50M-1 [get_nets ADC_test_ins1/clk_50M]
create_clock -period 20.000 -name clk_50M-2 [get_nets ADC_test_ins2/clk_50M]














