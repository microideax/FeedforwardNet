
################################################################
# This is a generated script based on design: empty
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2017.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_msg_id "BD_TCL-109" "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source empty_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xcvu9p-flgb2104-2-i
   set_property BOARD_PART xilinx.com:f1_cl:part0:1.0 [current_project]
}


# Design name
  set design_name conv_acc

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_msg_id "BD_TCL-001" "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_msg_id "BD_TCL-002" "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_msg_id "BD_TCL-003" "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_msg_id "BD_TCL-004" "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_msg_id "BD_TCL-005" "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_msg_id "BD_TCL-114" "ERROR" $errMsg}
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set S_CTRL_BUS_AXI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_CTRL_BUS_AXI ]
  set_property -dict [ list \
CONFIG.address_width {32} \
CONFIG.ARUSER_WIDTH {0} \
CONFIG.AWUSER_WIDTH {0} \
CONFIG.BUSER_WIDTH {0} \
CONFIG.DATA_WIDTH {32} \
CONFIG.HAS_BRESP {1} \
CONFIG.HAS_BURST {1} \
CONFIG.HAS_CACHE {1} \
CONFIG.HAS_LOCK {1} \
CONFIG.HAS_PROT {1} \
CONFIG.HAS_QOS {0} \
CONFIG.HAS_REGION {0} \
CONFIG.HAS_RRESP {1} \
CONFIG.HAS_WSTRB {1} \
CONFIG.ID_WIDTH {0} \
CONFIG.MAX_BURST_LENGTH {256} \
CONFIG.NUM_READ_OUTSTANDING {2} \
CONFIG.NUM_READ_THREADS {1} \
CONFIG.NUM_WRITE_OUTSTANDING {2} \
CONFIG.NUM_WRITE_THREADS {1} \
CONFIG.PROTOCOL {AXI4LITE} \
CONFIG.READ_WRITE_MODE {READ_WRITE} \
CONFIG.RUSER_BITS_PER_BYTE {0} \
CONFIG.RUSER_WIDTH {0} \
CONFIG.SUPPORTS_NARROW_BURST {1} \
CONFIG.WUSER_BITS_PER_BYTE {0} \
CONFIG.WUSER_WIDTH {0} \
 ] $S_CTRL_BUS_AXI 
  set S_PARAM_AXI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_PARAM_AXI ]
  set_property -dict [ list \
CONFIG.ARUSER_WIDTH {0} \
CONFIG.AWUSER_WIDTH {0} \
CONFIG.BUSER_WIDTH {0} \
CONFIG.DATA_WIDTH {32} \
CONFIG.HAS_BRESP {1} \
CONFIG.HAS_BURST {1} \
CONFIG.HAS_CACHE {1} \
CONFIG.HAS_LOCK {1} \
CONFIG.HAS_PROT {1} \
CONFIG.HAS_QOS {0} \
CONFIG.HAS_REGION {0} \
CONFIG.HAS_RRESP {1} \
CONFIG.HAS_WSTRB {1} \
CONFIG.ID_WIDTH {0} \
CONFIG.MAX_BURST_LENGTH {256} \
CONFIG.NUM_READ_OUTSTANDING {2} \
CONFIG.NUM_READ_THREADS {1} \
CONFIG.NUM_WRITE_OUTSTANDING {2} \
CONFIG.NUM_WRITE_THREADS {1} \
CONFIG.PROTOCOL {AXI4} \
CONFIG.READ_WRITE_MODE {READ_WRITE} \
CONFIG.RUSER_BITS_PER_BYTE {0} \
CONFIG.RUSER_WIDTH {0} \
CONFIG.SUPPORTS_NARROW_BURST {1} \
CONFIG.WUSER_BITS_PER_BYTE {0} \
CONFIG.WUSER_WIDTH {0} \
 ] $S_PARAM_AXI 
  set S_IN_DATA [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_IN_DATA ]
  set_property -dict [ list \
CONFIG.ARUSER_WIDTH {0} \
CONFIG.AWUSER_WIDTH {0} \
CONFIG.BUSER_WIDTH {0} \
CONFIG.DATA_WIDTH {32} \
CONFIG.HAS_BRESP {1} \
CONFIG.HAS_BURST {1} \
CONFIG.HAS_CACHE {1} \
CONFIG.HAS_LOCK {1} \
CONFIG.HAS_PROT {1} \
CONFIG.HAS_QOS {0} \
CONFIG.HAS_REGION {0} \
CONFIG.HAS_RRESP {1} \
CONFIG.HAS_WSTRB {1} \
CONFIG.ID_WIDTH {0} \
CONFIG.MAX_BURST_LENGTH {256} \
CONFIG.NUM_READ_OUTSTANDING {2} \
CONFIG.NUM_READ_THREADS {1} \
CONFIG.NUM_WRITE_OUTSTANDING {2} \
CONFIG.NUM_WRITE_THREADS {1} \
CONFIG.PROTOCOL {AXI4} \
CONFIG.READ_WRITE_MODE {READ_WRITE} \
CONFIG.RUSER_BITS_PER_BYTE {0} \
CONFIG.RUSER_WIDTH {0} \
CONFIG.SUPPORTS_NARROW_BURST {1} \
CONFIG.WUSER_BITS_PER_BYTE {0} \
CONFIG.WUSER_WIDTH {0} \
 ] $S_IN_DATA 
  set S_W_DATA [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_W_DATA ]
  set_property -dict [ list \
CONFIG.ARUSER_WIDTH {0} \
CONFIG.AWUSER_WIDTH {0} \
CONFIG.BUSER_WIDTH {0} \
CONFIG.DATA_WIDTH {32} \
CONFIG.HAS_BRESP {1} \
CONFIG.HAS_BURST {1} \
CONFIG.HAS_CACHE {1} \
CONFIG.HAS_LOCK {1} \
CONFIG.HAS_PROT {1} \
CONFIG.HAS_QOS {0} \
CONFIG.HAS_REGION {0} \
CONFIG.HAS_RRESP {1} \
CONFIG.HAS_WSTRB {1} \
CONFIG.ID_WIDTH {0} \
CONFIG.MAX_BURST_LENGTH {256} \
CONFIG.NUM_READ_OUTSTANDING {2} \
CONFIG.NUM_READ_THREADS {1} \
CONFIG.NUM_WRITE_OUTSTANDING {2} \
CONFIG.NUM_WRITE_THREADS {1} \
CONFIG.PROTOCOL {AXI4} \
CONFIG.READ_WRITE_MODE {READ_WRITE} \
CONFIG.RUSER_BITS_PER_BYTE {0} \
CONFIG.RUSER_WIDTH {0} \
CONFIG.SUPPORTS_NARROW_BURST {1} \
CONFIG.WUSER_BITS_PER_BYTE {0} \
CONFIG.WUSER_WIDTH {0} \
 ] $S_W_DATA 
  set S_OUT_DATA [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_OUT_DATA ]
  set_property -dict [ list \
CONFIG.ARUSER_WIDTH {0} \
CONFIG.AWUSER_WIDTH {0} \
CONFIG.BUSER_WIDTH {0} \
CONFIG.DATA_WIDTH {32} \
CONFIG.HAS_BRESP {1} \
CONFIG.HAS_BURST {1} \
CONFIG.HAS_CACHE {1} \
CONFIG.HAS_LOCK {1} \
CONFIG.HAS_PROT {1} \
CONFIG.HAS_QOS {0} \
CONFIG.HAS_REGION {0} \
CONFIG.HAS_RRESP {1} \
CONFIG.HAS_WSTRB {1} \
CONFIG.ID_WIDTH {0} \
CONFIG.MAX_BURST_LENGTH {256} \
CONFIG.NUM_READ_OUTSTANDING {2} \
CONFIG.NUM_READ_THREADS {1} \
CONFIG.NUM_WRITE_OUTSTANDING {2} \
CONFIG.NUM_WRITE_THREADS {1} \
CONFIG.PROTOCOL {AXI4} \
CONFIG.READ_WRITE_MODE {READ_WRITE} \
CONFIG.RUSER_BITS_PER_BYTE {0} \
CONFIG.RUSER_WIDTH {0} \
CONFIG.SUPPORTS_NARROW_BURST {1} \
CONFIG.WUSER_BITS_PER_BYTE {0} \
CONFIG.WUSER_WIDTH {0} \
 ] $S_OUT_DATA 

  # Create ports
  set ACLK [ create_bd_port -dir I -type clk ACLK ]
  set_property CONFIG.FREQ_HZ 125000000 [get_bd_ports ACLK]
  set ARESETN [ create_bd_port -dir I -type rst ARESETN ]

  # Create instances
  create_bd_cell -type ip -vlnv xilinx.com:hls:conv_mpool_acc_syn:1.0 conv_core_0
  set bias_buf [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 bias_buf ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $bias_buf
  set conv_param [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 conv_param ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $conv_param
  set pool_param [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 pool_param ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $pool_param
  set in_data_mb_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 in_data_mb_0 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $in_data_mb_0
  set in_data_bc_0_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_0
  set in_data_mem_0_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_0
  set in_data_bc_0_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_1
  set in_data_mem_0_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_1
  set in_data_bc_0_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_2
  set in_data_mem_0_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_2
  set in_data_bc_0_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_3
  set in_data_mem_0_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_3
  set in_data_bc_0_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_4
  set in_data_mem_0_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_4
  set in_data_bc_0_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_5
  set in_data_mem_0_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_5
  set in_data_bc_0_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_6
  set in_data_mem_0_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_6
  set in_data_bc_0_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_0_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_0_7
  set in_data_mem_0_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_0_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_0_7
  set in_data_mb_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 in_data_mb_1 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $in_data_mb_1
  set in_data_bc_1_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_0
  set in_data_mem_1_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_0
  set in_data_bc_1_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_1
  set in_data_mem_1_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_1
  set in_data_bc_1_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_2
  set in_data_mem_1_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_2
  set in_data_bc_1_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_3
  set in_data_mem_1_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_3
  set in_data_bc_1_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_4
  set in_data_mem_1_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_4
  set in_data_bc_1_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_5
  set in_data_mem_1_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_5
  set in_data_bc_1_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_6
  set in_data_mem_1_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_6
  set in_data_bc_1_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 in_data_bc_1_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $in_data_bc_1_7
  set in_data_mem_1_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 in_data_mem_1_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $in_data_mem_1_7
  set axi_interconnect_param [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_interconnect_param ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {3} \
  ] $axi_interconnect_param
  set in_buf [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 in_buf ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {2} \
  ] $in_buf
  set axi_bram_ctrl_conv [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_conv ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_conv
  set axi_bram_ctrl_pool [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_pool ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_pool
  set axi_bram_ctrl_bias [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_bias ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_bias
  set w_data_mb_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_0 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_0
  set w_data_bc_0_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_0
  set w_data_mem_0_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_0
  set w_data_bc_0_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_1
  set w_data_mem_0_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_1
  set w_data_bc_0_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_2
  set w_data_mem_0_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_2
  set w_data_bc_0_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_3
  set w_data_mem_0_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_3
  set w_data_bc_0_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_4
  set w_data_mem_0_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_4
  set w_data_bc_0_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_5
  set w_data_mem_0_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_5
  set w_data_bc_0_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_6
  set w_data_mem_0_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_6
  set w_data_bc_0_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_0_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_0_7
  set w_data_mem_0_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_0_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_0_7
  set w_data_mb_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_1 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_1
  set w_data_bc_1_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_0
  set w_data_mem_1_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_0
  set w_data_bc_1_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_1
  set w_data_mem_1_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_1
  set w_data_bc_1_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_2
  set w_data_mem_1_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_2
  set w_data_bc_1_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_3
  set w_data_mem_1_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_3
  set w_data_bc_1_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_4
  set w_data_mem_1_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_4
  set w_data_bc_1_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_5
  set w_data_mem_1_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_5
  set w_data_bc_1_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_6
  set w_data_mem_1_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_6
  set w_data_bc_1_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_1_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_1_7
  set w_data_mem_1_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_1_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_1_7
  set w_data_mb_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_2 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_2
  set w_data_bc_2_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_0
  set w_data_mem_2_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_0
  set w_data_bc_2_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_1
  set w_data_mem_2_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_1
  set w_data_bc_2_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_2
  set w_data_mem_2_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_2
  set w_data_bc_2_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_3
  set w_data_mem_2_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_3
  set w_data_bc_2_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_4
  set w_data_mem_2_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_4
  set w_data_bc_2_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_5
  set w_data_mem_2_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_5
  set w_data_bc_2_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_6
  set w_data_mem_2_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_6
  set w_data_bc_2_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_2_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_2_7
  set w_data_mem_2_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_2_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_2_7
  set w_data_mb_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_3 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_3
  set w_data_bc_3_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_0
  set w_data_mem_3_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_0
  set w_data_bc_3_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_1
  set w_data_mem_3_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_1
  set w_data_bc_3_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_2
  set w_data_mem_3_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_2
  set w_data_bc_3_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_3
  set w_data_mem_3_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_3
  set w_data_bc_3_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_4
  set w_data_mem_3_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_4
  set w_data_bc_3_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_5
  set w_data_mem_3_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_5
  set w_data_bc_3_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_6
  set w_data_mem_3_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_6
  set w_data_bc_3_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_3_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_3_7
  set w_data_mem_3_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_3_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_3_7
  set w_data_mb_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_4 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_4
  set w_data_bc_4_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_0
  set w_data_mem_4_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_0
  set w_data_bc_4_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_1
  set w_data_mem_4_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_1
  set w_data_bc_4_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_2
  set w_data_mem_4_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_2
  set w_data_bc_4_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_3
  set w_data_mem_4_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_3
  set w_data_bc_4_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_4
  set w_data_mem_4_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_4
  set w_data_bc_4_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_5
  set w_data_mem_4_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_5
  set w_data_bc_4_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_6
  set w_data_mem_4_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_6
  set w_data_bc_4_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_4_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_4_7
  set w_data_mem_4_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_4_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_4_7
  set w_data_mb_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_5 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_5
  set w_data_bc_5_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_0
  set w_data_mem_5_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_0
  set w_data_bc_5_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_1
  set w_data_mem_5_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_1
  set w_data_bc_5_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_2
  set w_data_mem_5_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_2
  set w_data_bc_5_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_3
  set w_data_mem_5_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_3
  set w_data_bc_5_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_4
  set w_data_mem_5_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_4
  set w_data_bc_5_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_5
  set w_data_mem_5_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_5
  set w_data_bc_5_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_6
  set w_data_mem_5_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_6
  set w_data_bc_5_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_5_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_5_7
  set w_data_mem_5_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_5_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_5_7
  set w_data_mb_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_6 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_6
  set w_data_bc_6_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_0
  set w_data_mem_6_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_0
  set w_data_bc_6_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_1
  set w_data_mem_6_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_1
  set w_data_bc_6_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_2
  set w_data_mem_6_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_2
  set w_data_bc_6_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_3
  set w_data_mem_6_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_3
  set w_data_bc_6_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_4
  set w_data_mem_6_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_4
  set w_data_bc_6_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_5
  set w_data_mem_6_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_5
  set w_data_bc_6_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_6
  set w_data_mem_6_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_6
  set w_data_bc_6_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_6_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_6_7
  set w_data_mem_6_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_6_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_6_7
  set w_data_mb_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_data_mb_7 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_data_mb_7
  set w_data_bc_7_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_0
  set w_data_mem_7_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_0
  set w_data_bc_7_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_1
  set w_data_mem_7_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_1
  set w_data_bc_7_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_2
  set w_data_mem_7_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_2
  set w_data_bc_7_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_3
  set w_data_mem_7_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_3
  set w_data_bc_7_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_4
  set w_data_mem_7_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_4
  set w_data_bc_7_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_5
  set w_data_mem_7_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_5
  set w_data_bc_7_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_6
  set w_data_mem_7_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_6
  set w_data_bc_7_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 w_data_bc_7_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $w_data_bc_7_7
  set w_data_mem_7_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 w_data_mem_7_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $w_data_mem_7_7
  set w_buf [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 w_buf ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $w_buf
  set out_data_mb_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 out_data_mb_0 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $out_data_mb_0
  set out_data_bc_0_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_0
  set out_data_mem_0_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_0
  set out_data_bc_0_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_1
  set out_data_mem_0_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_1
  set out_data_bc_0_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_2
  set out_data_mem_0_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_2
  set out_data_bc_0_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_3
  set out_data_mem_0_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_3
  set out_data_bc_0_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_4
  set out_data_mem_0_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_4
  set out_data_bc_0_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_5
  set out_data_mem_0_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_5
  set out_data_bc_0_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_6
  set out_data_mem_0_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_6
  set out_data_bc_0_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_0_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_0_7
  set out_data_mem_0_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_0_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_0_7
  set out_data_mb_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 out_data_mb_1 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {8} \
  ] $out_data_mb_1
  set out_data_bc_1_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_0
  set out_data_mem_1_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_0
  set out_data_bc_1_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_1
  set out_data_mem_1_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_1
  set out_data_bc_1_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_2
  set out_data_mem_1_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_2
  set out_data_bc_1_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_3
  set out_data_mem_1_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_3
  set out_data_bc_1_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_4
  set out_data_mem_1_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_4
  set out_data_bc_1_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_5
  set out_data_mem_1_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_5
  set out_data_bc_1_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_6
  set out_data_mem_1_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_6
  set out_data_bc_1_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 out_data_bc_1_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $out_data_bc_1_7
  set out_data_mem_1_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 out_data_mem_1_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $out_data_mem_1_7
  set out_buf [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 out_buf ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {2} \
  ] $out_buf

  # Create interface connections
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_0_PORTA] [get_bd_intf_pins in_data_mem_0_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_1_PORTA] [get_bd_intf_pins in_data_mem_0_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_2_PORTA] [get_bd_intf_pins in_data_mem_0_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_3_PORTA] [get_bd_intf_pins in_data_mem_0_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_4_PORTA] [get_bd_intf_pins in_data_mem_0_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_5_PORTA] [get_bd_intf_pins in_data_mem_0_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_6_PORTA] [get_bd_intf_pins in_data_mem_0_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_0_7_PORTA] [get_bd_intf_pins in_data_mem_0_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_0_PORTA] [get_bd_intf_pins in_data_mem_1_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_1_PORTA] [get_bd_intf_pins in_data_mem_1_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_2_PORTA] [get_bd_intf_pins in_data_mem_1_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_3_PORTA] [get_bd_intf_pins in_data_mem_1_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_4_PORTA] [get_bd_intf_pins in_data_mem_1_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_5_PORTA] [get_bd_intf_pins in_data_mem_1_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_6_PORTA] [get_bd_intf_pins in_data_mem_1_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/in_buf_1_7_PORTA] [get_bd_intf_pins in_data_mem_1_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_0/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_1/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_2/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_3/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_4/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_5/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_6/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_7/BRAM_PORTA] [get_bd_intf_pins in_data_mem_0_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_0/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_1/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_2/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_3/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_4/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_5/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_6/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_7/BRAM_PORTA] [get_bd_intf_pins in_data_mem_1_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_0/S_AXI] [get_bd_intf_pins in_data_mb_0/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_1/S_AXI] [get_bd_intf_pins in_data_mb_0/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_2/S_AXI] [get_bd_intf_pins in_data_mb_0/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_3/S_AXI] [get_bd_intf_pins in_data_mb_0/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_4/S_AXI] [get_bd_intf_pins in_data_mb_0/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_5/S_AXI] [get_bd_intf_pins in_data_mb_0/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_6/S_AXI] [get_bd_intf_pins in_data_mb_0/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_0_7/S_AXI] [get_bd_intf_pins in_data_mb_0/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_0/S_AXI] [get_bd_intf_pins in_data_mb_1/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_1/S_AXI] [get_bd_intf_pins in_data_mb_1/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_2/S_AXI] [get_bd_intf_pins in_data_mb_1/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_3/S_AXI] [get_bd_intf_pins in_data_mb_1/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_4/S_AXI] [get_bd_intf_pins in_data_mb_1/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_5/S_AXI] [get_bd_intf_pins in_data_mb_1/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_6/S_AXI] [get_bd_intf_pins in_data_mb_1/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_data_bc_1_7/S_AXI] [get_bd_intf_pins in_data_mb_1/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_buf/M00_AXI] -boundary_type upper [get_bd_intf_pins in_data_mb_0/S00_AXI]
  connect_bd_intf_net [get_bd_intf_pins in_buf/M01_AXI] -boundary_type upper [get_bd_intf_pins in_data_mb_1/S00_AXI]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_0_PORTA] [get_bd_intf_pins w_data_mem_0_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_1_PORTA] [get_bd_intf_pins w_data_mem_0_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_2_PORTA] [get_bd_intf_pins w_data_mem_0_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_3_PORTA] [get_bd_intf_pins w_data_mem_0_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_4_PORTA] [get_bd_intf_pins w_data_mem_0_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_5_PORTA] [get_bd_intf_pins w_data_mem_0_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_6_PORTA] [get_bd_intf_pins w_data_mem_0_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_0_7_PORTA] [get_bd_intf_pins w_data_mem_0_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_0_PORTA] [get_bd_intf_pins w_data_mem_1_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_1_PORTA] [get_bd_intf_pins w_data_mem_1_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_2_PORTA] [get_bd_intf_pins w_data_mem_1_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_3_PORTA] [get_bd_intf_pins w_data_mem_1_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_4_PORTA] [get_bd_intf_pins w_data_mem_1_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_5_PORTA] [get_bd_intf_pins w_data_mem_1_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_6_PORTA] [get_bd_intf_pins w_data_mem_1_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_1_7_PORTA] [get_bd_intf_pins w_data_mem_1_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_0_PORTA] [get_bd_intf_pins w_data_mem_2_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_1_PORTA] [get_bd_intf_pins w_data_mem_2_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_2_PORTA] [get_bd_intf_pins w_data_mem_2_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_3_PORTA] [get_bd_intf_pins w_data_mem_2_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_4_PORTA] [get_bd_intf_pins w_data_mem_2_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_5_PORTA] [get_bd_intf_pins w_data_mem_2_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_6_PORTA] [get_bd_intf_pins w_data_mem_2_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_2_7_PORTA] [get_bd_intf_pins w_data_mem_2_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_0_PORTA] [get_bd_intf_pins w_data_mem_3_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_1_PORTA] [get_bd_intf_pins w_data_mem_3_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_2_PORTA] [get_bd_intf_pins w_data_mem_3_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_3_PORTA] [get_bd_intf_pins w_data_mem_3_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_4_PORTA] [get_bd_intf_pins w_data_mem_3_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_5_PORTA] [get_bd_intf_pins w_data_mem_3_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_6_PORTA] [get_bd_intf_pins w_data_mem_3_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_3_7_PORTA] [get_bd_intf_pins w_data_mem_3_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_0_PORTA] [get_bd_intf_pins w_data_mem_4_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_1_PORTA] [get_bd_intf_pins w_data_mem_4_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_2_PORTA] [get_bd_intf_pins w_data_mem_4_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_3_PORTA] [get_bd_intf_pins w_data_mem_4_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_4_PORTA] [get_bd_intf_pins w_data_mem_4_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_5_PORTA] [get_bd_intf_pins w_data_mem_4_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_6_PORTA] [get_bd_intf_pins w_data_mem_4_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_4_7_PORTA] [get_bd_intf_pins w_data_mem_4_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_0_PORTA] [get_bd_intf_pins w_data_mem_5_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_1_PORTA] [get_bd_intf_pins w_data_mem_5_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_2_PORTA] [get_bd_intf_pins w_data_mem_5_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_3_PORTA] [get_bd_intf_pins w_data_mem_5_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_4_PORTA] [get_bd_intf_pins w_data_mem_5_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_5_PORTA] [get_bd_intf_pins w_data_mem_5_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_6_PORTA] [get_bd_intf_pins w_data_mem_5_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_5_7_PORTA] [get_bd_intf_pins w_data_mem_5_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_0_PORTA] [get_bd_intf_pins w_data_mem_6_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_1_PORTA] [get_bd_intf_pins w_data_mem_6_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_2_PORTA] [get_bd_intf_pins w_data_mem_6_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_3_PORTA] [get_bd_intf_pins w_data_mem_6_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_4_PORTA] [get_bd_intf_pins w_data_mem_6_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_5_PORTA] [get_bd_intf_pins w_data_mem_6_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_6_PORTA] [get_bd_intf_pins w_data_mem_6_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_6_7_PORTA] [get_bd_intf_pins w_data_mem_6_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_0_PORTA] [get_bd_intf_pins w_data_mem_7_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_1_PORTA] [get_bd_intf_pins w_data_mem_7_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_2_PORTA] [get_bd_intf_pins w_data_mem_7_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_3_PORTA] [get_bd_intf_pins w_data_mem_7_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_4_PORTA] [get_bd_intf_pins w_data_mem_7_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_5_PORTA] [get_bd_intf_pins w_data_mem_7_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_6_PORTA] [get_bd_intf_pins w_data_mem_7_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/w_buf_0_7_7_PORTA] [get_bd_intf_pins w_data_mem_7_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_0_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_1_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_2_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_3_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_4_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_5_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_6_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_0/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_1/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_2/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_3/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_4/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_5/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_6/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_7/BRAM_PORTA] [get_bd_intf_pins w_data_mem_7_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_0/S_AXI] [get_bd_intf_pins w_data_mb_0/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_1/S_AXI] [get_bd_intf_pins w_data_mb_0/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_2/S_AXI] [get_bd_intf_pins w_data_mb_0/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_3/S_AXI] [get_bd_intf_pins w_data_mb_0/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_4/S_AXI] [get_bd_intf_pins w_data_mb_0/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_5/S_AXI] [get_bd_intf_pins w_data_mb_0/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_6/S_AXI] [get_bd_intf_pins w_data_mb_0/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_0_7/S_AXI] [get_bd_intf_pins w_data_mb_0/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_0/S_AXI] [get_bd_intf_pins w_data_mb_1/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_1/S_AXI] [get_bd_intf_pins w_data_mb_1/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_2/S_AXI] [get_bd_intf_pins w_data_mb_1/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_3/S_AXI] [get_bd_intf_pins w_data_mb_1/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_4/S_AXI] [get_bd_intf_pins w_data_mb_1/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_5/S_AXI] [get_bd_intf_pins w_data_mb_1/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_6/S_AXI] [get_bd_intf_pins w_data_mb_1/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_1_7/S_AXI] [get_bd_intf_pins w_data_mb_1/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_0/S_AXI] [get_bd_intf_pins w_data_mb_2/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_1/S_AXI] [get_bd_intf_pins w_data_mb_2/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_2/S_AXI] [get_bd_intf_pins w_data_mb_2/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_3/S_AXI] [get_bd_intf_pins w_data_mb_2/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_4/S_AXI] [get_bd_intf_pins w_data_mb_2/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_5/S_AXI] [get_bd_intf_pins w_data_mb_2/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_6/S_AXI] [get_bd_intf_pins w_data_mb_2/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_2_7/S_AXI] [get_bd_intf_pins w_data_mb_2/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_0/S_AXI] [get_bd_intf_pins w_data_mb_3/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_1/S_AXI] [get_bd_intf_pins w_data_mb_3/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_2/S_AXI] [get_bd_intf_pins w_data_mb_3/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_3/S_AXI] [get_bd_intf_pins w_data_mb_3/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_4/S_AXI] [get_bd_intf_pins w_data_mb_3/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_5/S_AXI] [get_bd_intf_pins w_data_mb_3/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_6/S_AXI] [get_bd_intf_pins w_data_mb_3/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_3_7/S_AXI] [get_bd_intf_pins w_data_mb_3/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_0/S_AXI] [get_bd_intf_pins w_data_mb_4/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_1/S_AXI] [get_bd_intf_pins w_data_mb_4/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_2/S_AXI] [get_bd_intf_pins w_data_mb_4/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_3/S_AXI] [get_bd_intf_pins w_data_mb_4/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_4/S_AXI] [get_bd_intf_pins w_data_mb_4/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_5/S_AXI] [get_bd_intf_pins w_data_mb_4/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_6/S_AXI] [get_bd_intf_pins w_data_mb_4/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_4_7/S_AXI] [get_bd_intf_pins w_data_mb_4/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_0/S_AXI] [get_bd_intf_pins w_data_mb_5/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_1/S_AXI] [get_bd_intf_pins w_data_mb_5/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_2/S_AXI] [get_bd_intf_pins w_data_mb_5/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_3/S_AXI] [get_bd_intf_pins w_data_mb_5/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_4/S_AXI] [get_bd_intf_pins w_data_mb_5/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_5/S_AXI] [get_bd_intf_pins w_data_mb_5/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_6/S_AXI] [get_bd_intf_pins w_data_mb_5/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_5_7/S_AXI] [get_bd_intf_pins w_data_mb_5/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_0/S_AXI] [get_bd_intf_pins w_data_mb_6/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_1/S_AXI] [get_bd_intf_pins w_data_mb_6/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_2/S_AXI] [get_bd_intf_pins w_data_mb_6/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_3/S_AXI] [get_bd_intf_pins w_data_mb_6/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_4/S_AXI] [get_bd_intf_pins w_data_mb_6/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_5/S_AXI] [get_bd_intf_pins w_data_mb_6/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_6/S_AXI] [get_bd_intf_pins w_data_mb_6/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_6_7/S_AXI] [get_bd_intf_pins w_data_mb_6/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_0/S_AXI] [get_bd_intf_pins w_data_mb_7/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_1/S_AXI] [get_bd_intf_pins w_data_mb_7/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_2/S_AXI] [get_bd_intf_pins w_data_mb_7/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_3/S_AXI] [get_bd_intf_pins w_data_mb_7/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_4/S_AXI] [get_bd_intf_pins w_data_mb_7/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_5/S_AXI] [get_bd_intf_pins w_data_mb_7/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_6/S_AXI] [get_bd_intf_pins w_data_mb_7/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins w_data_bc_7_7/S_AXI] [get_bd_intf_pins w_data_mb_7/M07_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M00_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_0/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M01_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_1/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M02_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_2/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M03_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_3/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M04_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_4/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M05_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_5/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M06_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_6/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins w_buf/M07_AXI] -boundary_type upper [get_bd_intf_pins w_data_mb_7/S00_AXI]
  connect_bd_intf_net [get_bd_intf_ports S_W_DATA] -boundary_type upper [get_bd_intf_pins w_buf/S00_AXI]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_0_PORTA] [get_bd_intf_pins out_data_mem_0_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_1_PORTA] [get_bd_intf_pins out_data_mem_0_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_2_PORTA] [get_bd_intf_pins out_data_mem_0_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_3_PORTA] [get_bd_intf_pins out_data_mem_0_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_4_PORTA] [get_bd_intf_pins out_data_mem_0_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_5_PORTA] [get_bd_intf_pins out_data_mem_0_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_6_PORTA] [get_bd_intf_pins out_data_mem_0_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_0_7_PORTA] [get_bd_intf_pins out_data_mem_0_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_0_PORTA] [get_bd_intf_pins out_data_mem_1_0/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_1_PORTA] [get_bd_intf_pins out_data_mem_1_1/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_2_PORTA] [get_bd_intf_pins out_data_mem_1_2/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_3_PORTA] [get_bd_intf_pins out_data_mem_1_3/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_4_PORTA] [get_bd_intf_pins out_data_mem_1_4/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_5_PORTA] [get_bd_intf_pins out_data_mem_1_5/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_6_PORTA] [get_bd_intf_pins out_data_mem_1_6/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/out_buf_1_7_PORTA] [get_bd_intf_pins out_data_mem_1_7/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_0/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_1/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_2/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_3/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_4/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_5/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_6/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_7/BRAM_PORTA] [get_bd_intf_pins out_data_mem_0_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_0/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_0/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_1/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_1/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_2/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_2/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_3/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_3/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_4/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_4/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_5/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_5/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_6/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_6/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_7/BRAM_PORTA] [get_bd_intf_pins out_data_mem_1_7/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_0/S_AXI] [get_bd_intf_pins out_data_mb_0/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_1/S_AXI] [get_bd_intf_pins out_data_mb_0/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_2/S_AXI] [get_bd_intf_pins out_data_mb_0/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_3/S_AXI] [get_bd_intf_pins out_data_mb_0/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_4/S_AXI] [get_bd_intf_pins out_data_mb_0/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_5/S_AXI] [get_bd_intf_pins out_data_mb_0/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_6/S_AXI] [get_bd_intf_pins out_data_mb_0/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_0_7/S_AXI] [get_bd_intf_pins out_data_mb_0/M07_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_0/S_AXI] [get_bd_intf_pins out_data_mb_1/M00_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_1/S_AXI] [get_bd_intf_pins out_data_mb_1/M01_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_2/S_AXI] [get_bd_intf_pins out_data_mb_1/M02_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_3/S_AXI] [get_bd_intf_pins out_data_mb_1/M03_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_4/S_AXI] [get_bd_intf_pins out_data_mb_1/M04_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_5/S_AXI] [get_bd_intf_pins out_data_mb_1/M05_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_6/S_AXI] [get_bd_intf_pins out_data_mb_1/M06_AXI]
  connect_bd_intf_net [get_bd_intf_pins out_data_bc_1_7/S_AXI] [get_bd_intf_pins out_data_mb_1/M07_AXI]
 connect_bd_intf_net [get_bd_intf_pins out_buf/M00_AXI] -boundary_type upper [get_bd_intf_pins out_data_mb_0/S00_AXI]
 connect_bd_intf_net [get_bd_intf_pins out_buf/M01_AXI] -boundary_type upper [get_bd_intf_pins out_data_mb_1/S00_AXI]
  connect_bd_intf_net [get_bd_intf_ports S_OUT_DATA] -boundary_type upper [get_bd_intf_pins out_buf/S00_AXI]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/b_buf_0_PORTA] [get_bd_intf_pins bias_buf/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/conv_param_PORTA] [get_bd_intf_pins conv_param/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_pins conv_core_0/pool_param_PORTA] [get_bd_intf_pins pool_param/BRAM_PORTA]
  connect_bd_intf_net [get_bd_intf_ports S_CTRL_BUS_AXI] [get_bd_intf_pins conv_core_0/s_axi_CRTL_BUS]
  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_param/M00_AXI] [get_bd_intf_pins axi_bram_ctrl_conv/S_AXI]
  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_param/M01_AXI] [get_bd_intf_pins axi_bram_ctrl_pool/S_AXI]
  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_param/M02_AXI] [get_bd_intf_pins axi_bram_ctrl_bias/S_AXI]
  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_bias/BRAM_PORTA] [get_bd_intf_pins bias_buf/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_conv/BRAM_PORTA] [get_bd_intf_pins conv_param/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_pool/BRAM_PORTA] [get_bd_intf_pins pool_param/BRAM_PORTB]
  connect_bd_intf_net [get_bd_intf_ports S_PARAM_AXI] -boundary_type upper [get_bd_intf_pins axi_interconnect_param/S00_AXI]
  connect_bd_intf_net [get_bd_intf_ports S_IN_DATA] -boundary_type upper [get_bd_intf_pins in_buf/S00_AXI]

  # Create port connections
  connect_bd_net -net ACLK_1 [get_bd_ports ACLK]\
