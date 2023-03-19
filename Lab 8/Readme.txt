Run 'g++ analyze.cpp -o a' first. 

Driver python file is used to create the plots. It runs the executable with various values of y. x and z are hardcoded in the driver script and can be changed.

Driver script usage : python driver.py <input file> <lower bound of y> <higher bound of y>

Executable Usage : a.exe <vspace> <memoryspace> <diskblocks> <input file>

Executable Output : <% FIFO fault> <% LRU fault> <% Random fault>