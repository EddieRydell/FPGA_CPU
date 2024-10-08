@echo off
REM ****************************************************************************
REM Vivado (TM) v2024.1 (64-bit)
REM
REM Filename    : simulate.bat
REM Simulator   : AMD Vivado Simulator
REM Description : Script for simulating the design by launching the simulator
REM
REM Generated by Vivado on Tue Oct 08 22:16:09 -0600 2024
REM SW Build 5076996 on Wed May 22 18:37:14 MDT 2024
REM
REM Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
REM Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
REM
REM usage: simulate.bat
REM
REM ****************************************************************************
REM simulate design
echo "xsim operating_system_behav -key {Behavioral:sim_1:Functional:operating_system} -tclbatch operating_system.tcl -view C:/Users/eddie/FPGA_CPU/FPGA_CPU_Vivado/operating_system_behav.wcfg -log simulate.log"
call xsim  operating_system_behav -key {Behavioral:sim_1:Functional:operating_system} -tclbatch operating_system.tcl -view C:/Users/eddie/FPGA_CPU/FPGA_CPU_Vivado/operating_system_behav.wcfg -log simulate.log
if "%errorlevel%"=="0" goto SUCCESS
if "%errorlevel%"=="1" goto END
:END
exit 1
:SUCCESS
exit 0
