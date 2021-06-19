#include "systemc.h"

#include "memory.h"
#include "testbench.h"


int sc_main(int argc, char* argv[])
{
    //Memory and Testbench Instances
    mem_ctrl MC("MC");
    mem_testbench TB("TB");

    //Clock
    sc_clock clk("TestClock", 10, SC_NS, 0.5, 5, SC_NS);

    //Signals
    sc_signal <bool> reset, new_comm, complete;
    sc_signal < sc_uint<2> > comm;
    sc_signal < sc_uint<8> > addr;
    sc_signal_rv <8> data;
    // NOTE: _rv needed because of multiple drivers
  
    // Memory
    MC.clk(clk);
    MC.reset(reset);
    MC.data(data);
    MC.addr(addr);
    MC.comm(comm);
    MC.new_comm(new_comm);
    MC.complete(complete);

    // Test Bench
    TB.clk(clk);
    TB.reset(reset);
    TB.data(data);
    TB.addr(addr);
    TB.comm(comm);
    TB.new_comm(new_comm);
    TB.complete(complete);

    // Setup Tracing
    sc_trace_file* Tf = sc_create_vcd_trace_file("memory_traces");
    Tf->set_time_unit(1, SC_NS);
    sc_trace(Tf, clk  , "clk" );
    sc_trace(Tf, reset  , "reset" );
    sc_trace(Tf, comm  , "comm" );
    sc_trace(Tf, addr  , "addr" );
    sc_trace(Tf, data  , "data" );
    sc_trace(Tf, new_comm  , "new_comm" );
    sc_trace(Tf, complete  , "complete" );

    // Begin Simulation
    sc_start(500, SC_NS);
    sc_close_vcd_trace_file(Tf);
    return 0;
}