#ifndef Header__SuperpoweredAnalyzer
#define Header__SuperpoweredAnalyzer

#define SUPERPOWERED_WAVEFORM_POINTS_PER_SEC 150

struct offlineAnalyzerInternals;

/**
 @brief Performs bpm and key detection, loudness/peak analysis. Provides compact waveform data (150 points/sec and 1 point/sec resolution), beatgrid information.
 */
class SuperpoweredOfflineAnalyzer {
public:

/**
 @param samplerate The sample rate of the source.
 @param bpm If you know the accurate bpm value in advance, set it here. 0 means the analyzer will detect bpm.
 @param lengthSeconds The source's length in seconds.
 */
    SuperpoweredOfflineAnalyzer(unsigned int samplerate, float bpm = 0, int lengthSeconds = 0);
    ~SuperpoweredOfflineAnalyzer();

/**
 @brief Processes a chunk of audio.

 @param input 32-bit interleaved floating-point input.
 @param numberOfSamples How many samples to process.
 */
    void process(float *input, unsigned int numberOfSamples);

/**
 @brief Get results. Call this method ONCE, after all samples are processed.
 
 @param averageWaveform 150 points/sec waveform data displaying the average volume. Each sample is an unsigned char from 0 to 255. You take ownership on this (must free()).
 @param peakWaveform 150 points/sec waveform data displaying the peak volume. Each sample is an unsigned char from 0 to 255. You take ownership on this (must free()).
 @param waveformSize The number of points in averageWaveform and peakWaveform.
 @param overviewWaveform 1 point/sec waveform data displaying the average volume in decibels. Useful for displaying the overall structure of a track. Each sample is a signed char, from -128 to 0 decibel. You take ownership on this (must free()).
 @param averageDecibel The average loudness of all samples processed in decibel.
 @param loudpartsAverageDecibel The average loudness of the "loud" parts in the music in decibel. (Breakdowns and other quiet parts are excluded.)
 @param peakDecibel The loudest sample in decibel.
 @param bpm Beats per minute.
 @param beatgridStartMs The position where the beatgrid should start. On input set it to 0, or the ms position of the first audio sample.
 @param keyIndex The dominant key (chord) of the music. 0..11 are major keys from A to G#, 12..23 are minor keys from A to G#. Check the static constants in this header for musical, Camelot and Open Key notations.
 */
    void getresults(unsigned char **averageWaveform, unsigned char **peakWaveform, int *waveformSize, char **overviewWaveform, float *averageDecibel, float *loudpartsAverageDecibel, float *peakDecibel, float *bpm, float *beatgridStartMs, int *keyIndex);

private:
    offlineAnalyzerInternals *internals;
    SuperpoweredOfflineAnalyzer(const SuperpoweredOfflineAnalyzer&);
    SuperpoweredOfflineAnalyzer& operator=(const SuperpoweredOfflineAnalyzer&);
};

/**
 @return Returns the frequency of a specific note.
 
 @param note The number of the note. Note 0 is the standard A note at 440 Hz.
 */
float frequencyOfNote(int note);

static const char *musicalChordNames[24] = {
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", // major
    "Am", "A#m", "Bm", "Cm", "C#m", "Dm", "D#m", "Em", "Fm", "F#m", "Gm", "G#m" // minor
};

static const char *camelotChordNames[24] = {
    "11B", "6B", "1B", "8B", "3B", "10B", "5B", "12B", "7B", "2B", "9B", "4B", // major
    "8A", "3A", "10A", "5A", "12A", "7A", "2A", "9A", "4A", "11A", "6A", "1A" // minor
};

static const char *openkeyChordNames[24] = {
    "4d", "11d", "6d", "1d", "8d", "3d", "10d", "5d", "12d", "7d", "2d", "9d", // major
    "1m", "8m", "3m", "10m", "5m", "12m", "7m", "2m", "9m", "4m", "11m", "6m" // minor
};

#endif
