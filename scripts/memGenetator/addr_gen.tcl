# Address segments

assign_bd_address [get_bd_addr_segs {conv_acc_0/S_CTRL_BUS_AXI/Reg0 }]\
set_property offset 0x010000 [get_bd_addr_segs {aws_0/M_AXI_BAR1/SEG_conv_acc_0_Reg0}]\

assign_bd_address
