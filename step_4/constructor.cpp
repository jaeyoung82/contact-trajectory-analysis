#include "constructor.h"
//
Constructor::Constructor(){
    //
}

Constructor::~Constructor(){
    //
}

void Constructor::run(double contact_radius, int frame_per_second, std::ifstream& infile, std::string& scenario_name){
     //
     r_thershold = contact_radius;   // contact radius
     fps = frame_per_second;         // frame per second, 24 for NYC terminal (see: Zhong et al. AAMAS 2016)
     double epsilon = 0.001;
     //
     // open a trajectory file (frame/ id/ x/ y/ v_x/ v_y/ heading)
     //
     std::string line;
     //
     // prepare output files
     //
     std::stringstream filename_0;
     //filename_0 << "frame_R"<< (int)((r_thershold+epsilon)*10) << ".txt";
     filename_0 << scenario_name  <<".txt";
     //
     std::stringstream filename_neighbors;
     filename_neighbors << "neighbors_" << fps << "fps_"<< filename_0.str();
     std::ofstream write_neighbors(filename_neighbors.str().c_str(), std::ios_base::out | std::ofstream::app);
     //
     //std::stringstream filename_nodes;
     //filename_nodes << "nodes_"<< filename_0.str();
     //std::ofstream write_nodes(filename_nodes.str().c_str(), std::ios_base::out | std::ofstream::app);
     //
     std::stringstream filename_edgelist;
     filename_edgelist << "edgelist_" << fps << "fps_"<< filename_0.str();
     std::ofstream write_edgelist(filename_edgelist.str().c_str(), std::ios_base::out | std::ofstream::app);
     //
     std::stringstream filename_contact;
     filename_contact << "contact_" << fps << "fps_"<< filename_0.str();
     std::ofstream write_contact(filename_contact.str().c_str(), std::ios_base::out | std::ofstream::app);
     //
     std::stringstream filename_npeds;
     filename_npeds << "npeds_" << fps << "fps_"<< filename_0.str();
     std::ofstream write_npeds(filename_npeds.str().c_str(), std::ios_base::out | std::ofstream::app);
     //
     int frame_temp = 1;
     int frame_now = 1;
     //
     node_active = {};
     //node_active.clear();
     //
     if(infile){
          while(infile.good()){
          //
          // read the trajectory file line by line
          // trajectory file format: frame/ id/ x/ y/ v_x/ v_y/ heading
          //
          std::vector<std::string> row_values;
          split(line, '\t', row_values);
          Trajectory_record line_i;
          infile >> line_i.frame >> line_i.id >> line_i.x >> line_i.y >> line_i.v_x >> line_i.v_y >> line_i.heading;
          //
          frame_now = line_i.frame;
          std::vector<double> node_i = {line_i.id, line_i.x, line_i.y, line_i.v_x, line_i.v_y, line_i.heading};
          //
          if (frame_now == frame_temp){
               // update the sets of id and points
               node_active.push_back(node_i);
               //std::cout << frame_now <<"\t"<< node_i->get_id() <<"\n";
               //std::cout << frame_now <<"\t"<< node_i[0] <<"\t"<< node_i[1]<<"\t"<< node_i[2]<<"\n";
          }
          //
          // new frame
          //
          if (frame_now > frame_temp){
               //std::cout << "move to the next timestamp\tframe_" << frame_temp <<"\tn_nodes = "<< node_active.size() <<"\n";
               //
               // record the number of pedestrians
               //
               int n_peds = node_active.size();
               if (n_peds > 0) write_npeds << frame_temp <<"\t"<< n_peds <<"\n";
               //
               // get proximity network
               //
               check_neighbors(frame_temp, write_neighbors, write_edgelist, write_contact);
               //
               // clear the sets of id and points
               //
               node_active.clear();
               //
               // update the sets of id and points
               //
               node_active.push_back(node_i);
               //
               // move to the next frame
               //
               frame_temp = frame_now;
          }
          //
          // end of file
          //
          if (infile.eof()){
               //
               // get proximity network
               //
               check_neighbors(frame_temp, write_neighbors, write_edgelist, write_contact);
               //
               //std::cout << "move to the next timestamp\tframe_" << frame_temp <<"\tn_nodes = "<< node_active.size() <<"\n";
               //std::cout <<"end of file\n";
               }
          }
     }
     //
     write_neighbors.close();
     write_edgelist.close();
     write_npeds.close();
     //node_active.clear();
}

void Constructor::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    //
    // See
    // http://stackoverflow.com/a/236803/248823
    // https://stackoverflow.com/questions/40337177/c-how-to-use-fstream-to-read-tab-delimited-file-with-spaces
    //
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

