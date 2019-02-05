#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>
#include <list>
#include <shared_mutex>

extern "C"
{
    struct AVFormatContext;
    struct AVCodecContext;
    struct AVStream;
    struct AVFrame;
}

extern "C"
{
#define SDL_MAIN_HANDLED
#include "sdl/include/SDL.h"
}

#include "vamp-hostsdk/PluginLoader.h"
#include "JobPool.h"
#include <wx/progdlg.h>

class AudioManager;

class xLightsVamp
{
	Vamp::HostExt::PluginLoader *_loader;
	std::map<std::string, Vamp::Plugin *> _plugins;
	std::vector<Vamp::Plugin *> _loadedPlugins;
	std::map<std::string, Vamp::Plugin *> _allplugins;
	std::vector<Vamp::Plugin *> _allloadedPlugins;
	void LoadPlugins(AudioManager* paudio);

public:

	struct PluginDetails
	{
	public:
		std::string Name;
		std::string Description;
	};

	xLightsVamp();
	~xLightsVamp();
    static void ProcessFeatures(Vamp::Plugin::FeatureList &feature, std::vector<int> &starts, std::vector<int> &ends, std::vector<std::string> &labels);
	std::list<std::string> GetAvailablePlugins(AudioManager* paudio);
	std::list<std::string> GetAllAvailablePlugins(AudioManager* paudio);
	Vamp::Plugin* GetPlugin(std::string name);
};

typedef enum FRAMEDATATYPE {
	FRAMEDATA_HIGH,
	FRAMEDATA_LOW,
	FRAMEDATA_SPREAD,
	FRAMEDATA_VU,
	FRAMEDATA_ISTIMINGMARK,
	FRAMEDATA_NOTES
} FRAMEDATATYPE;

typedef enum MEDIAPLAYINGSTATE {
	PLAYING,
	PAUSED,
	STOPPED
} MEDIAPLAYINGSTATE;

typedef void (__cdecl * AudioManagerProgressCallback) (wxProgressDialog* dlg, int pct);

typedef enum SDLSTATE {
    SDLUNINITIALISED,
    SDLINITIALISED,
    SDLOPENED,
    SDLPLAYING,
    SDLNOTPLAYING
} SDLSTATE;

class AudioData
{
    public:
        static int __nextId;
        int _id;
        long  _audio_len;
        Uint8* _audio_pos;
        Uint8* _original_pos;
        int _volume = SDL_MIX_MAXVOLUME;
        int _rate;
        long _original_len;
        long _savedpos;
        long _trackSize;
        long _lengthMS;
        bool _paused;
        AudioData();
        ~AudioData() {}
        long Tell() const;
        void Seek(long ms);
        void SavePos();
        void RestorePos();
        void SeekAndLimitPlayLength(long pos, long len);
        void Pause(bool pause) { _paused = pause; }
};

class SDL
{
    SDL_AudioDeviceID _dev;
    SDL_AudioDeviceID _inputdev;
    SDLSTATE _state;
    std::list<AudioData*> _audioData;
    std::mutex _audio_Lock;
    float _playbackrate;
    SDL_AudioSpec _wanted_spec;
    SDL_AudioSpec _wanted_inputspec;
    int _initialisedRate;
    std::string _device;
    std::string _inputDevice;
    int _listeners;

    void Reopen();
    AudioData* GetData(int id);

public:
    SDL(const std::string& device = "", const std::string& inputDevice = "");
    virtual ~SDL();
    std::list<AudioData*> GetAudio() const { return _audioData; }
    long Tell(int id);
    void Seek(int id, long ms);
    void SetRate(float rate);
    int AddAudio(long len, Uint8* buffer, int volume, int rate, long tracksize, long lengthMS);
    void RemoveAudio(int id);
    void Play();
    void Pause();
    void Unpause();
    void TogglePause();
    void Stop();
    void SetVolume(int id, int volume);
    int GetVolume(int id);
    static void SetGlobalVolume(int volume);
    static int GetGlobalVolume();
    void SeekAndLimitPlayLength(int id, long pos, long len);
    void Pause(int id, bool pause);
    bool HasAudio(int id);
    static std::list<std::string> GetAudioDevices();
    bool AudioDeviceChanged();
    bool OpenAudioDevice(const std::string device);
    bool OpenInputAudioDevice(const std::string device);
    void SetAudioDevice(const std::string device);
    bool CloseAudioDevice();
    bool CloseInputAudioDevice();
    int GetInputMax(int ms);
    void PurgeAllButInputAudio(int ms);
    std::list<float> GetInputSpectrum(int ms);
    void PurgeInput();
    void DumpState(std::string device, int devid, SDL_AudioSpec* wanted, SDL_AudioSpec* actual);
    void StartListening();
    void StopListening();
};

