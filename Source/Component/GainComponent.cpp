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
GainComponent::GainComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

GainComponent::~GainComponent()
{
    leftSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    leftSlider.setPopupDisplayEnabled(true, false, this);
    leftSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::limegreen.withBrightness(0.6f));
    leftSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::limegreen);
    leftSlider.setTextValueSuffix(" db : Left Channel");
    
    rightSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    rightSlider.setPopupDisplayEnabled(true, false, this);
    rightSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::lightseagreen.withBrightness(0.6f));
    rightSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::lightseagreen);
    rightSlider.setTextValueSuffix(" db : Right Channel");
}

void GainComponent::paint (juce::Graphics& g)
{


}

void GainComponent::resized()
{

}

void 