\
[get_bd_pin axi_bram_ctrl_bias/s_axi_aclk]\
[get_bd_pin axi_bram_ctrl_conv/s_axi_aclk]\
[get_bd_pin axi_bram_ctrl_pool/s_axi_aclk]\
[get_bd_pin conv_core_0/ap_clk]\
[get_bd_pin axi_interconnect_param/ACLK]\
[get_bd_pin axi_interconnect_param/S00_ACLK]\
[get_bd_pin axi_interconnect_param/M00_ACLK]\
[get_bd_pin axi_interconnect_param/M01_ACLK]\
[get_bd_pin axi_interconnect_param/M02_ACLK]\
[get_bd_pin in_buf/ACLK]\
[get_bd_pin in_buf/S00_ACLK]\
[get_bd_pin in_buf/M00_ACLK]\
[get_bd_pin in_buf/M01_ACLK]\
[get_bd_pin in_data_mb_0/ACLK]\
[get_bd_pin in_data_mb_0/S00_ACLK]\
[get_bd_pin in_data_bc_0_0/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M00_ACLK]\
[get_bd_pin in_data_bc_0_1/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M01_ACLK]\
[get_bd_pin in_data_bc_0_2/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M02_ACLK]\
[get_bd_pin in_data_bc_0_3/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M03_ACLK]\
[get_bd_pin in_data_bc_0_4/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M04_ACLK]\
[get_bd_pin in_data_bc_0_5/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M05_ACLK]\
[get_bd_pin in_data_bc_0_6/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M06_ACLK]\
[get_bd_pin in_data_bc_0_7/s_axi_aclk]\
[get_bd_pin in_data_mb_0/M07_ACLK]\
[get_bd_pin in_data_mb_1/ACLK]\
[get_bd_pin in_data_mb_1/S00_ACLK]\
[get_bd_pin in_data_bc_1_0/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M00_ACLK]\
[get_bd_pin in_data_bc_1_1/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M01_ACLK]\
[get_bd_pin in_data_bc_1_2/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M02_ACLK]\
[get_bd_pin in_data_bc_1_3/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M03_ACLK]\
[get_bd_pin in_data_bc_1_4/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M04_ACLK]\
[get_bd_pin in_data_bc_1_5/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M05_ACLK]\
[get_bd_pin in_data_bc_1_6/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M06_ACLK]\
[get_bd_pin in_data_bc_1_7/s_axi_aclk]\
[get_bd_pin in_data_mb_1/M07_ACLK]\
[get_bd_pin w_buf/ACLK]\
[get_bd_pin w_buf/S00_ACLK]\
[get_bd_pin w_buf/M00_ACLK]\
[get_bd_pin w_buf/M01_ACLK]\
[get_bd_pin w_buf/M02_ACLK]\
[get_bd_pin w_buf/M03_ACLK]\
[get_bd_pin w_buf/M04_ACLK]\
[get_bd_pin w_buf/M05_ACLK]\
[get_bd_pin w_buf/M06_ACLK]\
[get_bd_pin w_buf/M07_ACLK]\
[get_bd_pin w_data_mb_0/ACLK]\
[get_bd_pin w_data_mb_0/S00_ACLK]\
[get_bd_pin w_data_bc_0_0/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M00_ACLK]\
[get_bd_pin w_data_bc_0_1/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M01_ACLK]\
[get_bd_pin w_data_bc_0_2/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M02_ACLK]\
[get_bd_pin w_data_bc_0_3/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M03_ACLK]\
[get_bd_pin w_data_bc_0_4/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M04_ACLK]\
[get_bd_pin w_data_bc_0_5/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M05_ACLK]\
[get_bd_pin w_data_bc_0_6/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M06_ACLK]\
[get_bd_pin w_data_bc_0_7/s_axi_aclk]\
[get_bd_pin w_data_mb_0/M07_ACLK]\
[get_bd_pin w_data_mb_1/ACLK]\
[get_bd_pin w_data_mb_1/S00_ACLK]\
[get_bd_pin w_data_bc_1_0/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M00_ACLK]\
[get_bd_pin w_data_bc_1_1/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M01_ACLK]\
[get_bd_pin w_data_bc_1_2/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M02_ACLK]\
[get_bd_pin w_data_bc_1_3/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M03_ACLK]\
[get_bd_pin w_data_bc_1_4/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M04_ACLK]\
[get_bd_pin w_data_bc_1_5/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M05_ACLK]\
[get_bd_pin w_data_bc_1_6/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M06_ACLK]\
[get_bd_pin w_data_bc_1_7/s_axi_aclk]\
[get_bd_pin w_data_mb_1/M07_ACLK]\
[get_bd_pin w_data_mb_2/ACLK]\
[get_bd_pin w_data_mb_2/S00_ACLK]\
[get_bd_pin w_data_bc_2_0/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M00_ACLK]\
[get_bd_pin w_data_bc_2_1/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M01_ACLK]\
[get_bd_pin w_data_bc_2_2/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M02_ACLK]\
[get_bd_pin w_data_bc_2_3/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M03_ACLK]\
[get_bd_pin w_data_bc_2_4/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M04_ACLK]\
[get_bd_pin w_data_bc_2_5/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M05_ACLK]\
[get_bd_pin w_data_bc_2_6/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M06_ACLK]\
[get_bd_pin w_data_bc_2_7/s_axi_aclk]\
[get_bd_pin w_data_mb_2/M07_ACLK]\
[get_bd_pin w_data_mb_3/ACLK]\
[get_bd_pin w_data_mb_3/S00_ACLK]\
[get_bd_pin w_data_bc_3_0/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M00_ACLK]\
[get_bd_pin w_data_bc_3_1/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M01_ACLK]\
[get_bd_pin w_data_bc_3_2/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M02_ACLK]\
[get_bd_pin w_data_bc_3_3/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M03_ACLK]\
[get_bd_pin w_data_bc_3_4/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M04_ACLK]\
[get_bd_pin w_data_bc_3_5/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M05_ACLK]\
[get_bd_pin w_data_bc_3_6/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M06_ACLK]\
[get_bd_pin w_data_bc_3_7/s_axi_aclk]\
[get_bd_pin w_data_mb_3/M07_ACLK]\
[get_bd_pin w_data_mb_4/ACLK]\
[get_bd_pin w_data_mb_4/S00_ACLK]\
[get_bd_pin w_data_bc_4_0/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M00_ACLK]\
[get_bd_pin w_data_bc_4_1/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M01_ACLK]\
[get_bd_pin w_data_bc_4_2/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M02_ACLK]\
[get_bd_pin w_data_bc_4_3/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M03_ACLK]\
[get_bd_pin w_data_bc_4_4/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M04_ACLK]\
[get_bd_pin w_data_bc_4_5/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M05_ACLK]\
[get_bd_pin w_data_bc_4_6/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M06_ACLK]\
[get_bd_pin w_data_bc_4_7/s_axi_aclk]\
[get_bd_pin w_data_mb_4/M07_ACLK]\
[get_bd_pin w_data_mb_5/ACLK]\
[get_bd_pin w_data_mb_5/S00_ACLK]\
[get_bd_pin w_data_bc_5_0/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M00_ACLK]\
[get_bd_pin w_data_bc_5_1/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M01_ACLK]\
[get_bd_pin w_data_bc_5_2/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M02_ACLK]\
[get_bd_pin w_data_bc_5_3/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M03_ACLK]\
[get_bd_pin w_data_bc_5_4/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M04_ACLK]\
[get_bd_pin w_data_bc_5_5/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M05_ACLK]\
[get_bd_pin w_data_bc_5_6/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M06_ACLK]\
[get_bd_pin w_data_bc_5_7/s_axi_aclk]\
[get_bd_pin w_data_mb_5/M07_ACLK]\
[get_bd_pin w_data_mb_6/ACLK]\
[get_bd_pin w_data_mb_6/S00_ACLK]\
[get_bd_pin w_data_bc_6_0/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M00_ACLK]\
[get_bd_pin w_data_bc_6_1/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M01_ACLK]\
[get_bd_pin w_data_bc_6_2/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M02_ACLK]\
[get_bd_pin w_data_bc_6_3/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M03_ACLK]\
[get_bd_pin w_data_bc_6_4/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M04_ACLK]\
[get_bd_pin w_data_bc_6_5/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M05_ACLK]\
[get_bd_pin w_data_bc_6_6/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M06_ACLK]\
[get_bd_pin w_data_bc_6_7/s_axi_aclk]\
[get_bd_pin w_data_mb_6/M07_ACLK]\
[get_bd_pin w_data_mb_7/ACLK]\
[get_bd_pin w_data_mb_7/S00_ACLK]\
[get_bd_pin w_data_bc_7_0/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M00_ACLK]\
[get_bd_pin w_data_bc_7_1/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M01_ACLK]\
[get_bd_pin w_data_bc_7_2/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M02_ACLK]\
[get_bd_pin w_data_bc_7_3/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M03_ACLK]\
[get_bd_pin w_data_bc_7_4/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M04_ACLK]\
[get_bd_pin w_data_bc_7_5/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M05_ACLK]\
[get_bd_pin w_data_bc_7_6/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M06_ACLK]\
[get_bd_pin w_data_bc_7_7/s_axi_aclk]\
[get_bd_pin w_data_mb_7/M07_ACLK]\
[get_bd_pin out_buf/ACLK]\
[get_bd_pin out_buf/S00_ACLK]\
[get_bd_pin out_buf/M00_ACLK]\
[get_bd_pin out_buf/M01_ACLK]\
[get_bd_pin out_data_mb_0/ACLK]\
[get_bd_pin out_data_mb_0/S00_ACLK]\
[get_bd_pin out_data_bc_0_0/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M00_ACLK]\
[get_bd_pin out_data_bc_0_1/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M01_ACLK]\
[get_bd_pin out_data_bc_0_2/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M02_ACLK]\
[get_bd_pin out_data_bc_0_3/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M03_ACLK]\
[get_bd_pin out_data_bc_0_4/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M04_ACLK]\
[get_bd_pin out_data_bc_0_5/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M05_ACLK]\
[get_bd_pin out_data_bc_0_6/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M06_ACLK]\
[get_bd_pin out_data_bc_0_7/s_axi_aclk]\
[get_bd_pin out_data_mb_0/M07_ACLK]\
[get_bd_pin out_data_mb_1/ACLK]\
[get_bd_pin out_data_mb_1/S00_ACLK]\
[get_bd_pin out_data_bc_1_0/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M00_ACLK]\
[get_bd_pin out_data_bc_1_1/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M01_ACLK]\
[get_bd_pin out_data_bc_1_2/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M02_ACLK]\
[get_bd_pin out_data_bc_1_3/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M03_ACLK]\
[get_bd_pin out_data_bc_1_4/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M04_ACLK]\
[get_bd_pin out_data_bc_1_5/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M05_ACLK]\
[get_bd_pin out_data_bc_1_6/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M06_ACLK]\
[get_bd_pin out_data_bc_1_7/s_axi_aclk]\
[get_bd_pin out_data_mb_1/M07_ACLK]


  connect_bd_net -net ARESETN_1 [get_bd_ports ARESETN]\
