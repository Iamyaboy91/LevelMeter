/*
  ==============================================================================

    Gain.h
    Created: 22 Mar 2024 11:13:18am
    Author:  斉藤孝亮

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class GainComponent : public juce::Component
//, public juce::Timer
{
public:
    GainComponent(juce::AudioProcessorValueTreeState &apvts, juce::String LeftSliderId, juce::String rightSliderId);
    ~GainComponent() override;
    void paint (juce::Graphics& g) override;
    void resized() override;

    
private:
    using attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    void setSliderWithLabels(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String parameterId, std::unique_ptr<attachment>& attachment);
    
    juce::Slider leftSlider;
    juce::Slider rightSlider;
    
    juce::Label gainLeft {"Left", "Left"};
    juce::Label gainRight {"Right", "Right"};

    using sliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<sliderAttachment> leftSliderAttachment;
    std::unique_ptr<sliderAttachment> rightSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainComponent)
};

