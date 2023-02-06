/*
  ==============================================================================

    EventTreeItem.h
    Created: 7 Jan 2023 10:08:28pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class EventTreeItem   : public TreeViewItem,
                        private ValueTree::Listener
{
public:
    EventTreeItem(const ValueTree& v, UndoManager& um);
    ~EventTreeItem();
    
    void itemOpennessChanged (bool isNowOpen) override;
    void paintItem (Graphics& g, int width, int height) override;
    bool mightContainSubItems() override;
    
    String getUniqueName() const override;
    String getAudioDataAsString();
    
    static void getSelectedTreeViewItems (TreeView& treeView, OwnedArray<ValueTree>& items);
    
    ValueTree* getThisTree();
    String getThisItemsAudio();
    
private:
    
    ValueTree tree;
    UndoManager& undoManager;
    
    void refreshSubItems();
    
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override         { treeChildrenChanged (parentTree); }
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override  { treeChildrenChanged (parentTree); }
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override    { treeChildrenChanged (parentTree); }
    void valueTreeParentChanged (ValueTree&) override {}

    void treeChildrenChanged (const ValueTree& parentTree);

};
