/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LevelMeterAudioProcessorEditor::LevelMeterAudioProcessorEditor (LevelMeterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(horizontalMeterL);
    addAndMakeVisible(horizontalMeterR);
    setSize (400, 300);
    
    startTimerHz(24);
    //startTimer(1000.f / 24);
}

LevelMeterAudioProcessorEditor::~LevelMeterAudioProcessorEditor()
{
}

//==============================================================================
void LevelMeterAudioProcessorEditor::timerCallback()
{
    horizontalMeterL.setLevel(audioProcessor.getRmsValue(0));
    horizontalMeterR.setLevel(audioProcessor.getRmsValue(1));
    
    horizontalMeterL.repaint();
    horizontalMeterR.repaint();
}
void LevelMeterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::darkgrey);


}

void LevelMeterAudioProcessorEditor::resized()
{
    auto meterX = 100;
    auto meterY = 100;
    auto meterWidth = 200;
    auto meterYOffset = 20;
    auto meterHeight = 15;
    
    horizontalMeterL.setBounds(meterX, meterY, meterWidth, meterHeight);
    horizontalMeterR.setBounds(meterX, meterY + meterYOffset, meterWidth, meterHeight);
    
}
