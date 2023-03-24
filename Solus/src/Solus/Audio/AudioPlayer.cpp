#include "supch.h"

#include "AudioPlayer.h"
#include "RingBuffer.h"

#include <libnyquist/Common.h>

namespace Solus {
	void audioPlayer::threadAudioPlayer(uint32_t desiredSampleRate)
	{
		SU_PROFILE_FUNCTION();
#ifdef SU_AUDIO
		m_MyDevice = CreateRef<AudioDevice>(CHANNELS, desiredSampleRate);
		m_MyDevice->Open(m_MyDevice->info.id);
		m_FileData = CreateRef<nqr::AudioData>();
#else
		return;
#endif
	}

	// Implimented for libnyquist example
	void audioPlayer::Play(std::string filePath)
	{
		SU_PROFILE_FUNCTION();
#ifdef SU_AUDIO
		try {
			auto memory = nqr::ReadFile(filePath);
			std::string extension = filePath.substr(filePath.find(".") + 1);
			m_Loader.Load(m_FileData.get(), extension, memory.buffer);

			if (m_FileData->channelCount == 1)
			{
				SU_CORE_INFO("Playing MONO for: {0} seconds...", m_FileData->lengthSeconds);
				std::vector<float> stereoCopy(m_FileData->samples.size() * 2);
				nqr::MonoToStereo(m_FileData->samples.data(), stereoCopy.data(), m_FileData->samples.size());
				m_MyDevice->Play(stereoCopy);
			}
			else
			{
				SU_CORE_INFO("Playing STEREO for: {0} seconds...", m_FileData->lengthSeconds);
				m_MyDevice->Play(m_FileData->samples);
			}
		}
		catch (nqr::UnsupportedExtensionEx)
		{
			SU_CORE_ASSERT(false, "UNSUPPORTED FILE TYPE!");
		}
		catch (nqr::LoadPathNotImplEx)
		{
			SU_CORE_ASSERT(false, "Loading from path not implemented!");
		}
		catch (nqr::LoadBufferNotImplEx)
		{
			SU_CORE_ASSERT(false, "Loading from buffer not implemented!");
		}
		catch (const std::exception& e)
		{
			SU_CORE_FATAL("Caught: {0}", e.what());
		}
#else
		SU_CORE_WARN("Audio Player not enabled. Enable with compile flag SU_AUDIO");
		return;
#endif
	}

	static RingBufferT<float> buffer(BUFFER_LENGTH);

	static int rt_callback(void* output_buffer, void* input_buffer, uint32_t num_bufferframes, double stream_time, RtAudioStreamStatus status, void* user_data)
	{
		SU_PROFILE_FUNCTION();
		if (status)	SU_CORE_WARN("[rtaudio] buffer over or underflow");

		// Playback
		if (buffer.getAvailableRead()) buffer.read((float*)output_buffer, BUFFER_LENGTH); 
		else memset(output_buffer, 0, BUFFER_LENGTH * sizeof(float));

		return 0;
	}

	AudioDevice::AudioDevice(int numChannels, int sampleRate, int deviceId)
	{
		SU_PROFILE_FUNCTION();
		rtaudio = Scope<RtAudio>(new RtAudio);
		info.id = (deviceId != -1) ? deviceId : rtaudio->getDefaultOutputDevice();
		info.numChannels = numChannels;
		info.sampleRate = sampleRate;
		info.frameSize = FRAME_SIZE;
	}

	AudioDevice::~AudioDevice()
	{
		SU_PROFILE_FUNCTION();
		if (rtaudio)
		{
			rtaudio->stopStream();
			if (rtaudio->isStreamOpen())
			{
				rtaudio->closeStream();
			}
		}
	}

	bool AudioDevice::Open(const int deviceId)
	{
		SU_PROFILE_FUNCTION();
		SU_CORE_ASSERT(rtaudio, "rtaudio not created yet");

		RtAudio::StreamParameters outputParams;
		outputParams.deviceId = info.id;
		outputParams.nChannels = info.numChannels;
		outputParams.firstChannel = 0;

		RtAudio::StreamParameters inputParams;
		inputParams.deviceId = rtaudio->getDefaultInputDevice();
		inputParams.nChannels = 2;
		inputParams.firstChannel = 0;
		
		rtaudio->openStream(&outputParams, &inputParams, RTAUDIO_FLOAT32, info.sampleRate, &info.frameSize, &rt_callback, (void*)&buffer);

		if (rtaudio->isStreamOpen())
		{
			rtaudio->startStream();
			return true;
		}
		return false;
	}

	int AudioDevice::ListAudioDevices()
	{
		SU_PROFILE_FUNCTION();
		Ref<RtAudio> tempDevice(new RtAudio);

		RtAudio::DeviceInfo info;
		uint32_t devices = tempDevice->getDeviceCount();

		SU_CORE_INFO("[rtaudio] Found: {0} device(s)", devices);

		for (uint32_t i = 0; i < devices; ++i)
		{
			info = tempDevice->getDeviceInfo(i);
			SU_CORE_INFO("Device: {0} - {1}", i, info.name);
		}
		return devices;
	}

	bool AudioDevice::Play(const std::vector<float>& data)
	{
		SU_PROFILE_FUNCTION();
		SU_CORE_ASSERT(rtaudio->isStreamOpen(), "rtaudio stream closed!");

		// Each frame is the (size/2) cause interleaved channels! 
		int sizeInFrames = ((int)data.size()) / (BUFFER_LENGTH);

		int writeCount = 0;

		while (writeCount < sizeInFrames)
		{
			auto a = data.data() + (writeCount * BUFFER_LENGTH);
			bool status = buffer.write(a, BUFFER_LENGTH);
			if (status) writeCount++;
		}

		return true;
	}

}