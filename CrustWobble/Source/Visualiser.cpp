/*
  ==============================================================================

    Visualiser.cpp
    Created: 7 Jan 2023 10:01:37pm
    Author:  Alfie Ransome

  ==============================================================================
*/

#include "Visualiser.h"
//////////////////////////////////////////////////
// AudioVisualiser Component
//////////////////////////////////////////////////

AudioVisualiser::AudioVisualiser() : thumbnailCache(5), thumbnail(512,thumbnailFormatManager,thumbnailCache)
{
    //register the basic audio file formats with the thumbnailFormatManager
    thumbnailFormatManager.registerBasicFormats();
}

AudioVisualiser::~AudioVisualiser() {
    
}

void AudioVisualiser::paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    //set the brush to background colour
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //Fill background
    g.fillRect(thumbnailBounds);
    //set the Waveform colour
    g.setColour(Colours::goldenrod);
    //draw the waveform
    thumbnail.drawChannels(g,
        thumbnailBounds,
        0.0,                                    // start time
        thumbnail.getTotalLength(),             // end time
        0.95f);                                 // vertical zoom
}

void AudioVisualiser::paintIfNoFileLoaded(Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    //set the brush to background colour
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //Fill background
    g.fillRect(thumbnailBounds);
    //set text colour
    g.setColour(Colours::grey);
    //draw text
    g.drawFittedText("Please Load an Event and File", thumbnailBounds, Justification::centred, 1);
}

void AudioVisualiser::thumbnailChanged()
{
    repaint();
}

AudioThumbnail& AudioVisualiser::getThumbnail()
{
    return thumbnail;
}

void AudioVisualiser::resized() {
    
}

void AudioVisualiser::paint(Graphics& g)
{
    //Rectangle of component bounds
    auto r = getLocalBounds();

    //If a thumbnail is loaded, paint x, if not, y
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, r);
    else
        paintIfFileLoaded(g, r);
}

void AudioVisualiser::loadData(String filePath)
{
    File temp = File(filePath);
    if(temp.existsAsFile())
    {
        thumbnail.setSource(new FileInputSource(temp));
    }
    repaint();
}
