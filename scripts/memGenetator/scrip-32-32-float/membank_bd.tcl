
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
  set design_name membank

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
  set BRAM_PORT_0 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_0 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_0
  set BRAM_PORT_1 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_1 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_1
  set BRAM_PORT_2 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_2 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_2
  set BRAM_PORT_3 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_3 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_3
  set BRAM_PORT_4 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_4 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_4
  set BRAM_PORT_5 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_5 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_5
  set BRAM_PORT_6 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_6 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_6
  set BRAM_PORT_7 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_7 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_7
  set BRAM_PORT_8 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_8 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_8
  set BRAM_PORT_9 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_9 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_9
  set BRAM_PORT_10 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_10 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_10
  set BRAM_PORT_11 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_11 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_11
  set BRAM_PORT_12 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_12 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_12
  set BRAM_PORT_13 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_13 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_13
  set BRAM_PORT_14 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_14 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_14
  set BRAM_PORT_15 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_15 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_15
  set BRAM_PORT_16 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_16 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_16
  set BRAM_PORT_17 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_17 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_17
  set BRAM_PORT_18 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_18 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_18
  set BRAM_PORT_19 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_19 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_19
  set BRAM_PORT_20 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_20 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_20
  set BRAM_PORT_21 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_21 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_21
  set BRAM_PORT_22 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_22 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_22
  set BRAM_PORT_23 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_23 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_23
  set BRAM_PORT_24 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_24 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_24
  set BRAM_PORT_25 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_25 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_25
  set BRAM_PORT_26 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_26 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_26
  set BRAM_PORT_27 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_27 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_27
  set BRAM_PORT_28 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_28 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_28
  set BRAM_PORT_29 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_29 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_29
  set BRAM_PORT_30 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_30 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_30
  set BRAM_PORT_31 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 BRAM_PORT_31 ]
  set_property -dict [ list \
CONFIG.MASTER_TYPE {BRAM_CTRL} \
CONFIG.MEM_SIZE {4096}] $BRAM_PORT_31

  set S00_AXI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S00_AXI ]
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
CONFIG.PROTOCOL {AXI4LITE} \
CONFIG.READ_WRITE_MODE {READ_WRITE} \
CONFIG.RUSER_BITS_PER_BYTE {0} \
CONFIG.RUSER_WIDTH {0} \
CONFIG.SUPPORTS_NARROW_BURST {1} \
CONFIG.WUSER_BITS_PER_BYTE {0} \
CONFIG.WUSER_WIDTH {0} \
 ] $S00_AXI 

  # Create ports
  set ACLK [ create_bd_port -dir I -type clk ACLK ]
  set_property CONFIG.FREQ_HZ 125000000 [get_bd_ports ACLK]
  set ARESETN [ create_bd_port -dir I -type rst ARESETN ]

  # Create instances
  set axi_interconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_interconnect_0 ]
  set_property -dict [ list \
  CONFIG.NUM_SI {1} \
  CONFIG.NUM_MI {32} \
  ] $axi_interconnect_0
  set axi_bram_ctrl_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_0 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_0
  set blk_mem_gen_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_0 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_0
  set axi_bram_ctrl_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_1 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_1
  set blk_mem_gen_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_1 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_1
  set axi_bram_ctrl_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_2 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_2
  set blk_mem_gen_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_2 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_2
  set axi_bram_ctrl_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_3 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_3
  set blk_mem_gen_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_3 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_3
  set axi_bram_ctrl_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_4 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_4
  set blk_mem_gen_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_4 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_4
  set axi_bram_ctrl_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_5 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_5
  set blk_mem_gen_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_5 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_5
  set axi_bram_ctrl_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_6 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_6
  set blk_mem_gen_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_6 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_6
  set axi_bram_ctrl_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_7 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_7
  set blk_mem_gen_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_7 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_7
  set axi_bram_ctrl_8 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_8 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_8
  set blk_mem_gen_8 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_8 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_8
  set axi_bram_ctrl_9 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_9 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_9
  set blk_mem_gen_9 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_9 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_9
  set axi_bram_ctrl_10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_10 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_10
  set blk_mem_gen_10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_10 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_10
  set axi_bram_ctrl_11 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_11 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_11
  set blk_mem_gen_11 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_11 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_11
  set axi_bram_ctrl_12 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_12 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_12
  set blk_mem_gen_12 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_12 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_12
  set axi_bram_ctrl_13 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_13 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_13
  set blk_mem_gen_13 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_13 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_13
  set axi_bram_ctrl_14 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_14 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_14
  set blk_mem_gen_14 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_14 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_14
  set axi_bram_ctrl_15 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_15 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_15
  set blk_mem_gen_15 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_15 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_15
  set axi_bram_ctrl_16 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_16 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_16
  set blk_mem_gen_16 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_16 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_16
  set axi_bram_ctrl_17 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_17 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_17
  set blk_mem_gen_17 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_17 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_17
  set axi_bram_ctrl_18 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_18 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_18
  set blk_mem_gen_18 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_18 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_18
  set axi_bram_ctrl_19 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_19 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_19
  set blk_mem_gen_19 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_19 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_19
  set axi_bram_ctrl_20 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_20 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_20
  set blk_mem_gen_20 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_20 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_20
  set axi_bram_ctrl_21 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_21 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_21
  set blk_mem_gen_21 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_21 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_21
  set axi_bram_ctrl_22 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_22 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_22
  set blk_mem_gen_22 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_22 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_22
  set axi_bram_ctrl_23 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_23 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_23
  set blk_mem_gen_23 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_23 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_23
  set axi_bram_ctrl_24 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_24 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_24
  set blk_mem_gen_24 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_24 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_24
  set axi_bram_ctrl_25 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_25 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_25
  set blk_mem_gen_25 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_25 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_25
  set axi_bram_ctrl_26 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_26 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_26
  set blk_mem_gen_26 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_26 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_26
  set axi_bram_ctrl_27 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_27 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_27
  set blk_mem_gen_27 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_27 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_27
  set axi_bram_ctrl_28 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_28 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_28
  set blk_mem_gen_28 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_28 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_28
  set axi_bram_ctrl_29 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_29 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_29
  set blk_mem_gen_29 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_29 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_29
  set axi_bram_ctrl_30 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_30 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_30
  set blk_mem_gen_30 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_30 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_30
  set axi_bram_ctrl_31 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_31 ]
  set_property -dict [ list \
  CONFIG.SINGLE_PORT_BRAM {1} \
  ] $axi_bram_ctrl_31
  set blk_mem_gen_31 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 blk_mem_gen_31 ]
  set_property -dict [ list \
CONFIG.Enable_B {Use_ENB_Pin} \
CONFIG.Memory_Type {True_Dual_Port_RAM} \
CONFIG.Port_B_Clock {100} \
CONFIG.Port_B_Enable_Rate {100} \
CONFIG.Port_B_Write_Rate {50} \
CONFIG.Use_RSTB_Pin {true} \
CONFIG.PRIM_type_to_Implement {URAM} \
] $blk_mem_gen_31

  # Create interface connections
  connect_bd_intf_net -intf_net S00_AXI_1 [get_bd_intf_ports S00_AXI] [get_bd_intf_pins axi_interconnect_0/S00_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_0 [get_bd_intf_ports BRAM_PORT_0] [get_bd_intf_pins blk_mem_gen_0/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_0_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_0/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_0/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M00_AXI_net [get_bd_intf_pins axi_bram_ctrl_0/S_AXI] [get_bd_intf_pins axi_interconnect_0/M00_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_1 [get_bd_intf_ports BRAM_PORT_1] [get_bd_intf_pins blk_mem_gen_1/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_1_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_1/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_1/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M01_AXI_net [get_bd_intf_pins axi_bram_ctrl_1/S_AXI] [get_bd_intf_pins axi_interconnect_0/M01_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_2 [get_bd_intf_ports BRAM_PORT_2] [get_bd_intf_pins blk_mem_gen_2/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_2_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_2/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_2/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M02_AXI_net [get_bd_intf_pins axi_bram_ctrl_2/S_AXI] [get_bd_intf_pins axi_interconnect_0/M02_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_3 [get_bd_intf_ports BRAM_PORT_3] [get_bd_intf_pins blk_mem_gen_3/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_3_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_3/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_3/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M03_AXI_net [get_bd_intf_pins axi_bram_ctrl_3/S_AXI] [get_bd_intf_pins axi_interconnect_0/M03_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_4 [get_bd_intf_ports BRAM_PORT_4] [get_bd_intf_pins blk_mem_gen_4/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_4_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_4/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_4/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M04_AXI_net [get_bd_intf_pins axi_bram_ctrl_4/S_AXI] [get_bd_intf_pins axi_interconnect_0/M04_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_5 [get_bd_intf_ports BRAM_PORT_5] [get_bd_intf_pins blk_mem_gen_5/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_5_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_5/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_5/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M05_AXI_net [get_bd_intf_pins axi_bram_ctrl_5/S_AXI] [get_bd_intf_pins axi_interconnect_0/M05_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_6 [get_bd_intf_ports BRAM_PORT_6] [get_bd_intf_pins blk_mem_gen_6/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_6_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_6/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_6/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M06_AXI_net [get_bd_intf_pins axi_bram_ctrl_6/S_AXI] [get_bd_intf_pins axi_interconnect_0/M06_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_7 [get_bd_intf_ports BRAM_PORT_7] [get_bd_intf_pins blk_mem_gen_7/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_7_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_7/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_7/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M07_AXI_net [get_bd_intf_pins axi_bram_ctrl_7/S_AXI] [get_bd_intf_pins axi_interconnect_0/M07_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_8 [get_bd_intf_ports BRAM_PORT_8] [get_bd_intf_pins blk_mem_gen_8/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_8_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_8/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_8/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M08_AXI_net [get_bd_intf_pins axi_bram_ctrl_8/S_AXI] [get_bd_intf_pins axi_interconnect_0/M08_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_9 [get_bd_intf_ports BRAM_PORT_9] [get_bd_intf_pins blk_mem_gen_9/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_9_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_9/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_9/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M09_AXI_net [get_bd_intf_pins axi_bram_ctrl_9/S_AXI] [get_bd_intf_pins axi_interconnect_0/M09_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_10 [get_bd_intf_ports BRAM_PORT_10] [get_bd_intf_pins blk_mem_gen_10/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_10_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_10/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_10/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M10_AXI_net [get_bd_intf_pins axi_bram_ctrl_10/S_AXI] [get_bd_intf_pins axi_interconnect_0/M10_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_11 [get_bd_intf_ports BRAM_PORT_11] [get_bd_intf_pins blk_mem_gen_11/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_11_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_11/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_11/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M11_AXI_net [get_bd_intf_pins axi_bram_ctrl_11/S_AXI] [get_bd_intf_pins axi_interconnect_0/M11_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_12 [get_bd_intf_ports BRAM_PORT_12] [get_bd_intf_pins blk_mem_gen_12/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_12_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_12/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_12/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M12_AXI_net [get_bd_intf_pins axi_bram_ctrl_12/S_AXI] [get_bd_intf_pins axi_interconnect_0/M12_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_13 [get_bd_intf_ports BRAM_PORT_13] [get_bd_intf_pins blk_mem_gen_13/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_13_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_13/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_13/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M13_AXI_net [get_bd_intf_pins axi_bram_ctrl_13/S_AXI] [get_bd_intf_pins axi_interconnect_0/M13_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_14 [get_bd_intf_ports BRAM_PORT_14] [get_bd_intf_pins blk_mem_gen_14/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_14_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_14/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_14/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M14_AXI_net [get_bd_intf_pins axi_bram_ctrl_14/S_AXI] [get_bd_intf_pins axi_interconnect_0/M14_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_15 [get_bd_intf_ports BRAM_PORT_15] [get_bd_intf_pins blk_mem_gen_15/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_15_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_15/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_15/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M15_AXI_net [get_bd_intf_pins axi_bram_ctrl_15/S_AXI] [get_bd_intf_pins axi_interconnect_0/M15_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_16 [get_bd_intf_ports BRAM_PORT_16] [get_bd_intf_pins blk_mem_gen_16/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_16_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_16/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_16/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M16_AXI_net [get_bd_intf_pins axi_bram_ctrl_16/S_AXI] [get_bd_intf_pins axi_interconnect_0/M16_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_17 [get_bd_intf_ports BRAM_PORT_17] [get_bd_intf_pins blk_mem_gen_17/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_17_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_17/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_17/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M17_AXI_net [get_bd_intf_pins axi_bram_ctrl_17/S_AXI] [get_bd_intf_pins axi_interconnect_0/M17_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_18 [get_bd_intf_ports BRAM_PORT_18] [get_bd_intf_pins blk_mem_gen_18/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_18_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_18/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_18/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M18_AXI_net [get_bd_intf_pins axi_bram_ctrl_18/S_AXI] [get_bd_intf_pins axi_interconnect_0/M18_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_19 [get_bd_intf_ports BRAM_PORT_19] [get_bd_intf_pins blk_mem_gen_19/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_19_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_19/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_19/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M19_AXI_net [get_bd_intf_pins axi_bram_ctrl_19/S_AXI] [get_bd_intf_pins axi_interconnect_0/M19_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_20 [get_bd_intf_ports BRAM_PORT_20] [get_bd_intf_pins blk_mem_gen_20/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_20_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_20/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_20/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M20_AXI_net [get_bd_intf_pins axi_bram_ctrl_20/S_AXI] [get_bd_intf_pins axi_interconnect_0/M20_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_21 [get_bd_intf_ports BRAM_PORT_21] [get_bd_intf_pins blk_mem_gen_21/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_21_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_21/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_21/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M21_AXI_net [get_bd_intf_pins axi_bram_ctrl_21/S_AXI] [get_bd_intf_pins axi_interconnect_0/M21_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_22 [get_bd_intf_ports BRAM_PORT_22] [get_bd_intf_pins blk_mem_gen_22/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_22_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_22/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_22/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M22_AXI_net [get_bd_intf_pins axi_bram_ctrl_22/S_AXI] [get_bd_intf_pins axi_interconnect_0/M22_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_23 [get_bd_intf_ports BRAM_PORT_23] [get_bd_intf_pins blk_mem_gen_23/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_23_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_23/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_23/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M23_AXI_net [get_bd_intf_pins axi_bram_ctrl_23/S_AXI] [get_bd_intf_pins axi_interconnect_0/M23_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_24 [get_bd_intf_ports BRAM_PORT_24] [get_bd_intf_pins blk_mem_gen_24/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_24_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_24/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_24/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M24_AXI_net [get_bd_intf_pins axi_bram_ctrl_24/S_AXI] [get_bd_intf_pins axi_interconnect_0/M24_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_25 [get_bd_intf_ports BRAM_PORT_25] [get_bd_intf_pins blk_mem_gen_25/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_25_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_25/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_25/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M25_AXI_net [get_bd_intf_pins axi_bram_ctrl_25/S_AXI] [get_bd_intf_pins axi_interconnect_0/M25_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_26 [get_bd_intf_ports BRAM_PORT_26] [get_bd_intf_pins blk_mem_gen_26/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_26_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_26/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_26/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M26_AXI_net [get_bd_intf_pins axi_bram_ctrl_26/S_AXI] [get_bd_intf_pins axi_interconnect_0/M26_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_27 [get_bd_intf_ports BRAM_PORT_27] [get_bd_intf_pins blk_mem_gen_27/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_27_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_27/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_27/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M27_AXI_net [get_bd_intf_pins axi_bram_ctrl_27/S_AXI] [get_bd_intf_pins axi_interconnect_0/M27_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_28 [get_bd_intf_ports BRAM_PORT_28] [get_bd_intf_pins blk_mem_gen_28/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_28_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_28/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_28/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M28_AXI_net [get_bd_intf_pins axi_bram_ctrl_28/S_AXI] [get_bd_intf_pins axi_interconnect_0/M28_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_29 [get_bd_intf_ports BRAM_PORT_29] [get_bd_intf_pins blk_mem_gen_29/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_29_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_29/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_29/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M29_AXI_net [get_bd_intf_pins axi_bram_ctrl_29/S_AXI] [get_bd_intf_pins axi_interconnect_0/M29_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_30 [get_bd_intf_ports BRAM_PORT_30] [get_bd_intf_pins blk_mem_gen_30/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_30_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_30/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_30/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M30_AXI_net [get_bd_intf_pins axi_bram_ctrl_30/S_AXI] [get_bd_intf_pins axi_interconnect_0/M30_AXI]
  connect_bd_intf_net -intf_net BRAM_PORT_net_31 [get_bd_intf_ports BRAM_PORT_31] [get_bd_intf_pins blk_mem_gen_31/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_bram_ctrl_31_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_31/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_31/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_interconnect_0_M31_AXI_net [get_bd_intf_pins axi_bram_ctrl_31/S_AXI] [get_bd_intf_pins axi_interconnect_0/M31_AXI]

  # Create port connections
  connect_bd_net -net ACLK_1 [get_bd_ports ACLK] [get_bd_pins axi_interconnect_0/ACLK] [get_bd_pins axi_interconnect_0/S00_ACLK] [get_bd_pins axi_bram_ctrl_0/s_axi_aclk] [get_bd_pins axi_interconnect_0/M00_ACLK] [get_bd_pins axi_bram_ctrl_1/s_axi_aclk] [get_bd_pins axi_interconnect_0/M01_ACLK] [get_bd_pins axi_bram_ctrl_2/s_axi_aclk] [get_bd_pins axi_interconnect_0/M02_ACLK] [get_bd_pins axi_bram_ctrl_3/s_axi_aclk] [get_bd_pins axi_interconnect_0/M03_ACLK] [get_bd_pins axi_bram_ctrl_4/s_axi_aclk] [get_bd_pins axi_interconnect_0/M04_ACLK] [get_bd_pins axi_bram_ctrl_5/s_axi_aclk] [get_bd_pins axi_interconnect_0/M05_ACLK] [get_bd_pins axi_bram_ctrl_6/s_axi_aclk] [get_bd_pins axi_interconnect_0/M06_ACLK] [get_bd_pins axi_bram_ctrl_7/s_axi_aclk] [get_bd_pins axi_interconnect_0/M07_ACLK] [get_bd_pins axi_bram_ctrl_8/s_axi_aclk] [get_bd_pins axi_interconnect_0/M08_ACLK] [get_bd_pins axi_bram_ctrl_9/s_axi_aclk] [get_bd_pins axi_interconnect_0/M09_ACLK] [get_bd_pins axi_bram_ctrl_10/s_axi_aclk] [get_bd_pins axi_interconnect_0/M10_ACLK] [get_bd_pins axi_bram_ctrl_11/s_axi_aclk] [get_bd_pins axi_interconnect_0/M11_ACLK] [get_bd_pins axi_bram_ctrl_12/s_axi_aclk] [get_bd_pins axi_interconnect_0/M12_ACLK] [get_bd_pins axi_bram_ctrl_13/s_axi_aclk] [get_bd_pins axi_interconnect_0/M13_ACLK] [get_bd_pins axi_bram_ctrl_14/s_axi_aclk] [get_bd_pins axi_interconnect_0/M14_ACLK] [get_bd_pins axi_bram_ctrl_15/s_axi_aclk] [get_bd_pins axi_interconnect_0/M15_ACLK] [get_bd_pins axi_bram_ctrl_16/s_axi_aclk] [get_bd_pins axi_interconnect_0/M16_ACLK] [get_bd_pins axi_bram_ctrl_17/s_axi_aclk] [get_bd_pins axi_interconnect_0/M17_ACLK] [get_bd_pins axi_bram_ctrl_18/s_axi_aclk] [get_bd_pins axi_interconnect_0/M18_ACLK] [get_bd_pins axi_bram_ctrl_19/s_axi_aclk] [get_bd_pins axi_interconnect_0/M19_ACLK] [get_bd_pins axi_bram_ctrl_20/s_axi_aclk] [get_bd_pins axi_interconnect_0/M20_ACLK] [get_bd_pins axi_bram_ctrl_21/s_axi_aclk] [get_bd_pins axi_interconnect_0/M21_ACLK] [get_bd_pins axi_bram_ctrl_22/s_axi_aclk] [get_bd_pins axi_interconnect_0/M22_ACLK] [get_bd_pins axi_bram_ctrl_23/s_axi_aclk] [get_bd_pins axi_interconnect_0/M23_ACLK] [get_bd_pins axi_bram_ctrl_24/s_axi_aclk] [get_bd_pins axi_interconnect_0/M24_ACLK] [get_bd_pins axi_bram_ctrl_25/s_axi_aclk] [get_bd_pins axi_interconnect_0/M25_ACLK] [get_bd_pins axi_bram_ctrl_26/s_axi_aclk] [get_bd_pins axi_interconnect_0/M26_ACLK] [get_bd_pins axi_bram_ctrl_27/s_axi_aclk] [get_bd_pins axi_interconnect_0/M27_ACLK] [get_bd_pins axi_bram_ctrl_28/s_axi_aclk] [get_bd_pins axi_interconnect_0/M28_ACLK] [get_bd_pins axi_bram_ctrl_29/s_axi_aclk] [get_bd_pins axi_interconnect_0/M29_ACLK] [get_bd_pins axi_bram_ctrl_30/s_axi_aclk] [get_bd_pins axi_interconnect_0/M30_ACLK] [get_bd_pins axi_bram_ctrl_31/s_axi_aclk] [get_bd_pins axi_interconnect_0/M31_ACLK] 
  connect_bd_net -net ARESETN_1 [get_bd_ports ARESETN] [get_bd_pins axi_interconnect_0/ARESETN] [get_bd_pins axi_interconnect_0/S00_ARESETN] [get_bd_pins axi_bram_ctrl_0/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M00_ARESETN] [get_bd_pins axi_bram_ctrl_1/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M01_ARESETN] [get_bd_pins axi_bram_ctrl_2/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M02_ARESETN] [get_bd_pins axi_bram_ctrl_3/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M03_ARESETN] [get_bd_pins axi_bram_ctrl_4/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M04_ARESETN] [get_bd_pins axi_bram_ctrl_5/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M05_ARESETN] [get_bd_pins axi_bram_ctrl_6/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M06_ARESETN] [get_bd_pins axi_bram_ctrl_7/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M07_ARESETN] [get_bd_pins axi_bram_ctrl_8/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M08_ARESETN] [get_bd_pins axi_bram_ctrl_9/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M09_ARESETN] [get_bd_pins axi_bram_ctrl_10/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M10_ARESETN] [get_bd_pins axi_bram_ctrl_11/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M11_ARESETN] [get_bd_pins axi_bram_ctrl_12/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M12_ARESETN] [get_bd_pins axi_bram_ctrl_13/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M13_ARESETN] [get_bd_pins axi_bram_ctrl_14/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M14_ARESETN] [get_bd_pins axi_bram_ctrl_15/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M15_ARESETN] [get_bd_pins axi_bram_ctrl_16/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M16_ARESETN] [get_bd_pins axi_bram_ctrl_17/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M17_ARESETN] [get_bd_pins axi_bram_ctrl_18/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M18_ARESETN] [get_bd_pins axi_bram_ctrl_19/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M19_ARESETN] [get_bd_pins axi_bram_ctrl_20/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M20_ARESETN] [get_bd_pins axi_bram_ctrl_21/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M21_ARESETN] [get_bd_pins axi_bram_ctrl_22/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M22_ARESETN] [get_bd_pins axi_bram_ctrl_23/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M23_ARESETN] [get_bd_pins axi_bram_ctrl_24/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M24_ARESETN] [get_bd_pins axi_bram_ctrl_25/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M25_ARESETN] [get_bd_pins axi_bram_ctrl_26/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M26_ARESETN] [get_bd_pins axi_bram_ctrl_27/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M27_ARESETN] [get_bd_pins axi_bram_ctrl_28/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M28_ARESETN] [get_bd_pins axi_bram_ctrl_29/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M29_ARESETN] [get_bd_pins axi_bram_ctrl_30/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M30_ARESETN] [get_bd_pins axi_bram_ctrl_31/s_axi_aresetn] [get_bd_pins axi_interconnect_0/M31_ARESETN] 
  # Create address segments
  create_bd_addr_seg -range 0x00001000 -offset 0x00000000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_0/S_AXI/Mem0] SEG_axi_bram_ctrl_0_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00001000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_1/S_AXI/Mem0] SEG_axi_bram_ctrl_1_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00002000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_2/S_AXI/Mem0] SEG_axi_bram_ctrl_2_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00003000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_3/S_AXI/Mem0] SEG_axi_bram_ctrl_3_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00004000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_4/S_AXI/Mem0] SEG_axi_bram_ctrl_4_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00005000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_5/S_AXI/Mem0] SEG_axi_bram_ctrl_5_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00006000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_6/S_AXI/Mem0] SEG_axi_bram_ctrl_6_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00007000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_7/S_AXI/Mem0] SEG_axi_bram_ctrl_7_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00008000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_8/S_AXI/Mem0] SEG_axi_bram_ctrl_8_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00009000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_9/S_AXI/Mem0] SEG_axi_bram_ctrl_9_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0000a000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_10/S_AXI/Mem0] SEG_axi_bram_ctrl_10_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0000b000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_11/S_AXI/Mem0] SEG_axi_bram_ctrl_11_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0000c000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_12/S_AXI/Mem0] SEG_axi_bram_ctrl_12_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0000d000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_13/S_AXI/Mem0] SEG_axi_bram_ctrl_13_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0000e000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_14/S_AXI/Mem0] SEG_axi_bram_ctrl_14_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0000f000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_15/S_AXI/Mem0] SEG_axi_bram_ctrl_15_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00010000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_16/S_AXI/Mem0] SEG_axi_bram_ctrl_16_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00011000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_17/S_AXI/Mem0] SEG_axi_bram_ctrl_17_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00012000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_18/S_AXI/Mem0] SEG_axi_bram_ctrl_18_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00013000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_19/S_AXI/Mem0] SEG_axi_bram_ctrl_19_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00014000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_20/S_AXI/Mem0] SEG_axi_bram_ctrl_20_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00015000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_21/S_AXI/Mem0] SEG_axi_bram_ctrl_21_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00016000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_22/S_AXI/Mem0] SEG_axi_bram_ctrl_22_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00017000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_23/S_AXI/Mem0] SEG_axi_bram_ctrl_23_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00018000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_24/S_AXI/Mem0] SEG_axi_bram_ctrl_24_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x00019000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_25/S_AXI/Mem0] SEG_axi_bram_ctrl_25_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0001a000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_26/S_AXI/Mem0] SEG_axi_bram_ctrl_26_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0001b000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_27/S_AXI/Mem0] SEG_axi_bram_ctrl_27_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0001c000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_28/S_AXI/Mem0] SEG_axi_bram_ctrl_28_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0001d000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_29/S_AXI/Mem0] SEG_axi_bram_ctrl_29_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0001e000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_30/S_AXI/Mem0] SEG_axi_bram_ctrl_30_Mem0
  create_bd_addr_seg -range 0x00001000 -offset 0x0001f000 [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_31/S_AXI/Mem0] SEG_axi_bram_ctrl_31_Mem0

  # Restore current instance
  current_bd_instance $oldCurInst
  regenerate_bd_layout
  save_bd_design

  # Export IP
  ipx::package_project -root_dir ./ip_repo/membank -vendor user.org -library user -taxonomy /UserIP -module membank -import_files
  set temp [get_property ip_repo_paths [current_project]]
  lappend temp ./ip_repo/membank
  set_property ip_repo_paths $temp [current_project]
  update_ip_catalog -rebuild
  

}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


common::send_msg_id "BD_TCL-1000" "WARNING" "This Tcl script was generated from a block design that has not been validated. It is possible that design <$design_name> may result in errors during validation."

