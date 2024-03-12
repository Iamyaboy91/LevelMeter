/*
  ==============================================================================

    VerticalGradientMeter.h
    Created: 12 Mar 2024 2:08:33pm
    Author:  斉藤孝亮

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui{ 
class VerticalGradientMeter : public juce::Component, public juce::Timer
{
public:
    VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
    {
        startTimerHz(24);
        grill = juce::ImageCache::getFromMemory(BinaryData::MeterGrill_png, BinaryData::MeterGrill_pngSize);
    }
    
    void paint(juce::Graphics& g) override
    {
        const auto level = valueSupplier();
        
        auto bounds = getLocalBounds().toFloat().reduced(2.f);
        
        g.setColour(juce::Colours::black);
        g.fillRect(bounds);
        g.setGradientFill(gradient);
        const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getHeight()));
        g.fillRect(bounds.removeFromBottom(scaledY));
    }
    void paintOverChildren(juce::Graphics& g) override
    {
        g.drawImage(grill, getLocalBounds().toFloat());
    }
    void resized() override
    {
        auto bounds = getLocalBounds().toFloat();
        gradient = juce::ColourGradient
        {
            juce::Colours::green,
            bounds.getBottomLeft(),
            juce::Colours::red,
            bounds.getTopLeft(),
            false
        };
        gradient.addColour(0.5, juce::Colours::yellow);
        
    }
    void timerCallback() override
    {
        repaint();
    }
private:
    std::function<float()> valueSupplier;
    juce::ColourGradient gradient;
    juce::Image grill;
};

}
