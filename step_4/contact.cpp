#include <cstdio>
#include <algorithm>
//
#include "contact.h"
//
Contact::Contact(int id_ego){
    ego_id = id_ego;
}

Contact::~Contact(){
}

void Contact::check_neighbor_set(int neighbor_id, int frame_now){
    //std::cout << frame_now <<"\t"<< ego_id <<"\t"<< neighbor_id <<"\n";
    //
    // check the existence of neighbor_id in neighbor_set
    //
    std::vector<int>::iterator it = std::find(neighbor_set.begin(), neighbor_set.end(), neighbor_id);
    //if(std::find(neighbor_set.begin(), neighbor_set.end(), neighbor_id) != neighbor_set.end()){
    if(it != neighbor_set.end()){
        // neighbor_id exits
        //
        // get index of element from neighbor_set using iterator
        //
        int contact_index = std::distance(neighbor_set.begin(), it);
        std::cout << frame_now <<"\t"<< ego_id <<"\t"<< neighbor_id <<"\t"<< contact_index <<"\n";
        /*
        int frame_last = records[contact_index].get_frame_end();
        //std::cout << frame_now <<"\t"<< ego_id <<"\t"<< neighbor_id <<"\t"<< frame_last <<"\n";
        //
        int frame_tol = 1; // imputation or filling missing gap/frames
        if ((frame_now-frame_last) <= frame_tol){
            // update frame_last
            records[contact_index].update_frame_end(frame_now);
            int frame_start = records[contact_index].get_frame_start();
            int n_repeats = records[contact_index].get_repeats();
            std::cout << frame_now <<"\t"<< ego_id <<"\t"<< neighbor_id <<"\t"<< n_repeats <<"\t"<< frame_start <<"\t"<< frame_now <<"\n";
        } else{
            int frame_start = records[contact_index].get_frame_start();
            int frame_end = records[contact_index].get_frame_end();
            int count_repeats = records[contact_index].get_repeats();
            //std::cout << frame_now <<"\t"<< ego_id <<"\t"<< neighbor_id <<"\t"<< count_repeats <<"\t"<< frame_start <<"\t"<< frame_end <<"\n";
            // new record
            int n_repeats = records[contact_index].get_repeats()+1;
            Record record_i = Record(ego_id, neighbor_id, n_repeats, frame_now, frame_now);
            //std::cout << frame_now <<"\t"<< ego_id <<"\t"<< neighbor_id <<"\t0\t"<< frame_now <<"\t"<< frame_now <<"\n";
        }
        */
        //
    } else{
        // neighbor_id does not exits
        neighbor_set.push_back(neighbor_id);
        Record record_i = Record(ego_id, neighbor_id, 0, frame_now, frame_now);
        //std::cout << frame_now <<"\t"<< ego_id <<"\t"<< neighbor_id <<"\t0\t"<< frame_now <<"\t"<< frame_now <<"\n";
        records.push_back(record_i);
    }
}

void Contact::update_contact(){
    // update edge information
}

/*
int Contact::check_last_frame(int index_temp){
    int frame_last = records[index_temp].get_frame_end();
    return frame_last;
}
*/

std::vector <std::vector <int>> Contact::records_out(){
    std::vector <std::vector <int>> output_records;
    //
    for (unsigned int i = 0; i < records.size(); i++){
        std::vector <int> output_temp;
        //
        int column_1 = records[i].get_ego_id();
        int column_2 = records[i].get_neighbor_id();
        int column_3 = records[i].get_repeats();
        int column_4 = records[i].get_frame_start();
        int column_5 = records[i].get_frame_end();
        //
        output_temp.push_back(column_1);
        output_temp.push_back(column_2);
        output_temp.push_back(column_3);
        output_temp.push_back(column_4);
        output_temp.push_back(column_5);
        output_records.push_back(output_temp);
        output_temp.clear();
    }
    //
    return output_records;
}
