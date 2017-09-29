proc start_step { step } {
  set stopFile ".stop.rst"
  if {[file isfile .stop.rst]} {
    puts ""
    puts "*** Halting run - EA reset detected ***"
    puts ""
    puts ""
    return -code error
  }
  set beginFile ".$step.begin.rst"
  set platform "$::tcl_platform(platform)"
  set user "$::tcl_platform(user)"
  set pid [pid]
  set host ""
  if { [string equal $platform unix] } {
    if { [info exist ::env(HOSTNAME)] } {
      set host $::env(HOSTNAME)
    }
  } else {
    if { [info exist ::env(COMPUTERNAME)] } {
      set host $::env(COMPUTERNAME)
    }
  }
  set ch [open $beginFile w]
  puts $ch "<?xml version=\"1.0\"?>"
  puts $ch "<ProcessHandle Version=\"1\" Minor=\"0\">"
  puts $ch "    <Process Command=\".planAhead.\" Owner=\"$user\" Host=\"$host\" Pid=\"$pid\">"
  puts $ch "    </Process>"
  puts $ch "</ProcessHandle>"
  close $ch
}

proc end_step { step } {
  set endFile ".$step.end.rst"
  set ch [open $endFile w]
  close $ch
}

proc step_failed { step } {
  set endFile ".$step.error.rst"
  set ch [open $endFile w]
  close $ch
}

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
set_msg_config -id {Synth 8-256} -limit 10000
set_msg_config -id {Synth 8-638} -limit 10000

start_step init_design
set rc [catch {
  create_msg_db init_design.pb
  set_param gui.test TreeTableDev
  set_param xicom.use_bs_reader 1
  debug::add_scope template.lib 1
  set_property design_mode GateLvl [current_fileset]
  set_property webtalk.parent_dir C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.cache/wt [current_project]
  set_property parent.project_path C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.xpr [current_project]
  set_property ip_repo_paths c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.cache/ip [current_project]
  set_property ip_output_repo c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.cache/ip [current_project]
  add_files -quiet C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/synth_1/kc705.dcp
  set_property edif_extra_search_paths F:/Ryuta/FPGA/KC705_V1.0/HDL/xillybus/core [current_fileset]
  add_files -quiet C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/coregen_sysclk_synth_1/coregen_sysclk.dcp
  set_property netlist_only true [get_files C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/coregen_sysclk_synth_1/coregen_sysclk.dcp]
  add_files -quiet C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/coregen_user_mem8_synth_1/coregen_user_mem8.dcp
  set_property netlist_only true [get_files C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/coregen_user_mem8_synth_1/coregen_user_mem8.dcp]
  add_files -quiet C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/coregen_clk_crossing_fifo32_synth_1/coregen_clk_crossing_fifo32.dcp
  set_property netlist_only true [get_files C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/coregen_clk_crossing_fifo32_synth_1/coregen_clk_crossing_fifo32.dcp]
  add_files -quiet C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/fifo_synth_1/fifo.dcp
  set_property netlist_only true [get_files C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/fifo_synth_1/fifo.dcp]
  add_files -quiet C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/fifo_8x2048_synth_1/fifo_8x2048.dcp
  set_property netlist_only true [get_files C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/fifo_8x2048_synth_1/fifo_8x2048.dcp]
  add_files -quiet C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/pcie_k7_vivado_synth_1/pcie_k7_vivado.dcp
  set_property netlist_only true [get_files C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/pcie_k7_vivado_synth_1/pcie_k7_vivado.dcp]
  read_xdc -mode out_of_context -ref coregen_sysclk -cells inst c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk_ooc.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk_ooc.xdc]
  read_xdc -prop_thru_buffers -ref coregen_sysclk -cells inst c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk_board.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk_board.xdc]
  read_xdc -ref coregen_sysclk -cells inst c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk.xdc]
  read_xdc -mode out_of_context -ref coregen_user_mem8 -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_user_mem8/coregen_user_mem8_ooc.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_user_mem8/coregen_user_mem8_ooc.xdc]
  read_xdc -mode out_of_context -ref coregen_clk_crossing_fifo32 -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32_ooc.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32_ooc.xdc]
  read_xdc -ref coregen_clk_crossing_fifo32 -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32.xdc]
  read_xdc -mode out_of_context -ref fifo -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/fifo/fifo_ooc.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/fifo/fifo_ooc.xdc]
  read_xdc -ref fifo -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/fifo/fifo/fifo.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/fifo/fifo/fifo.xdc]
  read_xdc -mode out_of_context -ref fifo_8x2048 -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/fifo_8x2048/fifo_8x2048_ooc.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/fifo_8x2048/fifo_8x2048_ooc.xdc]
  read_xdc -ref fifo_8x2048 -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/fifo_8x2048/fifo_8x2048/fifo_8x2048.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/fifo_8x2048/fifo_8x2048/fifo_8x2048.xdc]
  read_xdc -mode out_of_context -ref pcie_k7_vivado -cells inst c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/pcie_k7_vivado/synth/pcie_k7_vivado_ooc.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/pcie_k7_vivado/synth/pcie_k7_vivado_ooc.xdc]
  read_xdc -ref pcie_k7_vivado -cells inst c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/pcie_k7_vivado/source/pcie_k7_vivado-PCIE_X0Y0.xdc
  set_property processing_order EARLY [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/pcie_k7_vivado/source/pcie_k7_vivado-PCIE_X0Y0.xdc]
  read_xdc C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/xillydemo.xdc
  read_xdc C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705.xdc
  read_xdc -ref coregen_clk_crossing_fifo32 -cells U0 c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32_clocks.xdc
  set_property processing_order LATE [get_files c:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.srcs/sources_1/ip/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32_clocks.xdc]
  link_design -top kc705 -part xc7k325tffg900-2
  close_msg_db -file init_design.pb
} RESULT]
if {$rc} {
  step_failed init_design
  return -code error $RESULT
} else {
  end_step init_design
}

