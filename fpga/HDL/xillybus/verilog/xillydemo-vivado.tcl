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

# Create 'sources_1' fileset (if not found)
if {[string equal [get_filesets sources_1] ""]} {
  create_fileset -srcset sources_1
}

# Set 'sources_1' fileset properties
set obj [get_filesets sources_1]
set_property "edif_extra_search_paths" "[file normalize "$proj_dir/../../core"]" $obj
set_property "top" "xillydemo" $obj

# Add files to 'sources_1' fileset
set obj [get_filesets sources_1]
set files [list \
 "[file normalize "$proj_dir/../src/xillydemo.v"]"\
 "[file normalize "$proj_dir/../src/xillybus.v"]"\
 "[file normalize "$proj_dir/../src/xillybus_core.v"]"\
 "[file normalize "$essentials_dir/$pcie.v"]"\
 "[file normalize "$essentials_dir/${pcie}_pipe_clock.v"]"\
 "[file normalize "$essentials_dir/fifo_8x2048/fifo_8x2048.xci"]"\
 "[file normalize "$essentials_dir/fifo_32x512/fifo_32x512.xci"]"\
 "[file normalize "$essentials_dir/$pcie_vivado/$pcie_vivado.xci"]"\
]
add_files -norecurse -fileset $obj $files

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

puts "INFO: Project created: $proj_name"

# Uncomment the two following lines for a full implementation
#launch_runs -jobs 8 impl_1 -to_step write_bitstream
#wait_on_run impl_1
