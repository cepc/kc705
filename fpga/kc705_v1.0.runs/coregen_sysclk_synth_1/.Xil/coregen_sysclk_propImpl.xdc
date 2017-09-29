set_property SRC_FILE_INFO {cfile:f:/Ryuta/FPGA/KC705_V1.0/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk.xdc rfile:../../../kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk.xdc id:1 order:EARLY scoped_inst:inst} [current_design]
set_property src_info {type:SCOPED_XDC file:1 line:56 export:INPUT save:INPUT read:READ} [current_design]
set_input_jitter [get_clocks -of_objects [get_ports clk_in1_p]] 0.05
