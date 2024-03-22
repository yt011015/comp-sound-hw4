/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyPlugInAudioProcessorEditor::MyPlugInAudioProcessorEditor (MyPlugInAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    gainSlider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 50, 20);
    gainSlider.setRange(0.0f, 1.0f, 0.01f); // Example range
    gainSlider.setValue(1.0f); // Unity gain
    gainSlider.addListener(this);
    addAndMakeVisible (gainSlider);

    pitchShiftSwitch.addListener(this);
    addAndMakeVisible (pitchShiftSwitch);
    
    setSize (400, 300);
    
    
}

MyPlugInAudioProcessorEditor::~MyPlugInAudioProcessorEditor()
{
}

//==============================================================================
void MyPlugInAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void MyPlugInAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds (getWidth() / 2 - 50, getHeight() / 2 - 75, 100, 150);
    pitchShiftSwitch.setBounds (getWidth() / 2 - 13, getHeight() / 2 + 90, 30, 50);
}
