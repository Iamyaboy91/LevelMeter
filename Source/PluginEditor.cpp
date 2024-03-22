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
,leftSliderAttachment(p.getApvts(), "Left", leftSlider)
,rightSliderAttachment(p.getApvts(), "Right", rightSlider)
,rmsPeriodAttachment(p.getApvts(), "RmsPeriod", rmsPeriodSlider)
,enableSmoothingAttachment(p.getApvts(), "Smoothing", enableSmoothingButton)
,verticalGradientMeterL([&]()  {return audioProcessor.getRmsLevel(0); })
,verticalGradientMeterR([&]() {return audioProcessor.getRmsLevel(1); })
,verticalDiscreteMeterL([&]() {return audioProcessor.getRmsLevel(0); })
,verticalDiscreteMeterR([&]() {return audioProcessor.getRmsLevel(1); })
,circularMeterL([&](){return audioProcessor.getRmsLevel(0);}, juce::Colours::lawngreen)
,circularMeterR([&](){return audioProcessor.getRmsLevel(1);}, juce::Colours::lightgreen)
{
    addAndMakeVisible(rmsLevelHeading1);
    addAndMakeVisible(rmsLevelHeading2);
    addAndMakeVisible(currentRmsLabel);
    addAndMakeVisible(maxRmsLabel);
    addAndMakeVisible(currentRmsValue);
    addAndMakeVisible(maxRmsValue);
    addAndMakeVisible(rmsPeriodLabel);
    
    addAndMakeVisible(horizontalMeterL);
    addAndMakeVisible(horizontalMeterR);
    
    addAndMakeVisible(verticalGradientMeterL);
    addAndMakeVisible(verticalGradientMeterR);
    
    addAndMakeVisible(verticalDiscreteMeterL);
    addAndMakeVisible(verticalDiscreteMeterR);
    
    addAndMakeVisible(circularMeterL);
    addAndMakeVisible(circularMeterR);
    
    addAndMakeVisible(leftSlider);
    addAndMakeVisible(rightSlider);
    addAndMakeVisible(rmsPeriodSlider);
    addAndMakeVisible(enableSmoothingButton);
    
    rmsLevelHeading1.setText("dbFS", juce::dontSendNotification);
    rmsLevelHeading1.setFont(juce::Font{}.withStyle(juce::Font::FontStyleFlags::bold));
    rmsLevelHeading2.setText("Left  \t  Right", juce::dontSendNotification);
    rmsLevelHeading2.setFont(juce::Font{}.withStyle(juce::Font::FontStyleFlags::bold));
    currentRmsLabel.setText("Current RMS", juce::dontSendNotification);
    maxRmsLabel.setText("Max RMS", juce::dontSendNotification);
    rmsPeriodLabel.setText("RMS Period", juce::dontSendNotification);
    rmsPeriodLabel.setJustificationType(juce::Justification::right);
    
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
    
    
    rmsPeriodSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    rmsPeriodSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    rmsPeriodSlider.setPopupDisplayEnabled(true, false, this);
    rmsPeriodSlider.setTextValueSuffix(" ms");
    
    enableSmoothingButton.setButtonText("Enable Smoothing");
    
    setSize (400, 650);
    setResizable(true, true);
    startTimerHz(24);
    //startTimer(1000.f / 24);
}

LevelMeterAudioProcessorEditor::~LevelMeterAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void LevelMeterAudioProcessorEditor::timerCallback()
{
    if(++framesElapsed > 100)
    {
        framesElapsed = 0;
        maxRmsLeft = -100.f;
        maxRmsRight = -100.f;
    }
    const auto gainLeft = audioProcessor.getRmsLevel(0);
    const auto gainRight = audioProcessor.getRmsLevel(1);
    if(gainLeft > maxRmsLeft)
        maxRmsLeft = gainLeft;
    if(gainRight > maxRmsRight)
        maxRmsRight = gainRight;
    
    currentRmsValue.setText(juce::String{gainLeft, 2} + "  " + juce::String{gainRight, 2}, juce::sendNotification);
    maxRmsValue.setText(juce::String{maxRmsLeft, 2} + "  " + juce::String{maxRmsRight, 2}, juce::sendNotification);
    
    horizontalMeterL.setLevel(gainLeft);
    horizontalMeterL.repaint();
    
    horizontalMeterR.setLevel(gainRight);
    horizontalMeterR.repaint();
}
void LevelMeterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setGradientFill( juce::ColourGradient{juce::Colours::darkgrey, getLocalBounds().toFloat().getCentre(), juce::Colours::darkgrey.darker(0.7f), {}, true});
    g.fillRect(getLocalBounds());
    
    g.setColour(juce::Colours::black);
    g.fillEllipse(circularMeterL.getBounds().toFloat());
}

