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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <string> 

// A lot of functions in here are unused because I dont need them. 
// As this is an abstract class, these pure virtual functions still need to be implemented.
// See here for more details https://docs.juce.com/master/classAudioProcessor.html
NoisePluginAudioProcessor::NoisePluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), 
    // AudioProcessorValueTreeState constructor, manages entire state of the plugin
    apvts(*this, nullptr, juce::Identifier("apvts"), createParameterLayout()),
    // Init filter object
    filter()

#endif
{
    // Add sounds and voice to synth so it knows what sounds to create when playing midi notes
    // Automatically gets deleted by synthesizer class when no longer needed, so we don't need to delete them manually
    noiseSynth.addSound(new NoiseSynthSound());
    noiseSynth.addVoice(new NoiseSynthVoice());
}

// Destroy method, remove stuff from memory
NoisePluginAudioProcessor::~NoisePluginAudioProcessor() 
{
}

//==============================================================================
const juce::String NoisePluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoisePluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoisePluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NoisePluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NoisePluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoisePluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int NoisePluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoisePluginAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const juce::String NoisePluginAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void NoisePluginAudioProcessor::changeProgramName (int /*index*/, const juce::String& /*newName*/)
{
}

// Pre-playback initialisation
void NoisePluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Spec and Debug messages
    juce::String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlock = " << samplesPerBlock << "\n";
    message << " sampleRate = " << sampleRate << "\n";
    message << " numVoices = " << noiseSynth.getNumVoices();
    juce::Logger::getCurrentLogger()->writeToLog(message);

    auto numOutputChannels = getTotalNumOutputChannels();

    // Passes sample playback rate to synthesizer
    noiseSynth.setCurrentPlaybackSampleRate(sampleRate);

    // Prepare the synthesiser voice
    // We only have 1 Voice so the loop shouldnt be necessary here 
    // but lets leave it here in case we want to modify the numVoices later
    for (int i = 0; i < noiseSynth.getNumVoices(); i++)
    {
        // We need to cast the noiseSynth internal SynthesiserVoice to our own NoiseSynthVoice, so check if thats successful
        if (auto voice = dynamic_cast<NoiseSynthVoice*>(noiseSynth.getVoice(i)))
        {
            // Assign NoiseSynthVoice Pointer to Synthesiser
            voice->prepareToPlay(sampleRate, samplesPerBlock, numOutputChannels);
        }
    }

    // Prepare other modules that effect the sound
    filter.prepareToPlay(sampleRate, samplesPerBlock, numOutputChannels);

    filterNotes.prepareToPlay(sampleRate, samplesPerBlock, numOutputChannels);

    panner.prepareToPlay(sampleRate, samplesPerBlock, numOutputChannels);

    gain.prepareToPlay(apvts.getRawParameterValue("gain")->load());
}

