#include "graph.h"
#include <iostream>
#include <string>
#include <fstream>
#include "igraph/igraph.h"


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
    nodes.clear();
    n_nodes = 0;
    id = 0;
    selected_node = false;
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
void  graph_t::save(string xml_file_name)
{
    igraph_i_set_attribute_table(&igraph_cattribute_table);


    igraph_t g;


    igraph_vector_t v;

    size_t edge_n = 0;

    for(size_t i = 0; i<nodes.size();i++)
        edge_n += nodes[i]->to.size();

    igraph_vector_init(&v, edge_n*2);
    size_t c = 0;

    for(size_t i = 0; i<nodes.size();i++)
    {
        for(size_t j = 0; j<nodes[i]->to.size();j++)
            {
                cout << nodes[i]->to[j]->id << endl;
                VECTOR(v)[c]=i;VECTOR(v)[c+1]=nodes[i]->to[j]->id;
                c+=2;
            }
    }
    igraph_create(&g, &v, 0, 1);

    for(size_t i = 0; i<nodes.size();i++)
    {
        SETVAN(&g, "x", i, nodes[i]->pos.x);
        SETVAN(&g, "y", i, nodes[i]->pos.y);
    }



    FILE *ifile;
    ifile=fopen(xml_file_name.c_str(), "w");
    igraph_write_graph_graphml(&g,ifile);
    igraph_destroy(&g);
    igraph_vector_destroy(&v);
    fclose(ifile);

}

void  graph_t::load(string xml_file_name)
{

    igraph_i_set_attribute_table(&igraph_cattribute_table);

 /*   igraph_vector_t gtypes, vtypes, etypes;
    igraph_strvector_t gnames, vnames, enames;

    igraph_vector_init(&gtypes, 0);
    igraph_vector_init(&vtypes, 0);
    igraph_vector_init(&etypes, 0);
    igraph_strvector_init(&gnames, 0);
    igraph_strvector_init(&vnames, 0);
    igraph_strvector_init(&enames, 0);

    //igraph_cattribute_list(&g, &gnames, &gtypes, &vnames, &vtypes, &enames, &etypes);
*/

    FILE *ifile;
    ifile=fopen(xml_file_name.c_str(), "r");

    if (ifile==0)
    {
        cout << "File " << xml_file_name << " missing"<< endl;
        //fclose(ifile);
    }
    else
    {
        clear_all();

        igraph_vector_t v;
        igraph_t g;

        igraph_read_graph_graphml(&g,ifile,0);
        fclose(ifile);

        for(int i=0; i<igraph_vcount(&g); i++)
        {

            add_node(0.0,0.0);
            actual_node->id = i;
            actual_node->to.clear();
            actual_node->pos.x = VAN(&g,"x",i);
            actual_node->pos.y = VAN(&g,"y",i);
        }

        igraph_vector_init(&v,0);
        igraph_get_edgelist(&g,&v,false);

       // cout << igraph_vector_size(&v) << endl;
        //cout << igraph_ecount(&g) << endl;

        for(int i=0; i<igraph_vector_size(&v); i+=2)

        {

            //cout << VECTOR(v)[i]<< " " << VECTOR(v)[i+1] << endl;

            int sour = VECTOR(v)[i];
            int targ = VECTOR(v)[i+1];

            //cout << sour << " " << targ << endl;

          /*  for(size_t j = 0; j < nodes.size(); j++)
            {
                if (nodes[j]->id == ofToInt(source))
                        sour = j;
                if (nodes[j]->id == ofToInt(target))
                        targ = j;
            }*/

            nodes[sour]->to.push_back(nodes[targ]);
        }

        igraph_destroy(&g);
        igraph_vector_destroy(&v);
    }
}


