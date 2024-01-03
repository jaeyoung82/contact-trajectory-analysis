//
// edge.h
//
//#define _USE_MATH_DEFINES	// should appear before #include <cmath>
#define M_PI            3.14159265358979323846  /* pi */
//
// rounding number to d decimal places
// see: https://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
#define roundz(x, d)    ((std::floor(((x)*std::pow(10.0, d))+0.5))/std::pow(10.0, d))
#define M_epsilon       0.0001
#define get_min(a,b)    ((a>b)? b:a)
#define get_max(a,b)    ((a<b)? b:a)
//
#ifndef contact_h
#define contact_h
//
#include "vector2D.h"
#include <vector>
//
class Record{
    private:
        //

    public:
        //
        int ego_id;
        int neighbor_id;
        int n_repeat;
        int frame_start;
        int frame_end;
        //
        inline Record(int id_ego, int id_neighbor, int count_repeat, int fr_start, int fr_end){
             ego_id = id_ego; neighbor_id = id_neighbor; n_repeat = count_repeat; frame_start = fr_start; frame_end = fr_end;
        };
        //
        inline get_ego_id() const { return ego_id;}
        inline get_neighbor_id() const { return neighbor_id;}
        inline get_repeats() const { return n_repeat;}
        inline get_frame_start() const { return frame_start;}
        inline get_frame_end() const { return frame_end;}
        //
        inline update_frame_end(int fr_end) { frame_end = fr_end;}
};
//
class Contact{
    private:
        int ego_id;
        int neighbor_id;
        int frame_start;
        int frame_end;
        //
        std::vector <int> neighbor_set;
        std::vector <Record> records;
        //int frame_cout;

    public:
        Contact(int);
        ~Contact();
        //
        void check_neighbor_set(int, int);
        void update_contact();
        //int check_last_frame(int);
        std::vector <std::vector <int>> records_out();
};
//
#endif