void NoisePluginAudioProcessor::releaseResources()
{
    // When playback stops, this can be used to free up any spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoisePluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Check if the bus layout is supported. Some plugin hosts, such as certain GarageBand versions, 
    // will only load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Check if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// Actual sound processing, renders the next block every sample
void NoisePluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Disables Denormals. These are tiny floating point values close to zero, which can use up cpu time.
    // https://forum.juce.com/t/when-to-use-scopednodenormals-and-when-to-not/37112
    juce::ScopedNoDenormals noDenormals;

    // Clears any output data for all channels to avoid getting screaming feedback through leftover buffer data
    buffer.clear();

    // Update parameters for synthesiser voices and check if noiseSynth is correctly casting as NoiseSynthVoice
    for (int i = 0; i < noiseSynth.getNumVoices(); ++i) 
    {
        if (auto voice = dynamic_cast<NoiseSynthVoice*>(noiseSynth.getVoice(i)))
        {
            // Dereference voice parameters to get their current values. 
            // Parameter values come from their initially set values (done by this processor via creating
            // the parameter layout) and their individual UI / Slider counterparts via user input
            auto &attack = *apvts.getRawParameterValue("attack");
            auto &decay = *apvts.getRawParameterValue("decay");
            auto &sustain = *apvts.getRawParameterValue("sustain");
            auto &release = *apvts.getRawParameterValue("release");
            auto &noiseType = *apvts.getRawParameterValue("noiseType");
            auto &lfoFrequency = *apvts.getRawParameterValue("lfoFrequency");
            auto &lfoSwitch = *apvts.getRawParameterValue("lfoSwitch");

            // Pass parameters down to synthesiser
            voice->updateParameters(attack.load(), decay.load(), sustain.load(), release.load(), (int)noiseType.load(), lfoFrequency.load(), lfoSwitch.load());
        }
    }
    // Render the voice -> Play the noise
    // Midi is needed here so the synth knows when and what note is playing and the render should be triggered
    noiseSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Apply the high and lowpass filters to the audio buffer
    auto& filterCutoffHP = *apvts.getRawParameterValue("filterCutoffHP");
    auto& filterCutoffLP = *apvts.getRawParameterValue("filterCutoffLP");
    filter.updateParameters(filterCutoffHP, filterCutoffLP);
    filter.renderNextBlock(buffer);

    // Apply a lowpass filter that uses the frequency of the currently playing note as cutoff
    filterNotes.updateParameters((float)juce::MidiMessage::getMidiNoteInHertz(noiseSynth.getVoice(0)->getCurrentlyPlayingNote()));
    auto& filterNoteSwitch = *apvts.getRawParameterValue("filterNoteSwitch");
    if (filterNoteSwitch)
    {
        filterNotes.renderNextBlock(buffer);
    }

    // Apply pan
    auto& pan = *apvts.getRawParameterValue("panner");
    panner.updateParameters(pan);
    panner.renderNextBlock(buffer);

    // Apply gain
    auto& currentGain = *apvts.getRawParameterValue("gain");
    gain.updateParameters(currentGain);
    gain.renderNextBlock(buffer);
}

// When the host application bypasses the plugin, this processblockbypassed is executed.
// Bypassing means that the plugin is told to not output audio
void NoisePluginAudioProcessor::processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    // Free up audio buffer
    buffer.clear();
}

//==============================================================================
bool NoisePluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NoisePluginAudioProcessor::createEditor()
{
    return new NoisePluginAudioProcessorEditor (*this);
}

//==============================================================================
// Restore apvts state / parameters to memory block from xml
void NoisePluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

// Store apvts state / parameters from memory block
void NoisePluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// Create Parameter Layout of the plugin, used when initializing the apvts object. 
// Parameters could go in the initializer aswell, but we use this function to tidy things up
juce::AudioProcessorValueTreeState::ParameterLayout NoisePluginAudioProcessor::createParameterLayout()
{
    // Vector of Pointers of all Parameters in the plugin
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // Add each Parameter to the end of the vector with their default values
    // Arguments for quick reference: "parameter id", "parameter name", range, default value
    // Range reference: https://docs.juce.com/master/classNormalisableRange.html
    
    // Gain
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 
        juce::NormalisableRange<float>(0.0f, 0.5f, 0.005f), 0.4f));

    // Panner
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("panner", "Pan",
        juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f), 0.0f));

    // ADSR for synthesiser voice
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("attack", "Attack", 
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.2f), 0.001f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.2f), 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", 
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("release", "Release", 
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.2f), 0.0f));
            
    // Noise Type of synthesiser
    parameters.push_back(std::make_unique < juce::AudioParameterChoice>("noiseType", "Noise Type",
        juce::StringArray{ "WHITE NOISE", "PINK NOISE", "COMB-ISH NOISE" }, 0));

    // Filters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("filterCutoffHP", "Highpass Cutoff",
        juce::NormalisableRange<float>(10.0f, 22000.0f, 1.0f, 0.3f), 10.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("filterCutoffLP", "Lowpass Cutoff",
        juce::NormalisableRange<float>(10.0f, 22000.0f, 1.0f, 0.3f), 22000.0f));

    // LFO for synthesiser voice
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("lfoFrequency", "LFO Frequency",
        juce::NormalisableRange<float>(0.0f, 5.0f, 0.001f, 0.7f), 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("lfoSwitch", "LFO On / Off", false));

    // Filter for synthesiser voice notes
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("filterNoteSwitch", "Filter for Midi Scaling On / Off", false));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
// This is the starting point - creates an instance of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoisePluginAudioProcessor();
}
