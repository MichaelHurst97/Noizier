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
#include "UIComponentPainthelper.h"
#include "UIComponentSlider.h"

// LFO Panel for UI
class UIControlsLFO : public UIComponentPaintHelper
{
public:
    UIControlsLFO(juce::AudioProcessorValueTreeState& apvts);
    ~UIControlsLFO() override;

    void resized() override;

    void updateToggleState(juce::Button* button);

private:
    UIComponentSlider sliderLFO;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachmentLFOSwitch;
    juce::TextButton buttonLFOSwitch;
    juce::String buttonLFOStateText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIControlsLFO)
};