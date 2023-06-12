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

#include <JuceHeader.h>
#include "UIControlsPlugin.h"

UIControlsPlugin::UIControlsPlugin(juce::AudioProcessorValueTreeState& apvts)
{
    // Plugin Title Text
    titleLabel.setText("NOIZIER", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::backgroundColourId, juce::Colours::white);
    titleLabel.setColour(juce::Label::outlineColourId, juce::Colours::white);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    addAndMakeVisible(titleLabel);

    // Plugin Subtitle Text
    subtitleLabel.setText("A SIMPLE NOISE GENERATOR", juce::dontSendNotification);
    subtitleLabel.setJustificationType(juce::Justification::centred);
    subtitleLabel.setColour(juce::Label::backgroundColourId, juce::Colours::white);
    subtitleLabel.setColour(juce::Label::outlineColourId, juce::Colours::white);
    subtitleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    subtitleLabel.setFont(juce::Font(8.0f, juce::Font::bold));
    addAndMakeVisible(subtitleLabel);
    
    // Noise Type Selector
    attachmentNoiseType = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, 
        "noiseType", comboBoxNoiseType);
    juce::StringArray noiseTypes{ "WHITE NOISE", "PINK NOISE", "COMB-ISH NOISE" };
    comboBoxNoiseType.addItemList(noiseTypes, 1);
    comboBoxNoiseType.setSelectedItemIndex(0);
    comboBoxNoiseType.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(comboBoxNoiseType);
}

UIControlsPlugin::~UIControlsPlugin()
{
}

void UIControlsPlugin::resized()
{
    auto width = getWidth();
    auto height = getHeight();

    int comboBoxBoundsWidth = (int)(width * 0.7);
    int comboBoxBoundsHeight = (int)(height * 0.15);

    titleLabel.setBounds((width - comboBoxBoundsWidth) / 2, (int)(height / 2.7),
        comboBoxBoundsWidth, comboBoxBoundsHeight);

    subtitleLabel.setBounds((width - comboBoxBoundsWidth) / 2, titleLabel.getBottom(),
        comboBoxBoundsWidth, comboBoxBoundsHeight / 2);

    comboBoxNoiseType.setBounds((width - comboBoxBoundsWidth) / 2, height - height / 4, 
        comboBoxBoundsWidth, comboBoxBoundsHeight);
}