#include "supch.h"

#include "AudioPlayer.h"
#include "RingBuffer.h"

namespace Solus {
	audioPlayer::audioPlayer()
	{
		const int desiredSampleRate = 44100;
		const int desiredChannelCount = 2;
		m_MyDevice = CreateRef<AudioDevice>(desiredChannelCount, desiredSampleRate);
		m_MyDevice->Open(m_MyDevice->info.id);
		m_FileData = CreateRef<nqr::AudioData>();
	}

	// Will need implimenting from example
	void audioPlayer::Play(std::string filePath)
	{
		auto memory = nqr::ReadFile(filePath);
		std::string extension = filePath.substr(filePath.find(".") + 1);
		m_Loader.Load(m_FileData.get(), extension, memory.buffer);

		SU_CORE_INFO("Playing STEREO for: {0} seconds...", m_FileData->lengthSeconds);
		m_MyDevice->Play(m_FileData->samples);
	}

	static RingBufferT<float> buffer(BUFFER_LENGTH);

	static int rt_callback(void* output_buffer, void* input_buffer, uint32_t num_bufferframes, double stream_time, RtAudioStreamStatus status, void* user_data)
	{
		if (status) SU_CORE_INFO("[rtaudio] buffer over or underflow");

		// Playback
		if (buffer.getAvailableRead()) buffer.read((float*)output_buffer, BUFFER_LENGTH);
		else memset(output_buffer, 0, BUFFER_LENGTH * sizeof(float));

		return 0;
	}

	AudioDevice::AudioDevice(int numChannels, int sampleRate, int deviceId)
	{
		rtaudio = Scope<RtAudio>(new RtAudio);
		info.id = (deviceId != -1) ? deviceId : rtaudio->getDefaultOutputDevice();
		info.numChannels = numChannels;
		info.sampleRate = sampleRate;
		info.frameSize = FRAME_SIZE;
	}

	AudioDevice::~AudioDevice()
	{
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
		SU_CORE_ASSERT(rtaudio->isStreamOpen(), "rtaudio stream closed!");

		// Each frame is the (size/2) cause interleaved channels! 
		int sizeInFrames = ((int)data.size()) / (BUFFER_LENGTH);

		int writeCount = 0;

		while (writeCount < sizeInFrames)
		{
			bool status = buffer.write((data.data() + (writeCount * BUFFER_LENGTH)), BUFFER_LENGTH);
			if (status) writeCount++;
		}

		return true;
	}

}