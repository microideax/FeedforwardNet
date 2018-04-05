import sys

#Search Strings
design_name_str = "# Design name\n"
interface_ports_str = "# Create interface ports\n"
ports_str = "# Create ports\n"
instances_str = "# Create instances\n"
interface_connections_str = "# Create interface connections\n"
port_connections_str = "# Create port connections\n"
address_mapping_str = "# Create address segments\n"
export_ip_str = "# Export IP\n"

#Implementation Globals
freq = "125000000"

def createAXI4LitePort(name, mode, address_width, data_width):
	temp_text = ("  set " + name +  " [ create_bd_intf_port -mode "+ mode +" -vlnv xilinx.com:interface:aximm_rtl:1.0 " + name +  " ]" + "\n"
	"  set_property -dict [ list \\" + "\n"
	"CONFIG.address_width {" + str(address_width) + "} \\" + "\n"
	"CONFIG.ARUSER_WIDTH {0} \\" + "\n"
	"CONFIG.AWUSER_WIDTH {0} \\" + "\n"
	"CONFIG.BUSER_WIDTH {0} \\" + "\n"
	"CONFIG.DATA_WIDTH {" + str(data_width) + "} \\" + "\n"
	"CONFIG.HAS_BRESP {1} \\" + "\n"
	"CONFIG.HAS_BURST {1} \\" + "\n"
	"CONFIG.HAS_CACHE {1} \\" + "\n"
	"CONFIG.HAS_LOCK {1} \\" + "\n"
	"CONFIG.HAS_PROT {1} \\" + "\n"
	"CONFIG.HAS_QOS {0} \\" + "\n"
	"CONFIG.HAS_REGION {0} \\" + "\n"
	"CONFIG.HAS_RRESP {1} \\" + "\n"
	"CONFIG.HAS_WSTRB {1} \\" + "\n"
	"CONFIG.ID_WIDTH {0} \\" + "\n"
	"CONFIG.MAX_BURST_LENGTH {256} \\" + "\n"
	"CONFIG.NUM_READ_OUTSTANDING {2} \\" + "\n"
	"CONFIG.NUM_READ_THREADS {1} \\" + "\n"
	"CONFIG.NUM_WRITE_OUTSTANDING {2} \\" + "\n"
	"CONFIG.NUM_WRITE_THREADS {1} \\" + "\n"
	"CONFIG.PROTOCOL {AXI4LITE} \\" + "\n"
	"CONFIG.READ_WRITE_MODE {READ_WRITE} \\" + "\n"
	"CONFIG.RUSER_BITS_PER_BYTE {0} \\" + "\n"
	"CONFIG.RUSER_WIDTH {0} \\" + "\n"
	"CONFIG.SUPPORTS_NARROW_BURST {1} \\" + "\n"
	"CONFIG.WUSER_BITS_PER_BYTE {0} \\" + "\n"
	"CONFIG.WUSER_WIDTH {0} \\" + "\n"
	" ] $"+ name +  " \n")
	return temp_text

def createAXI4Port(name, mode,address_width, data_width):
	temp_text = ("  set " + name +  " [ create_bd_intf_port -mode " + mode + " -vlnv xilinx.com:interface:aximm_rtl:1.0 " + name +  " ]" + "\n"
	"  set_property -dict [ list \\" + "\n"
	#"CONFIG.address_width {" + str(address_width) + "} \\" + "\n"
	"CONFIG.ARUSER_WIDTH {0} \\" + "\n"
	"CONFIG.AWUSER_WIDTH {0} \\" + "\n"
	"CONFIG.BUSER_WIDTH {0} \\" + "\n"
	"CONFIG.DATA_WIDTH {" + str(data_width) + "} \\" + "\n"
	"CONFIG.HAS_BRESP {1} \\" + "\n"
	"CONFIG.HAS_BURST {1} \\" + "\n"
	"CONFIG.HAS_CACHE {1} \\" + "\n"
	"CONFIG.HAS_LOCK {1} \\" + "\n"
	"CONFIG.HAS_PROT {1} \\" + "\n"
	"CONFIG.HAS_QOS {0} \\" + "\n"
	"CONFIG.HAS_REGION {0} \\" + "\n"
	"CONFIG.HAS_RRESP {1} \\" + "\n"
	"CONFIG.HAS_WSTRB {1} \\" + "\n"
	"CONFIG.ID_WIDTH {0} \\" + "\n"
	"CONFIG.MAX_BURST_LENGTH {256} \\" + "\n"
	"CONFIG.NUM_READ_OUTSTANDING {2} \\" + "\n"
	"CONFIG.NUM_READ_THREADS {1} \\" + "\n"
	"CONFIG.NUM_WRITE_OUTSTANDING {2} \\" + "\n"
	"CONFIG.NUM_WRITE_THREADS {1} \\" + "\n"
	"CONFIG.PROTOCOL {AXI4} \\" + "\n"
	"CONFIG.READ_WRITE_MODE {READ_WRITE} \\" + "\n"
	"CONFIG.RUSER_BITS_PER_BYTE {0} \\" + "\n"
	"CONFIG.RUSER_WIDTH {0} \\" + "\n"
	"CONFIG.SUPPORTS_NARROW_BURST {1} \\" + "\n"
	"CONFIG.WUSER_BITS_PER_BYTE {0} \\" + "\n"
	"CONFIG.WUSER_WIDTH {0} \\" + "\n"
	" ] $"+ name +  " \n")
	return temp_text

