# Clock input at pin W5 (adjust if needed)
set_property PACKAGE_PIN W5 [get_ports clk]
set_property IOSTANDARD LVCMOS33 [get_ports clk]

# Reset input (assign the correct pin number)
set_property PACKAGE_PIN U18 [get_ports reset]
set_property IOSTANDARD LVCMOS33 [get_ports reset]

# LEDs
set_property PACKAGE_PIN L1 [get_ports LED_1]
set_property IOSTANDARD LVCMOS33 [get_ports LED_1]

# TX and RX for UART (assign the correct pin numbers)
set_property PACKAGE_PIN D18 [get_ports tx]
set_property IOSTANDARD LVCMOS33 [get_ports tx]

set_property PACKAGE_PIN D17 [get_ports rx]
set_property IOSTANDARD LVCMOS33 [get_ports rx]

# Set CFGBVS and CONFIG_VOLTAGE for bank 0
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]

