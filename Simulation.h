#include <vector>
#include "Eigen/Dense"
#include "Delaunay.hpp"
#include "Springs.cpp"
#include "Nodes.cpp"
using namespace Eigen;


struct InitialDataValues
{
    int     N;   // number of Nodes
    double mass_of_nodes; //mass of nodes, only valid if  the masses  are all identical
    
    double scaling_factor; //this is the scaling factor for the time of the input signal treble sinuosoidal wave (but can be anything else).
     
    double ux;   // First input values in x direction  TODO: Really needed?
    double uy;   // First input values in y direction

    double input_connectivity;  // [0,1] percentage of nodes that receive input
    // lower and upper range for input weights
    double min_input_weight; //The lowest value for the input weight
    double max_input_weight; //The highest value for the input weight

 
    double min_x_position;   // minimum value for the x position
    double max_x_position;  //maximum value for the x position
    double min_y_position;  //minimum value for the y position
    double max_y_position;  //maximum value for the y position



    double t0;      // time for first time step [s]
    double tmax;    // maximum time step [s]
    double dt;      // time step in seconds

    //The min and max values for k1, k3, d1, d3 values, either log uniform or uniform depending.
    double min_k3;
    double max_k3;
    double min_d3;
    double max_d3;

    double min_k1;
    double max_k1;
    double min_d1;
    double max_d1;
};



class Simulation
{

    private:
        int N;                                  // Number of mass points
        double input_connectivity;       //The proportion of nodes that are connected
        int num_input_nodes;    // Number of input nodes
        vector<Nodes> n;        // List of all nodes
        vector<Springs> s;      // List of all springs
        
        //Not sure I want to change the structure of the code at this point, esp down the line for checking.
        vector< vector<double> > EdgeList;   // Todo: Is that really part of Simulation class - yes
        vector<double> NodeList;             // Todo: Is that really part of Simulation class - yes
        vector<double> EdgeNodeList;         // Todo: Is that really part of Simulation class - yes
        

        //scaling factor for Treble Sinusoidal function
        double scaling_factor;

        //max and min values for the input weight w_in
		double input_weight_smallest_value;
	    double input_weight_largest_value;

       //starting value for t
        double t0;
        //maximum value for t
        double tmax;
        //dt, the timestep
        double dt;

        //These time variables are for the washout, learning phase and test data for the weights calcualated in learning phase.
        int wash_out_time;
        int learning_time;
        int learning_time_test;

        //the total time including washout time, learning time and testing time.
        int maxtimesteps;


        //add in variable for bias learning
        bool bias_learning = false;
        
        //stability variable
        bool stability = true;

        //Ranges for the delaunay triangulation
        double smallest_x_position;    
	    double largest_x_position;
	    double smallest_y_position;
	    double largest_y_position;

        //These are constant horizontal forces on the input nodes. If this changes so each input node receives a unique force we will have to modify the code
        // Todo: Better to call them Fx and Fy
        //The code at the moment does not use these.
        double ux;
        double uy;

        //min and max k1 and k3 value
        double min_k3;
        double max_k3;
        double min_d3;
        double max_d3;
        
        
        //min and max k1 and d1 values
        double min_k1;
        double max_k1;
        double min_d1;
        double max_d1;

        //This is for the chaos test;
        double k = 1.000;
        string str ="chaoscheck.csv";
        string str2 = "LM.csv";


        //This is the final function which SHOULD be like the target signal.
        vector<double> Output_Signal;

        //This is the learning_weights vector
        vector<double> Learning_Weights;

        //This is the target signal for learning
        vector<double> Target_Signal;

        //Input signal for system to be simulated.
        vector<double> Input_Signal;

        //LearningMatrix for learning weight multiplication
        // For collecting data for learning
        //LM is the matrix in total. Lm2 is the matrix for learning and lm3 is the final matrix.
        MatrixXd LM;
        MatrixXd LM2;
        MatrixXd LM3;
        
        //Target signals
        VectorXd TS;
        VectorXd TS2;
        VectorXd TS3; 

        //Output MatrixXd
        VectorXd Output;

        //For testing phase.
        VectorXd Test_Target;

        //Mean squared Error
        double Mean_Squared_Error;

        //Record of number of threads/webs independant of s for bug fixing.
        int number_of_threads_or_webs = 0;


    public:

        //Default constructor
        Simulation(InitialDataValues &data, vector<double> &Input_Signal, vector<double> &Target_Signal, int wash_out_time, int learning_time, int learning_time_test);

        //This is an overloaded default constructor. This is not randomly initialized mass spring system, this is a determined one.
        // Todo: Maybe derive a class for spiderweb simulation
        //That's probably a good idea.
        Simulation(double radius, int rounds, int no_of_points_per_round, InitialDataValues &data, vector<double> &Input_Signal, vector<double> &Target_Signal);

