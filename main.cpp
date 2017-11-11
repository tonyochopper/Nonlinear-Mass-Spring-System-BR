#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <vector>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel>
//#include <CGAL/Delaunay_triangulation_2.h>
#include "Delaunay.hpp" 
#include <sstream>  
#include <set>   
#include <fstream>      
using namespace std;

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
//typedef K::Point_2 Point;
// check if githubt works / HH

class Springs
{
	//natural length of each spring, i.e., distance between mass nodes as time 0. This might not be necessary to include.
    private:
	double l0;
	double l;
	//l is the current length.
	double x1;
	//l-l0 is the x1 at time(t)
	double x2;	
    //spring constants, not approriate for spring constnats.
	double k1;
	double d1;
	double k3;
	double d3;
	
	//which nodes are each spring connected to
	int nodea;
	int nodeb;
	
	//the weight on each spring.
	double wout;
	
	public:
	Springs(double k1, double d1, double k3, double d3, double l0, double nodea, double nodeb, double wout)
	{
		this->l0=l0;
		this->k1=k1;
	    this->k3=k3;
		this->d1=d1;
		this->d3=d3;	
		this->nodea = nodea;
		this->nodeb = nodeb;
		this->wout = wout;
		
		//The initial x1 = l0 - l0 =0, and initial spring velocity = 0
		this->x1 = 0;
		this->x2 = 0; 
	};
		
	void ForceEq(double &Fsum)
	{
		double p;
		double q;
		p=k3*x1*x1*x1 + k1*x1;
        q=d3*x2*x2*x2 + d1*x2;
        Fsum = -p-q;
	};		
	
	void Change_Length_And_Velocity(double &l, double &dt)
	{
		x1 = l - l0;
		double x2force = -(k3*x1*x1*x1 + k1*x1) - (d3*x2*x2*x2 + d1*x2);
		cout << x2force << endl;
		
		x2 += dt*x2force;
	}
	
	void Output()
	{
		cout <<"Node 1 is: " << nodea << endl;
		cout <<"Node 2 is: " << nodeb << endl;
		cout <<"Length of spring at rest is: " << l0 << endl;
	}
	
	double Nodea()
	{
		return nodea;
	}
	
	double Nodeb()
	{
		return nodeb;
	}
	
//	void InputNextX1()
//	{
//		x1 
//	}
};

class Nodes
{
	//the node has mass M=1
	private:
	double m =1;
	//Cartestian coordinates
	double x_position;
	double y_position;
	
	//The velocity of the nodes initially it is zero.
	double pxdot = 0;
	double pydot = 0;
	
	int connections;

	//spring constants, not approriate for spring constnats.
//	double k;
//	double d;
    //wheter node is connected to input or not.
    bool input_node=0;
    //If it is an input node, than input force u in the x and y directions
    //For the calculations it's not relevant.
    double ux;
    double uy;
    //if it is an input node, than what is the input weight?
    double win;
    //L non crossing
    //Each node has a certain number of springs attached to it. Each spring acts as a force on the node, which changes its position, and hence the position of the spring.
	int noofconnectedsprings;
    
    public:
     int test = 0;
     void declarepositionvariables(double x_position, double y_position)
	{
		this->x_position=x_position;
	    this->y_position=y_position;
	//	u = this->u;
	}
	

	
	
    void Input_Nodes(bool input_node, double ux, double uy, double win)

	 //void inputnodes(bool input_node)
	 {
	 	this->input_node = input_node;
	 	this->ux = ux;
	 	this->uy = uy;
	 	this->win = win;
	 }
	
	

	void Output_Position()
	{
		cout <<x_position << " " <<y_position <<" " << input_node;
	}
	
	double X_Position()
	{
		return x_position;
	}
	
	double Y_Position()
	{
		return y_position;
	}
	
	//This is the function that incrementally changes the nodes position in the next timestep;
	void Change_Position(double Fx, double  Fy, double  dt)
	{
		//double win = this->win;
	//	double ux = this->ux;
	//	double uy = this->uy;
	    
	    //declare acceleration;
	    //declare velocity;
	    
	    double pxdotdot;
	    double pydotdot;
	    
	    double pxdot;
	    double pydot;
		
		pxdotdot= (Fx + win*ux)/m;
        pydotdot= (Fy/m);
        
        //You want to calculate the velocity so that initial velocity is 0 and than calculate the corresponding change in position
        pydot = EulerMethod(pydotdot, pydotdot, dt);
        this->y_position = EulerMethod(y_position, pydot, dt);

        pxdot = EulerMethod(pxdotdot, pxdotdot, dt);
        this->x_position = EulerMethod(x_position, pxdot, dt);
	}
	
	double EulerMethod(double x0, double f, double dt)
	{
		double xnew = x0 + dt*f;
		return xnew;	
	}
	
	
	
