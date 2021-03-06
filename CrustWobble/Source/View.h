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
};

class DirectoryDisplay : public Component
{
public:
    DirectoryDisplay();
    ~DirectoryDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void loadDirectoryIntoFileTreeComponent(File& directory);

private:
    unique_ptr<FileTreeComponent> fileTree;
    unique_ptr<DirectoryContentsList> dirContents;
};

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

class MainView : public Component
{
public:
    MainView();
    ~MainView();

    //Overrides
    void paint(juce::Graphics& g) override;
    void resized() override;

    FilenameComponent* getDirChooser();
    TextButton* getConvertButton();
    
private:
    FileMenuBar fileMenuBar;
    
    FilenameComponent dirChooser;
    FilenameComponent* dirChooserPtr = &dirChooser;
    
    TextButton convertButton;
    TextButton* convertButtonPtr = &convertButton;
    
};
