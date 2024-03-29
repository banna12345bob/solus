#pragma once

#include <libnyquist/Decoders.h>

#include <rtaudio/RtAudio.h>

#define SU_AUDIO

namespace Solus {

	struct AudioDeviceInfo
	{
		uint32_t id;
		uint32_t numChannels;
		uint32_t sampleRate;
		uint32_t frameSize;
		bool isPlaying = false;
	};

	static const uint32_t FRAME_SIZE = 512;
	static const int32_t CHANNELS = 2;
	static const int32_t BUFFER_LENGTH = FRAME_SIZE * CHANNELS;

	class AudioDevice
	{
		Scope<RtAudio> rtaudio;
	public:
		AudioDeviceInfo info;
		AudioDevice(int numChannels, int sampleRate, int deviceId = -1);
		~AudioDevice();
		static int ListAudioDevices();
		bool Open(const int deviceId);
		bool Play(const std::vector<float>& data);
	};

	class audioPlayer
	{
	public:
		audioPlayer(uint32_t desiredSampleRate = 44100) {
			std::thread audioWorker(&audioPlayer::threadAudioPlayer, this, desiredSampleRate);
			audioWorker.detach();
		}
		~audioPlayer() = default;

		void threadAudioPlayer(uint32_t desiredSampleRate);
		void Play(std::string filePath);
		
		void PlayThreaded(std::string filePath)
		{
			SU_PROFILE_FUNCTION();
#ifdef SU_AUDIO
			std::thread audioWorker(&audioPlayer::Play, this, filePath);
			audioWorker.detach();
#else
			SU_CORE_WARN("Audio Player not enabled. Enable with compile flag SU_AUDIO");
			return;
#endif
		}
	private:
		nqr::NyquistIO m_Loader;
		Ref<nqr::AudioData> m_FileData;
		Ref<AudioDevice> m_MyDevice;
	};

}