#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <vector>
#include "Delaunay.hpp"
#include "Springs.hpp"
#include "Nodes.hpp"
#include <sstream>
#include <fstream>


class Simulation
{

private:

  //No of mass points
  int N;
  double input_connectivity;
  double total_input_nodes;
  vector<Nodes> n;
  vector<Springs> s;
  vector<vector<double>> EdgeList;
  vector<double> NodeList;
  double w_out_initial;
  double w_out_final;
  double initial_log_uniform;
  double final_log_uniform;
  double initial_uniform;
  double final_uniform;


public:

  //Default constructor
  Simulation(double N, double ux, double uy, double input_conectivity, bool input_node, double w_in_initial, double w_in_final, double w_out_initial, double w_out_final, double range0x, double range1x, double range0y, double range1y, double initial_log_uniform, double final_log_uniform, double initial_uniform, double final_uniform)
  {
    srand (time(NULL));
    //Total number of mass points and the horizontal and vertical force
    this->N = N;
    //Step 2: The positions of the nodes were initialized and 20% of the nodes are connected to the input.
    this->input_connectivity = input_connectivity;
    this->total_input_nodes = input_connectivity * ((double)N);
    this->w_out_initial = w_out_initial;
    this->w_out_final = w_out_final;
    this->initial_log_uniform = initial_log_uniform;
    this->final_log_uniform = initial_log_uniform;
    this->initial_uniform = initial_log_uniform;
    this->final_uniform = initial_log_uniform;

    //This does the Delaunay triangulation with the width (range1x - range0x) and the heigh (range1y - range0y)
    DelaunayTriangulation DT(abs(range1x-range0x), abs(range1y-range0y));
    //Does node initialization and adds points for delaunay triangulation
    for(int i=0; i<N; i++)
    {
      Initialize_Nodes(range0x, range1x, range0y, range1y, input_node, w_in_initial, w_in_final);
      if(i < (int)total_input_nodes) n[i].Input_Nodes(ux, uy, Uniform(w_in_initial, w_in_final));
      DT.AddPoint(Point(n[i]).X_Position(), n[i].Y_Position());
    }
    //Shuffles it after
    random_shuffle(&n[0], &n[N-1]);
    DT.print();
    //This executes the triangulation after the frame has been made and the points inputted
    Delaunay_Triangulation();
    Initialize_Springs();
  }

 //This initializes the nodes and puts in appropriate values for the ranges and the weights
  void Initialize_Nodes(double range0x, double range1x, double range0y, double range1y)
  {
     Nodes p(Uniform(range0x, range1x), Uniform(range0y,range1y));
     //The first input_connectivity percent of the nodes are marked as input nodes, and the
     n.push_back(p);
  }

  void Delaunay_Triangulation()
  {
    char i;
    int j;
    stringstream s1;

    int iter=0;
    int total = 0;
    int nodea = 0;
    int nodeb = 0;
    //vector<Springs> s;
    vector<string> tri;
    string es;
    int k =0;

    int node1;
    int node2;
    int node3;
    char sep=',';

    int noofconnectingedges = 0;
    int vertices = 0;

    bool oneortwo;
    bool twoorthree;
    bool oneorthree;

    //	This takes the nodes of each triangle and parses them so only the relevant nodes a
    	for(auto e: DT.triangles)
    	{
    		//cout <<"Node1:" <<get<0>(e) <<" " << "Node2:"<<" " <<get<1>(e)<< endl;
    		s1 << e;
    	//	cout << e << endl;
    		tri.push_back(s1.str());
    		tri.at(k) = tri.at(k).substr(1, tri.at(k).size()-3);
    		cout << tri.at(k) << endl;
    //		cout <<endl;
            s1.str("");
            istringstream iss(tri.at(k));

            iss >> node1;
          //  cout <<"node1: " <<node1 <<endl;
            iss >>sep;
            iss >>node2;
          //  cout <<"node2: "<<node2 << endl;
            iss >>sep;
            iss >>node3;
        //    cout <<"node3: " << node3 << endl;
            iss >>sep;

            if(node1!=0 && node1!=1 && node1!=2 && node1!=3) vertices++;
            if(node2!=0 && node2!=1 && node2!=2 && node2!=3) vertices++;
            if(node3!=0 && node3!=1 && node3!=2 && node3!=3) vertices++;

            if(vertices ==2)
            {
    		noofconnectingedges++;
    	//	cout << "Connect two nodes!: " <<node1 <<" " <<node2 <<" "<< node3 <<" ";
    		Sort(node1, node2, node3);
    		if(node1!=0 && node1!=1 && node1!=2 && node1!=3)
    		{
    			NodeList.push_back(node1);
    			NodeList.push_back(node2);
    			EdgeList.push_back(NodeList);
    			NodeList.clear();
    	    }
    		else if(node2!=0 && node2!=1 && node2!=2 && node2!=3)
    		{
    			NodeList.push_back(node2);
    			NodeList.push_back(node3);
    			EdgeList.push_back(NodeList);
    			NodeList.clear();
    		}

    		//Add a spring
    	//	Springs = new Spring()
    	    }

    	    if(vertices ==3)
            {
    		noofconnectingedges+=2;
    		//cout << "Connect all nodes: " <<node1 <<" " <<node2 <<" "<< node3 <<" ";
    		Sort(node1, node2, node3);
    		if(node1!=0 && node1!=1 && node1!=2 && node1!=3)
    		{
    			NodeList.push_back(node1);
    			NodeList.push_back(node2);
    			NodeList.push_back(node2);
    			NodeList.push_back(node3);
    			EdgeList.push_back(NodeList);
    			NodeList.clear();
    		}
    		else if(node2!=0 && node2!=1 && node2!=2 && node2!=3)
    		{
    			NodeList.push_back(node2);
    			NodeList.push_back(node3);
    			EdgeList.push_back(NodeList);
    			NodeList.clear();
    		}
    		//Add a spring
    	    }
            vertices = 0;
    		k++;
     }
     sort (EdgeList.begin(), EdgeList.end());
     RemoveDuplicates(EdgeList);
  }