        //radius rounds etc.
        Simulation(double radius, int rounds, int no_of_points_per_round, InitialDataValues &data, vector<double> &IS, vector<double> &TS, int wash_out_time, int learning_time, int learning_time_test);

        //This creates the nodes for the reservoir computer implementation
        // Todo: Maybe derive a class for spiderweb simulation
        void Initialize_Nodes(double radius, int rounds, int no_of_points_per_round, InitialDataValues &data);

        //This initializes the nodes and puts in appropriate values for the ranges and the weights
        void Initialize_Nodes(double smallest_x_position, double largest_x_position, double smallest_y_position, double largest_y_position);

        //This is for the csv files to test if there is chaos.
        void input_Magnitude_of_Chaos_Force(double k, const std::string& input, const std::string& input2);

        //This resets the positions of the springs and the nodes
        void Reset_Simulation();
        
        // Todo: Name is not ideal. Better would be to call it update() or similar
        void update(bool bias_learning);

        //this is where the Moore Penrose pseudoinverse is done to get learning weights.
        void Moore_Penrose_Pseudoinverse_and_Learning_Weights();

        //Range in Range, required.
        double Rand_In_Range_Exp(double min, double max);
        
        //This is for the treble sinusoidal function.
        double Treble_Sine_Function(double f1, double f2, double f3, double dt, double t, double T);

        //This does the delaunay triangulation for the two dimensional case and creates the springs for the reservoir computer, not the radial spider web
        void Delaunay_Triangulation_and_Spring_Creation();

        //Create EdgeNodeList, defunct function don't want to get rid of it.
        void Create_EdgeNodeList();

        //After delaunay triangulation, how many connecting edges hence springs.
        // Todo: Should the ouput be really double and not unsigned int?
        unsigned int Output_No_of_Edges();

        //This outputs the spring and node positions for the reservoir computer
        void Output_Spring_And_Node_Positions();

        //Get the triangles from the Delaunay Triangulation.
        void Get_Triangles(DelaunayTriangulation &Delaunay);

        //Create springs for reservoir computer nonlinear mass spring system
        void Initialize_Springs(InitialDataValues &data);

        //Does the Moore-Penrose pseudoinverse from Eigen library
        void Moore_Penrose_Pseudoinverse(MatrixXd& L);

        //Return the mean squared error generated by the simulation
        double return_MSE();

        //Output for Matlab plot
        void Output_For_Plot();

        //Mean Squared Error between vector A and estimator Ahat
        double MSE(vector<double>& A, vector<double>& Ahat);

        //Return learning matrix, MatrixXd defined in Eigen library
        MatrixXd& Return_Learning_Matrix();

        //Return Output Signal after learning phase and the mean squared error.
        double output_LearningMatrix_and_MeanSquaredError();

        //Return output signal on its own, for MSE test.
        void output_Output_Signal(string& s);

        //Return the weights after learning phase as a vector
        vector<double>& Return_Learning_Weights();

        //Populate the learning matrix with weights.
        void Populate_Learning_Weights(VectorXd& L);

        //Have N random input nodes.
        int Random_Input_Nodes(int N);

        //Randomly chosen number between N and M
        double Uniform(double M, double N);

        //Randomy chosen log to the base 10 uniform number between initial and final values.
        double Log_10_Uniform(double initial, double finalvalue);

        //Expression of log 10 uniform for spring and damping coefficients
        double Spring_And_Damping_Coefficient_1(double initial, double finalvalue);
        double Spring_And_Damping_Coefficient_2(double initial, double finalvalue);

        //Euclidean distance between two points on x y plane, will overload for 3D
        double Eucl_Dist(double x1, double y1, double x2, double y2);

        //Angle for line between two points.
        double Angle(double x0, double x1, double y0, double y1);

        //X component of vector from one point to another
        double X_Comp(double vectorsum, double theta);

        //Y component of vector from one point to another.
        double Y_Comp(double vectorsum, double theta);

        //Sorting three input numbers.
        void Sort(int &a, int &b, int &c);

        //Sorting two input numbers
        void Sort(int &a, int &b);


        //Remove duplicates from two dimensional vector.
        void Remove_Duplicates(vector<vector<double>> &x);

        //Return number of springs
        // Todo: Does it return number of springs (int) or an object of class spring?
        // This returns the objects directly.
        Springs Spring_Return(int i);

        //Return number of nodes
        //Todp: Same question here
        //This returns the nodes directly.
        Nodes Node_Return(int i);

        //Return number of edges from the triangle.    
        unsigned int Spring_List();
        
        //Return stability
        bool Stability_return();
};

class Spider_Web_Simulation: public Simulation
{
	private:
		double radius;
		int rounds; 
		int no_of_points_per_round;
				
		public:
		Spider_Web_Simulation(double radius, int rounds, int no_of_points_per_round);
		
		        void Initialize_Nodes();		
};
