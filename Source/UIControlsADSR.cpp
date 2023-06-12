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

#include "UIControlsADSR.h"

UIControlsADSR::UIControlsADSR(juce::AudioProcessorValueTreeState& apvts)
    : sliderAttack(apvts, "attack", "labelText", "ATTACK", "typeADSR", "fontHeight", 12.0f, 0.001f),
    sliderDecay(apvts, "decay", "labelText", "DECAY", "typeADSR", "fontHeight", 12.0f, 0.0f),
    sliderSustain(apvts, "sustain", "labelText", "SUSTAIN", "typeADSRSustain", "fontHeight", 12.0f, 1.0f),
    sliderRelease(apvts, "release", "labelText", "RELEASE", "typeADSR", "fontHeight", 12.0f, 0.0f)
{
    // Background Colour of the panel
    setBackgroundColour(juce::Colour(0.0f, 0.0f, 0.95f, 1.0f));

    addAndMakeVisible(sliderAttack);
    addAndMakeVisible(sliderDecay);
    addAndMakeVisible(sliderSustain);
    addAndMakeVisible(sliderRelease);
}

UIControlsADSR::~UIControlsADSR()
{
}

void UIControlsADSR::resized()
{
    auto width = getWidth();
    auto height = getHeight();

    // Scale off parents bounds
    int sliderBoundsWidth = (int)(width * 0.2);
    int sliderBoundsHeight = (int)(height * 0.8);
    auto widthOffsetCurrent = (width - (sliderBoundsWidth * 4)) / 5;
    auto heightOffsetCurrent = (height - sliderBoundsHeight) / 2;

    // Initial bounds setting
    sliderAttack.setBounds(widthOffsetCurrent, heightOffsetCurrent, 
        sliderBoundsWidth, sliderBoundsHeight);
    sliderDecay.setBounds(widthOffsetCurrent * 2 + sliderBoundsWidth, heightOffsetCurrent, 
        sliderBoundsWidth, sliderBoundsHeight);
    sliderSustain.setBounds(widthOffsetCurrent * 3 + sliderBoundsWidth * 2, heightOffsetCurrent, 
        sliderBoundsWidth, sliderBoundsHeight);
    sliderRelease.setBounds(widthOffsetCurrent * 4 + sliderBoundsWidth * 3, heightOffsetCurrent, 
        sliderBoundsWidth, sliderBoundsHeight);

    // Need to call this, because resized() by slider object doesnt get called automatically when parent resizes,
    // also new bounds relative to parent object must be passed, so we scale off this objects width and height
    sliderAttack.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
    sliderDecay.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
    sliderSustain.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
    sliderRelease.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
}