#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    show_image = true;

    background.loadImage("Delta.tif");
    background.resize(ofGetWindowWidth() , ofGetWindowHeight());
    z0 = 1.0;
    z = 1;

    B.load("data/1.graphml");
    instance = 1;

    yoff = 0.0;
    xoff = 0.0;

}

//--------------------------------------------------------------
void testApp::update(){


}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));

    ofTranslate(ofGetWindowWidth() / 2.0+xoff, ofGetWindowHeight() / 2.0 +yoff);

    ofScale(z,z,1);

    ofEnableAlphaBlending();

    if (show_image)
    {
        ofSetColor(120);

        background.draw(-ofGetWindowWidth() / 2.0,- ofGetWindowHeight() / 2.0 );
    }

    B.draw();
    ofDisableAlphaBlending();


}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key=='e'){

    B.clear_all();

    }

    if (key=='i')

    {
        show_image = !show_image;
    }

    if (key=='w'){

    cout << "ofgetwidth " << ofGetWidth() << "ofgetwidth " <<ofGetHeight() << endl;
    cout << "ofgetwindowwidth " << ofGetWindowWidth() << "ofgetwidth " <<ofGetWindowHeight() << endl;
    cout << "ofgetscreenwidth " << ofGetScreenWidth() << "ofgetwidth " <<ofGetScreenHeight() << endl;

    }

    if (key==127)
    {
        B.del_node();
    }

    if (key=='1')
    {
        B.load("data/1.graphml");
        instance = 1;
    }

    if (key=='2')
    {
        B.load("data/2.graphml");
        instance = 2;
    }

    if (key=='s')
    {
        B.save( "data/"+ofToString(instance)+".graphml");
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

    x = (x - ofGetWindowWidth()  / 2.0 - xoff);
    y = (y - ofGetWindowHeight() / 2.0) - yoff;

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    if(button==0)
    {



        x = (x - ofGetWindowWidth()  / 2.0-xoff)/z;
        y = (y - ofGetWindowHeight() / 2.0-yoff)/z;

        //xoff = x-x0;
        //yoff = y-y0;


        B.mouseDragged(x,y);
    }


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){


    int _x, _y;

    _x = (x - ofGetWindowWidth()  / 2.0-xoff)/z;
    _y = (y - ofGetWindowHeight() / 2.0-yoff)/z;

    x0 = _x;
    y0 = _y;


    int sel = B.mousePressed(_x,_y,button);

    if(button==3){
        z *= z0;
    }

    if(button==4){
        z /= z0;
    }

    /*if(sel==-1 && button==2)
    {
        xoff = -_x;
        yoff = -_y;

    }
        */
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){


}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