    //whether node is connected to output or not;
    //bool output_node;
    //That doesn't make a difference per se whether the node is an output node.
    
	
	//the node has position (x,y) which is determined by a uniform distribution. Keep it simple for the moment and have it determined by a uniform distribution between 0 and 1.
	/*
	void initializeposition()
	{
	    uniform_real_distribution<double> distribution(0.0,1.0);
	    cout << 
	}
	*/
};
/*
d.nInputs = 1; 				% number of inputs
d.in_conn  = 0.2; 		 	% input connectivity procentage
d.w_in_range = [-1 +1]; 	% range of random input connections
d.nOutputs = 1; 			% number of inputs
d.out_conn  = 1; 		 	% output connectivity (1=100% all nodes)
d.w_out_range = [-1 +1]; 	% range of output connections
d.w_fb_range = [-1 +1];     % range for feedback weights
d.fb_conn  = 0.0;  			% feedback connectivity 
*/

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//This should be int but it's double to take into account the percentage factor.
int randominputnodes(int N)
{
	return N*((int) rand() / (RAND_MAX));
}
/*
void shufflenodes(Nodes **m)
{
	std::random_shuffle(&m[0], &m[N-1]);
}
*/

 void load_points(std::vector<Point>& rPoints)
 {
  rPoints.push_back(Point(10,10));   // first point
  rPoints.push_back(Point(60,10));   // second point
  rPoints.push_back(Point(30,40));   // third point
  rPoints.push_back(Point(40,80));   // fourth point
 }


double uniform(double M, double N)
{
    
	return M + (rand() / ( RAND_MAX / (N-M) ) ) ;  
}

//This code was forked from wikipedia
double lognormal(double mu, double sigma, double initialvalue, double finalvalue)
{
	static const double epsilon = std::numeric_limits<double>::min();
	static const double two_pi = 2.0*3.14159265358979323846;

	thread_local double z1;
	thread_local bool generate;
	generate = !generate;

	if (!generate)
	   return z1 * sigma + mu;

	double u1, u2;
	do
	 {
	   u1 = uniform(initialvalue, finalvalue);
	   u2 = uniform(initialvalue, finalvalue);
	 }
	while ( u1 <= epsilon );

	double z0;
	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return exp(z0 * sigma + mu);
}

//double lognormal

double Spring_And_Damping_Coefficient_1(double mu, double stdev, double initial, double final)
{
	return lognormal(mu, stdev, initial, final);
}

double Spring_And_Damping_Coefficient_2(double initial, double final)
{
	return uniform(initial, final);
}

double EuclDist(double x1, double y1, double x2, double y2)
{
	return sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
}

double Angle(double x1, double x2, double y1, double y2)
{
	return atan((y2-y1)/(x2-x1));
}

double X_com(double vectorsum, double theta)
{
	return vectorsum*cos(theta);
}

double Y_com(double sum, double theta)
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




