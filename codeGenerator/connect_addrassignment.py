import math


# #
# for port_group in xrange(0,8):
#     for port_num in xrange(0, 32):
#         if port_num == 0:
#             print "connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/w_buf_0_" + str(port_group)+ "_" +str(port_num)+ "_PORTA] [get_bd_intf_pins in_weight_port_" +str(port_group)+ "/BRAM_PORTB]"
#         else:
#             print "connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/w_buf_0_" + str(port_group) + "_" + str(
#                 port_num) + "_PORTA] [get_bd_intf_pins in_weight_port_" + str(port_group) + "/BRAM_PORTB_" +str(port_num)+ "]"
#
# for port_group in xrange(0,1):
#     for port_num in xrange(0, 32):
#         if port_num == 0:
#             print "connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/out_buf_" + str(port_group)+ "_" +str(port_num)+ "_PORTA] [get_bd_intf_pins db_out_port_v2_" +str(port_group)+ "/BRAM_PORTA]"
#         else:
#             print "connect_bd_intf_net [get_bd_intf_pins conv_core_syn_0/out_buf_" + str(port_group) + "_" + str(
#                 port_num) + "_PORTA] [get_bd_intf_pins db_out_port_v2_" + str(port_group) + "/BRAM_PORTA_" +str(port_num)+ "]"

# for port_group in xrange(0,8):
#     print "connect_bd_net [get_bd_pins in_weight_port_" +str(port_group)+ "/ACLK] [get_bd_pins aws_0/clk_main_a0_out]"
#
# for port_group in xrange(0, 8):
#     print "connect_bd_net [get_bd_pins in_weight_port_" +str(port_group)+ "/ARESETN] [get_bd_pins aws_0/rst_main_n_out]"

# for port_group in xrange(0, 8):
#     print "connect_bd_intf_net [get_bd_intf_pins in_weight_port_" +str(port_group)+ "/S00_AXI] -boundary_type upper [get_bd_intf_pins axi_mem_intercon/M0" +str(port_group+7)+ "_AXI]"



#assign_bd_address [get_bd_addr_segs {in_weight_port_1/S00_AXI/Mem* }]
start_point = 8
step = 1
alpha = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'A', 'B', 'C', 'D', 'E', 'F']
counter = 0
port = 0
index = 0
#for index in xrange(8, 16):

# print "set_property -dict [list CONFIG.MASTER_TYPE {BRAM_CTRL}] [get_bd_intf_pins in_weight_port_*/BRAM_PORTB*]"
for port in xrange(0, 2):
    for round in xrange(0,2):
        for x in xrange(0, 16):
            print "assign_bd_address [get_bd_addr_segs {membank_"+str(port)+"/S00_AXI/Mem"+str(counter)+"}]"
            print "set_property offset 0x00000000040"+str(alpha[index])+str(alpha[x])+ "000 [get_bd_addr_segs {axi_cdma_0/Data/SEG_membank_"+str(port)+"_Mem"+str(counter)+"}]"
            print "set_property offset 0x00040"+str(alpha[index])+str(alpha[x])+"000 [get_bd_addr_segs {aws_0/M_AXI_PCIS/SEG_membank_"+str(port)+"_Mem"+str(counter)+"}]"
            counter = counter+1
            if counter == 32:
                counter = 0
        index += 1

#print "set_property -dict [list CONFIG.MASTER_TYPE {BRAM_CTRL}] [get_bd_intf_pins in_weight_port_*/BRAM_PORTB*]"
# for round in xrange(0,2):
#     for x in xrange(0, 16):
#         print "assign_bd_address [get_bd_addr_segs {acc_with_bank_0/S00_AXI_"+str(2+port)+"/Mem"+str(counter)+"}]"
#         print "set_property offset 0x00000000020"+str(alpha[index])+str(alpha[x])+ "000 [get_bd_addr_segs {axi_cdma_0/Data/SEG_acc_with_bank_0_Mem"+str(counter)+"}]"
#         #print "set_property range 4K [get_bd_addr_segs {axi_cdma_0/Data/SEG_acc_with_bank_0_Mem"+str(counter)+"}]"
#         print "set_property offset 0x00020"+str(alpha[index])+str(alpha[x])+"000 [get_bd_addr_segs {aws_0/M_AXI_PCIS/SEG_acc_with_bank_0_Mem"+str(counter)+"}]"
#         #print "set_property range 4K [get_bd_addr_segs {aws_0/M_AXI_PCIS/SEG_acc_with_bank_0_Mem"+str(counter)+"}]"
#         counter = counter+1
#     index += 1
#counter = 0

# for round in xrange(0,2):
#     for x in xrange(0, 16):
#         print "assign_bd_address [get_bd_addr_segs {conv_acc_2_0/S00_AXI_1/Mem"+str(counter)+"}]"
#         print "set_property offset 0x0000000000"+str(alpha[index] +4)+str(alpha[x])+"0000 [get_bd_addr_segs {axi_cdma_0/Data/SEG_conv_acc_2_0_Mem"+str(counter)+"}]"
#         print "set_property offset 0x000" + str(alpha[index]) + str(alpha[x]) + "000 [get_bd_addr_segs {aws_0/M_AXI_PCIS/SEG_conv_acc_2_0_Mem" + str(counter) + "}]"
#         counter = counter+1
#     index += 1
