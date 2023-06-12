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

// Plugin Name and Noise Type Selector Panel for UI
class UIControlsPlugin : public UIComponentPaintHelper
{
public:
    UIControlsPlugin(juce::AudioProcessorValueTreeState& apvts);
    ~UIControlsPlugin() override;

    void resized() override;

private:
    juce::Label titleLabel;
    juce::Label subtitleLabel;
    juce::HyperlinkButton linkButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachmentNoiseType;
    juce::ComboBox comboBoxNoiseType;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIControlsPlugin)
};