start_step opt_design
set rc [catch {
  create_msg_db opt_design.pb
  catch {write_debug_probes -quiet -force debug_nets}
  opt_design 
  write_checkpoint -force kc705_opt.dcp
  catch {report_drc -file kc705_drc_opted.rpt}
  close_msg_db -file opt_design.pb
} RESULT]
if {$rc} {
  step_failed opt_design
  return -code error $RESULT
} else {
  end_step opt_design
}

start_step place_design
set rc [catch {
  create_msg_db place_design.pb
  place_design 
  write_checkpoint -force kc705_placed.dcp
  catch { report_io -file kc705_io_placed.rpt }
  catch { report_clock_utilization -file kc705_clock_utilization_placed.rpt }
  catch { report_utilization -file kc705_utilization_placed.rpt -pb kc705_utilization_placed.pb }
  catch { report_control_sets -verbose -file kc705_control_sets_placed.rpt }
  close_msg_db -file place_design.pb
} RESULT]
if {$rc} {
  step_failed place_design
  return -code error $RESULT
} else {
  end_step place_design
}

start_step phys_opt_design
set rc [catch {
  create_msg_db phys_opt_design.pb
  phys_opt_design 
  write_checkpoint -force kc705_physopt.dcp
  close_msg_db -file phys_opt_design.pb
} RESULT]
if {$rc} {
  step_failed phys_opt_design
  return -code error $RESULT
} else {
  end_step phys_opt_design
}

  set_msg_config -id {Route 35-39} -severity "critical warning" -new_severity warning
start_step route_design
set rc [catch {
  create_msg_db route_design.pb
  route_design 
  write_checkpoint -force kc705_routed.dcp
  catch { report_drc -file kc705_drc_routed.rpt -pb kc705_drc_routed.pb }
  catch { report_timing_summary -max_paths 10 -file kc705_timing_summary_routed.rpt -rpx kc705_timing_summary_routed.rpx }
  catch { report_power -file kc705_power_routed.rpt -pb kc705_power_summary_routed.pb }
  catch { report_route_status -file kc705_route_status.rpt -pb kc705_route_status.pb }
  close_msg_db -file route_design.pb
} RESULT]
if {$rc} {
  step_failed route_design
  return -code error $RESULT
} else {
  end_step route_design
}

start_step post_route_phys_opt_design
set rc [catch {
  create_msg_db post_route_phys_opt_design.pb
  phys_opt_design 
  write_checkpoint -force kc705_postroute_physopt.dcp
  catch { report_timing_summary -warn_on_violation -max_paths 10 -file kc705_timing_summary_postroute_physopted.rpt -rpx kc705_timing_summary_postroute_physopted.rpx }
  close_msg_db -file post_route_phys_opt_design.pb
} RESULT]
if {$rc} {
  step_failed post_route_phys_opt_design
  return -code error $RESULT
} else {
  end_step post_route_phys_opt_design
}

start_step write_bitstream
set rc [catch {
  create_msg_db write_bitstream.pb
  set src_rc [catch { 
    puts "source C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/showstopper.tcl"
    source C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/showstopper.tcl
  } _RESULT] 
  if {$src_rc} { 
    send_msg_id runtcl-1 error "$_RESULT"
    send_msg_id runtcl-2 error "sourcing script C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/HDL/xillybus/vivado-essentials/showstopper.tcl failed"
    return -code error
  }
  write_bitstream -force kc705.bit 
  if { [file exists C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/synth_1/kc705.hwdef] } {
    catch { write_sysdef -hwdef C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1_ADCpattern/kc705_v1.0.runs/synth_1/kc705.hwdef -bitfile kc705.bit -meminfo kc705.mmi -file kc705.sysdef }
  }
  close_msg_db -file write_bitstream.pb
} RESULT]
if {$rc} {
  step_failed write_bitstream
  return -code error $RESULT
} else {
  end_step write_bitstream
}

