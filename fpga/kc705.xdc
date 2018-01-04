
create_clock -period 5.000 -name clk200M [get_nets clk200M]

create_clock -period 500.000 -name clk2M [get_nets clk2M]
#create_clock -period 500.000 -name cnvclk [get_nets rec_adc_packet_ins/CNVCLK]
create_clock -period 500.000 -name cnvclk -waveform {0.000 100.000} [get_nets rec_adc_packet_ins/CNVCLK]
set_false_path -to [get_clocks cnvclk]






create_debug_core u_ila_0 ila
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 4 [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER true [get_debug_cores u_ila_0]
set_property C_DATA_DEPTH 16384 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL true [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list AD_data_receiver/clk]]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
set_property port_width 16 [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list {AD_data_receiver/DATA8[0]} {AD_data_receiver/DATA8[1]} {AD_data_receiver/DATA8[2]} {AD_data_receiver/DATA8[3]} {AD_data_receiver/DATA8[4]} {AD_data_receiver/DATA8[5]} {AD_data_receiver/DATA8[6]} {AD_data_receiver/DATA8[7]} {AD_data_receiver/DATA8[8]} {AD_data_receiver/DATA8[9]} {AD_data_receiver/DATA8[10]} {AD_data_receiver/DATA8[11]} {AD_data_receiver/DATA8[12]} {AD_data_receiver/DATA8[13]} {AD_data_receiver/DATA8[14]} {AD_data_receiver/DATA8[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe1]
set_property port_width 16 [get_debug_ports u_ila_0/probe1]
connect_debug_port u_ila_0/probe1 [get_nets [list {AD_data_receiver/DATA12[0]} {AD_data_receiver/DATA12[1]} {AD_data_receiver/DATA12[2]} {AD_data_receiver/DATA12[3]} {AD_data_receiver/DATA12[4]} {AD_data_receiver/DATA12[5]} {AD_data_receiver/DATA12[6]} {AD_data_receiver/DATA12[7]} {AD_data_receiver/DATA12[8]} {AD_data_receiver/DATA12[9]} {AD_data_receiver/DATA12[10]} {AD_data_receiver/DATA12[11]} {AD_data_receiver/DATA12[12]} {AD_data_receiver/DATA12[13]} {AD_data_receiver/DATA12[14]} {AD_data_receiver/DATA12[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe2]
set_property port_width 16 [get_debug_ports u_ila_0/probe2]
connect_debug_port u_ila_0/probe2 [get_nets [list {AD_data_receiver/DATA11[0]} {AD_data_receiver/DATA11[1]} {AD_data_receiver/DATA11[2]} {AD_data_receiver/DATA11[3]} {AD_data_receiver/DATA11[4]} {AD_data_receiver/DATA11[5]} {AD_data_receiver/DATA11[6]} {AD_data_receiver/DATA11[7]} {AD_data_receiver/DATA11[8]} {AD_data_receiver/DATA11[9]} {AD_data_receiver/DATA11[10]} {AD_data_receiver/DATA11[11]} {AD_data_receiver/DATA11[12]} {AD_data_receiver/DATA11[13]} {AD_data_receiver/DATA11[14]} {AD_data_receiver/DATA11[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe3]
set_property port_width 16 [get_debug_ports u_ila_0/probe3]
connect_debug_port u_ila_0/probe3 [get_nets [list {AD_data_receiver/DATA7[0]} {AD_data_receiver/DATA7[1]} {AD_data_receiver/DATA7[2]} {AD_data_receiver/DATA7[3]} {AD_data_receiver/DATA7[4]} {AD_data_receiver/DATA7[5]} {AD_data_receiver/DATA7[6]} {AD_data_receiver/DATA7[7]} {AD_data_receiver/DATA7[8]} {AD_data_receiver/DATA7[9]} {AD_data_receiver/DATA7[10]} {AD_data_receiver/DATA7[11]} {AD_data_receiver/DATA7[12]} {AD_data_receiver/DATA7[13]} {AD_data_receiver/DATA7[14]} {AD_data_receiver/DATA7[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe4]
set_property port_width 7 [get_debug_ports u_ila_0/probe4]
connect_debug_port u_ila_0/probe4 [get_nets [list {AD_data_receiver/d_A[0]} {AD_data_receiver/d_A[1]} {AD_data_receiver/d_A[2]} {AD_data_receiver/d_A[3]} {AD_data_receiver/d_A[4]} {AD_data_receiver/d_A[5]} {AD_data_receiver/d_A[6]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe5]
set_property port_width 16 [get_debug_ports u_ila_0/probe5]
connect_debug_port u_ila_0/probe5 [get_nets [list {rec_adc_packet_ins/sample_data07[0]} {rec_adc_packet_ins/sample_data07[1]} {rec_adc_packet_ins/sample_data07[2]} {rec_adc_packet_ins/sample_data07[3]} {rec_adc_packet_ins/sample_data07[4]} {rec_adc_packet_ins/sample_data07[5]} {rec_adc_packet_ins/sample_data07[6]} {rec_adc_packet_ins/sample_data07[7]} {rec_adc_packet_ins/sample_data07[8]} {rec_adc_packet_ins/sample_data07[9]} {rec_adc_packet_ins/sample_data07[10]} {rec_adc_packet_ins/sample_data07[11]} {rec_adc_packet_ins/sample_data07[12]} {rec_adc_packet_ins/sample_data07[13]} {rec_adc_packet_ins/sample_data07[14]} {rec_adc_packet_ins/sample_data07[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe6]
set_property port_width 6 [get_debug_ports u_ila_0/probe6]
connect_debug_port u_ila_0/probe6 [get_nets [list {rec_adc_packet_ins/data_selector_inst/ch_id[0]} {rec_adc_packet_ins/data_selector_inst/ch_id[1]} {rec_adc_packet_ins/data_selector_inst/ch_id[2]} {rec_adc_packet_ins/data_selector_inst/ch_id[3]} {rec_adc_packet_ins/data_selector_inst/ch_id[4]} {rec_adc_packet_ins/data_selector_inst/ch_id[5]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe7]
set_property port_width 16 [get_debug_ports u_ila_0/probe7]
connect_debug_port u_ila_0/probe7 [get_nets [list {rec_adc_packet_ins/d07_out[0]} {rec_adc_packet_ins/d07_out[1]} {rec_adc_packet_ins/d07_out[2]} {rec_adc_packet_ins/d07_out[3]} {rec_adc_packet_ins/d07_out[4]} {rec_adc_packet_ins/d07_out[5]} {rec_adc_packet_ins/d07_out[6]} {rec_adc_packet_ins/d07_out[7]} {rec_adc_packet_ins/d07_out[8]} {rec_adc_packet_ins/d07_out[9]} {rec_adc_packet_ins/d07_out[10]} {rec_adc_packet_ins/d07_out[11]} {rec_adc_packet_ins/d07_out[12]} {rec_adc_packet_ins/d07_out[13]} {rec_adc_packet_ins/d07_out[14]} {rec_adc_packet_ins/d07_out[15]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe8]
set_property port_width 32 [get_debug_ports u_ila_0/probe8]
connect_debug_port u_ila_0/probe8 [get_nets [list {adc_fifo_out32[0]} {adc_fifo_out32[1]} {adc_fifo_out32[2]} {adc_fifo_out32[3]} {adc_fifo_out32[4]} {adc_fifo_out32[5]} {adc_fifo_out32[6]} {adc_fifo_out32[7]} {adc_fifo_out32[8]} {adc_fifo_out32[9]} {adc_fifo_out32[10]} {adc_fifo_out32[11]} {adc_fifo_out32[12]} {adc_fifo_out32[13]} {adc_fifo_out32[14]} {adc_fifo_out32[15]} {adc_fifo_out32[16]} {adc_fifo_out32[17]} {adc_fifo_out32[18]} {adc_fifo_out32[19]} {adc_fifo_out32[20]} {adc_fifo_out32[21]} {adc_fifo_out32[22]} {adc_fifo_out32[23]} {adc_fifo_out32[24]} {adc_fifo_out32[25]} {adc_fifo_out32[26]} {adc_fifo_out32[27]} {adc_fifo_out32[28]} {adc_fifo_out32[29]} {adc_fifo_out32[30]} {adc_fifo_out32[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe9]
set_property port_width 1 [get_debug_ports u_ila_0/probe9]
connect_debug_port u_ila_0/probe9 [get_nets [list adc_fifo_wren]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe10]
set_property port_width 1 [get_debug_ports u_ila_0/probe10]
connect_debug_port u_ila_0/probe10 [get_nets [list adcdata_wren]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe11]
set_property port_width 1 [get_debug_ports u_ila_0/probe11]
connect_debug_port u_ila_0/probe11 [get_nets [list clk2M]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe12]
set_property port_width 1 [get_debug_ports u_ila_0/probe12]
connect_debug_port u_ila_0/probe12 [get_nets [list AD_data_receiver/clk_div]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe13]
set_property port_width 1 [get_debug_ports u_ila_0/probe13]
connect_debug_port u_ila_0/probe13 [get_nets [list AD_data_receiver/CLKOUTN]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe14]
set_property port_width 1 [get_debug_ports u_ila_0/probe14]
connect_debug_port u_ila_0/probe14 [get_nets [list cnvclk]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe15]
set_property port_width 1 [get_debug_ports u_ila_0/probe15]
connect_debug_port u_ila_0/probe15 [get_nets [list fifo_write_veto_r]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe16]
set_property port_width 1 [get_debug_ports u_ila_0/probe16]
connect_debug_port u_ila_0/probe16 [get_nets [list rec_adc_packet_ins/flag07]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe17]
set_property port_width 1 [get_debug_ports u_ila_0/probe17]
connect_debug_port u_ila_0/probe17 [get_nets [list rec_adc_packet_ins/flag09]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe18]
set_property port_width 1 [get_debug_ports u_ila_0/probe18]
connect_debug_port u_ila_0/probe18 [get_nets [list full_async_fifo]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe19]
set_property port_width 1 [get_debug_ports u_ila_0/probe19]
connect_debug_port u_ila_0/probe19 [get_nets [list rec_adc_packet_ins/data_selector_inst/MEM_RD_FLAG]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe20]
set_property port_width 1 [get_debug_ports u_ila_0/probe20]
connect_debug_port u_ila_0/probe20 [get_nets [list AD_data_receiver/oe]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe21]
set_property port_width 1 [get_debug_ports u_ila_0/probe21]
connect_debug_port u_ila_0/probe21 [get_nets [list rst2M]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe22]
set_property port_width 1 [get_debug_ports u_ila_0/probe22]
connect_debug_port u_ila_0/probe22 [get_nets [list rst_command]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe23]
set_property port_width 1 [get_debug_ports u_ila_0/probe23]
connect_debug_port u_ila_0/probe23 [get_nets [list rst_pulse]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe24]
set_property port_width 1 [get_debug_ports u_ila_0/probe24]
connect_debug_port u_ila_0/probe24 [get_nets [list AD_data_receiver/SCK]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe25]
set_property port_width 1 [get_debug_ports u_ila_0/probe25]
connect_debug_port u_ila_0/probe25 [get_nets [list SR_Top_out]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe26]
set_property port_width 1 [get_debug_ports u_ila_0/probe26]
connect_debug_port u_ila_0/probe26 [get_nets [list AD_data_receiver/sync]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe27]
set_property port_width 1 [get_debug_ports u_ila_0/probe27]
connect_debug_port u_ila_0/probe27 [get_nets [list rec_adc_packet_ins/data_selector_inst/wr_en_pre]]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets clk200M]
