set_property PACKAGE_PIN AF20 [get_ports BUSY_p]
set_property PACKAGE_PIN AH21 [get_ports TRIGGER_CLOCK_p]
set_property PACKAGE_PIN AE23 [get_ports TRIGGER_p]





connect_debug_port u_ila_0/probe10 [get_nets [list circle_buffer_enb]]

create_debug_core u_ila_0 ila
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 1 [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER false [get_debug_cores u_ila_0]
set_property C_DATA_DEPTH 32768 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL false [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list sysclk_ins_test/inst/clk_out1]]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
set_property port_width 16 [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list {circle_buffer_addra[0]} {circle_buffer_addra[1]} {circle_buffer_addra[2]} {circle_buffer_addra[3]} {circle_buffer_addra[4]} {circle_buffer_addra[5]} {circle_buffer_addra[6]} {circle_buffer_addra[7]} {circle_buffer_addra[8]} {circle_buffer_addra[9]} {circle_buffer_addra[10]} {circle_buffer_addra[11]} {circle_buffer_addra[12]} {circle_buffer_addra[13]} {circle_buffer_addra[14]} {circle_buffer_addra[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe1]
set_property port_width 16 [get_debug_ports u_ila_0/probe1]
connect_debug_port u_ila_0/probe1 [get_nets [list {circle_buffer_addrb[0]} {circle_buffer_addrb[1]} {circle_buffer_addrb[2]} {circle_buffer_addrb[3]} {circle_buffer_addrb[4]} {circle_buffer_addrb[5]} {circle_buffer_addrb[6]} {circle_buffer_addrb[7]} {circle_buffer_addrb[8]} {circle_buffer_addrb[9]} {circle_buffer_addrb[10]} {circle_buffer_addrb[11]} {circle_buffer_addrb[12]} {circle_buffer_addrb[13]} {circle_buffer_addrb[14]} {circle_buffer_addrb[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe2]
set_property port_width 32 [get_debug_ports u_ila_0/probe2]
connect_debug_port u_ila_0/probe2 [get_nets [list {adc_fifo_out32[0]} {adc_fifo_out32[1]} {adc_fifo_out32[2]} {adc_fifo_out32[3]} {adc_fifo_out32[4]} {adc_fifo_out32[5]} {adc_fifo_out32[6]} {adc_fifo_out32[7]} {adc_fifo_out32[8]} {adc_fifo_out32[9]} {adc_fifo_out32[10]} {adc_fifo_out32[11]} {adc_fifo_out32[12]} {adc_fifo_out32[13]} {adc_fifo_out32[14]} {adc_fifo_out32[15]} {adc_fifo_out32[16]} {adc_fifo_out32[17]} {adc_fifo_out32[18]} {adc_fifo_out32[19]} {adc_fifo_out32[20]} {adc_fifo_out32[21]} {adc_fifo_out32[22]} {adc_fifo_out32[23]} {adc_fifo_out32[24]} {adc_fifo_out32[25]} {adc_fifo_out32[26]} {adc_fifo_out32[27]} {adc_fifo_out32[28]} {adc_fifo_out32[29]} {adc_fifo_out32[30]} {adc_fifo_out32[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe3]
set_property port_width 32 [get_debug_ports u_ila_0/probe3]
connect_debug_port u_ila_0/probe3 [get_nets [list {adc_fifo_din[0]} {adc_fifo_din[1]} {adc_fifo_din[2]} {adc_fifo_din[3]} {adc_fifo_din[4]} {adc_fifo_din[5]} {adc_fifo_din[6]} {adc_fifo_din[7]} {adc_fifo_din[8]} {adc_fifo_din[9]} {adc_fifo_din[10]} {adc_fifo_din[11]} {adc_fifo_din[12]} {adc_fifo_din[13]} {adc_fifo_din[14]} {adc_fifo_din[15]} {adc_fifo_din[16]} {adc_fifo_din[17]} {adc_fifo_din[18]} {adc_fifo_din[19]} {adc_fifo_din[20]} {adc_fifo_din[21]} {adc_fifo_din[22]} {adc_fifo_din[23]} {adc_fifo_din[24]} {adc_fifo_din[25]} {adc_fifo_din[26]} {adc_fifo_din[27]} {adc_fifo_din[28]} {adc_fifo_din[29]} {adc_fifo_din[30]} {adc_fifo_din[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe4]
set_property port_width 32 [get_debug_ports u_ila_0/probe4]
connect_debug_port u_ila_0/probe4 [get_nets [list {circle_buffer_out32[0]} {circle_buffer_out32[1]} {circle_buffer_out32[2]} {circle_buffer_out32[3]} {circle_buffer_out32[4]} {circle_buffer_out32[5]} {circle_buffer_out32[6]} {circle_buffer_out32[7]} {circle_buffer_out32[8]} {circle_buffer_out32[9]} {circle_buffer_out32[10]} {circle_buffer_out32[11]} {circle_buffer_out32[12]} {circle_buffer_out32[13]} {circle_buffer_out32[14]} {circle_buffer_out32[15]} {circle_buffer_out32[16]} {circle_buffer_out32[17]} {circle_buffer_out32[18]} {circle_buffer_out32[19]} {circle_buffer_out32[20]} {circle_buffer_out32[21]} {circle_buffer_out32[22]} {circle_buffer_out32[23]} {circle_buffer_out32[24]} {circle_buffer_out32[25]} {circle_buffer_out32[26]} {circle_buffer_out32[27]} {circle_buffer_out32[28]} {circle_buffer_out32[29]} {circle_buffer_out32[30]} {circle_buffer_out32[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe5]
set_property port_width 4 [get_debug_ports u_ila_0/probe5]
connect_debug_port u_ila_0/probe5 [get_nets [list {circle_buffer_ctrl_ins/BUFFER_STATE[0]} {circle_buffer_ctrl_ins/BUFFER_STATE[1]} {circle_buffer_ctrl_ins/BUFFER_STATE[2]} {circle_buffer_ctrl_ins/BUFFER_STATE[3]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe6]
set_property port_width 1 [get_debug_ports u_ila_0/probe6]
connect_debug_port u_ila_0/probe6 [get_nets [list adc_fifo_wren]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe7]
set_property port_width 1 [get_debug_ports u_ila_0/probe7]
connect_debug_port u_ila_0/probe7 [get_nets [list circle_buffer_wren]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe8]
set_property port_width 1 [get_debug_ports u_ila_0/probe8]
connect_debug_port u_ila_0/probe8 [get_nets [list circle_buffer_ctrl_ins/flag_buf_rd]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe9]
set_property port_width 1 [get_debug_ports u_ila_0/probe9]
connect_debug_port u_ila_0/probe9 [get_nets [list trigger_cnt_valid]]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets clk100M]
