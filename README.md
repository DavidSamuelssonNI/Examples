# Example CAN_C_API

NI original Examples could be found:  
> C:\Users\Public\Documents\National Instruments\NI-XNET\Examples  

And nixnet.h:  
> C:\Program Files (x86)\National Instruments\Shared\CVI\Include  

lib
C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\lib64\msvc
dll
C:\Program Files (x86)\National Instruments\NI-XNET

C:\Program Files (x86)\National Instruments\RT Images\NI-XNET\PharLap

OBS! Added files to C:\build\2024Q4\x64\sysroots\core2-64-nilrt-linux\usr\lib\ from /usr/lib/x86_64-linux-gnu  
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnet.so  
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnetDB.so.1  
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixntdm.so.1  
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnthc.so.2  
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixntpi.so.1  
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnttcpip.so.23  
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libxnetTrace.so.1  


## Plotting:  

./CAN_C_API_Example > plot.txt  

using gnuplot  

opkg install gnuplot  

write:  
gnuplot  
in gnuplot:  
 - set terminal dumb  
 - set title "Plot of First Column vs Row Number"  
 - set xlabel "Sample Index"  
 - set ylabel "Column 1 Value"  
 - plot 'plot.txt' using ($0+1):1 with linespoints title "Column 1"  

or:  
./CAN_C_API_Example | gnuplot -persist -e "set terminal dumb; plot '-' using 0:2 with linespoints title 'Column 1 vs Column 2'"  