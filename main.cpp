#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "Simulation.cpp"
#include "Eigen/Dense"
#include "Eigen/QR"


using namespace std;
using namespace Eigen;


unsigned long long rdtsc()
{
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}


int main(int argc, char** argv)
{

   auto begin = std::chrono::high_resolution_clock::now();


    InitialDataValues data;
    //send from processor
    srand(rdtsc());

    vector<double> Volterra;
    vector<double> Input;
    vector<string> Input_Lines;
    vector<string> Volterre_Lines;  // Todo: ???

    cout << "-- Start ---------------------------------------- " << endl;


    // Todo: I think it would be useful to define class for reading in files from csv, maybe best even encapsulated in a class
    // there are plenty of these out there
  //  ifstream file_Input ( "/Users/hh14185/Leverhulme_Trust_Proposal_spider_web/Xcode/Nonlinear-Mass-Spring-System-BR/input.csv" );
    ifstream file_Input ( "Data/inputsignal.csv" ); file_Input.precision(15);
    string tmp;

    while (getline(file_Input, tmp,'\n'))
    {
        Input_Lines.push_back(tmp); //Get each line of the file as a string
    }

    unsigned long s = Input_Lines.size();
    double x;
    for (unsigned int i=1; i<s; ++i)
    {
        size_t pos = Input_Lines[i].find(",");      // position of the end of the name of each one in the respective string
        x = stod(Input_Lines[i].substr(pos+1,Input_Lines[i].size()));
        x = x*1;
        Input.push_back(x); // convert string age to a double
    }



    // TODO:  again best to have that encapsulated in a class - makes the code much cleaner and we will use this code a lot
    //  ifstream file2 ("inputsignal.csv"); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/

  //  ifstream file_Volterra ("/Users/hh14185/Leverhulme_Trust_Proposal_spider_web/Xcode/Nonlinear-Mass-Spring-System-BR/volterra.csv");
      ifstream file_Volterra ("Data/volterra.csv");

    while (getline(file_Volterra, tmp,'\n'))
    {
        Volterre_Lines.push_back(tmp); //Get each line of the file as a string
    }

    s = Volterre_Lines.size();
    for (unsigned int i=1; i<s; ++i)
    {

        size_t pos = Volterre_Lines[i].find(",");      // position of the end of the name of each one in the respective string
        x = stod(Volterre_Lines[i].substr(pos+1,Volterre_Lines[i].size()));
        x = x*1;
        Volterra.push_back(x); // convert string age to a doubl
    }


    cout <<"Size of input signal is: "<< Input.size() << endl;
    cout <<"Size of target signal is: "<< Volterra.size() << endl;

    double wash_out_time = 20000;
    double learning_time = 200000;
    double learning_time_test = 15000;


    // setting parameters for simulation
    // This should be possible to read in from a text file
    data.N = 10;
    data.ux=0;
    data.uy= 0;

    data.input_connectivity_percentage = 20;
    //data.w_in_initial = -1;
    data.min_input_weight = -1;
    data.max_input_weight = 1;
    data.min_x_position = 0;
    data.max_x_position = 10;
    data.min_y_position  = 0;
    data.max_y_position = 10;

    data.min_k3 = 1;
    data.max_k3  = 100;
    data.min_d3 = 1;
    data.max_d3  = 100;

    data.min_k1 = 1;
    data.max_k1  = 200;
    data.min_d1 = 1;
    data.max_d1  = 200;


    data.dt = 0.001;
    data.t0 = wash_out_time*data.dt;
    data.tmax = (wash_out_time+learning_time+learning_time_test)*data.dt;

    vector<double> Sine_Wave;

    cout << "Initial Input is: "<< Input[0] << endl;

  //  Simulation sim(data, Volterra, Input, wash_out_time, learning_time, learning_time_test);
   //sim.Reset_Simulation();
  // sim.execute();
  // sim.output_LearningMatrix_and_MeanSquaredError();
/*
   sim.Reset_Simulation();
   sim.execute();
   sim.output_LearningMatrix_and_MeanSquaredError();

   sim.Reset_Simulation();
   sim.execute();
   sim.output_LearningMatrix_and_MeanSquaredError();

   sim.Reset_Simulation();
   sim.execute();
   sim.output_LearningMatrix_and_MeanSquaredError();
   */

   double Mean_Sq = 1000;
   double MSE = 0;

   double total_MSE = 0;

   for(int i=0; i<10; i++)
   {
        Simulation sim(data, Input, Volterra, wash_out_time, learning_time, learning_time_test);
        MSE = sim.output_LearningMatrix_and_MeanSquaredError();
        total_MSE += MSE;


        if(Mean_Sq>MSE)
        {
        Mean_Sq = MSE;
        cout <<"The best MSE at the moment is: " << Mean_Sq << endl;
        sim.output_Output_Signal();
        }

   }

   total_MSE = total_MSE/10;
   cout << "The average MSE: " << total_MSE << endl;
   cout << "The best MSE: " << Mean_Sq << endl;

//  cout <<"The number of nodes is: " << data.N << endl;
//  cout <<"The number of springs is: " << sim.Spring_List() << endl;

   auto end = std::chrono::high_resolution_clock::now();
   cout << "The time it took for the programme to run in total in milliseconds: ";
   std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms";


    return 0;
}
