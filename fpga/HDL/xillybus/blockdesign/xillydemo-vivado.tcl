# Xillydemo project generation script for Vivado 2014.1

set proj_name xillydemo
set proj_dir [file dirname [info script]]/vivado
set thepart "xc7k325tffg900-2"

set pcie_vivado pcie_k7_vivado
set pcie pcie_k7_8x

# Set the directory for essentials for Vivado
set essentials_dir "$proj_dir/../../vivado-essentials"

# Create project
create_project $proj_name "$proj_dir/"

# Set project properties
set obj [get_projects $proj_name]
set_property "default_lib" "xil_defaultlib" $obj
set_property "part" $thepart $obj
set_property "simulator_language" "Mixed" $obj
set_property "source_mgmt_mode" "DisplayOnly" $obj
set_property target_language Verilog $obj
set_property "ip_repo_paths" "[file normalize "$proj_dir/../xillybus_block"]" [current_fileset]
update_ip_catalog

# Create 'sources_1' fileset (if not found)
if {[string equal [get_filesets sources_1] ""]} {
  create_fileset -srcset sources_1
}

# Set 'sources_1' fileset properties
set obj [get_filesets sources_1]
set_property "top" "xillydemo" $obj

# Add files to 'sources_1' fileset
set obj [get_filesets sources_1]
set files [list \
 "[file normalize "$proj_dir/../src/xillydemo.v"]"\
 "[file normalize "$essentials_dir/$pcie.v"]"\
 "[file normalize "$essentials_dir/${pcie}_pipe_clock.v"]"\
 "[file normalize "$essentials_dir/$pcie_vivado/$pcie_vivado.xci"]"\
 "[file normalize "$proj_dir/../blockdesign/blockdesign.bd"]"\
]
add_files -norecurse -fileset $obj $files

set_msg_config -new_severity "INFO" -id {IP_Flow 19-3298} -string {{fifo_xillybus_}}

upgrade_ip [get_ips]

# Generate Xilinx' example project for the PCIe Integrated Block, make a copy
# of the pipe_clock module, and include the copy in the project.

generate_target example [get_ips $pcie_vivado]
file copy "[file normalize "$essentials_dir/$pcie_vivado/example_design/support/${pcie_vivado}_pipe_clock.v"]" "[file normalize "$essentials_dir"]"
add_files -norecurse -fileset $obj  "[file normalize "$essentials_dir/${pcie_vivado}_pipe_clock.v"]"

# Create 'constrs_1' fileset (if not found)
if {[string equal [get_filesets constrs_1] ""]} {
  create_fileset -constrset constrs_1
}

# Add files to 'constrs_1' fileset
set obj [get_filesets constrs_1]
add_files -fileset $obj -norecurse "[file normalize "$essentials_dir/xillydemo.xdc"]"
add_files -fileset $obj -norecurse "[file normalize "$proj_dir/../src/detach_clocks.xdc"]"

# Create 'sim_1' fileset (if not found)
if {[string equal [get_filesets sim_1] ""]} {
  create_fileset -simset sim_1
}

# Add files to 'sim_1' fileset
set obj [get_filesets sim_1]
# Empty (no sources present)

# Set 'sim_1' fileset properties
set obj [get_filesets sim_1]
set_property "runtime" "1000 ns" $obj
set_property "top" "unknown" $obj
set_property "unit_under_test" "UUT" $obj

# Create 'synth_1' run (if not found)
if {[string equal [get_runs synth_1] ""]} {
  create_run -name synth_1 -part $thepart -flow {Vivado Synthesis 2014} -strategy "Vivado Synthesis Defaults" -constrset constrs_1
}
set obj [get_runs synth_1]
set_property "part" $thepart $obj

# Create 'impl_1' run (if not found)
if {[string equal [get_runs impl_1] ""]} {
  create_run -name impl_1 -part $thepart -flow {Vivado Implementation 2014} -strategy "Vivado Implementation Defaults" -constrset constrs_1 -parent_run synth_1
}
set obj [get_runs impl_1]
set_property "part" $thepart $obj
set_property STEPS.PHYS_OPT_DESIGN.IS_ENABLED true $obj
set_property STEPS.POST_ROUTE_PHYS_OPT_DESIGN.IS_ENABLED true $obj
set_property STEPS.WRITE_BITSTREAM.TCL.PRE "$essentials_dir/showstopper.tcl" $obj

# The clocking wizard can't be part of blockdesign.bd, because the VLNV
# definition requires a specific version. So grab the VLNV from the list
# of IPs using wildcards, insert, configure and connect in Tcl.

open_bd_design $proj_dir/../blockdesign/blockdesign.bd

startgroup

create_bd_cell -type ip -vlnv [get_ipdefs *:clk_wiz:*] stream_clk_gen
set_property -dict [list CONFIG.PRIM_IN_FREQ.VALUE_SRC USER] \
    [get_bd_cells stream_clk_gen]

set_property -dict [list \
			CONFIG.PRIM_IN_FREQ {250} \
			CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {100} ] \
    [get_bd_cells stream_clk_gen]

connect_bd_net [get_bd_pins stream_clk_gen/clk_in1] [get_bd_pins xillybus_bundled_0/bus_clk]
connect_bd_net [get_bd_pins stream_clk_gen/clk_out1] [get_bd_pins xillybus_bundled_0/ap_clk]
connect_bd_net [get_bd_pins stream_clk_gen/reset] [get_bd_pins xillybus_bundled_0/quiesce]

endgroup

# Improve the graphic layout (hopefully)
regenerate_bd_layout

save_bd_design
close_bd_design [get_bd_designs blockdesign]

puts "INFO: Project created: $proj_name"

# Uncomment the two following lines for a full implementation
#launch_runs -jobs 8 impl_1 -to_step write_bitstream
#wait_on_run impl_1
