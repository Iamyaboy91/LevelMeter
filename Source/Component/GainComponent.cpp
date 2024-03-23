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
GainComponent::GainComponent(juce::Slider &slider, juce::Label& label, juce::AudioProcessorValueTreeState &apvts, juce::String LeftSliderId, juce::String RightSliderId)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSliderWithLabels(leftSlider, gainLeft, apvts, LeftSliderId, leftSliderAttachment);
    setSliderWithLabels(rightSlider, gainRight, apvts, RightSliderId, rightSliderAttachment);
}

GainComponent::~GainComponent()
{
}

void GainComponent::paint (juce::Graphics& g)
{


}

void GainComponent::resized()
{
    const auto container = getBounds().reduced(20);
    auto bounds = container;
    auto sliderBounds = bounds.reduced(5);
    
    sliderBounds = sliderBounds.withSizeKeepingCentre(sliderBounds.proportionOfWidth(0.6f), sliderBounds.getHeight());
    rightSlider.setBounds(sliderBounds.removeFromLeft(sliderBounds.proportionOfWidth(0.5f)));
    sliderBounds.removeFromLeft(5);
    rightSlider.setBounds(sliderBounds);}

void GainComponent::setSliderWithLabels(juce::Slider &slider, juce::Label& label, juce::AudioProcessorValueTreeState &apvts, juce::String parameterId, std::unique_ptr<attachment> &attachment)
{
    
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    slider.setPopupDisplayEnabled(true, false, this);
    slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::limegreen.withBrightness(0.6f));
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::limegreen);
    
    auto channel = 0;
    if (channel >= 0)
    {
        slider.setTextValueSuffix(" db : Right Channel");
    } else
        slider.setTextValueSuffix(" db : Left Channel");
}
