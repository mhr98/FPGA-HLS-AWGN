# HLS-based implementation of AWGN generator for FPGAs
High-level synthesis (HLS) based implementation of additive white Gaussian noise generator (AWGN). A very light and simple hardware implementation using Vitis HLS 2022.1 software.

The code can be synthesised, implemented and exported as IP block using Vitis HLS software. The IP block can be used in block based design using Vivado software, to be intgrated with other hardware blocks. The implemntation uses Box-Muller method [1] to generate two AWGN generated samples using two random integer values. The random integer function was implemented using linear feedback shift register (LSFR).

The cos, sin, and the sequar root functions were implemented usin lookup tables of length 2048 that was generated and saved in .h file.

The hardware was synthesised using Vitis HLS 2022.1, with clock 100 MHz, and the design was piplined with initiation interval (II) of 1, and with reasonable resource utlization. The tool report is also provided with the files as a reference.

The main Code: **`gau_gen_v5.cpp`**

Lookup tables file: **`look_up_tables.h`**

Report: **`report.txt`**

[1] G. E. P. Box, Mervin E. Muller "A Note on the Generation of Random Normal Deviates"