class AudioManager
{
	JobPool _jobPool;
	Job* _job;
    std::shared_timed_mutex _mutex;
    Job* _jobAudioLoad;
    std::shared_timed_mutex _mutexAudioLoad;
    long _loadedData;
    std::vector<std::vector<std::list<float>>> _frameData;
	std::string _audio_file;
	xLightsVamp _vamp;
	long _rate;
	int _channels;
    long _trackSize;
	int _bits;
	int _extra;
	std::string _resultMessage;
	int _state;
	float *_data[2]; // audio data
	Uint8* _pcmdata;
	long _pcmdatasize;
	std::string _title;
	std::string _artist;
	std::string _album;
	int _intervalMS;
	long _lengthMS;
	bool _frameDataPrepared;
	float _bigmax;
	float _bigspread;
	float _bigmin;
	float _bigspectogrammax;
	MEDIAPLAYINGSTATE _media_state;
	bool _polyphonicTranscriptionDone;
    int _sdlid;
    bool _ok;
    std::string _hash;

	void GetTrackMetrics(AVFormatContext* formatContext, AVCodecContext* codecContext, AVStream* audioStream);
	void LoadTrackData(AVFormatContext* formatContext, AVCodecContext* codecContext, AVStream* audioStream);
	void ExtractMP3Tags(AVFormatContext* formatContext);
	long CalcLengthMS() const;
	void SplitTrackDataAndNormalize(signed short* trackData, long trackSize, float* leftData, float* rightData) const;
    static void NormalizeMonoTrackData(signed short* trackData, long trackSize, float* leftData);
	int OpenMediaFile();
	void PrepareFrameData(bool separateThread);
    static int decodebitrateindex(int bitrateindex, int version, int layertype);
	int decodesamplerateindex(int samplerateindex, int version) const;
    static int decodesideinfosize(int version, int mono);
	std::list<float> CalculateSpectrumAnalysis(const float* in, int n, float& max, int id) const;
    void LoadAudioData(bool separateThread, AVFormatContext* formatContext, AVCodecContext* codecContext, AVStream* audioStream, AVFrame* frame);
    void SetLoadedData(long pos);

public:
    bool IsOk() const { return _ok; }
    static size_t GetAudioFileLength(std::string filename);
	void Seek(long pos) const;
	void Pause();
	void Play();
    void Play(long posms, long lenms);
    void Stop();
    void AbsoluteStop();
    long GetLoadedData();
    bool IsDataLoaded(long pos = -1);
    static void SetPlaybackRate(float rate);
	MEDIAPLAYINGSTATE GetPlayingState() const;
	long Tell() const;
	xLightsVamp* GetVamp() { return &_vamp; };
	AudioManager(const std::string& audio_file, int step = 4096, int block = 32768);
	~AudioManager();
	void SetVolume(int volume) const;
    int GetVolume() const;
    static void SetGlobalVolume(int volume);
    static int GetGlobalVolume();
    static void SetAudioDevice(const std::string device);
    static std::list<std::string> GetAudioDevices();
    long GetTrackSize() const { return _trackSize; };
	long GetRate() const { return _rate; };
	int GetChannels() const { return _channels; };
	int GetState() const { return _state; };
	std::string GetResultMessage() const { return _resultMessage; };
	std::string Title() const { return _title; };
	std::string Artist() const { return _artist; };
	std::string Album() const { return _album; };
	std::string FileName() const { return _audio_file; };
    std::string Hash();
	long LengthMS() const { return _lengthMS; };
	float GetRightData(long offset);
	float GetLeftData(long offset);
    void GetLeftDataMinMax(long start, long end, float& minimum, float& maximum);
	float* GetRightDataPtr(long offset);
	float* GetLeftDataPtr(long offset);
	void SetStepBlock(int step, int block);
	void SetFrameInterval(int intervalMS);
	int GetFrameInterval() const { return _intervalMS; }
	std::list<float>* GetFrameData(int frame, FRAMEDATATYPE fdt, std::string timing);
	std::list<float>* GetFrameData(FRAMEDATATYPE fdt, std::string timing, long ms);
	void DoPrepareFrameData();
	void DoPolyphonicTranscription(wxProgressDialog* dlg, AudioManagerProgressCallback progresscallback);
	bool IsPolyphonicTranscriptionDone() const { return _polyphonicTranscriptionDone; };
    void DoLoadAudioData(AVFormatContext* formatContext, AVCodecContext* codecContext, AVStream* audioStream, AVFrame* frame);
    
    static bool WriteAudioFrame(AVFormatContext *oc, AVStream *st, float *sampleBuff, int sampleCount, bool clearQueue = false);
    static bool CreateAudioFile(const std::vector<float>& left, const std::vector<float>& right, const std::string& targetFile, long bitrate);

    bool AudioDeviceChanged();

    static SDL* GetSDL();
};

#endif
