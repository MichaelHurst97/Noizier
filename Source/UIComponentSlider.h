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

// Creates a slider and does some basic style changes to it in one go
class UIComponentSlider : public juce::Component
{
public:
    UIComponentSlider(juce::AudioProcessorValueTreeState& apvts, juce::String parameterID, juce::String labelID, juce::String labelText, juce::String labelType,
        juce::String fontHeightID, float fontHeight, double doubleClickValue);

    void resized() override;

    void updateBounds(int newSliderBoundsWidth, int newSldierBoundsHeight);

    juce::Slider slider;

private:
    int sliderBoundsWidth;
    int sliderBoundsHeight;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponentSlider)
};