void Constructor::check_neighbors(int frame_now, std::ofstream& write_neighbors, std::ofstream& write_edgelist, std::ofstream& write_contact){
     int frame_i = frame_now;
     int n_nodes = node_active.size();
     //
     // generate tree structure
     //
     pointVec points;
     for (int i = 0; i < n_nodes; i++) {
          double x_i = node_active[i][1];
          double y_i = node_active[i][2];
          point_t pt = {x_i, y_i};
          points.push_back(pt);
     }
     KDTree tree(points);
     //
     for (int i = 0; i < n_nodes; i++) { // for each individual
          //
          int id_i = node_active[i][0];
          double x_i = node_active[i][1];
          double y_i = node_active[i][2];
          double vx_i = node_active[i][3];
          double vy_i = node_active[i][4];
          double v_i = std::sqrt(vx_i*vx_i+vy_i*vy_i);
          double heading_i = node_active[i][5];
          point_t pt_query = {x_i, y_i};
          //
          auto neighbors_2m = tree.neighborhood_indices(pt_query, r_thershold);       // neighbors within contact radius of 2m
          auto neighbors_4m = tree.neighborhood_indices(pt_query, r_thershold+2.0);   // neighbors within radius of 4m
          //
          int n_neighgbors = neighbors_2m.size(); // here, individual i is also counted
          int degree_i = n_neighgbors-1; // the number of neighbors that individual i has
          //
          // polarization -- local order parameter
          // see Lu et al. PRR 2020, Vicsek et al. PRL 1995, and others
          //
          double polarization_num = 0.0;
          int polarization_den = 0;
          double polarization = 0.0;
          //
          // collect information of neighbors within 2 m
          //
          write_neighbors << frame_i <<"\t"<< id_i <<"\t"<< degree_i <<"\t[";
          for (unsigned int j = 0; j < neighbors_2m.size(); j++) {
               int j_index = neighbors_2m[j];          // index of neighbor node
               int id_j = node_active[j_index][0];     // id of neighbor node
               double x_j = node_active[j_index][1];
               double y_j = node_active[j_index][2];
               double vx_j = node_active[j_index][3];
               double vy_j = node_active[j_index][4];
               double v_j = std::sqrt(vx_j*vx_j+vy_j*vy_j);
               double heading_j = node_active[j_index][5];
               //
               if (id_i != id_j){
                    write_neighbors << id_j << ",";
                    write_edgelist << frame_i <<"\t"<< id_i << "\t" << id_j <<"\n";
                    // update polarization
                    if ((v_i*v_j) != 0){
                         polarization_num += ((vx_i*vx_j+vy_i*vy_j)/(v_i*v_j));
                         polarization_den += 1;
                    }
                    //
                    // check distance and relative speed
                    //
                    double delta_x = x_j-x_i;
                    double delta_y = y_j-y_i;
                    double d_ij = std::sqrt(delta_x*delta_x+delta_y*delta_y);
                    //
                    double delta_vx = vx_j-vx_i;
                    double delta_vy = vy_j-vy_i;
                    double v_ij = std::sqrt(delta_vx*delta_vx+delta_vy*delta_vy);
                    //
                    double angle_diff = std::abs(heading_i-heading_j);
                    angle_diff = std::min(angle_diff, 2*M_PI-angle_diff);
                    //
                    // write contact recrods
                    //
                    write_contact << frame_i <<"\t"<< id_i <<"\t"<< id_j <<"\t"<< id_i <<"_"<< id_j <<"\t"<< degree_i <<"\t";
                    write_contact << std::setprecision(3) << x_i <<"\t"<< y_i <<"\t"<< delta_x <<"\t"<< delta_y <<"\t"<< d_ij <<"\t";
                    write_contact << heading_i <<"\t"<< heading_j <<"\t"<< angle_diff <<"\t";
                    write_contact << v_i <<"\t"<< v_j  <<"\t"<< v_ij <<"\t";
                    write_contact << polarization <<"\n";
               }
          }
          write_neighbors << "]\n";
          if (polarization_den > 0) polarization = polarization_num/polarization_den;
          //
          // collect additional information of neighbors within 4m
          //
          /*
          for (unsigned int j = 0; j < neighbors_4m.size(); j++) {
               // retrieve information of individual j
               int j_index = neighbors_4m[j];          // index of neighbor node
               int id_j = node_active[j_index][0];     // id of neighbor node
               double x_j = node_active[j_index][1];
               double y_j = node_active[j_index][2];
               double vx_j = node_active[j_index][3];
               double vy_j = node_active[j_index][4];
               double v_j = std::sqrt(vx_j*vx_j+vy_j*vy_j);
               double heading_j = node_active[j_index][5];
               //
               if (id_i != id_j){
                    //
                    // check distance and relative speed
                    //
                    double delta_x = x_j-x_i;
                    double delta_y = y_j-y_i;
                    double d_ij = std::sqrt(delta_x*delta_x+delta_y*delta_y);
                    //
                    double delta_vx = vx_j-vx_i;
                    double delta_vy = vy_j-vy_i;
                    double v_ij = std::sqrt(delta_vx*delta_vx+delta_vy*delta_vy);
                    //
                    double angle_diff = std::abs(heading_i-heading_j);
                    angle_diff = std::min(angle_diff, 2*M_PI-angle_diff);
                    //
                    // write contact recrods
                    //
                    write_contact << frame_i <<"\t"<< id_i <<"\t"<< id_j <<"\t"<< id_i <<"_"<< id_j <<"\t"<< degree_i <<"\t";
                    write_contact << std::setprecision(3) << x_i <<"\t"<< y_i <<"\t"<< delta_x <<"\t"<< delta_y <<"\t"<< d_ij <<"\t";
                    write_contact << heading_i <<"\t"<< heading_j <<"\t"<< angle_diff <<"\t";
                    write_contact << v_i <<"\t"<< v_j  <<"\t"<< v_ij <<"\t";
                    write_contact << polarization <<"\n";
               }
          }
          */
     }
}
