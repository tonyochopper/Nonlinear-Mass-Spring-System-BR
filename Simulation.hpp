#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <vector>
#include "Delaunay.hpp"
#include "Springs.hpp"
#include "Nodes.hpp"
#include "Eigen/Dense"
#include <sstream>
#include <fstream>
using namespace std;
using namespace Eigen;



struct InitialDataValues
{
	double N;
	double ux;
	double uy;
	double input_connectivity;
	double w_in_initial;
	double w_in_final;
	double w_out_initial;
	double w_out_final;
	double range0x;
	double range1x;
	double range0y;
	double range1y;
	double initial_log_uniform;
	double final_log_uniform;
	double initial_uniform;
	double final_uniform;
  double t0;
	double tmax;
	double dt;
};



class Simulation
{

private:

  //No of mass points
  int N;
  double input_connectivity;
  double total_input_nodes;
  vector<Nodes> n;
  vector<Springs> s;
	vector<vector<double>> Loutput;
  vector<vector<double>> EdgeList;
  vector<double> NodeList;
	vector<double> EdgeNodeList;
	vector<double> Learning_Weights;
  double w_out_initial;
  double w_out_final;
  double w_in_initial;
  double w_in_final;
  double initial_log_uniform;
  double final_log_uniform;
  double initial_uniform;
  double final_uniform;
  double t0;
  double tmax;
  double dt;

  bool input_node;

//Ranges for the delaunay triangulation
  double range0x;
  double range0y;
  double range1x;
  double range1y;

  //These are constant horizontal forces on the input nodes. If this changes so each input node receives a unique force we will have to modify the code
	double ux;
	double uy;

	//Learning weight vector after decomposition.




public:

  //Default constructor
  Simulation(InitialDataValues &data)
  {
    srand (time(NULL));
    //Total number of mass points and the horizontal and vertical force
    this->N = data.N;
    //Step 2: The positions of the nodes were initialized and 20% of the nodes are connected to the input.
    this->input_connectivity = data.input_connectivity;
    this->total_input_nodes = data.input_connectivity * ((double)N);
    this->w_out_initial = data.w_out_initial;
    this->w_out_final = data.w_out_final;
    this->initial_log_uniform= data.initial_log_uniform;
    this->final_log_uniform = data.initial_log_uniform;
    this->initial_uniform = data.initial_log_uniform;
    this->final_uniform = data.initial_log_uniform;
    this->t0 = data.t0;
    this->tmax = data.tmax;
    this->dt = data.dt;

		this->range0x = data.range0x;
		this->range0y = data.range0y;
		this->range1x = data.range1x;
		this->range1y = data.range1y;

    this->ux = data.ux;
    this->uy = data.uy;

		Initialize_Nodes(range0x, range1x, range0y, range1y);
    Delaunay_Triangulation_and_Spring_Creation();

		if(EdgeList.size() < N)
		{
			cout <<"Delaunay Triangulation needs to be done again, there are unconnected nodes.";
		}

  }

	void Delaunay_Triangulation_and_Spring_Creation()
	{
			DelaunayTriangulation DT(abs(range1x-range0x), abs(range1y-range0y));

			//Does node initialization and adds points for delaunay triangulation
			for(int i=0; i<N; i++)
			{
				if(i < (int)total_input_nodes) n[i].Input_Node(ux, uy, Uniform(w_in_initial, w_in_final));
				DT.AddPoint(Point(n[i].X_Position(),n[i].Y_Position()));
			//  DT.AddPoint(Point(n[i].X_Position(), Point(n[i].Y_Position());
			}

			random_shuffle(&n[0], &n[N-1]);
			DT.print();

			Get_Triangles(DT);
			Initialize_Springs();
			Execute_In_Time();
	}

	void Create_EdgeNodeList()
	{
		for(int i=0; i<EdgeList.size(); i++)
		{
			EdgeNodeList.push_back(s[i].Nodea());
			EdgeNodeList.push_back(s[i].Nodeb());
		}

		sort( EdgeNodeList.begin(), EdgeNodeList.end() );
		EdgeNodeList.erase( unique( EdgeNodeList.begin(), EdgeNodeList.end() ), EdgeNodeList.end() );

	}

	double Output_No_of_Edges()
	{
		return EdgeList.size();
	}

	void Output_Spring_And_Node_Positions()
	{
		for (int i =0; i<s.size(); i++)
		{
			s[i].Output();
			cout << endl;
			cout << "The position of Node a of spring " <<i << " " << n[s[i].Nodea()].X_Position() <<"," <<n[s[i].Nodea()].Y_Position();
			cout << endl;
			cout << "The position of Node b of spring " <<i << " " << n[s[i].Nodeb()].X_Position() <<"," <<n[s[i].Nodeb()].Y_Position();
			cout << endl;
		}
	}

