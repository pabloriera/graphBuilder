#pragma once

#include "ofMain.h"

using namespace std;


class node_t
{
    public:

        int id;
        vector <node_t*> to;

        //basic atributes
        ofVec2f pos;
        bool selected;
        double rad;

        vector <string> att_str;
        vector <float> att_float;
};

class link_t
{
    public:

        int id;

        node_t *from,*to;

        //atributes
        bool selected;
        size_t type;
};


class graph_t
{

    public:

        graph_t(){};

        vector <node_t*> nodes;
        vector <link_t*> links;
        vector <string> att_names;

        int n_nodes;
        int n_links;
};
