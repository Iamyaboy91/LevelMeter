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
, verticalGradientMeterL([&]()  {return audioProcessor.getRmsValue(0); })
,verticalGradientMeterR([&]() {return audioProcessor.getRmsValue(1); })
{
    addAndMakeVisible(horizontalMeterL);
    addAndMakeVisible(horizontalMeterR);
    
    addAndMakeVisible(verticalGradientMeterL);
    addAndMakeVisible(verticalGradientMeterR);
    setSize (400, 500);
    
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
    auto meterY2 = 200;
    auto meterWidth = 200;
    auto meterWidth2 = 25;
    auto meterOffset = 30;
    auto meterHeight = 15;
    auto meterHeight2 = 200;
    
    horizontalMeterL.setBounds(meterX, meterY, meterWidth, meterHeight);
    horizontalMeterR.setBounds(meterX, meterY + 20, meterWidth, meterHeight);
    
    verticalGradientMeterL.setBounds(meterX, meterY2, meterWidth2, meterHeight2);
    verticalGradientMeterR.setBounds(meterX + meterOffset, meterY2, meterWidth2, meterHeight2);
    
}
