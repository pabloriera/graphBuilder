#include "graph.h"
//#include <strings>


graph_t::graph_t()
{
    id = 0;
    dist = 10;
    actual_node = NULL;
    selected_node = false;
    color0 = ofColor::fromHsb( 85, 10, 255 );
    colorSelected = ofColor::fromHsb( 85, 160, 180 );
}

void  graph_t::add_node(int x,int y)

{
    //cout << x << "\t" << y <<endl;
    old_node = actual_node;
    actual_node = new node_t;

    actual_node->pos.x = x;
    actual_node->pos.y = y;
    actual_node->id = id;
    id++;
    //actual_node.type = type;
    actual_node->rad = 8;
    actual_node->selected = true;

    nodes.push_back(actual_node);
    n_nodes++;

    if (selected_node) //prevents failure at start
    {
        //cout << nodes[old_node].pos.x << " " << nodes[old_node].pos.y  << endl;
        actual_node->to.push_back(old_node);
        old_node->selected=false;
    }
    else
        selected_node = true;

}

void graph_t::del_node(){

   // cout << "ix "<< node_ix << endl;

    if (actual_node!=NULL)
    {
        for(size_t i =0; i < nodes.size();i++)
            for(size_t j =0; j < nodes[i]->to.size();j++)
            {
                //cout << "i "<<i << endl;
                //cout << "j "<< j << endl;
                //cout << "itoj "<< nodes[i].to[j] << endl;
                if(nodes[i]->to[j] == actual_node)
                {
                    //cout << "del" << endl;
                    nodes[i]->to[j]=nodes[i]->to[nodes[i]->to.size()-1];
                    nodes[i]->to.pop_back();
                    break;
                }
            }

        for(size_t i = 0 ; i < nodes.size();i++)
            if (nodes[i]==actual_node)
                {
                    nodes[i]=nodes[nodes.size()-1];
                    nodes.pop_back();
                }

        delete actual_node;
        if (nodes.size()>0)
        {
                actual_node=nodes[nodes.size()-1];
                actual_node->selected=true;
        }

        else
        {
            actual_node=NULL;
            selected_node=false;
        }

    }
}

void  graph_t::clear_all()
{
    /*nodes.clear();
    selected_node = false;*/
}


int  graph_t::node_near(int x, int y)
{
    size_t sel;
    sel =  -1;
    for (size_t i = 0; i <  nodes.size(); i ++)
    {
        if (ofDist(x,y, nodes[i]->pos.x,nodes[i]->pos.y) < nodes[i]->rad*1.8)
        {
            sel = i;
            break;
        }

    }
    return sel;

}


void  graph_t::connect(int x,int y)
{
    bool link_deleted=false;
    int sel = node_near(x,y);

    if (sel!=-1){

        for(size_t j =0; j < actual_node->to.size();j++)
            if(actual_node->to[j]==nodes[sel])
            {
                actual_node->to[j]=actual_node->to[actual_node->to.size()-1];
                actual_node->to.pop_back();
                link_deleted = true;
                break;
            }

        if (!link_deleted)
            for(size_t j =0; j < nodes[sel]->to.size();j++)
                if(nodes[sel]->to[j] == actual_node)
                {
                    nodes[sel]->to[j]=nodes[sel]->to[nodes[sel]->to.size()-1];
                    nodes[sel]->to.pop_back();
                    link_deleted = true;
                    break;
                }

        if (!link_deleted)
        {
            old_node = actual_node;
            actual_node = nodes[sel];

            actual_node->to.push_back(old_node);
            actual_node->selected = true;
            old_node->selected = false;

        }
    }
}

void graph_t::mousePressed(int x,int y)
{
        if (selected_node) //prevents failure at start
        {
            actual_node->selected = false;
        }

        int sel = node_near(x,y);

        if (sel!=-1)
        {
            nodes[sel]->selected = true;
            actual_node = nodes[sel];
        }

        if (sel==-1)
            add_node(x,y);
}

void graph_t::mouseDragged(int x,int y)
{

    if (selected_node)
    {
        actual_node->pos.x = x;
        actual_node->pos.y = y;
    }

}

void  graph_t::draw()
{
    ofSetLineWidth(2.0);

    //cout << endl << "N_nodes: " << nodes.size() << ":";
    for(size_t i = 0; i < nodes.size();i++)
    {
        //cout << " " << nodes[i]->to.size() << ",";

        for(size_t j = 0; j < ((nodes[i])->to.size()); j++)
        {
            ofSetColor(color0);
            ofLine(nodes[i]->pos.x,nodes[i]->pos.y,  nodes[i]->to[j]->pos.x , nodes[i]->to[j]->pos.y );
        }

        if (nodes[i]->selected)
        {
            ofFill();
            ofSetColor(colorSelected);
            ofCircle(nodes[i]->pos.x,nodes[i]->pos.y,nodes[i]->rad*1.2);
        }
        else
        {
            ofNoFill();
            ofSetColor(color0);
            ofCircle(nodes[i]->pos.x,nodes[i]->pos.y,nodes[i]->rad);
        }


    }

}
void  graph_t::save(string fname)
{
/*
    outputFile.open(fname.c_str());

    for (size_t i = 0; i <  nodes.size(); i ++)
    {
        outputFile << i << " ";

        for(size_t j =0; j < nodes[i].to.size();j++)
        {
           outputFile << nodes[i].id << " ";
        }

        outputFile << endl;
    }

    outputFile.close();*/
}



