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

#include "ModellGain.h"

void ModellGain::prepareToPlay(const float initGain)
{
    previousGain = initGain;
}

void ModellGain::renderNextBlock(juce::AudioBuffer<float>& outputBuffer)
{
    // Use a gain ramp to eliminate sound artifacts from rapid gain changes (e.g. quick user knob movement)
    if (juce::approximatelyEqual(currentGain, previousGain))
    {
        outputBuffer.applyGain(currentGain);
    }
    else
    {
        outputBuffer.applyGainRamp(0, outputBuffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }
}

void ModellGain::updateParameters(const float inputGain)
{
    currentGain = inputGain;
}