\
[get_bd_pin axi_bram_ctrl_bias/s_axi_aresetn]\
[get_bd_pin axi_bram_ctrl_conv/s_axi_aresetn]\
[get_bd_pin axi_bram_ctrl_pool/s_axi_aresetn]\
[get_bd_pin conv_core_0/ap_rst_n]\
[get_bd_pin axi_interconnect_param/ARESETN]\
[get_bd_pin axi_interconnect_param/S00_ARESETN]\
[get_bd_pin axi_interconnect_param/M00_ARESETN]\
[get_bd_pin axi_interconnect_param/M01_ARESETN]\
[get_bd_pin axi_interconnect_param/M02_ARESETN]\
[get_bd_pin in_buf/ARESETN]\
[get_bd_pin in_buf/S00_ARESETN]\
[get_bd_pin in_buf/M00_ARESETN]\
[get_bd_pin in_buf/M01_ARESETN]\
[get_bd_pin in_data_mb_0/ARESETN]\
[get_bd_pin in_data_mb_0/S00_ARESETN]\
[get_bd_pin in_data_bc_0_0/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M00_ARESETN]\
[get_bd_pin in_data_bc_0_1/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M01_ARESETN]\
[get_bd_pin in_data_bc_0_2/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M02_ARESETN]\
[get_bd_pin in_data_bc_0_3/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M03_ARESETN]\
[get_bd_pin in_data_bc_0_4/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M04_ARESETN]\
[get_bd_pin in_data_bc_0_5/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M05_ARESETN]\
[get_bd_pin in_data_bc_0_6/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M06_ARESETN]\
[get_bd_pin in_data_bc_0_7/s_axi_aresetn]\
[get_bd_pin in_data_mb_0/M07_ARESETN]\
[get_bd_pin in_data_mb_1/ARESETN]\
[get_bd_pin in_data_mb_1/S00_ARESETN]\
[get_bd_pin in_data_bc_1_0/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M00_ARESETN]\
[get_bd_pin in_data_bc_1_1/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M01_ARESETN]\
[get_bd_pin in_data_bc_1_2/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M02_ARESETN]\
[get_bd_pin in_data_bc_1_3/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M03_ARESETN]\
[get_bd_pin in_data_bc_1_4/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M04_ARESETN]\
[get_bd_pin in_data_bc_1_5/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M05_ARESETN]\
[get_bd_pin in_data_bc_1_6/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M06_ARESETN]\
[get_bd_pin in_data_bc_1_7/s_axi_aresetn]\
[get_bd_pin in_data_mb_1/M07_ARESETN]\
[get_bd_pin w_buf/ARESETN]\
[get_bd_pin w_buf/S00_ARESETN]\
[get_bd_pin w_buf/M00_ARESETN]\
[get_bd_pin w_buf/M01_ARESETN]\
[get_bd_pin w_buf/M02_ARESETN]\
[get_bd_pin w_buf/M03_ARESETN]\
[get_bd_pin w_buf/M04_ARESETN]\
[get_bd_pin w_buf/M05_ARESETN]\
[get_bd_pin w_buf/M06_ARESETN]\
[get_bd_pin w_buf/M07_ARESETN]\
[get_bd_pin w_data_mb_0/ARESETN]\
[get_bd_pin w_data_mb_0/S00_ARESETN]\
[get_bd_pin w_data_bc_0_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M00_ARESETN]\
[get_bd_pin w_data_bc_0_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M01_ARESETN]\
[get_bd_pin w_data_bc_0_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M02_ARESETN]\
[get_bd_pin w_data_bc_0_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M03_ARESETN]\
[get_bd_pin w_data_bc_0_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M04_ARESETN]\
[get_bd_pin w_data_bc_0_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M05_ARESETN]\
[get_bd_pin w_data_bc_0_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M06_ARESETN]\
[get_bd_pin w_data_bc_0_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_0/M07_ARESETN]\
[get_bd_pin w_data_mb_1/ARESETN]\
[get_bd_pin w_data_mb_1/S00_ARESETN]\
[get_bd_pin w_data_bc_1_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M00_ARESETN]\
[get_bd_pin w_data_bc_1_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M01_ARESETN]\
[get_bd_pin w_data_bc_1_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M02_ARESETN]\
[get_bd_pin w_data_bc_1_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M03_ARESETN]\
[get_bd_pin w_data_bc_1_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M04_ARESETN]\
[get_bd_pin w_data_bc_1_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M05_ARESETN]\
[get_bd_pin w_data_bc_1_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M06_ARESETN]\
[get_bd_pin w_data_bc_1_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_1/M07_ARESETN]\
[get_bd_pin w_data_mb_2/ARESETN]\
[get_bd_pin w_data_mb_2/S00_ARESETN]\
[get_bd_pin w_data_bc_2_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M00_ARESETN]\
[get_bd_pin w_data_bc_2_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M01_ARESETN]\
[get_bd_pin w_data_bc_2_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M02_ARESETN]\
[get_bd_pin w_data_bc_2_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M03_ARESETN]\
[get_bd_pin w_data_bc_2_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M04_ARESETN]\
[get_bd_pin w_data_bc_2_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M05_ARESETN]\
[get_bd_pin w_data_bc_2_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M06_ARESETN]\
[get_bd_pin w_data_bc_2_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_2/M07_ARESETN]\
[get_bd_pin w_data_mb_3/ARESETN]\
[get_bd_pin w_data_mb_3/S00_ARESETN]\
[get_bd_pin w_data_bc_3_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M00_ARESETN]\
[get_bd_pin w_data_bc_3_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M01_ARESETN]\
[get_bd_pin w_data_bc_3_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M02_ARESETN]\
[get_bd_pin w_data_bc_3_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M03_ARESETN]\
[get_bd_pin w_data_bc_3_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M04_ARESETN]\
[get_bd_pin w_data_bc_3_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M05_ARESETN]\
[get_bd_pin w_data_bc_3_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M06_ARESETN]\
[get_bd_pin w_data_bc_3_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_3/M07_ARESETN]\
[get_bd_pin w_data_mb_4/ARESETN]\
[get_bd_pin w_data_mb_4/S00_ARESETN]\
[get_bd_pin w_data_bc_4_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M00_ARESETN]\
[get_bd_pin w_data_bc_4_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M01_ARESETN]\
[get_bd_pin w_data_bc_4_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M02_ARESETN]\
[get_bd_pin w_data_bc_4_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M03_ARESETN]\
[get_bd_pin w_data_bc_4_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M04_ARESETN]\
[get_bd_pin w_data_bc_4_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M05_ARESETN]\
[get_bd_pin w_data_bc_4_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M06_ARESETN]\
[get_bd_pin w_data_bc_4_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_4/M07_ARESETN]\
[get_bd_pin w_data_mb_5/ARESETN]\
[get_bd_pin w_data_mb_5/S00_ARESETN]\
[get_bd_pin w_data_bc_5_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M00_ARESETN]\
[get_bd_pin w_data_bc_5_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M01_ARESETN]\
[get_bd_pin w_data_bc_5_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M02_ARESETN]\
[get_bd_pin w_data_bc_5_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M03_ARESETN]\
[get_bd_pin w_data_bc_5_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M04_ARESETN]\
[get_bd_pin w_data_bc_5_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M05_ARESETN]\
[get_bd_pin w_data_bc_5_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M06_ARESETN]\
[get_bd_pin w_data_bc_5_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_5/M07_ARESETN]\
[get_bd_pin w_data_mb_6/ARESETN]\
[get_bd_pin w_data_mb_6/S00_ARESETN]\
[get_bd_pin w_data_bc_6_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M00_ARESETN]\
[get_bd_pin w_data_bc_6_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M01_ARESETN]\
[get_bd_pin w_data_bc_6_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M02_ARESETN]\
[get_bd_pin w_data_bc_6_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M03_ARESETN]\
[get_bd_pin w_data_bc_6_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M04_ARESETN]\
[get_bd_pin w_data_bc_6_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M05_ARESETN]\
[get_bd_pin w_data_bc_6_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M06_ARESETN]\
[get_bd_pin w_data_bc_6_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_6/M07_ARESETN]\
[get_bd_pin w_data_mb_7/ARESETN]\
[get_bd_pin w_data_mb_7/S00_ARESETN]\
[get_bd_pin w_data_bc_7_0/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M00_ARESETN]\
[get_bd_pin w_data_bc_7_1/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M01_ARESETN]\
[get_bd_pin w_data_bc_7_2/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M02_ARESETN]\
[get_bd_pin w_data_bc_7_3/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M03_ARESETN]\
[get_bd_pin w_data_bc_7_4/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M04_ARESETN]\
[get_bd_pin w_data_bc_7_5/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M05_ARESETN]\
[get_bd_pin w_data_bc_7_6/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M06_ARESETN]\
[get_bd_pin w_data_bc_7_7/s_axi_aresetn]\
[get_bd_pin w_data_mb_7/M07_ARESETN]\
[get_bd_pin out_buf/ARESETN]\
[get_bd_pin out_buf/S00_ARESETN]\
[get_bd_pin out_buf/M00_ARESETN]\
[get_bd_pin out_buf/M01_ARESETN]\
[get_bd_pin out_data_mb_0/ARESETN]\
[get_bd_pin out_data_mb_0/S00_ARESETN]\
[get_bd_pin out_data_bc_0_0/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M00_ARESETN]\
[get_bd_pin out_data_bc_0_1/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M01_ARESETN]\
[get_bd_pin out_data_bc_0_2/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M02_ARESETN]\
[get_bd_pin out_data_bc_0_3/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M03_ARESETN]\
[get_bd_pin out_data_bc_0_4/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M04_ARESETN]\
[get_bd_pin out_data_bc_0_5/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M05_ARESETN]\
[get_bd_pin out_data_bc_0_6/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M06_ARESETN]\
[get_bd_pin out_data_bc_0_7/s_axi_aresetn]\
[get_bd_pin out_data_mb_0/M07_ARESETN]\
[get_bd_pin out_data_mb_1/ARESETN]\
[get_bd_pin out_data_mb_1/S00_ARESETN]\
[get_bd_pin out_data_bc_1_0/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M00_ARESETN]\
[get_bd_pin out_data_bc_1_1/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M01_ARESETN]\
[get_bd_pin out_data_bc_1_2/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M02_ARESETN]\
[get_bd_pin out_data_bc_1_3/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M03_ARESETN]\
[get_bd_pin out_data_bc_1_4/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M04_ARESETN]\
[get_bd_pin out_data_bc_1_5/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M05_ARESETN]\
[get_bd_pin out_data_bc_1_6/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M06_ARESETN]\
[get_bd_pin out_data_bc_1_7/s_axi_aresetn]\
[get_bd_pin out_data_mb_1/M07_ARESETN]


  # Create address segments
  assign_bd_address [get_bd_addr_segs {conv_core_0/s_axi_CRTL_BUS/Reg }]
 set_property range 4K [get_bd_addr_segs {S_CTRL_BUS_AXI/SEG_conv_core_0_Reg}]
 set_property offset 0x00000000 [get_bd_addr_segs {S_CTRL_BUS_AXI/SEG_conv_core_0_Reg}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_0_Mem0}]
 set_property offset 0x04000000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_0_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_1_Mem0}]
 set_property offset 0x04001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_1_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_2_Mem0}]
 set_property offset 0x04002000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_2_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_3_Mem0}]
 set_property offset 0x04003000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_3_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_4_Mem0}]
 set_property offset 0x04004000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_4_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_5_Mem0}]
 set_property offset 0x04005000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_5_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_6_Mem0}]
 set_property offset 0x04006000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_6_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_0_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_7_Mem0}]
 set_property offset 0x04007000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_0_7_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_0_Mem0}]
 set_property offset 0x04008000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_0_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_1_Mem0}]
 set_property offset 0x04009000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_1_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_2_Mem0}]
 set_property offset 0x0400a000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_2_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_3_Mem0}]
 set_property offset 0x0400b000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_3_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_4_Mem0}]
 set_property offset 0x0400c000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_4_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_5_Mem0}]
 set_property offset 0x0400d000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_5_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_6_Mem0}]
 set_property offset 0x0400e000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_6_Mem0}]
