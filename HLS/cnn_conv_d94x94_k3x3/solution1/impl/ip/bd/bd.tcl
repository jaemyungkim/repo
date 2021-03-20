# ==============================================================
# File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
# Version: 2016.4
# Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
# 
# ==============================================================

source -notrace [file join [file dirname [get_property XML_FILE_NAME [get_ipdefs -all -quiet xilinx.com:ip:xbip_utils:3.0]]] "common_tcl/common.tcl"]
common_tcl::gen_procs xilinx.com:hls:cnn_conv_d94x94_k3x3:1.0

source_subcore_ipfile xilinx.com:ip:xbip_utils:3.0 "common_tcl/vip.tcl"

proc init {cellpath otherInfo } {
    set cellobj [get_bd_cells $cellpath]

    vip_set_datatype_file "none"
    vip $cellobj puts_debug "INIT ..............." ;# This will create the Virtual IP object
}

proc post_config_ip {cellpath otherInfo } {
    # Any updates to interface properties based on user configuration
    set cellobj [get_bd_cells $cellpath]

    vip $cellobj puts_debug "POST_CONFIG_IP Starting..............."

    vip $cellobj update_busparams true
    vip $cellobj update_datatypes

    vip $cellobj puts_debug "...........POST_CONFIG_IP Complete"
}

proc propagate { cellpath otherInfo } {
    set cellobj [get_bd_cells $cellpath]

    # Refresh again.
    vip $cellobj update_busparams
    vip $cellobj update_datatypes
}

proc post_propagate { cellpath otherInfo } {
    set cellobj [get_bd_cells $cellpath]

    vip $cellobj puts_debug "POST_PROPAGATE Starting..............."
    vip $cellobj validate_datatypes
    vip $cellobj puts_debug "...........POST_PROPAGATE Complete"
}