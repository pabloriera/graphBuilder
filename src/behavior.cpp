#include "behavior.h"
#include <iostream>
#include <string>
#include <fstream>


#define nodes (G->nodes)
#define links (G->links)

behavior_t::behavior_t()
{
    G = new graph_t;

    actual_node = NULL;
    selected_node = false;

    color0 = ofColor::fromHsb( 85, 10, 255 );
    colorNodeSelected = ofColor::fromHsb( 85, 160, 180 );
    colorLinkSelected = ofColor::fromHsb( 10, 160, 180 );
}

void  behavior_t::add_node(int x,int y)
{
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

    actual_link = new link_t;
    actual_link->selected = false;
    actual_link->from = old_node;
    actual_link->to   = actual_node;

    if (selected_node) //prevents failure at start
    {
        links.push_back(actual_link);
        old_node->selected=false;
    }
    else
        selected_node = true;

}

void behavior_t::del_node(){

    if (actual_node!=NULL && selected_node)
    {
        for(size_t i=0; i < links.size(); i++)
        {
            if(links[i]->from == actual_node ||links[i]->to == actual_node )
            {
                links[i] = links[links.size()-1];
                links.pop_back();
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

void  behavior_t::clear_all()
{
    nodes.clear();
    links.clear();
    id = 0;
    selected_node = false;
}


int  behavior_t::node_near(int x, int y)
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

int behavior_t::link_near(int x, int y)
{
    size_t sel =  -1;
    ofVec2f p(x,y);

    int hh = 10;
    int vv = 10;
    int dist = 50;

    for (size_t i = 0; i <  links.size(); i ++)
    {

        ofVec2f a = links[i]->from->pos;
        ofVec2f b = links[i]->to->pos;

        bool bh = false;
        bool bv = false;

        //point x,y is in the rectangle with the link as diagonal?
        if((a.x < b.x && x > a.x-hh && x < b.x+hh) || (a.x > b.x && x > b.x-hh && x < a.x+hh ))
                bh = true;

        if((a.y < b.y && y > a.y-vv && y < b.y+vv) || (a.y > b.y && y > b.y-vv && y < a.y+vv ))
                bv = true;

        if(bh && bv)
        {
            //point distance to line segment
            ofVec2f n = (links[i]->to->pos - links[i]->from->pos);
            n.normalize();
            ofVec2f aux = (a-p)-n.dot((a-p))*n;

            if (aux.lengthSquared() < dist)
                sel = i;
        }
    }

    return sel;
}

void behavior_t::connect(int sel)
{
    bool link_deleted=false;

    if (sel!=-1){

        for(size_t i=0; i < links.size(); i++)
        {
            if((links[i]->from == actual_node && links[i]->to==nodes[sel]) || (links[i]->to == actual_node && links[i]->from ==nodes[sel]))
            {
                links[i] = links[links.size()-1];
                links.pop_back();
                link_deleted = true;

                old_node = actual_node;
                actual_node = nodes[sel];
                actual_node->selected = true;
                old_node->selected = false;

                break;
            }
        }

        if (!link_deleted && nodes[sel]!=actual_node)
        {

            old_node = actual_node;
            actual_node = nodes[sel];

            actual_link = new link_t;
            actual_link->from = old_node;
            actual_link->to = actual_node;
            links.push_back(actual_link );

            actual_node->selected = true;
            old_node->selected = false;

        }
    }
}

int behavior_t::mousePressed(int x,int y,int button)
{
        int sel_node = node_near(x,y);
        int sel_link = link_near(x,y);

        if (button==0)
        {
            if (selected_node) //prevents failure at start
            {
                actual_node->selected = false;
                selected_node = false;
            }

            if (selected_link) //prevents failure at start
            {
                actual_link->selected = false;
                selected_link = false;
            }


            if (sel_node!=-1)
            {
                nodes[sel_node]->selected = true;
                actual_node = nodes[sel_node];
                selected_node = true;
            }

            else if (sel_link!=-1)
            {
                links[sel_link]->selected = true;
                actual_link = links[sel_link];
                selected_link= true;
            }

        }
        else if(button==2)
        {

            if (sel_node==-1)
                add_node(x,y);
            else
                connect(sel_node);
        }

        return sel_node;
}

void behavior_t::mouseDragged(int x,int y)
{

    if (selected_node)
    {
        actual_node->pos.x = x;
        actual_node->pos.y = y;
    }

}

void behavior_t::draw()
{
    ofSetLineWidth(2.0);

    for(size_t j = 0; j < links.size(); j++)
    {
        if(links[j]->selected)
            ofSetColor(colorLinkSelected);
        else
            ofSetColor(color0);

        ofLine( links[j]->from->pos.x, links[j]->from->pos.y,  links[j]->to->pos.x , links[j]->to->pos.y );
    }

    for(size_t i = 0; i < nodes.size();i++)
    {
        if (nodes[i]->selected)
        {
            ofFill();
            ofSetColor(colorNodeSelected);
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

void behavior_t::save(string xml_file_name)
{
    igraph_i_set_attribute_table(&igraph_cattribute_table);


    igraph_t g;


    igraph_vector_t v;

    igraph_vector_init(&v, links.size()*2);
    size_t c = 0;

    for(size_t i = 0; i<links.size();i++)
    {
        VECTOR(v)[c]=links[i]->from->id;
        VECTOR(v)[c+1]=links[i]->to->id;
        c+=2;
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

void behavior_t::load(string xml_file_name)
{

    igraph_i_set_attribute_table(&igraph_cattribute_table);

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
            //actual_node->to.clear();
            actual_node->pos.x = VAN(&g,"x",i);
            actual_node->pos.y = VAN(&g,"y",i);
        }

        links.clear();

        igraph_vector_init(&v,0);
        igraph_get_edgelist(&g,&v,false);

        for(int i=0; i<igraph_vector_size(&v); i+=2)
        {
            int source = VECTOR(v)[i];
            int target = VECTOR(v)[i+1];

            actual_link = new link_t;
            actual_link->selected = false;
            actual_link->from = nodes[source];
            actual_link->to = nodes[target];
            links.push_back(actual_link );
        }

        igraph_destroy(&g);
        igraph_vector_destroy(&v);
    }
}



