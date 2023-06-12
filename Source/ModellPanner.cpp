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

#include "ModellPanner.h"

ModellPanner::ModellPanner() 
{
    setRule(juce::dsp::PannerRule::linear);
}

void ModellPanner::prepareToPlay(double sampleRateInput, int samplesPerBlock, int numOutputChannels)
{
    reset();

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRateInput;
    spec.numChannels = numOutputChannels;

    prepare(spec);
}

void ModellPanner::renderNextBlock(juce::AudioBuffer<float>& outputBuffer)
{
    juce::dsp::AudioBlock<float> dspAudioBlock{ outputBuffer };

    process(juce::dsp::ProcessContextReplacing<float>(dspAudioBlock));
}

void ModellPanner::updateParameters(const float panInput)
{
    setPan(panInput);
}