#include <algorithm>    // std::count
#include <cmath>		// for _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <iomanip>      // std::setprecision
#include <sstream>
#include <string>
#include <vector>

//#include "vector2D.h"
//#include "KDTree.hpp"
//#include "contact.h"
#include "constructor.h"

int main(int argc, char *argv[]){
     std::stringstream filename_log;
     filename_log << "log.txt";
     std::ofstream write_log(filename_log.str().c_str(), std::ios_base::out | std::ofstream::app);
     //
     int n_argc = 2; // the number of required arguments
     if (argc != (n_argc+1)){
          // print out a simple guide
          std::cerr << "Usage:\n" << argv[1] <<"\t"<< "argument_1" <<"\t"<< "argument_2\n\n";
          std::cout << "Required arguments:\n";
          std::cout << "argument_1: scenario_name\n";
          std::cout << "argument_2: frame per second (fps) value\n";
          return 1;
     } else {
          // std::stringstream scenario_name;
          // infile_name << "tnxyv_" << scenario_name.str() <<".txt";
          std::string scenario_name(argv[1]);
          std::stringstream infile_name;
          infile_name << "tnxyv_" << scenario_name <<".txt";
          std::ifstream infile (infile_name.str());
          std::vector <double> r_threshold = {2.0}; // contact radius range, m
          //int fps = 25; // MAKE SURE THIS IS CORRECT!
          int fps = atoi(argv[2]);
          //
          for (unsigned int j = 0; j < r_threshold.size(); j++){
               double r_contact = r_threshold[j];
               std::cout <<"r_threshold = "<< r_contact << " m\n";
               //
               time_t begin, end;	//
               time(&begin);		// tic
               //
               Constructor contact_process;
               contact_process.run(r_contact, fps, infile, scenario_name);
               //
               time(&end);			// toc
               //write_log << std::fixed << std::setprecision(2) << r_contact <<"\t"<< difftime(end, begin) <<"\n";
               // write_log << scenario_name <<"\t"<< std::fixed << std::setprecision(2) << r_contact <<"\t"<< difftime(end, begin) <<"\n";
               write_log << scenario_name <<"\t"<< std::fixed << std::setprecision(2) << difftime(end, begin) <<"\n";
          }
          //
          write_log.close();
          //
          return 0;
     }
}
