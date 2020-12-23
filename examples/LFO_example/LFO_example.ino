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
// Parameters:
// *) t_lfo_param_delta = duration for demo of changing waveform (in µs)
// *) lfo_waveform = mode 0 -> DC/off, mode 1 -> saw, mode 2 -> triangle, mode 3 -> sin, mode 4 -> square [0,4]
// *) lfo_ampl = amplitude, scales from 0 to _dacSize - 1
// *) lfo_ampl_offset = amplitude offset, scales from 0 to _dacSize - 1
// *) lfo_freq_sync = sync mode: false -> free, true -> synced to track
// *) lfo_freq_mode0_free = frequency in Hz
// *) lfo_freq_mode1_bpm = BPM of track
// *) lfo_freq_mode1_rate = Rate to link to BPM of track
// *) lfo_freq_mode1_phase = phase - scales from 0 to 1
//
// --------------------------------------------------


#include <lfo.h>        // required for function generation

#define DACSIZE 4096             // vertical resolution of the DACs

// variables
int             lfo_waveform = 1;                   // saw wave (to start out with)
int             lfo_ampl = DACSIZE-1;               // amplitude -> from 0 to DACSIZE-1
int             lfo_ampl_offset = 0;                // sustain level -> from 0 to DACSIZE-1
bool            lfo_freq_sync = false;              // sync mode: false -> free, true -> synced to track
float           lfo_freq_mode0_free = 30;           // frequency in Hz
float           lfo_freq_mode1_bpm = 120;           // BPM of track
int             lfo_freq_mode1_rate = 15;           // Rate to link to BPM of track
float           lfo_freq_mode1_phase = 0;           // phase - scales from 0 to 1

// internal variables
unsigned long   t = 0;                              // timestamp: current time
unsigned long   t_lfo_param_delta = 3000000;        // time in µs to changed lfo parameters
unsigned long   t_lfo_param_0 = 0;                  // timestamp: last time parameter change

// internal classes
lfo             lfo_class(DACSIZE);

void setup() {
  delay(100);
  analogWriteResolution(12);                        // set the analog output resolution to 12 bit (4096 levels) -> ARDUINO DUE ONLY

  lfo_class.setWaveForm(lfo_waveform);              // initialize waveform
  lfo_class.setAmpl(lfo_ampl);                      // initialize amplitude
  lfo_class.setAmplOffset(lfo_ampl_offset);         // initialize amplitude offset
  lfo_class.setFreqSync(lfo_freq_sync);             // initialize sync mode
  lfo_class.setFreqMode0Free(lfo_freq_mode0_free);  // initialize Hz in free-running mode
  lfo_class.setFreqMode1Bpm(lfo_freq_mode1_bpm);    // initialize bpm in sync mode
  lfo_class.setFreqMode1Rate(lfo_freq_mode1_rate);  // initialize rate in sync mode -> see _freq_array in lfo.h
  lfo_class.setFreqMode1Phase(lfo_freq_mode1_phase);// initialize phase
}

void loop() {
  t = micros();                                     // take timestamp

  analogWrite(DAC0, lfo_class.getWave(t));          // update ADSR and write to DAC
  
  if (t > t_lfo_param_0 + t_lfo_param_delta) {            // check if lfo parameters are to be changed
    int l_waveform = lfo_class.getWaveForm();  
    l_waveform++;
    if (l_waveform > 4)
      l_waveform = 0;
    lfo_class.setWaveForm(l_waveform);
    t_lfo_param_0 = t;
  }
}