void LevelMeterAudioProcessorEditor::resized()
{
    const auto container = getBounds().reduced(20);
    auto bounds = container;
    
    auto labelBounds = bounds.removeFromTop(container.proportionOfHeight(0.12f));
    auto controlBounds = labelBounds.removeFromRight(container.proportionOfWidth(0.35f));
    
    const auto labelHeight = labelBounds.proportionOfHeight(0.33f);
    
    auto labelRow1 = labelBounds.removeFromTop(labelHeight);
    rmsLevelHeading1.setBounds(labelRow1.removeFromLeft(labelRow1.proportionOfWidth(0.5f)));
    rmsLevelHeading2.setBounds(labelRow1);
    
    auto labelRow2 = labelBounds.removeFromTop(labelHeight);
    maxRmsLabel.setBounds(labelRow2.removeFromLeft(labelRow2.proportionOfWidth(0.5f)));
    maxRmsValue.setBounds(labelRow2);
    
    auto labelRow3 = labelBounds;
    currentRmsLabel.setBounds(labelRow3.removeFromLeft(labelRow3.proportionOfWidth(0.5f)));
    currentRmsValue.setBounds(labelRow3);
    
    rmsPeriodLabel.setBounds(controlBounds.removeFromTop(labelHeight));
    rmsPeriodSlider.setBounds(controlBounds.removeFromTop(labelHeight));
    enableSmoothingButton.setBounds(controlBounds);
    
    auto horizontalMeterBounds = bounds.removeFromTop(container.proportionOfHeight(0.1f)).reduced(5);
    horizontalMeterL.setBounds(horizontalMeterBounds.removeFromTop(horizontalMeterBounds.proportionOfHeight(0.5f)).reduced(5));
    horizontalMeterR.setBounds(horizontalMeterBounds.reduced(5));
    
    auto verticalMeterBounds = bounds.removeFromTop(container.proportionOfHeight(0.35f)).reduced(container.proportionOfWidth(0.1f), 5);
    auto verticalMeter1 = verticalMeterBounds.removeFromLeft(verticalMeterBounds.proportionOfWidth(0.5f)).withSizeKeepingCentre(55, 200);
    verticalGradientMeterL.setBounds(verticalMeter1.removeFromLeft(25));
    verticalMeter1.removeFromLeft(5);
    verticalGradientMeterR.setBounds(verticalMeter1);
    
    auto verticalMeter2 = verticalMeterBounds.withSizeKeepingCentre(55, verticalMeterBounds.getHeight());
    verticalDiscreteMeterL.setBounds(verticalMeter2.removeFromLeft(25));
    verticalMeter2.removeFromLeft(5);
    verticalDiscreteMeterR.setBounds(verticalMeter2);
    
    auto circularMeterBounds = bounds.removeFromTop(container.proportionOfHeight(0.3f)).reduced(5);
    const auto diameter = juce::jmin(circularMeterBounds.getWidth(), circularMeterBounds.getHeight());
    circularMeterBounds = circularMeterBounds.withSizeKeepingCentre(diameter, diameter);
    circularMeterL.setBounds(circularMeterBounds);
    circularMeterR.setBounds(circularMeterBounds);
    
    auto sliderBounds = bounds.reduced(5);
    sliderBounds = sliderBounds.withSizeKeepingCentre(sliderBounds.proportionOfWidth(0.6f), sliderBounds.getHeight());
    leftSlider.setBounds(sliderBounds.removeFromLeft(sliderBounds.proportionOfWidth(0.5f)));
    sliderBounds.removeFromLeft(5);
    rightSlider.setBounds(sliderBounds);
    
    
    
}

