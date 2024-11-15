#getdna for 100t/gbox may need some ajustments in the future


get_dna.tcl
# set default hw_server connection 
set HW_SERVER localhost:3121

open_hw
if {[llength [get_hw_servers]] == 0 } then {
    connect_hw_server -quiet -url $HW_SERVER
    set disconnect_en 1
}

# update list of targets
refresh_hw_server -quiet

# close open targets
foreach TARGET [get_hw_targets] {
    if { [get_property IS_OPENED $TARGET] } then {
        close_hw_target -quiet $TARGET
  }
}

# loop for all open targets and get DNA values for all programmable FPGAs
foreach TARGET [get_hw_targets] {
    open_hw_target -quiet $TARGET
    refresh_hw_target -quiet $TARGET
    foreach DEVICE [get_hw_devices] {
        refresh_hw_device -quiet $DEVICE
        if { [get_property PROGRAM.IS_SUPPORTED $DEVICE] } then {
            set DNA_HEX [get_property REGISTER.EFUSE.FUSE_DNA $DEVICE]
            puts "FPGA DNA: $TARGET=>$DEVICE = DNA(hex)=$DNA_HEX"
}
  }
  close_hw_target -quiet $TARGET
}

if {$disconnect_en == 1 } {
    disconnect_hw_server -quiet
}
