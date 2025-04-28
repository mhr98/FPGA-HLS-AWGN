// gau_gen_v5: This program is used to implement AWGN generator utlizing HLS method,  
// the hardware can be exported as an IP after synthesis and implementation to be
// used in Vivado block based design.
// Copyright (C) 2025  Mohammad Safa
// GitHub Repository: https://github.com/mhr98/FPGA-HLS-AWGN
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <hls_math.h>
#include <ap_int.h>
#include <stdio.h>
#include <string.h>
#include "look_up_tables.h"


// Random integer generation functions
// implemented using two uncorrelated and
// independent functions for better accuracy 
// and parallel operation.

/////////////// random integer 1 /////////////

unsigned int get_rand()
{
	  static ap_uint<32> lfsr = 0x12345678;
	  unsigned int r;

	  //Implementing the feedback tabs
	  bool b_32 = lfsr.get_bit(32-32);
	  bool b_22 = lfsr.get_bit(32-22);
	  bool b_2  = lfsr.get_bit(32-2);
	  bool b_1  = lfsr.get_bit(32-1);
	  bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;
	  lfsr = lfsr >> 1;
	  lfsr.set_bit(31, new_bit);

	  r = lfsr%2048;
	  return r;
}

/////////////// random integer 2 /////////////

unsigned int get_rand2()
{
	  static ap_uint<32> lfsr = 0x45896371;
	  unsigned int r;

	  //Implementing the feedback tabs
	  bool b_32 = lfsr.get_bit(32-32);
	  bool b_22 = lfsr.get_bit(32-22);
	  bool b_2  = lfsr.get_bit(32-2);
	  bool b_1  = lfsr.get_bit(32-1);
	  bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;
	  lfsr = lfsr >> 1;
	  lfsr.set_bit(31, new_bit);

	  r = lfsr%2048;
	  return r;
}

///////////////////////////////////////////////////////////////


/////////////// hardware /////////////////

void gau_gen_v5(volatile float *output1 ,volatile float *output2 , volatile float *input)
{
#pragma HLS INTERFACE mode=m_axi bundle=BUS_1 depth=128 port=output1 offset=slave
#pragma HLS INTERFACE mode=m_axi bundle=BUS_2 depth=128 port=output2 offset=slave
#pragma HLS INTERFACE mode=m_axi bundle=BUS_1 depth=1 port=input offset=slave
#pragma HLS INTERFACE mode=s_axilite port=return

	//taking input data
	float sigma = *input;

	//used variable
	float sin_val, cos_val, sqrt_val,gau1,gau2;

	//uniform random integer samples
	unsigned int uni_sample1;
	unsigned int uni_sample2;


	for(int i=0; i<128; i++)
	{
		uni_sample1 = get_rand();
		uni_sample2 = get_rand2();

		//reading value from our tables (generated using matlab)
		sqrt_val = sqrt_look_up[uni_sample2];
		cos_val = cos_look_up[uni_sample1];
		sin_val = sin_look_up[uni_sample1];

		// box-Muller transformation
		gau1 = sigma * sqrt_val *  cos_val;
		gau2 = sigma * sqrt_val *  sin_val;

		*(output1 + i) = gau1;
		*(output2 + i) = gau2;
	}
}

