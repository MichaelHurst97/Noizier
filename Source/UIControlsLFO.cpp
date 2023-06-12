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

#include "UIControlsLFO.h"

UIControlsLFO::UIControlsLFO(juce::AudioProcessorValueTreeState& apvts)
    : sliderLFO(apvts, "lfoFrequency", "labelText", "FREQ", "typeLFO", "fontHeight", 12.0f, 0.0f)
{
    setBackgroundColour(juce::Colour(0.0f, 0.0f, 0.9f, 1.0f));

    addAndMakeVisible(sliderLFO);

    attachmentLFOSwitch = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts,
        "lfoSwitch", buttonLFOSwitch);
    buttonLFOSwitch.setClickingTogglesState(true);
    auto& lfoState = *apvts.getRawParameterValue("lfoSwitch");
    buttonLFOStateText = lfoState ? "LFO ON" : "LFO OFF";
    buttonLFOSwitch.setButtonText(buttonLFOStateText);
    buttonLFOSwitch.onClick = [this] { updateToggleState(&buttonLFOSwitch); };
    addAndMakeVisible(buttonLFOSwitch);
}

UIControlsLFO::~UIControlsLFO()
{
}

void UIControlsLFO::resized()
{
    auto width = getWidth();
    auto height = getHeight();

    // Scale off parents bounds
    int sliderBoundsWidth = (int)(width * 0.4);
    int sliderBoundsHeight = (int)(height * 0.8);
    auto widthOffsetCurrent = (width - (sliderBoundsWidth * 2)) / 3;
    auto heightOffsetCurrent = (height - sliderBoundsHeight) / 2;

    // Initial bounds setting
    buttonLFOSwitch.setBounds(widthOffsetCurrent, heightOffsetCurrent,
        sliderBoundsWidth, sliderBoundsHeight);
    sliderLFO.setBounds(widthOffsetCurrent * 2 + sliderBoundsWidth, heightOffsetCurrent,
        sliderBoundsWidth, sliderBoundsHeight);

    // Need to call this, because resized() by slider object doesnt get called automatically when parent resizes,
    // also new bounds relative to parent object must be passed, so we scale off this objects width and height
    sliderLFO.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
}

void UIControlsLFO::updateToggleState(juce::Button* button)
{
    auto state = button->getToggleState();
    juce::String stateString = state ? "LFO ON" : "LFO OFF";

    button->setButtonText(stateString);
}