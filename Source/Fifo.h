/*
  ==============================================================================

    Fifo.h
    Created: 19 Mar 2024 4:50:27pm
    Author:  斉藤孝亮

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Utility 
{
class AbstractWriter
{
public:
    AbstractWriter() = default;
    
    void reset(int capacity, int samplesToRead) noexcept
    {
        jassert(capacity > 0);
        jassert(samplesToRead > 0 && samplesToRead < capacity);
        bufferSize = capacity;
        readSize = samplesToRead;
        writePos = 0;
        writePos.store(0);
    }
    void prepareToWrite(int numToWrite, int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const
    {
        const int ve = writePos.load();
        const int localReadSisze = readSize.load();
        
        numToWrite = juce::jmin(numToWrite, bufferSize - localReadSisze);
        
        if(numToWrite <= 0)
        {
            startIndex1 = 0;
            startIndex2 = 0;
            blockSize1 = 0;
            blockSize2 = 0;
        }
        else
        {
            startIndex1 = ve;
            startIndex2 = 0;
            blockSize1 = juce::jmin(bufferSize - ve, numToWrite);
            numToWrite -= blockSize1;
            blockSize2 = numToWrite <= 0 ? 0 : juce::jmin(numToWrite, bufferSize - localReadSisze);
        }
    }
    
    void finishedWrite(int numWritten) noexcept
    {
        jassert(numWritten >= 0 && numWritten < bufferSize);
        
        int newEnd = writePos.load() + numWritten;
        if (newEnd >= bufferSize) 
            newEnd -= bufferSize;
        
        writePos.store(newEnd);
    }
    
    void prepareToRead(int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const noexcept
    {
        const int localReadSize = readSize.load();
        
        int vs = writePos.load() - localReadSize;
        if(vs < 0) vs += bufferSize;
        
        int numWanted = localReadSize;
        
        startIndex1 = vs;
        startIndex2 = 0;
        blockSize1 = juce::jmin(bufferSize - vs, localReadSize);
        numWanted -= blockSize1;
        blockSize2 = numWanted <= 0 ? 0 : numWanted;
    }
    
    void setReadSize(int newValue) noexcept
    {
        jassert(newValue > 0 && newValue < bufferSize);
        readSize.store(newValue);
        
    }
private:
    int bufferSize;
    std::atomic<int> readSize;
    std::atomic<int> writePos;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbstractWriter)
};

class Fifo
{
public:
    void reset(int  numberOfChannels, int capacity)
    {
        fifo.reset(capacity, capacity - 1);
        buffer.clear();
        buffer.setSize(numberOfChannels, capacity, true, false, true);
    }
    void push(const juce::AudioBuffer<float>& data)
    {
        const auto numberOfItems = data.getNumSamples();
        jassert(numberOfItems < buffer.getNumSamples());
        jassert(data.getNumChannels() == buffer.getNumChannels());
        
        int start1, size1, start2, size2;
        fifo.prepareToWrite(numberOfItems, start1, size1, start2, size2);
        
        if(size1 > 0)
            for(auto channel = 0; channel < buffer.getNumChannels(); channel++)
                buffer.copyFrom(channel, start1, data, channel, 0, size1);
        if(size2 > 0)
            for (auto channel = 0; channel < buffer.getNumChannels(); channel++)
                buffer.copyFrom(channel, start2, data, channel, size1, size2);
        fifo.finishedWrite(size1 + size2);
    }
    void pull(juce::AudioBuffer<float>& data, int numberOfItems)
    {
        jassert(numberOfItems < buffer.getNumSamples());
        jassert(data.getNumChannels() == buffer.getNumChannels());
        
        int start1, size1, start2, size2;
        fifo.setReadSize(numberOfItems);
        fifo.prepareToRead(start1, size1, start2, size2);
        
        if(size1 > 0)
            for(auto channel = 0; channel < buffer.getNumChannels(); channel++)
                data.copyFrom(channel, 0, buffer, channel, start1, size1);
        if(size2 > 0)
            for(auto channel = 0; channel < buffer.getNumChannels(); channel++)
                data.copyFrom(channel, size1, buffer, channel, start2, size2);
    }
    
    void pull(float* destination, int channel, int numberOfItems)
    {
        jassert(numberOfItems < buffer.getNumSamples());
        jassert(channel >= 0 && channel < buffer.getNumChannels());
        
        int start1, size1, start2, size2;
        fifo.setReadSize(numberOfItems);
        fifo.prepareToRead(start1, size1, start2, size2);
        
        const auto* source = buffer.getReadPointer(channel);
        if (size1 > 0)
            std::copy_n(source + start1, size1, destination);
        if(size2 > 0)
            std::copy_n(source + start2, size2, destination + size1);
    }
    
private:
    AbstractWriter fifo;
    juce::AudioBuffer<float> buffer;
};
}
