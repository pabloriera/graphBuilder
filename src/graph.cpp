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
     XML.setVerbose(true);
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


    XML.clear();

    XML.addTag("graphml");
    XML.pushTag("graphml");
    XML.addTag("graph");
    XML.pushTag("graph");

    for(size_t i = 0;i < nodes.size();i++)
    {
        XML.addTag("node");
        XML.pushTag("node");
        XML.addValue("data",nodes[i]->pos.x);
        XML.addValue("data",nodes[i]->pos.y);
        XML.addAttribute("node","id",ofToString(i+1),i);


//


        //XML.addTag("data");

        //XML.setAttribute("data","key","x",i*2);


        //XML.addTag("data");
        //XML.setAttribute("data","key","y",i*2+1);
        //XML.setValue("data",nodes[i]->pos.y,i*2+1);

        //XML.popTag();
   }

   XML.popTag();
   XML.popTag();

   XML.saveFile(xml_file_name);

   cout << xml_file_name << " saved!" <<endl;
}

void  graph_t::load(string xml_file_name)
{

    clear_all();


    if( XML.loadFile(xml_file_name) ){
		cout << xml_file_name << " loaded!" <<endl;
	}else{
		cout << "unable to load " << xml_file_name << " check data/ folder" << endl;
	}

    XML.pushTag("graphml",0);
    XML.pushTag("graph",0);

    for(int i = 0; i<XML.getNumTags("node");i++)
    {
        add_node(0.0,0.0);

        actual_node->id = ofToInt(XML.getAttribute("node","id","nada",i))-1;

        XML.pushTag("node",i);

        for(int j = 0; j<XML.getNumTags("data"); j++)
        {

            string aux = XML.getAttribute("data","key","nada",j);


            if(aux=="x")
                actual_node->pos.x = (ofToFloat(XML.getValue("data","nada",j)) - 0.5)*ofGetWidth();



            if(aux=="y")
                actual_node->pos.y = -(ofToFloat(XML.getValue("data","nada",j)) - 0.5)*ofGetHeight();


        }

        XML.popTag();

    }



    for(int i = 0; i<XML.getNumTags("edge");i++)
    {
        string source = XML.getAttribute("edge","source","nada",i);
        string target = XML.getAttribute("edge","target","nada",i);

        int sour;
        int targ;

        for(size_t j = 0; j < nodes.size(); j++)
        {
            if (nodes[j]->id == ofToInt(source))
                    sour = j;
            if (nodes[j]->id == ofToInt(target))
                    targ = j;
        }

        nodes[sour]->to.push_back(nodes[targ]);
    }

    XML.popTag();
    XML.popTag();
}


