/*
  ==============================================================================

    GainComponent.cpp
    Created: 22 Mar 2024 1:18:45pm
    Author:  斉藤孝亮

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainComponent.h"



//==============================================================================
GainComponent::GainComponent(juce::AudioProcessorValueTreeState &apvts, juce::String leftSliderId, juce::String rightSliderId)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSliderWithLabels(leftSlider, gainLeft, apvts, leftSliderId, leftSliderAttachment);
    setSliderWithLabels(rightSlider, gainRight, apvts, rightSliderId, rightSliderAttachment);
}

GainComponent::~GainComponent()
{
}

void GainComponent::paint (juce::Graphics& g)
{

}

void GainComponent::resized()
{
    auto bounds = getLocalBounds().toFloat().reduced(5);
    auto sliderBounds = bounds.reduced(5);
    sliderBounds = sliderBounds.withSizeKeepingCentre(sliderBounds.proportionOfWidth(0.6f), sliderBounds.getHeight());
    
}

void GainComponent::setSliderWithLabels(juce::Slider &slider, juce::Label& label, juce::AudioProcessorValueTreeState &apvts, juce::String parameterId, std::unique_ptr<attachment> &attachment)
{
    
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    slider.setPopupDisplayEnabled(true, false, this);
    slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::limegreen.withBrightness(0.6f));
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::limegreen);
    
    if (auto channel = 0)
    {
        slider.setTextValueSuffix(" db : Right Channel");
    } else if(channel == 1)
    {
        slider.setTextValueSuffix(" db : Left Channel");
    }

}
