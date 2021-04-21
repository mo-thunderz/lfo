// --------------------------------------------------
//
// LFO - based on micros() function - Arduino Due
// by mo_thunderz (mo-thunderz @github)
//
// this example generates an LFO signal and periodically changes
// the frequency and waveform. The code is optimized for the
// Arduino Due. For use on other boards just reduce the
// DACSIZE, remove the analogWriteResolution(12) statement and write
// to a different port in this statement:
// analogWrite(DAC0, lfo_class.getWave(t));
//
// Parameters: please refer to lfo.h
 //
// --------------------------------------------------


#include <lfo.h>        // required for function generation

#define DACSIZE 4096             // vertical resolution of the DACs

// variables
int             lfo_waveform = 1;                   // saw wave (to start out with)

// internal variables
unsigned long   t = 0;                              // timestamp: current time
unsigned long   t_lfo_param_delta = 3000000;        // time in µs to changed lfo parameters (needed only to periodically change waveform)
unsigned long   t_lfo_param_0 = 0;                  // timestamp: last time parameter change (needed only to periodically change waveform)

// internal classes
lfo             lfo_class(DACSIZE);

void setup() {
  delay(100);
  analogWriteResolution(12);                        // set the analog output resolution to 12 bit (4096 levels) -> ARDUINO DUE ONLY

  lfo_class.setWaveForm(lfo_waveform);              // initialize waveform
  lfo_class.setAmpl(DACSIZE-1);                     // set amplitude to maximum
  lfo_class.setAmplOffset(0);                       // no offset to the amplitude
  lfo_class.setMode(0);                             // set sync mode to mode0 -> no sync to BPM
  lfo_class.setMode0Freq(30);                       // set LFO to 30 Hz
}

void loop() {
  t = micros();                                     // take timestamp

  analogWrite(DAC0, lfo_class.getWave(t));          // update LFO and write to DAC
  
  if (t > t_lfo_param_0 + t_lfo_param_delta) {            // check if lfo parameters are to be changed
    lfo_waveform++;
    if (lfo_waveform > 4)
      lfo_waveform = 1;
    lfo_class.setWaveForm(lfo_waveform);
    t_lfo_param_0 = t;
  }
}
