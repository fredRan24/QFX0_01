#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : controllerObject(mainView, dirConvertorObject)
{
    addAndMakeVisible(mainView);
    
    setSize(1200, 720);
    
}

MainComponent::~MainComponent()
{

}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
 
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    mainView.setBounds(getLocalBounds());
}