assign_bd_address [get_bd_addr_segs {in_data_bc_1_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_7_Mem0}]
 set_property offset 0x0400f000 [get_bd_addr_segs {S_IN_DATA/SEG_in_data_bc_1_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_0_Mem0}]
 set_property offset 0x06000000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_1_Mem0}]
 set_property offset 0x06001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_2_Mem0}]
 set_property offset 0x06002000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_3_Mem0}]
 set_property offset 0x06003000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_4_Mem0}]
 set_property offset 0x06004000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_5_Mem0}]
 set_property offset 0x06005000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_6_Mem0}]
 set_property offset 0x06006000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_0_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_7_Mem0}]
 set_property offset 0x06007000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_0_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_0_Mem0}]
 set_property offset 0x06008000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_1_Mem0}]
 set_property offset 0x06009000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_2_Mem0}]
 set_property offset 0x0600a000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_3_Mem0}]
 set_property offset 0x0600b000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_4_Mem0}]
 set_property offset 0x0600c000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_5_Mem0}]
 set_property offset 0x0600d000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_6_Mem0}]
 set_property offset 0x0600e000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_1_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_7_Mem0}]
 set_property offset 0x0600f000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_1_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_0_Mem0}]
 set_property offset 0x06010000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_1_Mem0}]
 set_property offset 0x06011000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_2_Mem0}]
 set_property offset 0x06012000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_3_Mem0}]
 set_property offset 0x06013000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_4_Mem0}]
 set_property offset 0x06014000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_5_Mem0}]
 set_property offset 0x06015000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_6_Mem0}]
 set_property offset 0x06016000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_2_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_7_Mem0}]
 set_property offset 0x06017000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_2_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_0_Mem0}]
 set_property offset 0x06018000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_1_Mem0}]
 set_property offset 0x06019000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_2_Mem0}]
 set_property offset 0x0601a000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_3_Mem0}]
 set_property offset 0x0601b000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_4_Mem0}]
 set_property offset 0x0601c000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_5_Mem0}]
 set_property offset 0x0601d000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_6_Mem0}]
 set_property offset 0x0601e000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_3_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_7_Mem0}]
 set_property offset 0x0601f000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_3_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_0_Mem0}]
 set_property offset 0x06020000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_1_Mem0}]
 set_property offset 0x06021000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_2_Mem0}]
 set_property offset 0x06022000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_3_Mem0}]
 set_property offset 0x06023000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_4_Mem0}]
 set_property offset 0x06024000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_5_Mem0}]
 set_property offset 0x06025000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_6_Mem0}]
 set_property offset 0x06026000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_4_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_7_Mem0}]
 set_property offset 0x06027000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_4_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_0_Mem0}]
 set_property offset 0x06028000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_1_Mem0}]
 set_property offset 0x06029000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_2_Mem0}]
 set_property offset 0x0602a000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_3_Mem0}]
 set_property offset 0x0602b000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_4_Mem0}]
 set_property offset 0x0602c000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_5_Mem0}]
 set_property offset 0x0602d000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_6_Mem0}]
 set_property offset 0x0602e000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_5_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_7_Mem0}]
 set_property offset 0x0602f000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_5_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_0_Mem0}]
 set_property offset 0x06030000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_1_Mem0}]
 set_property offset 0x06031000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_2_Mem0}]
 set_property offset 0x06032000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_3_Mem0}]
 set_property offset 0x06033000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_4_Mem0}]
 set_property offset 0x06034000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_5_Mem0}]
 set_property offset 0x06035000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_6_Mem0}]
 set_property offset 0x06036000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_6_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_7_Mem0}]
 set_property offset 0x06037000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_6_7_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_0_Mem0}]
 set_property offset 0x06038000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_0_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_1_Mem0}]
 set_property offset 0x06039000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_1_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_2_Mem0}]
 set_property offset 0x0603a000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_2_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_3_Mem0}]
 set_property offset 0x0603b000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_3_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_4_Mem0}]
 set_property offset 0x0603c000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_4_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_5_Mem0}]
 set_property offset 0x0603d000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_5_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_6_Mem0}]
 set_property offset 0x0603e000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_6_Mem0}]
