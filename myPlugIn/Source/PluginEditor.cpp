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
    gainSlider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 50, 20);
    gainSlider.setRange(0.0f, 1.0f, 0.01f);
    gainSlider.setValue(1.0f);
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
    gainSlider.setBounds (getWidth() / 2 - 50, getHeight() / 2 - 75, 100, 150);
    pitchShiftSwitch.setBounds (getWidth() / 2 - 13, getHeight() / 2 + 75, 30, 50);
    
}
