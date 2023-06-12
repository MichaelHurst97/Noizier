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

// Create a look and feel that we can apply to all elements instead of styling each element individually
// Separated from PluginEditor to tidy things up
// https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Global l&f setup and colours for elements if not custom made via a method below
        LookAndFeel::setDefaultLookAndFeel(this);

        setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
        setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
        setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colours::white);

        setColour(juce::Label::textColourId, juce::Colours::black);
        setColour(juce::Label::backgroundColourId, juce::Colours::white);
        setColour(juce::Label::outlineColourId, juce::Colours::black);

        setColour(juce::HyperlinkButton::textColourId, juce::Colours::grey);

        setColour(juce::ComboBox::backgroundColourId, juce::Colours::white);
        setColour(juce::ComboBox::outlineColourId, juce::Colours::black);
        setColour(juce::ComboBox::textColourId, juce::Colours::black);
        setColour(juce::ComboBox::buttonColourId, juce::Colours::white);
        setColour(juce::ComboBox::arrowColourId, juce::Colours::black);
        setColour(juce::ComboBox::focusedOutlineColourId, juce::Colours::black);

        setColour(juce::PopupMenu::backgroundColourId, juce::Colours::white);
        setColour(juce::PopupMenu::textColourId, juce::Colours::black);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::black);
        setColour(juce::PopupMenu::highlightedTextColourId, juce::Colours::white);
        setColour(juce::PopupMenu::headerTextColourId, juce::Colours::black);

        setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        setColour(juce::TextButton::buttonOnColourId, juce::Colours::black);
        setColour(juce::TextButton::textColourOffId, juce::Colours::black);
        setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    }

    // Custom font
    // https://forum.juce.com/t/guide-juce-font-embedding-2019/35041
    static const juce::Font getCustomFont()
    {
        // Open Source font Roboto from Google Fonts
        // https://fonts.google.com/specimen/Roboto
        static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoBlackItalic_ttf, BinaryData::RobotoBlackItalic_ttfSize);
        return juce::Font(typeface);
    }

    // Globally change/inject custom font
    juce::Typeface::Ptr getTypefaceForFont(const juce::Font& /*f*/) override
    {
        return getCustomFont().getTypefacePtr();
    }

    // Style Rotary Sliders
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - lineThickness;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        
        // Add a dropshadow behind the slider
        auto dropShadow = juce::DropShadow(juce::Colours::black, 5, juce::Point(0, 0));
        juce::Path dropShadowPath;
        dropShadowPath.addEllipse(rx, ry, rw, rw);
        dropShadow.drawForPath(g, dropShadowPath);

        // Fill the slider with a colour
        g.setColour(juce::Colours::white);
        g.fillEllipse(rx, ry, rw, rw);

        // Outline
        g.setColour(juce::Colours::black);
        g.drawEllipse(rx, ry, rw, rw, lineThickness);

        // Dial pointer line via path
        juce::Path p;
        auto pointerLength = radius * 0.3f;
        auto pointerThickness = lineThickness * 2;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        g.setColour(juce::Colours::black);
        g.fillPath(p);

        // Label text and value inside the slider
        //https://forum.juce.com/t/tip-how-to-include-value-and-label-on-rotaryknob-without-declaring-a-label/35926/17
        g.setFont(juce::Font(getCustomFont().withHeight(slider.getProperties()["fontHeight"])));

        juce::String labelText = slider.getProperties()["labelText"];
        juce::String labelType = slider.getProperties()["labelType"];

        float value = (float)round((float)slider.getValue() * 10000) / 100;
        juce::String text;
        if (labelType == "typeGain") {
            text = labelText + juce::newLine + juce::String(roundf((value / 50) * 100)) + " %";
        }
        else if (labelType == "typeADSR") {
            text = labelText + juce::newLine + juce::String(value * 10) + " ms";
        }
        else if (labelType == "typeADSRSustain") {
            text = labelText + juce::newLine + juce::String(value) + " %";
        }
        else if (labelType == "typePan") {
            text = labelText + juce::newLine + juce::String(value);
        }
        else if (labelType == "typeLFO") {
            text = labelText + juce::newLine + juce::String(value / 100) + " Hz";
        }

        g.drawFittedText(text, slider.getLocalBounds(), juce::Justification::centred, true);
    }

    // Noise Type Font
    juce::Font getComboBoxFont(juce::ComboBox& /*box*/)
    {
        return juce::Font(getCustomFont().withHeight(14.0f));
    }

    // Noise Type Dropdown Menu Font
    juce::Font getPopupMenuFont()
    {
        return juce::Font(getCustomFont().withHeight(14.0f));
    }

    // Style Noise Type Selector
    void drawComboBox(juce::Graphics& g, int width, int height, bool /*isButtonDown*/, int buttonX,
        int buttonY, int buttonW, int buttonH, juce::ComboBox& b)
    {
        g.setFont(juce::Font(14.0f, juce::Font::bold));

        g.setColour(juce::Colours::white);
        g.fillRect(0, 0, width, height);
        g.setColour(b.findColour(juce::ComboBox::textColourId));
        g.drawRect(b.getLocalBounds(), (int)lineThickness);

        float spacingWidth = buttonW / 4.0f;
        float spacingHeight = buttonH / 3.0f;
        float offsetWidth = 1.0f;

        juce::Path p;
        p.addTriangle((buttonX + buttonW / 2) - offsetWidth, buttonY + spacingHeight * 2.2f, // middle path
            buttonX + buttonW - spacingWidth - offsetWidth, buttonY + spacingHeight, // right 
            buttonX + spacingWidth - offsetWidth, buttonY + spacingHeight); // left

        g.setColour(b.findColour(juce::ComboBox::textColourId));
        g.fillPath(p);
    }

    // Style Toggle buttons
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
    {
        auto bounds = button.getLocalBounds().reduced(4, 4);
        auto baseColour = backgroundColour;

        auto dropShadow = juce::DropShadow(juce::Colours::black, 4, juce::Point(0, 0));
        juce::Path dropShadowPath;
        dropShadowPath.addRectangle(bounds);
        dropShadow.drawForPath(g, dropShadowPath);

        g.setColour(baseColour);

        g.fillRect(bounds);
        g.setColour(button.findColour(juce::ComboBox::outlineColourId));
        g.drawRect(bounds, (int)lineThickness);
    }

private:
    static constexpr float lineThickness = 3;
};
