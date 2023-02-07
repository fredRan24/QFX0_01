/*
  ==============================================================================

    yggdrasil.cpp
    Created: 28 Dec 2022 2:02:18pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "Yggdrasil.h"

Yggdrasil::Yggdrasil() {
    //Load the event root dir into local variable
    findRootDIR();
    //load a root tree from the local events directory
    createFreshEventTree();
    eventItem.reset (new EventTreeItem (getEventViewTree(), undoManager));
  
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

ValueTree Yggdrasil::createTree (const String& desc, const String& path)
{
    //create a fresh ValueTree with the ID item
    ValueTree t ("Item");
    
    //set its name and pathname properties from the arguments the function takes
    t.setProperty ("name", desc, nullptr);
    t.setProperty ("fullPathname", path, nullptr);
    
    //return the new tree
    return t;
}

void Yggdrasil::createFreshEventTree()
{
    //Absolute Path to my Events directory - NEEDS REPLACING
    String path = rootDIR;
    
    //WavFormatReader for use in checking if a file is a .wav file
    WavAudioFormat wavFormat;
    
    // Get the directory for the Events folder
    File eventDirectory = File(path);

    //Create a RootTree called Events with the pathname DIR
    auto vt = createTree ("Events", "DIR");
    
    //Declare a local unique_ptr which will be used to remember which child VT we are at when we want to append the next child VY
    unique_ptr<ValueTree> currentParent;
    
    //Set the unique_ptr to the root tree we just created
    currentParent.reset(new ValueTree(vt));
    
    //If the Event directory exists
    if (eventDirectory.exists())
    {
        //Iterate over the directory, grabbing nested files and Directories
        for (DirectoryEntry entry : RangedDirectoryIterator(File(eventDirectory), true, "*", File::findFilesAndDirectories))
        {
            //Get the item at each iteration
            File temp = entry.getFile();
            
            //Check if it exists
            if (temp.exists() && temp.getFileName() != ".DS_Store")
            {
                //Get the pathname and the filename/dirname from the item
                String dirString = temp.getFullPathName();
                String nameString = temp.getFileName();
                
                //initialise a string to be used for content
                String contentString = "DIR";
                
                //Get the items parent
                auto parent = temp.getParentDirectory();
                
                //get the items parents filename
                String parentName = parent.getFileName();
                
                //get the Current ValueTrees parent's name
                String vtParentName = currentParent->getProperty("name").toString();
                
                //If the parents dont lign up, its because the outer for loop has gone back up the directory tree
                while (parentName != currentParent->getProperty("name").toString())
                {
                    //DBG("ParentDIR: " + parentName + "  ParentVT:  " + vtParentName);
                    
                    //Get the currentParent's parent
                    auto p = currentParent->getParent();
                    
                    //if there is a parent, then we reset the currentParent to its own parent, else we set it to the root tree, as we are at the top of the higherarchy
                    if(p.isValid())
                        currentParent.reset(new ValueTree(p));
                    else
                        currentParent.reset(new ValueTree(vt));
                    
                    //DBG(p.getProperty("name").toString());
                }
                
                //As we know the parents lign up,
                //We can create a new tree with matching name and path
                auto newTree = createTree(nameString, dirString);
                
                //Then append that as a child to the correct ValueTree node
                currentParent->appendChild(newTree, nullptr);
                
                //Then set the currentParent to point to the tree we just made, as this is supposedly the "deepest" tree, and the loop above will make sure things are correct when this loops
                currentParent.reset(new ValueTree(newTree));
                
                //DBG(nameString + ":  " + dirString);
            }
            else
            {
                DBG("ERROR: The entry at some itration through the directory in createRootValueTree did not exist somehow...");
            }
            DBG("SUCCESS: Directory Was scanned successfully...");
        }
    }
    else
    {
        DBG("ERROR: A FILE IN THE DIRECTORY SUPPLIED TO createRootValueTree() WAS NOT REAL SOMEHOW...");
    }
    
    //Release the currentParent pointer
    currentParent.reset(currentParent.release());
    
    //Set the treeEventRoot tree to this newly created one...
    treeEVENTROOT = vt;
}

void Yggdrasil::searchTree(ValueTree tree, ValueTree& resultTree, const var& childName)
{
    //Create an Identifier called name
    const Identifier id = "name";
    
    //Iterate over the children of the current tree
    for (const auto& child : tree)
    {
        //check if the child has the name property corresponding with the item we are looking for
        auto check = child.getChildWithProperty(id, childName);
        
        //If it is valid, that child was found
        if(check.isValid())
        {
            //So set the result to the child which has been found
            resultTree = check;
            //DBG("SUCCESS: Found a valid child for ya!");
        }
        else
        {
            //As the child was not located here, Recursively iterate deeper
            searchTree(child, resultTree, childName);
        }
    }
}

ValueTree Yggdrasil::getEventViewTree()
{
    return treeEVENTROOT;
}

EventTreeItem* Yggdrasil::getEventViewItemPointer()
{
    return eventItem.get();
}

UndoManager* Yggdrasil::getUndoManager()
{
    return &undoManager;
}

void Yggdrasil::findRootDIR()
{
    auto start = File::getSpecialLocation(File::currentExecutableFile);
    
    //From the starting point, move outwards until you hit the Builds folder
    while (start.exists() && !start.isRoot() && start.getFileName() != "Builds")
        start = start.getParentDirectory();
    
    //Check that the file we are at is indeed the Builds file, although it really should be otherwise we'd  be stuck in the loop still..
    if(start.getFileName() == "Builds")
    {
        //If this is the Builds folder, we know the Events dir is in the same dir, aka is a sibling of builds
        auto events = start.getSiblingFile("Events");
        //Check that it is a Dir and then set the Event rootDIR
        if(events.isDirectory())
            rootDIR = events.getFullPathName();
        else
            DBG("ERROR: The events folder was not a directory for some reason.");
    }
    else
        DBG("ERROR: The file name was not Builds, when it should have been. \n check if something has changed in the root directory for the program.");
}

String Yggdrasil::getRootDIR()
{
    return rootDIR;
}
