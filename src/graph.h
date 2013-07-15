#pragma once

#include "ofMain.h"
#include <vector>

using namespace std;

// type for idxex of rivers or nodes or small? things
typedef unsigned int idx_int;
typedef vector<idx_int> vec_idx;
typedef vector<double> vec_double;
typedef vector<bool> vec_bool;

class node_t
{
public:
    //node_t() {to.clear();};
    ofVec2f pos;
	bool selected;
    idx_int type;
    double rad;
    int id;
    vector <node_t*> to;
};

// nodos
class graph_t
{

public:

    graph_t();

    void clear_all();
    void add_node(int x, int y);
    void del_node();
    void connect(int x, int y);

    void draw();

    void save(string fname);

    void mousePressed(int x,int y);
    void mouseDragged(int x,int y);


    int node_near(int x, int y);

    vector <node_t*> nodes;
    int n_nodes;

    node_t *actual_node;
    node_t *old_node;

    bool selected_node;

    ofstream outputFile;

    ofColor color0, colorSelected;

    float dist;
    int id;



};
//]
