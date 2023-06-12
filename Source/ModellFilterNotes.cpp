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

#include "ModellFilterNotes.h"

// Lowpass Filter that uses the frequency of the currently playing note as cutoff.
// Stacks 3 individual filters with 12db slope each.
// For a steeper cutoff, these filters are applied in series, leaving us with a 36db slope
ModellFilterNotes::ModellFilterNotes()
{
    // Set all filters to lowpass
    for (int i = 0; i < filters.size(); ++i) 
    {
        filters[i].setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    }
}

void ModellFilterNotes::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels)
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

void ModellFilterNotes::renderNextBlock(juce::AudioBuffer<float>& outputBuffer)
{
    juce::dsp::AudioBlock<float> dspAudioBlock{ outputBuffer };

    for (auto& filter : filters) 
    {
        filter.process(juce::dsp::ProcessContextReplacing<float>(dspAudioBlock));
    }
}

void ModellFilterNotes::updateParameters(const float midiHertzInput)
{
    midiHertz = midiHertzInput;
    if (midiHertzPrevious == midiHertz) 
    {
        return;
    }
    midiHertzPrevious = midiHertz;

    // Everything above 1046.50 Hertz (Note C6) should be limited to this same frequency. 
    // I dont want a range that goes beyond that value for convenience when producing music
    if (midiHertz > 1046.50f)
    {
        midiHertz = 1046.50f;
    }

    // The lowest possible midi note Hertz should be mapped to set the lowest possible filter cutoff to 261.63 Hertz (C4)
    // Everything above 1046.50 Hertz opens the filter completely
    auto midiHertzAdjusted = juce::jmap(midiHertz, 8.18f, 1046.50f, 261.63f, 20000.0f);

    for (int i = 0; i < filters.size(); ++i) 
    {
        filters[i].setCutoffFrequency(midiHertzAdjusted);
    }
}

void ModellFilterNotes::reset()
{
    for (auto& filter : filters) 
    {
        filter.reset();
    }
}