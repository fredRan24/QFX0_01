/*
  ==============================================================================

    yggdrasil.cpp
    Created: 28 Dec 2022 2:02:18pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "Yggdrasil.h"

Yggdrasil::Yggdrasil() {
    
    
}

Yggdrasil::~Yggdrasil() {
    
    
}

void Yggdrasil::growNewTree() {
    
    treeROOT = ValueTree(ROOT);
    treeVIEWS = ValueTree(VIEWS);
    treeFILEBAR = ValueTree(FILEBAR);
    treeLISTWINDOW = ValueTree(LISTWINDOW);
    treeVISUALISER = ValueTree(VISUALISER);
    treeCONTROLS = ValueTree(CONTROLS);
    treeEVENTROOT = ValueTree(EVENTROOT);
    
    treeROOT.appendChild(treeVIEWS, nullptr);
    treeROOT.appendChild(treeEVENTROOT, nullptr);
    
    treeVIEWS.appendChild(treeFILEBAR, nullptr);
    treeVIEWS.appendChild(treeLISTWINDOW, nullptr);
    treeVIEWS.appendChild(treeVISUALISER, nullptr);
    treeVIEWS.appendChild(treeCONTROLS, nullptr);
    
    DBG("Created a new Value Tree that looks like:" << treeROOT.toXmlString());
}

ValueTree* Yggdrasil::getROOT() {
    return rootPointer;
}

