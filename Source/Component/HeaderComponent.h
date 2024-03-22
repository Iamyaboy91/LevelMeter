/*
  ==============================================================================

    HeaderComponent.h
    Created: 22 Mar 2024 11:24:32am
    Author:  斉藤孝亮

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class HeaderComponent  : public juce::Component
{
public:
    HeaderComponent();
    ~HeaderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComponent)
};