 //This initializes the nodes and puts in appropriate values for the ranges and the weights
  void Initialize_Nodes(double range0x, double range1x, double range0y, double range1y)
  {
     for(int i=0; i<N; i++)
		 {
		 Nodes p(Uniform(range0x, range1x), Uniform(range0y,range1y));
     //The first input_connectivity percent of the nodes are marked as input nodes, and the
     n.push_back(p);
	   }
  }



  void Get_Triangles(DelaunayTriangulation &Delaunay)
  {
    stringstream s1;

    vector<string> tri;
    string es;
    int k =0;

    int node1;
    int node2;
    int node3;
    char sep=',';

		double vertices = 0;
		double noofconnectingedges = 0;


    //	This takes the nodes of each triangle and parses them so only the relevant nodes a
    	for(auto e: Delaunay.triangles)
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

		 		RemoveDuplicates(EdgeList);
   //Remove Duplicates from EdgeNodeList
  }

  void Initialize_Springs()
  {
    cout <<"The number of edges for: " <<N << " mass points is: " << EdgeList.size() << endl;
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

	void Moore_Penrose_Pseudoinverse(Eigen::MatrixXd& L, int maxtimesteps)
	{
		MatrixXd pinv = L.completeOrthogonalDecomposition().pseudoInverse();
	}

	void MatrixMultiply(MatrixXd& A, MatrixXd& B, MatrixXd& Output)
	{
		 int maxtimesteps = (int)((tmax-t0)/dt);
		 for(int i=0; i<maxtimesteps; i++)
		 {
			 for(int j=0; j<EdgeList.size(); j++)
			 {
				 cout <<"At time: " <<i*dt <<" The " <<j <<"th " <<"spring has length: " <<A(i,j);
				 cout <<endl;
			 }
		 }
	}

	void Populate_Learning_Weights(MatrixXd& L, MatrixXd& T, MatrixXd& Output, int maxtimesteps)
	{
		Moore_Penrose_Pseudoinverse(L, maxtimesteps);
		MatrixMultiply(L, T, Output);
	}

	double SineWave(double currenttime)
	{
		return sin(currenttime);
	}


  //This changes position of springs and nodes dynamically in time.
  void Execute_In_Time()
  {
    double Fsum =0;
    double Fx =0;
    double Fy =0;
    double theta = 0;
    double l = 0;

  //  ofstream ofs ("test.csv", ofstream::out);
	//Not interested in outputting at this stage

    double nodea =0;
    double nodeb = 0;

		double x0 = 0;
		double x1 = 0;
		double y0 = 0;
		double y1 = 0;

		//This loop does not include the initial timestep i=0
		//Everything is set initially.
    int maxtimesteps = (int)((tmax-t0)/dt);
		MatrixXd L(maxtimesteps,EdgeList.size());
		MatrixXd T(maxtimesteps, EdgeList.size());
		MatrixXd Output(maxtimesteps, EdgeList.size());

		double currentlength = 0;

		for(int j=0; j<EdgeList.size(); j++)
		{
			L(0,j)=s[j].Return_Original_Length();
			T(0,j)=SineWave(0);
		}

		for(int i=1; i<maxtimesteps; i++)
		{
 		for(int j=0; j<EdgeList.size(); j++)
    	{
				 s[j].ForceEq(Fsum);

    	   nodea = s[j].Nodea();
    	   nodeb = s[j].Nodeb();

    	   x0 = n[nodea].X_Position();
    	   x1 = n[nodeb].X_Position();
    	   y0 = n[nodea].Y_Position();
    	   y1 = n[nodeb].Y_Position();

    	   theta = Angle(x0, x1, y0, y1);
    	   Fx = X_Comp(Fsum, theta);
    	   Fy = Y_Comp(Fsum, theta);

    	   n[nodea].Change_Position(Fx, Fy, dt);
    	   n[nodeb].Change_Position(Fx, Fy, dt);

    	   x0 = n[nodea].X_Position();
    	   x1 = n[nodeb].X_Position();

    	   y0 = n[nodea].Y_Position();
    	   y1 = n[nodeb].Y_Position();

    	   l = Eucl_Dist(x0, x1, y0, y1);
				 currentlength = l;

				 L(i,j) = currentlength;
				 T(i,j) = SineWave(i*dt);

         s[j].Change_Length_And_Velocity(dt, l);
     }
	 }

	 Populate_Learning_Weights(L, T, Output, maxtimesteps);

  }

  int Random_Input_Nodes(int N)
  {
  	return N*((int) rand() / (RAND_MAX));
  }

  double Uniform(double M, double N)
  {
  	return M + (rand() / ( RAND_MAX / (N-M) ) ) ;
  }

	double Log_10_Uniform(double initial, double final)
	{
		return exp(Uniform(initial, final)/(2.302585093));
	}

  double Spring_And_Damping_Coefficient_1(double initial, double final)
  {
  	return Log_10_Uniform(initial, final);
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
  	 sort (x.begin(), x.end());
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

	Springs Spring_Return(int i)
	{
		return s[i];
	}

	Nodes NodeReturn(int i)
	{
		return n[i];
	}


	double Spring_List()
	{
		return EdgeList.size();
	}

};
