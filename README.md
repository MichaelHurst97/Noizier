
# Noizier
## A Simple Noise Generator

![Noizier Screenshot](https://github.com/MichaelHurst97/Noizier/blob/main/Assets/Noizier_Screenshot.png?raw=true)
Noizier is mainly created to generate noise sounds quick and easily with a selected range of modulators to shape the sound.


How to install
======
**Windows:** Use the installer or copy Noizier.vst3 to *C:\Program Files\Common Files\VST3*
**Mac:** Copy Noizier.vst3 to *Library/Audio/Plug-ins/VST3*
**Linux:** Copy Noizier.lv2 folder to your LV2 Plugins Folder


How to use
======
**Play Midi notes** to generate the sound.

**Noise type:** Can be changed by using the dropdown menu.
Currently there are 3 Noise types:
+ White Noise
+ Pink Noise
+ Comb-ish Noise (Sounds a bit like applying a comb filter, hence the name)

**ADSR Panel:** Change the envelope of the Notes
**LFO Button:** Turn on LFO Amplitude Modulation and change the Frequency with the dial
**HPF and LPF dials:** Can be used to apply a highpass and lowpass filter (fixed 36db per octave slope each)
**Keytrack button:** Enables a fake pitch mode. Here, a lowpass filter is applied to the signal depending on the note you are playing


Sources
======
The code for this project has code adaptions based on the JUCE Documentation and Tutorials (https://docs.juce.com/master/index.html) and Joshua Hodge aka The Audio Programmer (https://github.com/TheAudioProgrammer/)



License
======
Noizier is released under the GNU General Public Licence v3 or later (GPL-3.0-or-later). 
The license is found in the "LICENSE" file in the root of this repository, or at
https://www.gnu.org/licenses/gpl-3.0.en.html