def createAXIInterconnect(name, num_master, num_slave):
	temp_text = ("  set "+ name +  " [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 " + name + " ]" + "\n"
	"  set_property -dict [ list \\" + "\n"
	"  CONFIG.NUM_SI {" + str(num_slave) +"} \\" + "\n"
	"  CONFIG.NUM_MI {" + str(num_master) +"} \\" + "\n"
	"  ] $" + name + "\n")
	return temp_text

def createBRAMControllerSingle(name):
	temp_text = ("  set " + name + " [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 " + name + " ]" + "\n"
  	"  set_property -dict [ list \\" + "\n"
   	"  CONFIG.SINGLE_PORT_BRAM {1} \\" + "\n"
 	"  ] $" + name + "\n") 
 	return temp_text

def createBRAM(name, address_width, data_width, mem_size, standalone):
	if(standalone == True):
		temp_text = ("  set " + name + " [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 "+ name + " ]" + "\n" 
	  	"set_property -dict [ list \\" + "\n"
		"CONFIG.Assume_Synchronous_Clk {true} \\" + "\n"
		"CONFIG.Byte_Size {8} \\" + "\n"
		"CONFIG.Enable_32bit_Address {false} \\" + "\n"
		"CONFIG.Enable_B {Use_ENB_Pin} \\" + "\n"
		"CONFIG.Memory_Type {True_Dual_Port_RAM} \\" + "\n"
		"CONFIG.Operating_Mode_A {NO_CHANGE} \\" + "\n"
		"CONFIG.Operating_Mode_B {NO_CHANGE} \\" + "\n"
		"CONFIG.PRIM_type_to_Implement {URAM} \\" + "\n"
		"CONFIG.Port_B_Clock {100} \\" + "\n"
		"CONFIG.Port_B_Enable_Rate {100} \\" + "\n"
		"CONFIG.Port_B_Write_Rate {50} \\" + "\n"
		"CONFIG.Read_Width_A {32} \\" + "\n"
		"CONFIG.Read_Width_B {32} \\" + "\n"
		"CONFIG.Register_PortA_Output_of_Memory_Core {false} \\" + "\n"
		"CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \\" + "\n"
		"CONFIG.Register_PortB_Output_of_Memory_Primitives {false} \\" + "\n"
		"CONFIG.Use_Byte_Write_Enable {false} \\" + "\n"
		"CONFIG.Use_REGCEA_Pin {false} \\" + "\n"
		"CONFIG.Use_REGCEB_Pin {false} \\" + "\n"
		"CONFIG.Use_RSTA_Pin {false} \\" + "\n"
		"CONFIG.Use_RSTB_Pin {false} \\" + "\n"
		"CONFIG.Write_Width_A {32} \\" + "\n"
		"CONFIG.Write_Width_B {32} \\" + "\n"
		"CONFIG.Write_Depth_A {" + str(mem_size) + "} \\" + "\n"
		"CONFIG.use_bram_block {Stand_Alone} \\" + "\n"
		" ] $" + name + "\n")
	else:
		temp_text = ("  set " + name + " [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.3 " + name + " ]" + "\n"
		"  set_property -dict [ list \\" + "\n"
		"CONFIG.Enable_B {Use_ENB_Pin} \\" + "\n"
		"CONFIG.Memory_Type {True_Dual_Port_RAM} \\" + "\n"
		"CONFIG.Port_B_Clock {100} \\" + "\n"
		"CONFIG.Port_B_Enable_Rate {100} \\" + "\n"
		"CONFIG.Port_B_Write_Rate {50} \\" + "\n"
		"CONFIG.Use_RSTB_Pin {true} \\" + "\n"
		"CONFIG.PRIM_type_to_Implement {URAM} \\" + "\n"
		"] $" + name + "\n" )

	return temp_text

def createAXICDMA(name):
	temp_text = ("set "+ name + " [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_cdma:4.1 "+ name + " ]\n"
  	"set_property -dict [ list \\" + "\n"
	"CONFIG.C_ADDR_WIDTH {64} \\" + "\n"
	"CONFIG.C_INCLUDE_SG {0} \\" + "\n"
	"CONFIG.C_M_AXI_DATA_WIDTH {512} \\" + "\n"
	"CONFIG.C_M_AXI_MAX_BURST_LEN {64} \\" + "\n"
 	"] $"+ name + "\n")
 	return temp_text

def createExportDesign(name, vivado_project_path):
	temp_text =  "  ipx::package_project -root_dir " + vivado_project_path + "/" + name +" -vendor user.org -library user -taxonomy /UserIP -module " + name + " -import_files\n"
	temp_text = temp_text + "  set temp [get_property ip_repo_paths [current_project]]\n"
	temp_text = temp_text + "  lappend temp " + vivado_project_path + "/" + name + "\n"
	temp_text = temp_text + "  set_property ip_repo_paths $temp [current_project]\n"
	temp_text = temp_text + "  update_ip_catalog -rebuild\n"
	return temp_text 

