from accSystemCons import addressAlloc

address_allocation = "# Address segments\n"

def addrAlloc(Instance, Bname, num_in_ports):
    temp_text = ""
    for bank_idx in range(0, num_in_ports):
        temp_text = temp_text + "assign_bd_address [get_bd_addr_segs {"+ Instance +"/" + Bname + "/Mem" + str(bank_idx) + "}]\n"
        # temp_text = temp_text + " set_property range 0x" + '{:08x}'.format(range_addr) + " [get_bd_addr_segs {"+Pname+"/SEG_"+Bname+"_" + str(bank_idx) + "_" + str(port_idx) + "_Mem0}]\n"
        # temp_text = temp_text + " set_property offset 0x" + '{:08x}'.format(curr_loc) + " [get_bd_addr_segs {"+Pname+"/SEG_"+Bname+"_" + str(bank_idx) + "_" + str(port_idx) + "_Mem0}]\n"
        # curr_loc = curr_loc + range_addr
    return temp_text

def addrforAcc(num_in_banks, num_in_ports, in_buf_start_addr, range_addr):
    template_fn = "addr.tcl"
    out_fn = "addr_gen.tcl"

    # open file for appending
    with open(template_fn, "r+") as f:
        out_text = f.read()

    temp_text = ""
    temp_text = temp_text + "\n"
    temp_text = temp_text + "\n"
    temp_text = temp_text + "assign_bd_address [get_bd_addr_segs {conv_acc_0/S_CTRL_BUS_AXI/Reg0 }]\\" + "\n"
    temp_text = temp_text + "set_property offset 0x010000 [get_bd_addr_segs {aws_0/M_AXI_BAR1/SEG_conv_acc_0_Reg0}]\\" + "\n"

    temp_text = temp_text + "\n"
    # temp_text = temp_text + addrAlloc("conv_acc_0", "S_IN_DATA", num_in_banks*num_in_ports*2)

    temp_text = temp_text + "assign_bd_address" + "\n"

    tcl_index = out_text.find(address_allocation) + len(address_allocation)
    out_text = out_text[:tcl_index] + temp_text + out_text[tcl_index:]

    with open(out_fn, 'w+') as f:
        f.write(out_text)


def main():
    num_in_banks = 1
    num_in_ports = 8
    in_buf_start_addr = 0x00400000
    in_w_start_addr = 0x00600000
    out_buf_start_addr = 0x00800000
    range_addr = 0x00001000

    addrforAcc(num_in_banks, num_in_ports, in_buf_start_addr, range_addr)


if __name__ == "__main__":
    main()
