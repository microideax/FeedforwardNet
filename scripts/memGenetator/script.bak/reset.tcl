update_compile_order -fileset sources_1
remove_files  /home/genex/Uni/Cad/Yao/Integration_Acc_Mem/projects/cl_convmpool_clkbuf/testing/project_1/project_1.srcs/sources_1/bd/membank/membank.bd
file delete -force /home/genex/Uni/Cad/Yao/Integration_Acc_Mem/projects/cl_convmpool_clkbuf/testing/project_1/project_1.srcs/sources_1/bd/membank/membank.bd
remove_files  /home/genex/Uni/Cad/Yao/Integration_Acc_Mem/projects/cl_convmpool_clkbuf/testing/project_1/project_1.srcs/sources_1/bd/conv_acc/conv_acc.bd
file delete -force /home/genex/Uni/Cad/Yao/Integration_Acc_Mem/projects/cl_convmpool_clkbuf/testing/project_1/project_1.srcs/sources_1/bd/conv_acc
set_property  ip_repo_paths  /home/genex/Uni/user-ip-local [current_project]
update_ip_catalog