int main(int argc, char** argv) 
{
//	initializeposition();
   //Step 1: The nodes were initialized according to an initial pattern and number of nodes N
   srand (time(NULL));
   //Total number of mass points
   int N = 70;
   // HOrizontal force
   double ux = 1;
   //Vertical force
   double uy = 0;
   Nodes* n = new Nodes[N];
   cout << n[0].test;
   
   cout <<randominputnodes(0.2) <<endl;
   
   //Step 2: The positions of the nodes were initialized and 20% of the nodes are connected to the input.
   double input_connectivity = 0.2;
   double totalinputnodes = input_connectivity * ((double)N);
 //  addinputnodes(&n, N, input_connectivity);
   
   for(int i=0; i<N; i++) 
   {
       n[i].declarepositionvariables(uniform(0,1), uniform(0,1));
   if(i < (int)totalinputnodes) n[i].Input_Nodes(1, ux, uy, uniform(-1, 1));
   }
   
   random_shuffle(&n[0], &n[N-1]);
   for(int i=0; i<N; i++)
   {
   n[i].Output_Position();
   cout <<endl;
   }
   
  // Step 3: USe the delauney triangulation to determien how many nodes have springs and where
   
   
   

 //for(Delaunay::Finite_edges_iterator it = dt.finite_edges_begin(); it != dt.finite_edges_end(); ++it)
// {
// }
   
   
   
   
   
    DelaunayTriangulation DT(1, 1);
   
   
//    for (int i = 0; i < N; i++)
//  {
 //   cout << "i: " << i << " / " << N << endl;
 //   DT.AddPoint(Point(n[i].xposition(), n[i].yposition()));
//  } 

  
  DT.AddPoint(Point(0.1, 0.41));
  DT.AddPoint(Point(0.15, 0.41));
   DT.AddPoint(Point(0.125, 0.38));
//  DT.AddPoint(Point(0.1, 0.42));
//  DT.AddPoint(Point(0.1, 0.40));
//  DT.AddPoint(Point(0.1, 0.42));
//  DT.AddPoint(Point(0.1, 0.41));
// DT.AddPoint(Point(0.1, 0.40));
//  DT.AddPoint(Point(0.1, 0.42));
 // DT.AddPoint(Point(0.3, 0.41));
// DT.AddPoint(Point(0.2, 0.24));
//  DT.AddPoint(Point(0.18, 0.82));
//  DT.AddPoint(Point(0.35, 0.99));
//  DT.AddPoint(Point(0.67, 0.9));
//  DT.AddPoint(Point(0.75,0.25));
// DT.AddPoint(Point(0.59, 0.49));
//    DT.AddPoint(Point(0.5, 0.5));
   cout << "Reporting" << endl;
    //DT.print();
    
    int relevantedges = 0;
    
    for(int i=0; i<DT.triangles.size(); i++)
    {
    //	cout << get<0>(DT.edges[i]) << get<1>(DT.edges[i]);
    //	cout << endl;
    //DT.triangles
    
    //	if((>3) relevantedges++;
    //	cout << endl;
	}
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
	
	vector<vector<double>> EdgeList;
	vector<double> NodeList;
	
//	This takes the nodes of each triangle and once the code is completed will 
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
        cout <<"node1: " <<node1 <<endl;
        iss >>sep;
        iss >>node2;
        cout <<"node2: "<<node2 << endl;
        iss >>sep;
        iss >>node3;
        cout <<"node3: " << node3 << endl;
        iss >>sep;
        
        if(node1!=0 && node1!=1 && node1!=2 && node1!=3) vertices++;
        if(node2!=0 && node2!=1 && node2!=2 && node2!=3) vertices++;
        if(node3!=0 && node3!=1 && node3!=2 && node3!=3) vertices++;
        
        if(vertices ==2) 
        {
	//	noofconnectingedges++;
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
		cout << "Connect all nodes: " <<node1 <<" " <<node2 <<" "<< node3 <<" ";
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
        
        
        /*
        //This takes the total number of edges that connect up the nodes, or in this case the node points.
        if(oneortwo || oneorthree || twoorthree)
        {
        cout <<"This has a connecting edge!";
		noofconnectingedges++;
	    }
		oneortwo = 0;
		oneorthree = 0;
		twoorthree = 0;
		*/
		k++;
			
	}
	
	//I have each node number. I need to take the ones that connect up to each node. Neglect points 0, 1, 2 and 3.
	
	cout <<"The number of edges should be: " << EdgeList.size() << endl;
	sort (EdgeList.begin(), EdgeList.end());
		for(int i =0; i<EdgeList.size(); i++)
	{
		cout <<EdgeList[i].at(0) <<" " << EdgeList[i].at(1);
		cout <<endl;
	}
	RemoveDuplicates(EdgeList);
	cout <<"The number of unduplicated edges should be " << EdgeList.size() << endl;
	
		for(int i =0; i<EdgeList.size(); i++)
	{
		cout <<EdgeList[i].at(0) <<" " << EdgeList[i].at(1);
		cout <<endl;
		
	}
	
	
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
//	double l0 = ;
	
	//for(int i=0; i<)
	vector<Springs> s;
	
	//For uniform and log normal distribution
	double initialvalue = 0;
	double finalvalue = 1;
	double mean = 0;
	double stdev = 1;
	
	for(int i=0; i<EdgeList.size(); i++)
	{
		  arraysubscript1 = EdgeList[i].at(0) - 4;
		  arraysubscript2 = EdgeList[i].at(1) - 4;
		  x0 = n[arraysubscript1].X_Position();
		  cout << endl << x0;
		  x1 = n[arraysubscript2].X_Position();
		  cout << endl << x1;
		  y0 = n[arraysubscript1].Y_Position();
		  cout << endl << y0;
		  y1 = n[arraysubscript2].Y_Position();
		  cout << endl << y1;
		  cout <<endl;
		  
		    //These damping and spring coefficients are not working properly, i will have to fix this somehow.
		  	k1 = Spring_And_Damping_Coefficient_1(mean, stdev, initialvalue, finalvalue);
            d1 = Spring_And_Damping_Coefficient_1(mean, stdev, initialvalue, finalvalue);
	        k3 = Spring_And_Damping_Coefficient_2(initialvalue, finalvalue);
	        d3 = Spring_And_Damping_Coefficient_2(initialvalue, finalvalue);
	        
	        cout <<"k1 is: " << k1 << endl;
	        cout <<"d1 is: " << d1 << endl;
	        cout <<"k3 is: " << k3 << endl;
	        cout <<"d3 is: " << d3 << endl;
		  
		  
		  l0 = EuclDist(x0, y0, x1, y1);
		  wout = uniform(-1, 1);
		  cout <<"output weight" << wout;
		  cout <<endl;
		  //cout <<"Output length: " << l0 << endl;
	      s.push_back(Springs(k1, d1, k3, d3, l0, arraysubscript1, arraysubscript2, wout));
	      s[i].Output();
	//    springs.push_back(s);
	 //   cout <<endl;
   }
   
 //  double wout;
   //So you want the connectivity of the nodes. So the output weights and lengths of the springs 
   
   double tmax = 1;
   double t0 = 0;
   //Execute this in time, 1000 time steps. First, each node is iterated through one time step according to each spring and node number."
   //First calculate the force on each node from each spring.
   double Fsum =0;
   double Fx =0;
   double Fy =0;
   double theta = 0;
   double dt = 0.001;
   double l = 0;
   
   ofstream ofs ("test.csv", ofstream::out);


   
   int maxtimesteps = (int)(tmax/dt);
   
   nodea =0;
   nodeb = 0;
   
  // double usum = 1;
   //double utheta = 
   


   for(int i=0; i<maxtimesteps; i++)

   {
   	  // cout <<"The position of the node at time " << i*dt <<  " is: " << n[nodea].xposition() << endl;
   	 //  cout <<"The position of the node at time " << i*dt <<  " is: " << n[nodea].yposition() << endl;
   	   
   	
		  
		  
		for(int j=0; j<EdgeList.size(); j++)
   	   {
   	   	s[j].ForceEq(Fsum);
   	   	//FindAngle
   	   	
   	   	nodea = s[j].Nodea();
   	    nodeb = s[j].Nodeb();
   	   	
   	   	x0 = n[nodea].X_Position();
   	   	x1 = n[nodeb].X_Position();
   	   	y0 = n[nodea].Y_Position();
   	   	y1 = n[nodeb].Y_Position();
   	   	
   	   	theta = Angle(x0, x1, y0, y1);
   	  // 	cout << "Theta is" << atan((y1-y0)/(x1-x0)) << endl;
   	   	cout <<"Theta is: "<< theta << endl;
   	   	Fx = X_com(Fsum, theta);
   	   	Fy = Y_com(Fsum, theta);

   	   	  
   	   n[nodea].Change_Position(Fx, Fy, dt);
   	   n[nodeb].Change_Position(Fx, Fy, dt);  
   	   
   	   x0 = n[nodea].X_Position();
   	   x1 = n[nodeb].X_Position();
   	   
   	   y0 = n[nodea].Y_Position();
   	   y1 = n[nodeb].Y_Position();
   	   
   	   
   	   
   	   l = EuclDist(x0, x1, y0, y1);
		  
       s[j].Change_Length_And_Velocity(l, dt);	
   	   
   	   cout <<"The position of the node" <<nodea << " at time " << i*dt <<  " is: " << n[nodea].X_Position() << endl;
   	   cout <<"The position of the node" <<nodeb << " at time " << i*dt <<  " is: " << n[nodea].Y_Position() << endl;
       }
       
       ofs << i*dt;
   	   ofs <<",";
   	   ofs << n[nodea].X_Position();
   	   ofs <<endl;
       
   	
   }


   ofs.close();
	

	

	

	
	
//	while(!s1.eof)
//	{
//		tri.at(k) = s1.str;
//	}
	
	//Here declare the random variables.
	
//	d.k_lim  =[100 200; 1 10];     % k spring constants range
   // d.d_lim  =[100 200; 1 10];     % d damping constants range
   //initial distance.
  // double lo;

//Experimental thing that didn't work out
/*
	while(!s1.eof())
	{
		s1 >> i;
		cout << i << endl;
		j = i - '0';
		if(j>3) 
		{
		iter++;
		if(nodea==0) nodea=j;
		else nodeb=j;
		cout << "Iter is: " << iter;
		cout << endl;
	    }
		if(i==')')
		{
			if(iter==2)
			{
			total++;
			lo = EuclDist(n[nodea].xposition(), n[nodea].yposition(), n[nodeb].xposition(), n[nodeb].yposition());
		//	Springs sp = new Springs(uniform(100, 200), uniform(100, 200), uniform(100, 200), uniform(100, 200), lo);
			s.push_back(Springs(uniform(100, 200), uniform(100, 200), uniform(100, 200), uniform(100, 200), lo));
			nodea=0;
			nodeb=0;
		    }
		    iter =0;
		}
	}
	*/
//	cout << endl << total << endl;
  //  const char* p = s1.str();  
	
//	cout << endl;
//	tri = p;
  //  cout << s1.str();

  //  cout << endl << DT.edges.size() << endl;  
//	cout << endl << relevantedges << endl;    
 //   DT.
    
 //   int number_of_springs;
    /*
    for(int i=0; i<sizeof(triangles); i++)
    {
    	
	}
*/


	return 0;
}
