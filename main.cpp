#include <iostream>
#include <cstdlib>

#include "Simulation.hpp"

using namespace std;



int main(int argc, char** argv)
{

  InitialDataValues data;
  srand(time(NULL));

  data.N =2;
  data.ux=1;
  data.uy= 0;
  data.input_connectivity = 0.2;
  data.w_in_initial = -1;
  data.w_in_final = 1;
  data.w_out_initial = -1;
  data.w_out_final = 1;
  data.range0x = 0;
  data.range1x = 1;
  data.range0y = 0;
  data.range1y = 1;
  data.initial_log_uniform = 1;
  data.final_log_uniform = 100;
  data.initial_uniform = 100;
  data.final_uniform = 200;
  data.t0 = 0;
  data.tmax = 10;
  data.dt = 0.001;

  Simulation sim(data);


  return 0;
}
