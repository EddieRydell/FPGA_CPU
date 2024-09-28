add_force clk {0 0ns} {1 5ns} -repeat_every 10ns
add_force reset 1
run 10
add_force reset 0
