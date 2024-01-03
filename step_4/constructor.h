//
// constructor.h
//
#ifndef constructor_h
#define constructor_h
//
#define _USE_MATH_DEFINES
//
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>     // std::setprecision
#include <memory>
#include <vector>
#include <string>
#include <sstream>
//
//#include <algorithm>   // std::count
//#include <cmath>		// for _USE_MATH_DEFINES
//
//
//
#include "vector2D.h"
#include "KDTree.hpp"
//
struct Trajectory_record {
	int frame, id;
	double x, y, v_x, v_y, heading;
};
//
class Constructor{
    private:
        //
        double r_thershold;
        int fps;
        int delta_t;
        std::vector<std::vector<double>> node_active;

    public:
        //
        Constructor();
        ~Constructor();
        //void run(double, int, std::ifstream&, std::stringstream&);
        void run(double, int, std::ifstream&, std::string&);
        void split(const std::string&, char, std::vector<std::string>&);
        void check_neighbors(int, std::ofstream&, std::ofstream&, std::ofstream&);
        void update_nodes(int, std::ofstream&);
        void update_edges(int, std::ofstream&);
        //
        inline double get_contact_radius() const    { return r_thershold;}
        inline int get_fps() const                  { return fps;}
        inline int get_delta_a() const              { return delta_t;}

};
//
#endif
