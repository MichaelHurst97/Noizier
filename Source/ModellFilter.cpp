/*
 * Noizier - a simple noise generator, built by Michael Hurst
 *
 * Copyright 2023, with code adaptions based on the JUCE Documentation
 * and Joshua Hodge (The Audio Programmer), as described in the GitHub
 * https://github.com/MichaelHurst97/Noizier
 *
 * Noizier is released under the GNU General Public Licence v3
 * or later (GPL-3.0-or-later). The license is found in the "LICENSE"
 * file in the root of this repository, or at
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 */

#include "ModellFilter.h"

// High and Lowpass Filters that stacks 3 individual filters with 12db slope each.
// For a steeper cutoff, these filters are applied in series, leaving us with a 36db slope
ModellFilter::ModellFilter()
{
    // Set first 3 filters to highpass, and last 3 to lowpass
    for (int i = 0; i < filters.size(); ++i) 
    {
        if (i < 3) 
        {
            filters[i].setType(juce::dsp::StateVariableTPTFilterType::highpass);
        }
        else 
        {
            filters[i].setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        }
    }
}

void ModellFilter::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels)
{
    reset();

    // Pass specs so the filter algorithm knows how to process the sound
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;

    for (auto& filter : filters) 
    {
        filter.prepare(spec);
    }
}

void ModellFilter::renderNextBlock(juce::AudioBuffer<float>& outputBuffer)
{
    juce::dsp::AudioBlock<float> dspAudioBlock{ outputBuffer };

    for (auto& filter : filters) 
    {
        filter.process(juce::dsp::ProcessContextReplacing<float>(dspAudioBlock));
    }
}

void ModellFilter::updateParameters(const float frequencyHP, const float frequencyLP)
{
    for (int i = 0; i < filters.size(); ++i) 
    {
        if (i < 3) 
        {
            filters[i].setCutoffFrequency(frequencyHP);
        }
        else 
        {
            filters[i].setCutoffFrequency(frequencyLP);
        }
    }
}

void ModellFilter::reset() 
{
    for (auto& filter : filters) 
    {
        filter.reset();
    }
}