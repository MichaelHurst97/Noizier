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

#pragma once

#include <JuceHeader.h>

 // Component that produces a pixel art when the gain knob is being turned
class UIComponentAnimatedBackground : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
public:
    UIComponentAnimatedBackground(juce::AudioProcessorValueTreeState& apvts);
    ~UIComponentAnimatedBackground() override;

    // Declared from Component
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Declared from APVTS Listener
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    // Reference to apvts object
    juce::AudioProcessorValueTreeState& parameters;

    // Used for pixel art calculation
    float gainParameterValue;
    juce::Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIComponentAnimatedBackground)
};