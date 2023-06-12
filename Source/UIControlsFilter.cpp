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

#include "UIControlsFilter.h"

UIControlsFilter::UIControlsFilter(juce::AudioProcessorValueTreeState& apvts)
    : sliderFilterHP(apvts, "filterCutoffHP", "labelText", "HPF", "typeLFO", "fontHeight", 12.0f, 0.0f),
    sliderFilterLP(apvts, "filterCutoffLP", "labelText", "LPF", "typeLFO", "fontHeight", 12.0f, 0.0f)
{
    setBackgroundColour(juce::Colour(0.0f, 0.0f, 0.95f, 1.0f));

    // Filter knobs
    addAndMakeVisible(sliderFilterHP);
    addAndMakeVisible(sliderFilterLP);

    // Keytrack Filter on off button
    attachmentNoteSwitch = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts,
        "filterNoteSwitch", buttonNoteSwitch);
    buttonNoteSwitch.setClickingTogglesState(true);
    auto& lfoState = *apvts.getRawParameterValue("filterNoteSwitch");
    buttonNoteStateText = lfoState ? "KEYTRACK ON" : "KEYTRACK OFF";
    buttonNoteSwitch.setButtonText(buttonNoteStateText);
    buttonNoteSwitch.onClick = [this] { updateToggleState(&buttonNoteSwitch); };
    addAndMakeVisible(buttonNoteSwitch);
    
    // Rectangles to make the button look like a piano
    juce::Colour keytrackNotesColour = lfoState ? juce::Colours::white : juce::Colours::black;
    for (auto& rectangle : rectangles) {
        rectangle.setFill(keytrackNotesColour);
        addAndMakeVisible(rectangle);
    }
}

UIControlsFilter::~UIControlsFilter()
{
}

void UIControlsFilter::resized()
{
    auto width = getWidth();
    auto height = getHeight();

    // Scale off parents bounds
    int sliderBoundsWidth = (int)(width * 0.4f);
    int sliderBoundsHeight = (int)(height * 0.4f);
    auto widthOffsetCurrent = (width - (sliderBoundsWidth * 2)) / 3;

    // Initial bounds setting for knobs and button
    sliderFilterHP.setBounds(widthOffsetCurrent, widthOffsetCurrent,
        sliderBoundsWidth, sliderBoundsHeight);

    sliderFilterLP.setBounds(widthOffsetCurrent * 2 + sliderBoundsWidth, widthOffsetCurrent,
        sliderBoundsWidth, sliderBoundsHeight);

    buttonNoteSwitch.setBounds(widthOffsetCurrent, sliderFilterHP.getBottom() + widthOffsetCurrent,
        sliderFilterLP.getRight() - widthOffsetCurrent, sliderBoundsHeight);

    // Rectangles for button piano look
    auto rectangleBounds = buttonNoteSwitch.getBounds();
    auto interval = rectangleBounds.getWidth() / 8;
    auto rectLength = 12;
    auto offsetButtonHeight = 7;
    auto rectHeight = 12;

    for (int i = 2; i <= 7; ++i) {
        juce::Point<float> startPoint((float)(interval * i), (float)rectangleBounds.getY() + offsetButtonHeight);
        juce::Point<float> endPoint((float)(interval * i) + rectLength, (float)rectangleBounds.getY() + offsetButtonHeight);
        juce::Point<float> controlPoint((float)(interval * i), (float)rectangleBounds.getY() + rectHeight);

        // Cases are intervals of the rectangles in a piano like order..
        switch (i) {
        case 2:
            rectangles[0].setRectangle(juce::Parallelogram<float>(startPoint, endPoint, controlPoint));
            break;
        case 3:
            rectangles[1].setRectangle(juce::Parallelogram<float>(startPoint, endPoint, controlPoint));
            break;
        case 5:
            rectangles[2].setRectangle(juce::Parallelogram<float>(startPoint, endPoint, controlPoint));
            break;
        case 6:
            rectangles[3].setRectangle(juce::Parallelogram<float>(startPoint, endPoint, controlPoint));
            break;
        case 7:
            rectangles[4].setRectangle(juce::Parallelogram<float>(startPoint, endPoint, controlPoint));
            break;
        default:
            break;
        }
    }

    // Need to call this, because resized() by slider object doesnt get called automatically when parent resizes,
    // also new bounds relative to parent object must be passed, so we scale off this objects width and height
    sliderFilterHP.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
    sliderFilterLP.updateBounds(sliderBoundsWidth, sliderBoundsHeight);
}

// Checks the state of the parameter and changes button text accordingly
void UIControlsFilter::updateToggleState(juce::Button* button)
{
    auto state = button->getToggleState();
    juce::String stateString = state ? "KEYTRACK ON" : "KEYTRACK OFF";
    button->setButtonText(stateString);

    // Keytrack rect colour change
    juce::Colour keytrackNotesColour = state ? juce::Colours::white : juce::Colours::black;
    for (auto& rectangle : rectangles) {
        rectangle.setFill(keytrackNotesColour);
    }
}