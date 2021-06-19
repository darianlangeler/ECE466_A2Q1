#include "systemc.h"
#ifndef MEMORY_H
#define MEMORY_H

#define BLKSIZE 4

// Define Operations
#define RDBLK 0
#define RDBYT 1
#define WTBYT 2
#define WTBLK 3

// High-Z
// static const sc_lv<8> Z = "ZZZZZZZZ";

SC_MODULE(mem_testbench) 
{

  sc_in_clk clk;
  sc_out <bool> reset;
  sc_out <bool> new_comm;
  sc_in <bool> complete;

  sc_out < sc_uint<2> > comm;
  sc_out < sc_uint<8> > addr;
  sc_inout <sc_lv<8> > data;

  void test() 
  {
  	// Declare test data
  	sc_lv<8> blockdata[BLKSIZE] = {0x0f, 0x33, 0xcc, 0xf0};
  	sc_lv<8> writedata_1 = 0xff;
  	sc_lv<8> writedata_2 = 0x00;
  	sc_lv<8> writedata_3 = 0x55;
  	
  	// Reset Memory
  	reset.write(true);
  	wait();
  	reset.write(false);

  	//Write first WRBYT command
  	write_byte(writedata_1, 0);
  	wait();

  	// Begin Second WRBYT
  	write_byte(writedata_2, 255);
  	wait();

  	// Begin first RDBYT
  	read_byte(0);
  	wait();

  	// Begin second RDBYT
  	read_byte(255);
  	wait();

  	// Write Block @ 64
  	write_block(blockdata, 64);
  	wait();

  	// Write Byte @ 128
  	write_byte(writedata_3, 128);
  	wait();

  	// Read block @ 64
  	read_block(64);
	wait();

  	// Read byte @ 128
  	read_byte(128);
  	wait();

  }

  void read_block(sc_uint<8> addr_s)
  {
	new_comm.write(true);
	comm.write(RDBLK);
  	addr.write(addr_s);
  	
  	// Wait for memory to complete
  	while(!complete.read())
  	{
  		wait();
  	}
  	new_comm.write(false);
  	wait();
  }

  void write_block(sc_lv<8> data_s[BLKSIZE], sc_uint<8> addr_s)
  {
	new_comm.write(true);
	comm.write(WTBLK);
  	addr.write(addr_s);

  	// Write byte of data at each clock pulse
  	for(int i = 0; i < BLKSIZE; i++)
  	{
  		data.write(data_s[i]);
  		wait();
  	}


  	// Wait for memory to complete
  	while(!complete.read())
  	{
  		wait();
  	}
  	new_comm.write(false);

  	// Release data line
  	data.write(Z);
  	wait();
  }
  void read_byte(sc_uint<8> addr_s)
  {  
  void write_byte(sc_lv<8> data_s, sc_uint<8> addr_s)
  {
  SC_CTOR(mem_testbench)
  {
  	SC_CTHREAD(test, clk.pos());
  }
};
#endif