/*
  ==============================================================================

    EventTreeItem.cpp
    Created: 7 Jan 2023 10:08:28pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "EventTreeItem.h"
//////////////////////////////////////////////////
// EventTreeItem Class
//////////////////////////////////////////////////

EventTreeItem::EventTreeItem(const ValueTree& v, UndoManager& um) : tree (v), undoManager(um)
{
    tree.addListener(this);
}

EventTreeItem::~EventTreeItem()
{
    
}

String EventTreeItem::getUniqueName() const
{
    return tree["name"].toString();
}

bool EventTreeItem::mightContainSubItems()
{
    return tree.getNumChildren() > 0;
}

void EventTreeItem::paintItem (Graphics& g, int width, int height)
{
    g.setColour(Colours::white);
    g.drawRoundedRectangle(0, 0, width, height, 0.5, 0.8);
    g.fillAll(Colours::darkslategrey);

    if (isSelected())
        g.fillAll (Colours::orange);
    
    g.setColour (Colours::whitesmoke);
    g.setFont (20.0f);

    g.drawText (tree["name"].toString(),
                4, 0, width - 4, height,
                Justification::centredLeft, true);
}

void EventTreeItem::itemOpennessChanged (bool isNowOpen)
{
    if (isNowOpen && getNumSubItems() == 0)
        refreshSubItems();
    else
        clearSubItems();
}


void EventTreeItem::treeChildrenChanged (const ValueTree& parentTree)
{
    if (parentTree == tree)
    {
        refreshSubItems();
        treeHasChanged();
        setOpen (true);
    }
}

void EventTreeItem::refreshSubItems()
{
    clearSubItems();

    for (int i = 0; i < tree.getNumChildren(); ++i) {
        addSubItem (new EventTreeItem(tree.getChild (i), undoManager));
    }
}

void EventTreeItem::valueTreePropertyChanged (ValueTree&, const Identifier&)
{
    repaintItem();
}

void EventTreeItem::getSelectedTreeViewItems (TreeView& treeView, OwnedArray<ValueTree>& items)
{
    auto numSelected = treeView.getNumSelectedItems();

    for (int i = 0; i < numSelected; ++i)
        if (auto* vti = dynamic_cast<EventTreeItem*> (treeView.getSelectedItem (i)))
            items.add (new ValueTree (vti->tree));
}

String EventTreeItem::getAudioDataAsString()
{
    return tree.getProperty("audio");
}


ValueTree* EventTreeItem::getThisTree()
{
    return &tree;
}

String EventTreeItem::getThisItemsAudio()
{
    return tree.getProperty("audio");
}