  void Initialize_Springs()
  {
    cout <<"The number of edges for: " <<N << "Mass points is: " << EdgeList.size() << endl;
  	//cout <<"The number of unduplicated edges should be " << EdgeList.size() << endl;
    //Spring and damping coefficients
  	double k1 = 0;
  	double d1 = 0;
  	double k3 = 0;
  	double d3 = 0;
  	double l0 = 0;

  	double x0;
  	double y0;
  	double x1;
  	double y1;
  	double wout;

  	int arraysubscript1=0;
  	int arraysubscript2=0;;
  	double initialvalue = 0;
  	double finalvalue = 1;

  	for(int i=0; i<EdgeList.size(); i++)
  	{
  		  //These take the arraysubscripts and disregard the first four points
  		  arraysubscript1 = EdgeList[i].at(0) - 4;
  		  arraysubscript2 = EdgeList[i].at(1) - 4;

  		  x0 = n[arraysubscript1].X_Position();
  		  x1 = n[arraysubscript2].X_Position();
  		  y0 = n[arraysubscript1].Y_Position();
  		  y1 = n[arraysubscript2].Y_Position();

  		  //These damping and spring coefficiets are done according to the specs in the paper. i need more guidance on how to do it properly though, it needs testing.
  		  k1 = Spring_And_Damping_Coefficient_1(initial_log_uniform, final_log_uniform);
        d1 = Spring_And_Damping_Coefficient_1(initial_log_uniform, final_log_uniform);
  	    k3 = Spring_And_Damping_Coefficient_2(initial_uniform, final_uniform);
  	    d3 = Spring_And_Damping_Coefficient_2(initial_uniform, final_uniform);

  		  l0 = Eucl_Dist(x0, y0, x1, y1);
  		  wout = Uniform(w_out_initial, w_out_final);

  	    s.push_back(Springs(k1, d1, k3, d3, l0, arraysubscript1, arraysubscript2, wout));
  	    s[i].Output();
     }
  }

  void WorkitOUt


  int Random_Input_Nodes(int N)
  {
  	return N*((int) rand() / (RAND_MAX));
  }

  double Uniform(double M, double N)
  {
  	return M + (rand() / ( RAND_MAX / (N-M) ) ) ;
  }

  double Log_Uniform(double initial, double final)
  {
  	return exp(Uniform(initial, final));
  }

  double Spring_And_Damping_Coefficient_1(double initial, double final)
  {
  	return Log_Uniform(initial, final);
  }

  double Spring_And_Damping_Coefficient_2(double initial, double final)
  {
  	return Uniform(initial, final);
  }

  double Eucl_Dist(double x1, double y1, double x2, double y2)
  {
  	return sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
  }

  double Angle(double x1, double x2, double y1, double y2)
  {
  	return atan((y2-y1)/(x2-x1));
  }

  double X_Comp(double vectorsum, double theta)
  {
  	return vectorsum*cos(theta);
  }

  double Y_Comp(double sum, double theta)
  {
  	return sum*sin(theta);
  }

  void Sort(int &a, int &b, int &c){
      if(a>b)
  	{
          int tmp = a;
          a = b;
          b = tmp;
      }
      if(a>c)
  	{
          int tmp = a;
          a=c;
          c = tmp;
      }
      if(b>c)
  	{
          int tmp = b;
          b=c;
          c=tmp;
      }
  }

  void Sort(int &a, int &b)
  {
      if(a>b)
  	{
          int tmp = a;
          a = b;
          b = tmp;
      }
  }

  void RemoveDuplicates(vector<vector<double>> &x)
  {
  	// sort (x.begin(), x.end());
  	 int i=1;
  	 while(i<x.size())
  	 {
  	 	if(x[i][1] ==x[i-1][1] && x[i][0] ==x[i-1][0])
  	 	{
  		  x.erase(x.begin()+(i-1));
  		  i-=1;
      	}
        i++;
  	 }
  }

}
