# Setup clock and signals
add_force -repeat_every 10 clk 0 0 1 5  ;# Clock with 100 MHz frequency
add_force reset 1  ;# Set reset initially
add_force rx 1  ;# UART idle state is high (1)

# Run simulation for 100 ns to allow for initialization
run 100

# De-assert reset after initialization
add_force reset 0
run 100

# Procedure to send a byte over UART via rx signal
# Parameters: 
# - byte: 8-bit value to send over UART

proc send_uart_byte { byte } {
    # UART start bit (0)
    add_force rx 0
    run 8680  ;# 1 bit time at 115200 baud = 8.68 us (8680 ns)
    
    # Send each data bit (LSB first)
    for {set i 0} {$i < 8} {incr i} {
        set bit [expr {($byte >> $i) & 1}]
        add_force rx $bit
        run 8680  ;# Send each bit for 8.68 us
    }

    # UART stop bit (1)
    add_force rx 1
    run 8680
}

# Example: Send the byte 0x50 ('P') over UART
send_uart_byte 0x50

# Run simulation long enough to allow byte transmission
run 1000