def createMembank(name, num_weight_banks, num_ports, address_width, data_width, mem_size):
	temp_text = createAXIInterconnect("axi_interconnect_mb_" + name, num_ports,1)
	for port_idx in range(0, num_ports):
		temp_text = temp_text + createBRAMControllerSingle("axi_bram_ctrl_mb_" + name + "_" + str(port_idx))
		temp_text = temp_text + createBRAM("blk_mem_gen_mb_" + name + "_" + str(port_idx), address_width, data_width, mem_size, False)
	return temp_text


def generateMembank(design_name, vivado_project_path, num_ports, address_width, data_width, mem_size, range_addr, start_addr):
	template_fn = "template_bd.tcl"
	out_fn = design_name +"_bd.tcl"

	#open file for appending
	with open(template_fn, "r+") as f:
		out_text = f.read()

	#design name
	temp_text = ""
	temp_text = temp_text + "  set design_name " + design_name + "\n"
	temp_text = temp_text + "  set_property part  xcvu190-flgc2104-2-e\n" 
	temp_text = temp_text + "  set_property BOARD_PART xilinx.com:vcu118:part0:1.1 [current_project]\n"

	tcl_index = out_text.find(design_name_str) + len(design_name_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	##### Interface Ports #####
	temp_text = ""

	#BRAM PORTS
	for port_idx in range(0,num_ports):
		interface_port_name = "BRAM_PORT_" + str(port_idx)
		temp_text = (temp_text + "  set "+ interface_port_name +" [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:bram_rtl:1.0 " + interface_port_name +" ]" + "\n"
 		"  set_property -dict [ list \\" + "\n" 
		"CONFIG.MASTER_TYPE {BRAM_CTRL} \\" + "\n" 
		"CONFIG.MEM_SIZE {"+ str(mem_size) +"}"
 		"] $" + interface_port_name + "\n")
	temp_text = temp_text + "\n"

	#AXI PORT
	temp_text = temp_text + createAXI4Port("S00_AXI", "Slave", address_width, data_width)

	tcl_index = out_text.find(interface_ports_str) + len(interface_ports_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	##### Creating Ports #####
	temp_text = ""

	#clk and reset
	temp_text = temp_text + "  set ACLK [ create_bd_port -dir I -type clk ACLK ]" + "\n"
	temp_text = temp_text + "  set_property CONFIG.FREQ_HZ "+ freq +" [get_bd_ports ACLK]" + "\n"
 	temp_text = temp_text + "  set ARESETN [ create_bd_port -dir I -type rst ARESETN ]\n" 

	tcl_index = out_text.find(ports_str) + len(ports_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	###### Creating Instances #####
	temp_text = ""
	#Axi Interconnect
	temp_text = temp_text + createAXIInterconnect("axi_interconnect_0", num_ports,1)

	#BRAM Controller and Memory Generator
	for port_idx in range(0, num_ports):
		temp_text = temp_text + createBRAMControllerSingle("axi_bram_ctrl_" + str(port_idx))

		temp_text = temp_text + createBRAM("blk_mem_gen_" + str(port_idx), address_width, data_width, mem_size, False)

	tcl_index = out_text.find(instances_str) + len(instances_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	###### Creating Interface Connections #####
	temp_text = ""
	#Axi connection
	temp_text = temp_text + "  connect_bd_intf_net -intf_net S00_AXI_1 [get_bd_intf_ports S00_AXI] [get_bd_intf_pins axi_interconnect_0/S00_AXI]" + "\n"
	for port_idx in range (0, num_ports):
		#BRAM
		temp_text = temp_text + "  connect_bd_intf_net -intf_net BRAM_PORT_net_" + str(port_idx) + " [get_bd_intf_ports BRAM_PORT_" + str(port_idx) + "] [get_bd_intf_pins blk_mem_gen_" + str(port_idx) + "/BRAM_PORTB]" + "\n"
		#BRAM Controller
		temp_text = temp_text + "  connect_bd_intf_net -intf_net axi_bram_ctrl_" + str(port_idx) + "_BRAM_PORT_net [get_bd_intf_pins axi_bram_ctrl_" + str(port_idx) + "/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_" + str(port_idx) + "/BRAM_PORTA]" + "\n"
		#Interconnect
		temp_text = temp_text + "  connect_bd_intf_net -intf_net axi_interconnect_0_M" + '{:02d}'.format(port_idx) + "_AXI_net [get_bd_intf_pins axi_bram_ctrl_" + str(port_idx) + "/S_AXI] [get_bd_intf_pins axi_interconnect_0/M" + '{:02d}'.format(port_idx) + "_AXI]" + "\n"
	
	tcl_index = out_text.find(interface_connections_str) + len(interface_connections_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	##### Creating Port Connections #####
	temp_text = ""
	temp_text = temp_text + "  connect_bd_net -net ACLK_1 [get_bd_ports ACLK] [get_bd_pins axi_interconnect_0/ACLK] [get_bd_pins axi_interconnect_0/S00_ACLK] "
	for port_idx in range(0, num_ports):
		temp_text = temp_text + "[get_bd_pins axi_bram_ctrl_" + str(port_idx) + "/s_axi_aclk] "
		temp_text = temp_text + "[get_bd_pins axi_interconnect_0/M" + '{:02d}'.format(port_idx) +"_ACLK] "
	temp_text = temp_text + "\n"

	temp_text = temp_text + "  connect_bd_net -net ARESETN_1 [get_bd_ports ARESETN] [get_bd_pins axi_interconnect_0/ARESETN] [get_bd_pins axi_interconnect_0/S00_ARESETN] "
	for port_idx in range(0, num_ports):
		temp_text = temp_text + "[get_bd_pins axi_bram_ctrl_" + str(port_idx) + "/s_axi_aresetn] "
		temp_text = temp_text + "[get_bd_pins axi_interconnect_0/M" + '{:02d}'.format(port_idx) +"_ARESETN] "


	tcl_index = out_text.find(port_connections_str) + len(port_connections_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	###### Mapping Address Space #####
	temp_text = ""
	for port_idx in range(0, num_ports):
		temp_text = temp_text + "  create_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" + '{:08x}'.format(start_addr + port_idx*range_addr) + " [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_bram_ctrl_" + str(port_idx) + "/S_AXI/Mem0] SEG_axi_bram_ctrl_" + str(port_idx) + "_Mem0\n"
	 	  
	tcl_index = out_text.find(address_mapping_str) + len(address_mapping_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	###### Exporting Design #####
	temp_text = createExportDesign(design_name, vivado_project_path)

 	tcl_index = out_text.find(export_ip_str) + len(export_ip_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	with open(out_fn, 'w+') as f:
		f.write(out_text)

#combining acc and membanks, no longer doing this
def generateConvAcc(design_name, vivado_project_path, num_ports, address_width, data_width, mem_size, range_addr, start_addr, num_weight_banks):
	template_fn = "template_bd.tcl"
	out_fn = design_name +"_bd.tcl"

	#open file for appending
	with open(template_fn, "r+") as f:
		out_text = f.read()

	#design name
	temp_text = ""
	temp_text = temp_text + "  set design_name " + design_name + "\n"
	#temp_text = temp_text + "  set_property part  xcvu190-flgc2104-2-e\n" 
	temp_text = temp_text + "  set_property BOARD_PART xilinx.com:vcu118:part0:1.1 [current_project]\n"

	tcl_index = out_text.find(design_name_str) + len(design_name_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	#####  Interface Ports #####
	temp_text = ""

	#control bus
	temp_text = temp_text + createAXI4LitePort("S_CTRL_BUS_AXI", "Slave", address_width, data_width)

	#cmda 
	#temp_text = temp_text + createAXI4LitePort("S_AXI_CDMA", "Slave", address_width, data_width)

	#PCIS
	#temp_text = temp_text + createAXI4Port("S_AXI_PCIS", "Slave", address_width, data_width)	

	#Param
	temp_text = temp_text + createAXI4Port("S_PARAM_AXI", "Slave", address_width, data_width)

	#bias
	temp_text = temp_text + createAXI4Port("S_BIAS_AXI", "Slave", address_width, data_width)

	#input buf
	temp_text = temp_text + createAXI4Port("S_INPUT_AXI", "Slave", address_width, data_width)

	#dram ports
	#temp_text = temp_text + createAXI4Port("M_AXI_DDRA", "Master" ,address_width, data_width)
	#temp_text = temp_text + createAXI4Port("M_AXI_DDRB", "Master" ,address_width, data_width)
	#temp_text = temp_text + createAXI4Port("M_AXI_DDRC", "Master" ,address_width, data_width)
	#temp_text = temp_text + createAXI4Port("M_AXI_DDRD", "Master" ,address_width, data_width)

	#membank ports
	temp_text = temp_text + createAXI4Port("S_MEMBANK_OUT_AXI", "Slave", address_width, data_width)
	for bank_idx in range(0,num_weight_banks):
		temp_text = temp_text + createAXI4Port("S_MEMBANK_" + str(bank_idx) + "_AXI", "Slave", address_width, data_width)


	tcl_index = out_text.find(interface_ports_str) + len(interface_ports_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	##### Creating Ports #####
	temp_text = ""

	#clock and reset signals
	temp_text = temp_text + "  set ACLK [ create_bd_port -dir I -type clk ACLK ]" + "\n"
	temp_text = temp_text + "  set_property CONFIG.FREQ_HZ "+ freq +" [get_bd_ports ACLK]" + "\n"
 	temp_text = temp_text + "  set ARESETN [ create_bd_port -dir I -type rst ARESETN ]\n"

	tcl_index = out_text.find(ports_str) + len(ports_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	##### Creating Instances #####
	temp_text = ""

	#core conv syn
	temp_text = temp_text + "  create_bd_cell -type ip -vlnv xilinx.com:hls:conv_core_syn:1.0 conv_core_syn_0\n"

	#bias buf
	temp_text = temp_text + createBRAM("blk_mem_gen_bias", address_width, data_width, mem_size, False)

	#param buf conv
	temp_text = temp_text + createBRAM("blk_mem_gen_conv_param", address_width, data_width, mem_size, False)

	#param buf pool
	temp_text = temp_text + createBRAM("blk_mem_gen_pool_param", address_width, data_width, mem_size, False)

	#in buf
	temp_text = temp_text + "  create_bd_cell -type ip -vlnv user.org:user:in_data_port:1.0 in_data_port_0\n"

	#out buf
	temp_text = temp_text + createMembank("out", num_weight_banks, num_ports, address_width, data_width, mem_size)


	#membanks
	for bank_idx in range(0, num_weight_banks):
		temp_text = temp_text + createMembank(str(bank_idx), num_weight_banks, num_ports, address_width, data_width, mem_size)

	#Axi Interconnects
	#temp_text = temp_text + createAXIInterconnect("axi_interconnect_0", 8 + num_weight_banks, 2)
	temp_text = temp_text + createAXIInterconnect("axi_interconnect_param", 2, 1 )
 
	#Bram controllers
	temp_text = temp_text + createBRAMControllerSingle("axi_bram_ctrl_conv")
	temp_text = temp_text + createBRAMControllerSingle("axi_bram_ctrl_pool")
	temp_text = temp_text + createBRAMControllerSingle("axi_bram_ctrl_bias")

	#axi cdma 
	#temp_text = temp_text + createAXICDMA("axi_cdma_0")

	tcl_index = out_text.find(instances_str) + len(instances_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	##### Creating Interface Connections #####
	temp_text = ""

	#connecting acc ip to bram banks
	for bank_idx in range(0, num_weight_banks):
		for port_idx in range(0, num_ports):
			temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/w_buf_0_"+ str(bank_idx) + "_" + str(port_idx) + "_PORTA] [get_bd_intf_pins blk_mem_gen_mb_" + str(bank_idx) + "_" + str(port_idx)+"/BRAM_PORTB"+ "]\n"

	#connecting controllers to bram banks
	for bank_idx in range(0, num_weight_banks):
		for port_idx in range(0, num_ports):
				temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_mb_"+str(bank_idx)+"_"+str(port_idx)+"/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_mb_"+str(bank_idx)+"_"+str(port_idx)+"/BRAM_PORTA]\n"
	
	#connecting acc ip to in buf
	#TODO fix the naming of the in buf so that indexing can be easier
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/in_buf_0_0_PORTA] [get_bd_intf_pins in_data_port_0/BRAM_PORTB]\n"
	for bank_idx in range(1,num_weight_banks):
		temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/in_buf_0_" + str(bank_idx) + "_PORTA] [get_bd_intf_pins in_data_port_0/BRAM_PORTB_" + str(bank_idx) + "]\n"

	#connecting acc ip to out buf
	for port_idx in range(0,num_ports):
		temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/out_buf_0_" + str(port_idx) +"_PORTA] [get_bd_intf_pins blk_mem_gen_mb_out_" + str(port_idx) + "/BRAM_PORTB" + "]\n"

	#connecting controllers to out buf
	for port_idx in range(0,num_ports):
		temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_mb_out_"+str(port_idx)+"/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_mb_out_" + str(port_idx) + "/BRAM_PORTA" + "]\n"

	#connecting out controllers to out interconnect
	for port_idx in range(0, num_ports):
		temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_mb_out_"+str(port_idx)+"/S_AXI] -boundary_type upper [get_bd_intf_pins axi_interconnect_mb_out/M"+'{:02d}'.format(port_idx)+"_AXI]\n"

	#connecting membank controllers to membank interconnect
	for bank_idx in range(0, num_weight_banks):
		for port_idx in range(0, num_ports):
			temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_mb_"+str(bank_idx)+"_"+str(port_idx)+"/S_AXI] -boundary_type upper [get_bd_intf_pins axi_interconnect_mb_"+str(bank_idx)+"/M"+'{:02d}'.format(port_idx)+"_AXI]\n"

	#connecting acc ip to bias buf
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/b_buf_0_PORTA] [get_bd_intf_pins blk_mem_gen_bias/BRAM_PORTA]\n"

	#connecting acc ip to param conv buf
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/conv_param_PORTA] [get_bd_intf_pins blk_mem_gen_conv_param/BRAM_PORTA]\n"

	#connecting acc ip to param conv buf
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/pool_param_PORTA] [get_bd_intf_pins blk_mem_gen_pool_param/BRAM_PORTA]\n"

	#connecting acc ip to external control bus
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_CTRL_BUS_AXI] [get_bd_intf_pins conv_core_syn_0/s_axi_CRTL_BUS]\n"

	#connecting interconnect to external port pcis
	#temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_AXI_PCIS] -boundary_type upper [get_bd_intf_pins axi_interconnect_0/S01_AXI]\n"

	#connecting interconnect to external dram ports
	#temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports M_AXI_DDRA] -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M00_AXI]\n"
	#temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports M_AXI_DDRB] -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M01_AXI]\n"
	#temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports M_AXI_DDRC] -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M02_AXI]\n"
	#temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports M_AXI_DDRD] -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M03_AXI]\n"

	#connecting interconnect to param, bias, and in/out data buffers
	#temp_text = temp_text + "  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M04_AXI] [get_bd_intf_pins in_data_port_0/S00_AXI]\n"
	#temp_text = temp_text + "  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M05_AXI] [get_bd_intf_pins axi_interconnect_mb_out/S00_AXI]\n"
	#temp_text = temp_text + "  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M06_AXI] [get_bd_intf_pins axi_interconnect_param/S00_AXI]\n"
	#temp_text = temp_text + "  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M07_AXI] [get_bd_intf_pins axi_bram_ctrl_bias/S_AXI]\n"

	#connecting param interconnect to param and bias
	temp_text = temp_text + "  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_param/M00_AXI] [get_bd_intf_pins axi_bram_ctrl_conv/S_AXI]\n"
	temp_text = temp_text + "  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_param/M01_AXI] [get_bd_intf_pins axi_bram_ctrl_pool/S_AXI]\n"

	#connecting param controllers to bram
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_bias/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_bias/BRAM_PORTB]\n"
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_conv/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_conv_param/BRAM_PORTB]\n"
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_bram_ctrl_pool/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_pool_param/BRAM_PORTB]\n"

	#connecting interconnect to membufs
	#for bank_idx in range(0, num_weight_banks):
		#temp_text = temp_text + "  connect_bd_intf_net -boundary_type upper [get_bd_intf_pins axi_interconnect_0/M"+ '{:02d}'.format(bank_idx + 8) +"_AXI] [get_bd_intf_pins axi_interconnect_mb_"+ str(bank_idx) +"/S00_AXI]\n"

	#connecting cdma to interconnect
	#temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_pins axi_cdma_0/M_AXI] -boundary_type upper [get_bd_intf_pins axi_interconnect_0/S00_AXI]\n"

	#connecting cdma to external pins
	#temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_AXI_CDMA] [get_bd_intf_pins axi_cdma_0/S_AXI_LITE]\n"

	#connecting input buf to external pins
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_INPUT_AXI] -boundary_type upper [get_bd_intf_pins in_data_port_0/S00_AXI]\n"

	#connecting membanks to external ports
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_BIAS_AXI] -boundary_type upper [get_bd_intf_pins axi_bram_ctrl_bias/S_AXI]\n" 
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_PARAM_AXI] -boundary_type upper [get_bd_intf_pins axi_interconnect_param/S00_AXI]\n" 
	temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_MEMBANK_OUT_AXI] -boundary_type upper [get_bd_intf_pins axi_interconnect_mb_out/S00_AXI]\n"
	for bank_idx in range(0, num_weight_banks):
		temp_text = temp_text + "  connect_bd_intf_net [get_bd_intf_ports S_MEMBANK_" + str(bank_idx) + "_AXI] -boundary_type upper [get_bd_intf_pins axi_interconnect_mb_"+str(bank_idx)+"/S00_AXI]\n"


	tcl_index = out_text.find(interface_connections_str) + len(interface_connections_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	##### Creating Port Connections #####
	temp_text = ""

	#clock general
	temp_text = (temp_text + "  connect_bd_net -net ACLK_1 [get_bd_ports ACLK]\\" + "\n"
		#cmda
		#"[get_bd_pins axi_cdma_0/m_axi_aclk] [get_bd_pins axi_cdma_0/s_axi_lite_aclk]\\" + "\n"

		#interconnect
		#"[get_bd_pin axi_interconnect_0/ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/S00_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/S01_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M00_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M01_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M02_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M03_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M04_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M05_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M06_ACLK]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M07_ACLK]\\" + "\n"

		#interconnect param
		"[get_bd_pin axi_interconnect_param/ACLK]\\" + "\n"
		"[get_bd_pin axi_interconnect_param/S00_ACLK]\\" + "\n"
		"[get_bd_pin axi_interconnect_param/M00_ACLK]\\" + "\n"
		"[get_bd_pin axi_interconnect_param/M01_ACLK]\\" + "\n"

		#bram controllers
		"[get_bd_pin axi_bram_ctrl_bias/s_axi_aclk]\\" + "\n"
		"[get_bd_pin axi_bram_ctrl_conv/s_axi_aclk]\\" + "\n"
		"[get_bd_pin axi_bram_ctrl_pool/s_axi_aclk]\\" + "\n"

		#input buffer
		"[get_bd_pin in_data_port_0/ACLK]\\" + "\n"

		#conv_acc_syn
		"[get_bd_pins conv_core_syn_0/ap_clk]")

	#clock interconnect (continued from above)
	#for bank_idx in range(0, num_weight_banks):
		#temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_0/M" + '{:02d}'.format(bank_idx + 8) + "_ACLK]"

	#clock out buffer(continued from above)
	for bank_idx in range(0, num_weight_banks):
		for port_idx in range(0, num_ports):
			temp_text = temp_text + "\\" + "\n[get_bd_pin axi_bram_ctrl_mb_out_" + str(port_idx) + "/s_axi_aclk]"
	temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_out/ACLK]"
	temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_out/S00_ACLK]"
	for port_idx in range(0, num_ports):
		temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_out/M" + '{:02d}'.format(port_idx) + "_ACLK]"
	

	#clock membank (continued from above)
	for bank_idx in range(0, num_weight_banks):
		temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_" + str(bank_idx) + "/ACLK]"
		temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_" + str(bank_idx) + "/S00_ACLK]"
		for port_idx in range (0, num_ports):
			#bram controller
			temp_text = temp_text + "\\" + "\n[get_bd_pin axi_bram_ctrl_mb_" + str(bank_idx) + "_" + str(port_idx) + "/s_axi_aclk]"
			#interconnect
			temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_" + str(bank_idx) + "/M"+ '{:02d}'.format(port_idx) +  "_ACLK]"
	temp_text = temp_text + "\n"

	#reset general
	temp_text = (temp_text + "  connect_bd_net -net ARESETN_1 [get_bd_ports ARESETN]\\" + "\n"
		#cmda
		#"[get_bd_pins axi_cdma_0/s_axi_lite_aresetn]\\" + "\n"

		#interconnect
		#"[get_bd_pin axi_interconnect_0/ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/S00_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/S01_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M00_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M01_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M02_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M03_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M04_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M05_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M06_ARESETN]\\" + "\n"
		#"[get_bd_pin axi_interconnect_0/M07_ARESETN]\\" + "\n"
		#todo finish ports

		#interconnect param
		"[get_bd_pin axi_interconnect_param/ARESETN]\\" + "\n"
		"[get_bd_pin axi_interconnect_param/S00_ARESETN]\\" + "\n"
		"[get_bd_pin axi_interconnect_param/M00_ARESETN]\\" + "\n"
		"[get_bd_pin axi_interconnect_param/M01_ARESETN]\\" + "\n"

		#bram controllers
		"[get_bd_pin axi_bram_ctrl_bias/s_axi_aresetn]\\" + "\n"
		"[get_bd_pin axi_bram_ctrl_conv/s_axi_aresetn]\\" + "\n"
		"[get_bd_pin axi_bram_ctrl_pool/s_axi_aresetn]\\" + "\n"

		#input buffer
		"[get_bd_pin in_data_port_0/ARESETN]\\" + "\n"

		#conv_acc_syn
		"[get_bd_pins conv_core_syn_0/ap_rst_n]"
	)

	#reset interconnect (continued from above)
	#for bank_idx in range(0, num_weight_banks):
		#temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_0/M" + '{:02d}'.format(bank_idx + 8) + "_ARESETN]"

	#reset out buffer(continued from above)
	for bank_idx in range(0, num_weight_banks):
		for port_idx in range(0, num_ports):
			temp_text = temp_text + "\\" + "\n[get_bd_pin axi_bram_ctrl_mb_out_" + str(port_idx) + "/s_axi_aresetn]"
	temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_out/ARESETN]"
	temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_out/S00_ARESETN]"
	for port_idx in range(0, num_ports):
		temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_out/M" + '{:02d}'.format(port_idx) + "_ARESETN]"

	#reset membank (continued from above)
	for bank_idx in range(0, num_weight_banks):
		temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_" + str(bank_idx) + "/ARESETN]"
		temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_" + str(bank_idx) + "/S00_ARESETN]" 
		for port_idx in range (0, num_ports):
			#bram controller
			temp_text = temp_text + "\\" + "\n[get_bd_pin axi_bram_ctrl_mb_" + str(bank_idx) + "_" + str(port_idx) + "/s_axi_aresetn]"
			#interconnect
			temp_text = temp_text + "\\" + "\n[get_bd_pin axi_interconnect_mb_" + str(bank_idx) + "/M"+ '{:02d}'.format(port_idx) +  "_ARESETN]"
	temp_text = temp_text + "\n"

	tcl_index = out_text.find(port_connections_str) + len(port_connections_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]


	##### Mapping Address Space #####
	temp_text = ""

	#S_AXI_CTRL BUS
	temp_text = temp_text + "  assign_bd_address [get_bd_addr_segs {conv_core_syn_0/s_axi_CRTL_BUS/Reg }]\n"

	#S_AXI_CDMA
	#temp_text = temp_text + "  assign_bd_address [get_bd_addr_segs {axi_cdma_0/S_AXI_LITE/Reg }]\n"

	#big address mapping block
	curr_loc = start_addr
	#membanks
	for port_idx in range(0, num_ports):
		temp_text = temp_text + "  create_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" + '{:08x}'.format(curr_loc) + " [get_bd_addr_spaces S_MEMBANK_OUT_AXI] [get_bd_addr_segs axi_bram_ctrl_mb_out_" + str(port_idx) + "/S_AXI/Mem0] SEG_axi_bram_ctrl_mb_out_" + str(port_idx) + "_S_AXI\n"
		curr_loc = curr_loc + range_addr
	#controllers
	for bank_idx in range(0, num_weight_banks):
		for port_idx in range(0, num_ports):
			temp_text = temp_text + "  create_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" + '{:08x}'.format(curr_loc) + " [get_bd_addr_spaces S_MEMBANK_"+str(bank_idx)+"_AXI] [get_bd_addr_segs axi_bram_ctrl_mb_"+ str(bank_idx) + "_" + str(port_idx) + "/S_AXI/Mem0] SEG_axi_bram_ctrl_mb_"+ str(bank_idx) + "_" + str(port_idx) + "_S_AXI\n"
			curr_loc = curr_loc + range_addr
	#input HERE YAO
	for bank_idx in range(0, num_weight_banks):
		#temp_text = temp_text + "  create_bd_addr_seg -range 0x00010000 -offset 0x" + '{:08x}'.format(curr_loc) + " [get_bd_addr_spaces S_INPUT_AXI] [get_bd_addr_segs in_data_port_0/S00_AXI/Mem"+str(bank_idx)+"] SEG_in_data_port_0_"+ str(bank_idx) + "_S_AXI\n"
		#curr_loc = curr_loc + 0x00010000
		temp_text = temp_text + "  assign_bd_address [get_bd_addr_segs in_data_port_0/S00_AXI/Mem"+str(bank_idx)+"]\n"
	temp_text = temp_text + "  assign_bd_address [get_bd_addr_segs {axi_bram_ctrl_conv/S_AXI/Mem0 }]\n"
	temp_text = temp_text + "  assign_bd_address [get_bd_addr_segs {axi_bram_ctrl_pool/S_AXI/Mem0 }]\n"
	temp_text = temp_text + "  assign_bd_address [get_bd_addr_segs {axi_bram_ctrl_bias/S_AXI/Mem0 }]\n"

	#Interconnect same as PCIS 
	#curr_loc = start_addr
	#for bank_idx in range(0, num_weight_banks):
	#	for port_idx in range(0, num_ports):
	#		temp_text = temp_text + "  create_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " #-offset 0x" + '{:08x}'.format(curr_loc) + " [get_bd_addr_spaces S_AXI_PCIS] [#get_bd_addr_segs membank_" + str(bank_idx) + "/S00_AXI/Mem" + str(port_idx) +# "] #SEG_membank_" + str(bank_idx) + "_Mem" + str(port_idx) + "\n"#
	#		curr_loc = curr_loc + range_addr#
	#for port_idx in range(0,num_ports):#
	#	temp_text = temp_text + "  crea#te_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr#) + " -offset #0x" + '{:08x}'.format(curr_#loc) + " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_#segs #membank_out/S00_AXI/Mem" + #str(port_idx) + "] SEG_membank_out_Mem" + str(port_idx#) + "#\n"
	#	curr_loc = curr_loc + range_add#r#
	##RANGES HERE?#
	#for bank_idx #in range(0,num_weight_b#anks):
	#	temp_text# = temp_text + "  creat#e_bd_addr_seg -range 0x" + '{:08x}'.format(0x00010000) + " -offset #0x" +# '{:08x}'.format(curr_l#oc) + " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs #in_da#ta_port_0/S00_AXI/Mem" #+ str(bank_idx) + "] SEG_in_data_port_0_Mem" + str(bank_id#x) + "\n"
	#	curr_loc #= curr_loc + 0x00010000#
	##single segme#nts#
	#temp_text = t#emp#_text + "  create_bd#_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" +# '{:08x}'#.fo#rmat(curr_loc) + " [#get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs M_AXI_DDRA/#M_AXI_DDR#A/R#eg] SEG_M_AXI_DDRA_M#em\n"#
	#curr_loc = cu#rr_#loc + range_addr#
	#temp_text = t#emp#_text + "  creat#e_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" +# '{:08x}'#.fo#rmat(curr_loc) +# " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs M_AXI_DDRB/#M_AXI_DDR#B/R#eg] SEG_M_AXI_DD#RB_Mem\n"#
	#curr_loc = cu#rr_#loc + range_addr#
	#temp_text = t#emp#_text + "  creat#e_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" +# '{:08x}'#.fo#rmat(curr_loc) +# " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs M_AXI_DDRC/#M_AXI_DDR#C/R#eg] SEG_M_AXI_DD#RC_Mem\n"#
	#curr_loc = cu#rr_#loc + range_addr#
	#temp_text = t#emp#_text + "  creat#e_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" +# '{:08x}'#.fo#rmat(curr_loc) +# " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs M_AXI_DDRD/#M_AXI_DDR#D/R#eg] SEG_M_AXI_DD#RD_Mem\n"#
	#curr_loc = cu#rr_#loc + range_addr#
	#temp_text = t#emp#_text + "  creat#e_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" +# '{:08x}'#.fo#rmat(curr_loc) +# " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs axi_bram_ctrl_bias/#S_AXI/Mem#0] #SEG_axi_bram_ctr#l_bias_Mem0\n"#
	#curr_loc = cu#rr_#loc + range_addr#
	#temp_text = t#emp#_text + "  creat#e_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" +# '{:08x}'#.fo#rmat(curr_loc) +# " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs axi_bram_ctrl_conv/#S_AXI/Mem#0] #SEG_axi_bram_ctr#l_conv_Mem0\n"#
	#curr_loc = cu#rr_#loc + range_addr#
	#temp_text = temp_text + "  creat#e_bd_addr_seg -range 0x" + '{:08x}'.format(range_addr) + " -offset 0x" +# '{:08x}'.format(curr_loc) + " [get_bd_addr_spaces S_AXI_PCIS] [get_bd_addr_segs axi_bram_ctrl_pool/#S_AXI/Mem0] SEG_axi_bram_ctrl_pool_Mem0\n"
	#curr_loc = curr_loc + range_addr


	tcl_index = out_text.find(address_mapping_str) + len(address_mapping_str)
	out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]


	if 1 == 0:
	#HERE
		#Exporting design
		temp_text = temp_text = createExportDesign(design_name, vivado_project_path)

 		tcl_index = out_text.find(export_ip_str) + len(export_ip_str)
		out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

	with open(out_fn, 'w+') as f:
		f.write(out_text)

#generate acc ip and membank together in a single block diagram
#def generateC


def main():
	#read membank input params
	#vivado_project_path = r"./inference_net_project_vivado/ip_repo"
	vivado_project_path = r"./ip_repo" #TODO Change to ip repo
	membank_name = "membank"
	conv_acc_name = "conv_acc"
	num_ports = 32
	address_width = 32
	data_width = 32
	num_weight_banks = 32
	mem_size = 4096
	start_addr = 0x00000000
	range_addr = 0x00001000

	#generate weight membank
	#generateMembank(membank_name, vivado_project_path, num_ports, address_width, data_width, mem_size,range_addr, start_addr)
	generateConvAcc(conv_acc_name, vivado_project_path, num_ports, address_width, data_width, mem_size, range_addr, start_addr , num_weight_banks)

if __name__ == "__main__":
	main()