assign_bd_address [get_bd_addr_segs {w_data_bc_7_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_7_Mem0}]
 set_property offset 0x0603f000 [get_bd_addr_segs {S_W_DATA/SEG_w_data_bc_7_7_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_0_Mem0}]
 set_property offset 0x0a000000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_0_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_1_Mem0}]
 set_property offset 0x0a001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_1_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_2_Mem0}]
 set_property offset 0x0a002000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_2_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_3_Mem0}]
 set_property offset 0x0a003000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_3_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_4_Mem0}]
 set_property offset 0x0a004000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_4_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_5_Mem0}]
 set_property offset 0x0a005000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_5_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_6_Mem0}]
 set_property offset 0x0a006000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_6_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_0_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_7_Mem0}]
 set_property offset 0x0a007000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_0_7_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_0/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_0_Mem0}]
 set_property offset 0x0a008000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_0_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_1/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_1_Mem0}]
 set_property offset 0x0a009000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_1_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_2/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_2_Mem0}]
 set_property offset 0x0a00a000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_2_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_3/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_3_Mem0}]
 set_property offset 0x0a00b000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_3_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_4/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_4_Mem0}]
 set_property offset 0x0a00c000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_4_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_5/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_5_Mem0}]
 set_property offset 0x0a00d000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_5_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_6/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_6_Mem0}]
 set_property offset 0x0a00e000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_6_Mem0}]
