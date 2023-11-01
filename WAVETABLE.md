# Wavetable

Author: Owen Cochell
Score: scores/wavetable.score

This file describes the implementation of the Wavetable component.
We have implemented the following features:

- Wave playback on demand
- Envelope Generation
- Pitch
- Loop Points
- Glissando

## Wave playback on demand

The wavetable has the ability to playback samples at arbitrary pitches.
before this can be done, a sample must be loaded in the score file like so:

```xml
<samples>
    <sample name="test" path="..\01-DXPiano_F2mono.wav"/>
</samples>
```

When defined, the sequencer will load the sample under the specified name.
This sample will be shared between all components, so it is not necessary
to waste time and memory if components use the same sample.

Once the sample is loaded, the instrument can be defined like so:

```xml
<instrument instrument="WaveTable" sample="test" start="8830" stop="-1" base="F2">
```

Where `sample` is the name of the sample that was loaded earlier.
It is also necessary to specify the base pitch under the `base` attribute.

In our example score, we use a electronic piano sample provided on the class website.

## Envelope Generation

This component uses an ADSR envelope that is shared between components.

## Pitch

When we encounter a note, we can pitch the output to match the pitch of the given note.
We use linear interpolation to achieve this goal.

## Loop Points

Loop points are defined in the score file.
The `start` value determines the sample that start the sustain wave.
We play the attack portion of the wave, but when we reach the end of the wavetable
and need to repeat, we loop back to the sample defined in the score.

## Glissando

The wavetable implements glissando by using the `FreqManager` class,
which automatically handles pitch ramps to new values.
When the note is loaded in the wavetable,
it is passed to the `FreqManager`,
which automatically determines if the note will be unchanging,
or if the note will ramp to a value.
The wavetable can then retrieve pitch values from the `FreqManager`
over it's lifetime.
Notes in the score file can define a `gliss` attribute:

```xml
<note measure="8" beat="1" duration="9" note="D4" gliss="D5"/>
```

Over the course of the note's duration, the pitch will gliss up
to the pitch defined in the `gliss` attribute.
