/*
  ==============================================================================

    FileBar.h
    Created: 7 Jan 2023 10:03:56pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ConvertNewEventView   : public Component
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
