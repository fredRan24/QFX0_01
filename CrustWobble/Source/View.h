/*
  ==============================================================================

    View.h
    Created: 28 Oct 2021 6:43:51pm
    Author:  Alfie

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>

using namespace std;
using namespace juce;

// ----------------------------------------------------------

class ConvertNewEventView : public Component
{
public:
    ConvertNewEventView();
    ~ConvertNewEventView();

    //Overrides
    void paint(juce::Graphics& g) override;
    void resized() override;

    FilenameComponent* getDirChooser();
    TextButton* getConvertButton();
    
private:
    FilenameComponent dirChooser;
    FilenameComponent* dirChooserPtr = &dirChooser;
    
    TextButton convertButton;
    TextButton* convertButtonPtr = &convertButton;
    
};

// ----------------------------------------------------------

class FileMenuBar : public Component, 
                    public MenuBarModel
{
public:
    FileMenuBar();
    ~FileMenuBar();

    //Overrides
    void paint(juce::Graphics& g) override;
    void resized() override;
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;


    //Enums
    enum  Menus
    {
        FileMenu = 0,

        NumMenus
    };

    /** The items within the File Menu

        @see Menus
                                                                                        */
    enum  FileMenuItems
    {
        AudioPrefs = 1,

        NumFileItems
    };

private:
    MenuBarComponent menuBar;
    ConvertNewEventView converter;
};

// ----------------------------------------------------------

class EventTreeItem : public TreeViewItem, private ValueTree::Listener
{
public:
    EventTreeItem(const ValueTree& v, UndoManager& um);
    ~EventTreeItem();
    
    void itemOpennessChanged (bool isNowOpen) override;
    void paintItem (Graphics& g, int width, int height) override;
    bool mightContainSubItems() override;
    String getUniqueName() const override;
    
    static void getSelectedTreeViewItems (TreeView& treeView, OwnedArray<ValueTree>& items);
    
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
// ----------------------------------------------------------


class DirectoryDisplay : public Component
{
public:
    DirectoryDisplay();
    ~DirectoryDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

    static ValueTree createTree (const String& desc);
    static ValueTree createRootValueTree();
    static ValueTree createRandomTree (int& counter, int depth);
    
    void deleteSelectedItems();
    void loadDirectoryIntoFileTreeComponent(File& directory);

private:
    TreeView eventTree;
    UndoManager undoManager;
    
    unique_ptr<EventTreeItem> eventItem;
    unique_ptr<FileTreeComponent> fileTree;
    unique_ptr<DirectoryContentsList> dirContents;
};

// ----------------------------------------------------------

class ControlsView : public Component
{
public:
    ControlsView();
    ~ControlsView();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:

    //playwinowdtoggle
    //downloadwindowtoggle
    //convertwindowtoggle

};


// ----------------------------------------------------------


class AudioVisualiser : public Component
{
public:

    AudioVisualiser();
    ~AudioVisualiser();
                                               
    void paint(Graphics& g) override;

    void resized() override;

    AudioThumbnail& getThumbnail();
                                                          
    void thumbnailChanged();

private:

    void paintIfNoFileLoaded(Graphics& g, const juce::Rectangle<int>& thumbnailBounds);

    void paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);

    AudioFormatManager thumbnailFormatManager;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioVisualiser)

};

// ----------------------------------------------------------


class TopWindowView : public Component
{
public:
    TopWindowView();
    ~TopWindowView();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void updateVisualiser();
    
private:
    AudioVisualiser visualiser;
    ControlsView controls;
};


// ----------------------------------------------------------


class MainView : public Component
{
public:
    MainView();
    ~MainView();

    //Overrides
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    
    FileMenuBar fileMenuBar;
    TopWindowView topWindow;
    DirectoryDisplay dirDisplay;
};


// ----------------------------------------------------------


