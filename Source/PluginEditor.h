/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Component/HorizontalMeter.h"
#include "Component/VerticalGradientMeter.h"
#include "Component/VerticalDiscreteMeter.h"
#include "Component/CircularMeter.h"

//==============================================================================
/**
*/
class LevelMeterAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    LevelMeterAudioProcessorEditor (LevelMeterAudioProcessor&);
    ~LevelMeterAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LevelMeterAudioProcessor& audioProcessor;
    
    juce::Slider leftSlider, rightSlider, rmsPeriodSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment leftSliderAttachment, rightSliderAttachment, rmsPeriodAttachment;
    juce::ToggleButton enableSmoothingButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment enableSmoothingAttachment;
    
    juce::Label rmsLevelHeading1, rmsLevelHeading2;
    juce::Label currentRmsLabel, maxRmsLabel;
    juce::Label currentRmsValue, maxRmsValue;
    juce::Label rmsPeriodLabel;
    
    float maxRmsLeft{}, maxRmsRight{};
    int framesElapsed = 0;
    
    Gui::HorizontalMeter horizontalMeterL, horizontalMeterR;
    Gui::VerticalGradientMeter verticalGradientMeterL, verticalGradientMeterR;
    Gui::VerticalDiscreteMeter verticalDiscreteMeterL, verticalDiscreteMeterR;
    Gui::CircularMeter circularMeterL, circularMeterR;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeterAudioProcessorEditor)
};
