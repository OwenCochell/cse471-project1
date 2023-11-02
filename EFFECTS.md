# Effect 
Author: Mateja Milicevic Score: scores/effect.score

This file describes the implementation of effects. The following effects have been implemented:

* Chorus
* Noise Gate
* DRC
* Flanger
* Reverb
* Ring Modulator

## Effect Send

Effects are implemented via effect sends in the score file. First, an effect must be added to the score.
These effects are added under the `effect` tag, and must be defined in the root XML node:

```xml
<effects>
      <effect name="delay" maxdelay="80000"/>
      <effect name="ringmod" freq="440.0" samp="44100"/>
      <effect name="reverb" samp="44100"/>
      <effect name="flanger" samp="44100"/>
      <effect name="chorus" samp="44100"/>
      <effect name="noise" threshold="0.5"/>
      <effect name="drc" threshhold="0.5" ratio="0.5" attack="0.5" release="0.5" knee="0.5"/>
</effects>
```
Here, each effect is defined, and the configuration parameters are set.
To send instrument audio to an effect, you can use the `effect` tag under each instrument:

```xml
<effect which="noise" wet="0.5" dry="0.25"/>
```

`which` is the name of the effect to send to. The parameters `wet` is the amplitude of the audio exiting the effect,
and the `dry` parameter is the amplitude of instrument audio entering the effect.
The effect audio is then mixed together with the other effects.
Multiple effects can be provided, and the effects will apply to all notes under the specified instrument.

Instruments can also supply the `none` effect, which applies the `wet` and `dry`
amplitude changes, but otherwise does not change the audio.
If no effects are provided, then the sequencer will automatically apply the `none` effect with `wet` and `dry`
at `1`, effectively leaving the audio unchanged.

## Chorus

Audio effect class which includes methods to configure the chorus settings, generate a delayed signal based on a Low Frequency Oscillator (LFO), process an input audio frame to apply the chorus effect, and initialize settings from an XML node. 

## Noise Gate

Processing an input audio frame through the noise gate, adjusting the gate value based on a threshold, and initializing settings from an XML node.

## DRC

Dynamic Range Compression is used to reduce the volume of loud sounds or amplify quiet sounds in audio processing. The class has methods to apply compression to an input audio frame (Compress), process an input frame (Process), and initialize settings from an XML node (FromXML).

## Flanger 

The process method implements the core Flanger effect. It calculates a delay time based on a sinusoidal function of a phase variable, which is then used to obtain a delayed version of the input signal via the ``SimpleDelay`` object. The method returns an average of the input and delayed signals, achieving the Flanger effect.

## Reverb 

The `` process `` method first accumulates the output of all ``CombFilter`` objects by feeding the input signal through them and summing their outputs. This summed output is then sequentially processed through two ``AllPassFilter`` objects. The output of the last ``AllPassFilter`` is returned as the processed signal, representing the reverberated sound.

## Ring Modulator

Calculates a sinusoidal modulator signal based on the current phase, updates the phase for the next sample, and ensures the phase wraps within the range [0, 2π]. The method returns the product of the input signal and the sinusoidal modulator signal, thereby achieving the ring modulation effect.