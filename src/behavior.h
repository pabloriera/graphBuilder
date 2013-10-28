#pragma once

#include "ofMain.h"
#include "graph.h"
#include "igraph/igraph.h"

using namespace std;

class behavior_t
{

public:

    behavior_t();

    void clear_all();
    void add_node(int x, int y);
    void del_node();
    void connect(int sel);
    void change_type();

    void draw();

    void save(string fname);
    void load(string fname);
    void print_graph();

    int mousePressed(int x,int y, int button);
    void mouseDragged(int x,int y);

    int node_near(int x, int y);
    int link_near(int x, int y);

    node_t *actual_node;
    node_t *old_node;

    link_t *actual_link;
    //link_t *actual_link;

    graph_t *G;

    bool selected_node;
    bool selected_link;

    ofColor color0, colorNodeSelected, colorLinkSelected;

    int id;

};