assign_bd_address [get_bd_addr_segs {out_data_bc_1_7/S_AXI/Mem0}]
 set_property range 0x00001000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_7_Mem0}]
 set_property offset 0x0a00f000 [get_bd_addr_segs {S_OUT_DATA/SEG_out_data_bc_1_7_Mem0}]
  assign_bd_address [get_bd_addr_segs {axi_bram_ctrl_conv/S_AXI/Mem0 }]
 set_property range 4K [get_bd_addr_segs {S_PARAM_AXI/SEG_axi_bram_ctrl_conv_Mem0 }]
 set_property offset 0x00010000 [get_bd_addr_segs {S_PARAM_AXI/SEG_axi_bram_ctrl_conv_Mem0 }]
  assign_bd_address [get_bd_addr_segs {axi_bram_ctrl_pool/S_AXI/Mem0 }]
 set_property range 4K [get_bd_addr_segs {S_PARAM_AXI/SEG_axi_bram_ctrl_pool_Mem0 }]
 set_property offset 0x00020000 [get_bd_addr_segs {S_PARAM_AXI/SEG_axi_bram_ctrl_pool_Mem0 }]
  assign_bd_address [get_bd_addr_segs {axi_bram_ctrl_bias/S_AXI/Mem0 }]
 set_property range 4K [get_bd_addr_segs {S_PARAM_AXI/SEG_axi_bram_ctrl_bias_Mem0 }]
 set_property offset 0x00030000 [get_bd_addr_segs {S_PARAM_AXI/SEG_axi_bram_ctrl_bias_Mem0 }]

  # Restore current instance
  current_bd_instance $oldCurInst
  regenerate_bd_layout
  save_bd_design

  # Export IP
  

}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


common::send_msg_id "BD_TCL-1000" "WARNING" "This Tcl script was generated from a block design that has not been validated. It is possible that design <$design_name> may result in errors during validation."

