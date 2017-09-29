#-------------------------------------------------------------
# Generated Example Tcl script for IP 'pcie_k7_vivado' (xilinx.com:ip:pcie_7x:3.0)
#-------------------------------------------------------------

# Declare source IP directory
set srcIpDir "f:/Ryuta/FPGA/KC705_V1.0/HDL/xillybus/vivado-essentials/pcie_k7_vivado"

# Create project
create_project -name pcie_k7_vivado_example -force
set_property part xc7k325tffg900-2 [current_project]
set_property target_language verilog [current_project]
set_property simulator_language MIXED [current_project]
set returnCode 0

# Import the original IP (excluding example files)
import_ip -files [list [file join $srcIpDir pcie_k7_vivado.xci]] -name pcie_k7_vivado
reset_target {example} [get_ips pcie_k7_vivado]

# Generate the IP
proc _filter_supported_targets {targets ip} {
  set res {}
  set all [get_property SUPPORTED_TARGETS $ip]
  foreach target $targets {
    lappend res {*}[lsearch -all -inline -nocase $all $target]
  }
  return $res
}
generate_target [_filter_supported_targets {instantiation_template synthesis simulation implementation shared_logic} [get_ips pcie_k7_vivado]] [get_ips pcie_k7_vivado]

# Add example synthesis HDL files
add_files -scan_for_includes -quiet -fileset [current_fileset] \
  [list [file join $srcIpDir example_design/EP_MEM.v]] \
  [list [file join $srcIpDir example_design/PIO_EP_MEM_ACCESS.v]] \
  [list [file join $srcIpDir example_design/PIO_EP.v]] \
  [list [file join $srcIpDir example_design/PIO_RX_ENGINE.v]] \
  [list [file join $srcIpDir example_design/PIO_TO_CTRL.v]] \
  [list [file join $srcIpDir example_design/PIO_TX_ENGINE.v]] \
  [list [file join $srcIpDir example_design/PIO.v]] \
  [list [file join $srcIpDir example_design/pcie_app_7x.v]] \
  [list [file join $srcIpDir example_design/xilinx_pcie_2_1_ep_7x.v]] \
  [list [file join $srcIpDir example_design/support/pcie_k7_vivado_support.v]] \
  [list [file join $srcIpDir example_design/support/pcie_k7_vivado_pipe_clock.v]]

# Add example miscellaneous synthesis files
add_files -quiet -fileset [current_fileset] \
  [list [file join $srcIpDir hierarchy.txt]]

# Add example XDC files
add_files -quiet -fileset [current_fileset -constrset] \
  [list [file join $srcIpDir example_design/xilinx_pcie_7x_ep_x8g1_KC705_REVC.xdc]]


# Add example simulation HDL files
if { [catch {current_fileset -simset} exc] } { create_fileset -simset sim_1 }
add_files -quiet -scan_for_includes -fileset [current_fileset -simset] \
  [list [file join $srcIpDir simulation/dsport/pci_exp_expect_tasks.vh]] \
  [list [file join $srcIpDir simulation/dsport/pci_exp_usrapp_cfg.v]] \
  [list [file join $srcIpDir simulation/dsport/pci_exp_usrapp_com.v]] \
  [list [file join $srcIpDir simulation/dsport/pci_exp_usrapp_pl.v]] \
  [list [file join $srcIpDir simulation/dsport/pci_exp_usrapp_rx.v]] \
  [list [file join $srcIpDir simulation/dsport/pci_exp_usrapp_tx.v]] \
  [list [file join $srcIpDir simulation/functional/sys_clk_gen_ds.v]] \
  [list [file join $srcIpDir simulation/functional/sys_clk_gen.v]] \
  [list [file join $srcIpDir simulation/dsport/pcie_axi_trn_bridge.v]] \
  [list [file join $srcIpDir simulation/functional/board.v]] \
  [list [file join $srcIpDir simulation/functional/board_common.vh]] \
  [list [file join $srcIpDir simulation/tests/sample_tests1.vh]] \
  [list [file join $srcIpDir simulation/tests/tests.vh]] \
  [list [file join $srcIpDir simulation/dsport/pcie_2_1_rport_7x.v]] \
  [list [file join $srcIpDir simulation/dsport/xilinx_pcie_2_1_rport_7x.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pci_exp_expect_tasks.vh]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pci_exp_usrapp_cfg.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pci_exp_usrapp_com.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pci_exp_usrapp_pl.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pci_exp_usrapp_rx.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pci_exp_usrapp_tx.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/functional/sys_clk_gen_ds.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/functional/sys_clk_gen.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pcie_axi_trn_bridge.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/functional/board.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/functional/board_common.vh]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/tests/sample_tests1.vh]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/tests/tests.vh]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/pcie_2_1_rport_7x.v]]
set_property USED_IN_SYNTHESIS false [get_files [file join $srcIpDir simulation/dsport/xilinx_pcie_2_1_rport_7x.v]]

# Import all files while preserving hierarchy
import_files

# Set top
set_property TOP [lindex [find_top] 0] [current_fileset]

# Update compile order
update_compile_order -fileset [current_fileset]
update_compile_order -fileset [current_fileset -simset]

set dfile f:/Ryuta/FPGA/KC705_V1.0/HDL/xillybus/vivado-essentials/pcie_k7_vivado/oepdone.txt
set doneFile [open $dfile w]
puts $doneFile "Open Example Project DONE"
close $doneFile
if { $returnCode != 0 } {
  error "Problems were encountered while executing the example design script, please review the log files."
}
