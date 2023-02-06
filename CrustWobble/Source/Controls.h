/*
  ==============================================================================

    Controls.h
    Created: 7 Jan 2023 10:07:28pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
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

