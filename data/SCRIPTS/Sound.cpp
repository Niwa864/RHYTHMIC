// CSound�N���X����
#include "Sound.h"

// �O���[�o���ϐ�
static const WCHAR* g_pszFileBGM[MAX_BGM] = {
	L"morning_fair.mp3",
	L"select.mp3",
	L"Grazing_land.mp3",
	L"Lesson!.mp3",
};
static const WCHAR* g_pszFileSE[MAX_SE] = {
	L"SE1.mp3",
	L"SE2.wav",
	L"SE4.mp3",
	L"SE5.mp3",
};

static const WCHAR* g_pszDirSound = L"../data/sound/";
static const WCHAR* g_pszDirBGM = L"bgm/";
static const WCHAR* g_pszDirSE = L"se/";

// �ÓI�����o
IXAudio2*				CSound::m_pXAudio2 = NULL;
IXAudio2MasteringVoice*	CSound::m_pMasteringVoice = NULL;
CSoundStream*			CSound::m_pBgm = NULL;
CSoundEffect*			CSound::m_pSe = NULL;

// �֐��v���g�^�C�v
static HRESULT CreateMFReader(LPCWSTR mediaFile, COMPTR(IMFSourceReader)& reader, WAVEFORMATEX* wfx, size_t maxwfx);

// �R���X�g���N�^
CSound::CSound(CScene* pScene) : CBaseObj(pScene)
{
	_uID = ID_SOUND;
}

// �f�X�g���N�^
CSound::~CSound(void)
{
}

// ������
bool CSound::Init(void)
{
	HRESULT hr = S_OK;
	if (!m_pXAudio2) {
		UINT32 flags = 0;
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		flags |= XAUDIO2_DEBUG_ENGINE;
#endif
		hr = XAudio2Create(&m_pXAudio2, flags);
		if (FAILED(hr)) {
			m_pXAudio2 = NULL;
		} else {
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
			XAUDIO2_DEBUG_CONFIGURATION debug = {0};
			debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
			debug.BreakMask = XAUDIO2_LOG_ERRORS;
			m_pXAudio2->SetDebugConfiguration(&debug, 0);
#endif
			hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
			if (FAILED(hr)) {
				m_pMasteringVoice = NULL;
				SAFE_RELEASE(m_pXAudio2);
			} else {
				hr = MFStartup(MF_VERSION);
				if (FAILED(hr)) {
					SAFE_DESTROYVOICE(m_pMasteringVoice);
					SAFE_RELEASE(m_pXAudio2);
				}
			}
		}
	}
	if (m_pXAudio2) {
		m_pBgm = new CSoundStream[MAX_BGM];
		m_pSe = new CSoundEffect[MAX_SE];
		WCHAR szPath[_MAX_PATH];
		for (int i = 0; i < MAX_BGM; ++i) {
			lstrcpyW(szPath, g_pszDirSound);
			lstrcatW(szPath, g_pszDirBGM);
			lstrcatW(szPath, g_pszFileBGM[i]);
			if (FAILED(m_pBgm[i].Open(m_pXAudio2, szPath))) {
				::MessageBoxW(NULL, szPath, L"BGM:error", MB_OK);
			}
		}
		for (int i = 0; i < MAX_SE; ++i) {
			lstrcpyW(szPath, g_pszDirSound);
			lstrcatW(szPath, g_pszDirSE);
			lstrcatW(szPath, g_pszFileSE[i]);
			if (FAILED(m_pSe[i].Open(m_pXAudio2, szPath))) {
				::MessageBoxW(NULL, szPath, L"SE:error", MB_OK);
			}
		}
	}
	return true;
}

// �|�[�����O
void CSound::Update(void)
{
	if (m_pBgm) {
		for (int i = 0; i < MAX_BGM; ++i) {
			m_pBgm[i].Update();
		}
	}
}

// ���
void CSound::Fin(void)
{
	if (m_pBgm) {
		for (int i = 0; i < MAX_BGM; ++i) {
			m_pBgm[i].Close();
		}
	}
	if (m_pSe) {
		for (int i = 0; i < MAX_SE; ++i) {
			m_pSe[i].Close();
		}
	}
	MFShutdown();
	SAFE_DELETE_ARRAY(m_pSe);
	SAFE_DELETE_ARRAY(m_pBgm);
	SAFE_DESTROYVOICE(m_pMasteringVoice);
	SAFE_RELEASE(m_pXAudio2);
}

// BGM�Đ�
void CSound::Play(eBGM bgm)
{
	if (!m_pBgm || bgm < 0 || bgm >= MAX_BGM) return;
	m_pBgm[bgm].Play();
}

// SE�Đ�
void CSound::Play(eSE se)
{
	if (!m_pSe || se < 0 || se >= MAX_SE) return;
	m_pSe[se].Play();
}

// BGM�Đ����H
bool CSound::IsPlaying(eBGM bgm)
{
	if (!m_pBgm || bgm < 0 || bgm >= MAX_BGM) return false;
	return m_pBgm[bgm].IsPlaying();
}

// BGM�Đ���~
void CSound::Stop(eBGM bgm)
{
	if (!m_pBgm || bgm < 0 || bgm >= MAX_BGM) return;
	m_pBgm[bgm].Stop();
}

// SE�Đ���~
void CSound::Stop(eSE se)
{
	if (!m_pSe || se < 0 || se >= MAX_SE) return;
	m_pSe[se].Stop();
}

// �}�X�^�[�{�����[���ݒ�
void CSound::SetVolume(float fVol)
{
	if (m_pMasteringVoice) {
		m_pMasteringVoice->SetVolume(fVol);
	}
}

// �}�X�^�[�{�����[���ݒ�
float CSound::GetVolume()
{
	float fVol = 0.0f;
	if (m_pMasteringVoice) {
		m_pMasteringVoice->GetVolume(&fVol);
	}
	return fVol;
}

// BGM�{�����[���ݒ�
void CSound::SetVolume(eBGM bgm, float fVol)
{
	if (!m_pBgm || bgm < 0 || bgm >= MAX_BGM) {
		return;
	}
	m_pBgm[bgm].SetVolume(fVol);
}

// BGM�{�����[���擾
float CSound::GetVolume(eBGM bgm)
{
	if (!m_pBgm || bgm < 0 || bgm >= MAX_BGM) {
		return 0.0f;
	}
	return m_pBgm[bgm].GetVolume();
}

// SE�{�����[���ݒ�
void CSound::SetVolume(eSE se, float fVol)
{
	if (!m_pSe || se < 0 || se >= MAX_SE) {
		return;
	}
	m_pSe[se].SetVolume(fVol);
}

// SE�{�����[���擾
float CSound::GetVolume(eSE se)
{
	if (!m_pSe || se < 0 || se >= MAX_SE) {
		return 0.0f;
	}
	return m_pSe[se].GetVolume();
}

// BGM�N���X �R���X�g���N�^
CSoundStream::CSoundStream() :
	m_pSourceVoice(NULL),
	m_reader(NULL),
	m_currentStreamBuffer(0),
	m_endOfStream(false),
	m_status(SS_STOP)
{
	for (int i = 0; i < MAX_BUFFER_COUNT; ++i) {
		m_buffers[i].resize(32768);
	}
}

// BGM�N���X �f�X�g���N�^
CSoundStream::~CSoundStream()
{
	SAFE_DESTROYVOICE(m_pSourceVoice);
}

// BGM�I�[�v��
HRESULT CSoundStream::Open(IXAudio2* pXAudio2, LPCWSTR pszPath)
{
	if (!pszPath || !*pszPath)
		return E_INVALIDARG;

	HRESULT hr;
	WAVEFORMATEX wfx;
	::ZeroMemory(&wfx, sizeof(wfx));
	hr = CreateMFReader(pszPath, m_reader, &wfx, sizeof(wfx));
	if (FAILED(hr)) {
		return hr;
	}

	hr = pXAudio2->CreateSourceVoice(&m_pSourceVoice, &wfx, XAUDIO2_VOICE_NOPITCH, 1.0f, NULL);
	return hr;
}

// BGM�|�[�����O
void CSoundStream::Update()
{
	if (m_status != SS_PLAY) {
		return;
	}
	if (m_endOfStream) {
		Stop();
		Play();
		return;
	}
	if (!m_pSourceVoice) {
		return;
	}
	XAUDIO2_VOICE_STATE state = {0};
	m_pSourceVoice->GetState(&state);
	if (state.BuffersQueued >= MAX_BUFFER_COUNT - 1) {
		return;
	}
	if (!m_reader) return;
	COMPTR(IMFSample) sample;
	DWORD flags = 0;
	HRESULT hr;
	hr = m_reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, &flags, NULL, &sample);
	if (FAILED(hr))
		return;
	if (flags & MF_SOURCE_READERF_ENDOFSTREAM) {
		m_endOfStream = true;
		return;
	}
	COMPTR(IMFMediaBuffer) mediaBuffer;
	hr = sample->ConvertToContiguousBuffer(&mediaBuffer);
	if (FAILED(hr)) {
		return;
	}
	BYTE* audioData = NULL;
	DWORD sampleBufferLength = 0;
	hr = mediaBuffer->Lock(&audioData, NULL, &sampleBufferLength);
	if (FAILED(hr)) {
		return;
	}
	std::vector<BYTE>& buffer = m_buffers[m_currentStreamBuffer];
	m_currentStreamBuffer++;
	m_currentStreamBuffer %= MAX_BUFFER_COUNT;
	buffer.resize(sampleBufferLength);
	memcpy_s(&buffer[0], sampleBufferLength, audioData, sampleBufferLength);
	hr = mediaBuffer->Unlock();
	if (FAILED(hr)) {
		return;
	}
	XAUDIO2_BUFFER buf = {0};
	buf.AudioBytes = sampleBufferLength;
	buf.pAudioData = &buffer[0];
	m_pSourceVoice->SubmitSourceBuffer(&buf);
}

// BGM�N���[�Y
void CSoundStream::Close()
{
	Stop();
	if (m_reader) {
		m_reader->Flush(MF_SOURCE_READER_FIRST_AUDIO_STREAM);
	}
	SAFE_DESTROYVOICE(m_pSourceVoice);
}

// BGM�Đ�
void CSoundStream::Play()
{
	if (m_pSourceVoice && m_status != SS_PLAY) {
		m_pSourceVoice->Start();
		m_status = SS_PLAY;
	}
}

// BGM��~
void CSoundStream::Stop()
{
	if (m_pSourceVoice && m_status != SS_STOP) {
		m_status = SS_STOP;
		m_pSourceVoice->Stop();
		m_pSourceVoice->FlushSourceBuffers();
		if (m_reader) {
			PROPVARIANT var = {0};
			var.vt = VT_I8;
			m_reader->SetCurrentPosition(GUID_NULL, var);
		}
		m_endOfStream = false;
	}
}

// BGM�ꎞ��~
void CSoundStream::Pause()
{
	if (m_pSourceVoice && m_status != SS_PAUSE) {
		m_status = SS_PAUSE;
		m_pSourceVoice->Stop();
	}
}

// BGM�Đ����`�F�b�N
bool CSoundStream::IsPlaying()
{
	return m_status == SS_PLAY && !m_endOfStream;
}

// BGM�{�����[���ݒ�
void CSoundStream::SetVolume(float fVol)
{
	if (m_pSourceVoice) {
		m_pSourceVoice->SetVolume(fVol);
	}
}

// BGM�{�����[���擾
float CSoundStream::GetVolume()
{
	float fVol = 0.0f;
	if (m_pSourceVoice) {
		m_pSourceVoice->GetVolume(&fVol);
	}
	return fVol;
}

// SE�N���X �R���X�g���N�^
CSoundEffect::CSoundEffect() : m_bufferLength(0)
{
	for (int i = 0; i < MAX_DUP; ++i) {
		m_pSourceVoice[i] = NULL;
		m_status[i] = SS_STOP;
	}
}

// SE�N���X �f�X�g���N�^
CSoundEffect::~CSoundEffect()
{
	for (int i = 0; i < MAX_DUP; ++i) {
		SAFE_DESTROYVOICE(m_pSourceVoice[i]);
	}
}

// SE�I�[�v��
HRESULT CSoundEffect::Open(IXAudio2* pXAudio2, LPCWSTR pszPath)
{
	if (!pszPath || !*pszPath)
		return E_INVALIDARG;

	HRESULT hr;
	COMPTR(IMFSourceReader) reader;
	WAVEFORMATEX wfx;
	::ZeroMemory(&wfx, sizeof(wfx));
	hr = CreateMFReader(pszPath, reader, &wfx, sizeof(wfx));
	if (FAILED(hr)) {
		return hr;
	}
	for (int i = 0; i < MAX_DUP; ++i) {
		hr = pXAudio2->CreateSourceVoice(&m_pSourceVoice[i], &wfx, XAUDIO2_VOICE_NOPITCH, 1.0f, NULL);
		if (FAILED(hr)) {
			return hr;
		}
	}

	m_bufferLength = 0;
	for (;;) {
		COMPTR(IMFSample) sample;
		DWORD flags = 0;
		hr = reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, &flags, NULL, &sample);
		if (FAILED(hr)) {
			return hr;
		}
		if (flags & MF_SOURCE_READERF_ENDOFSTREAM) {
			break;
		}
		COMPTR(IMFMediaBuffer) mediaBuffer;
		hr = sample->ConvertToContiguousBuffer(&mediaBuffer);
		if (FAILED(hr)) {
			return hr;
		}
		BYTE* audioData = NULL;
		DWORD sampleBufferLength = 0;
		hr = mediaBuffer->Lock(&audioData, NULL, &sampleBufferLength);
		if (FAILED(hr)) {
			return hr;
		}
		m_buffer.resize(m_bufferLength + sampleBufferLength);
		memcpy_s(&m_buffer[m_bufferLength], sampleBufferLength, audioData, sampleBufferLength);
		m_bufferLength += sampleBufferLength;
		hr = mediaBuffer->Unlock();
		if (FAILED(hr)) {
			return hr;
		}
	}

	reader->Flush(MF_SOURCE_READER_FIRST_AUDIO_STREAM);

	return hr;
}

// SE�N���[�Y
void CSoundEffect::Close()
{
	Stop();
	for (int i = 0; i < MAX_DUP; ++i) {
		SAFE_DESTROYVOICE(m_pSourceVoice[i]);
	}
}

// SE�Đ�
void CSoundEffect::Play()
{
	int i;
	for (i = 0; i < MAX_DUP; ++i) {
		if (!m_pSourceVoice[i]) {
			continue;
		}
		if (m_status[i] == SS_STOP) {
			break;
		}
	}
	if (i >= MAX_DUP) {
		for (i = 0; i < MAX_DUP; ++i) {
			if (!m_pSourceVoice[i]) {
				continue;
			}
			if (m_status[i] == SS_PAUSE) {
				Stop(i);
				break;
			}
		}
		if (i >= MAX_DUP) {
			XAUDIO2_VOICE_STATE state = {0};
			for (i = 0; i < MAX_DUP; ++i) {
				if (!m_pSourceVoice[i]) {
					continue;
				}
				m_pSourceVoice[i]->GetState(&state);
				if (state.BuffersQueued > 0) {
					continue;
				}
				Stop(i);
				break;
			}
		}
		if (i >= MAX_DUP) {	// �ő�܂ő��d�Đ���
			i = rand() % MAX_DUP;
			if (!m_pSourceVoice[i]) {
				return;		// �Đ��s��
			}
			Stop(i);		// �����œK���ɂǂꂩ���~�߂�
		}
	}
	XAUDIO2_BUFFER buf = {0};
	buf.AudioBytes = m_bufferLength;
	buf.pAudioData = &m_buffer[0];
	buf.Flags = XAUDIO2_END_OF_STREAM;
	m_pSourceVoice[i]->SubmitSourceBuffer(&buf);
	m_pSourceVoice[i]->Start();
	m_status[i] = SS_PLAY;
}

// SE��~
void CSoundEffect::Stop(int n)
{
	if (n < 0 || n >= MAX_DUP) {
		for (int i = 0; i < MAX_DUP; ++i) {
			if (m_pSourceVoice[i] && m_status[i] != SS_STOP) {
				m_status[i] = SS_STOP;
				m_pSourceVoice[i]->Stop();
				m_pSourceVoice[i]->FlushSourceBuffers();
			}
		}
		return;
	}
	if (m_pSourceVoice[n] && m_status[n] != SS_STOP) {
		m_status[n] = SS_STOP;
		m_pSourceVoice[n]->Stop();
		m_pSourceVoice[n]->FlushSourceBuffers();
	}
}

// SE�ꎞ��~
void CSoundEffect::Pause()
{
	for (int i = 0; i < MAX_DUP; ++i) {
		if (m_pSourceVoice[i] && m_status[i] != SS_PAUSE) {
			m_status[i] = SS_PAUSE;
			m_pSourceVoice[i]->Stop();
		}
	}
}

// SE�Đ����`�F�b�N
bool CSoundEffect::IsPlaying()
{
	for (int i = 0; i < MAX_DUP; ++i) {
		if (m_pSourceVoice[i] && m_status[i] == SS_PLAY) {
			return true;
		}
	}
	return false;
}

// SE�{�����[���ݒ�
void CSoundEffect::SetVolume(float fVol)
{
	for (int i = 0; i < MAX_DUP; ++i) {
		if (m_pSourceVoice[i]) {
			m_pSourceVoice[i]->SetVolume(fVol);
		}
	}
}

// SE�{�����[���擾
float CSoundEffect::GetVolume()
{
	float fVol = 0.0f;
	for (int i = 0; i < MAX_DUP; ++i) {
		if (m_pSourceVoice[i]) {
			m_pSourceVoice[i]->GetVolume(&fVol);
			break;
		}
	}
	return fVol;
}

//--------------------------------------------------------------------------------------
// Helper for setting up the MF source reader
//--------------------------------------------------------------------------------------
HRESULT CreateMFReader(LPCWSTR mediaFile, COMPTR(IMFSourceReader)& reader, WAVEFORMATEX* wfx, size_t maxwfx)
{
	if (!mediaFile || !wfx) {
		return E_INVALIDARG;
	}

	HRESULT hr;
	COMPTR(IMFAttributes) attr;
	hr = MFCreateAttributes(&attr, 1);
	if (FAILED(hr))
		return hr;
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
	hr = attr->SetUINT32(MF_LOW_LATENCY, TRUE);
	if (FAILED(hr))
		return hr;
#endif

	hr = MFCreateSourceReaderFromURL(mediaFile, attr, &reader);
	if (FAILED(hr))
		return hr;

	//
	// Make the output from Media Foundation PCM so XAudio2 can consume it
	//
	COMPTR(IMFMediaType) mediaType;
	hr = MFCreateMediaType(&mediaType);
	if (FAILED(hr))
		return hr;

	hr = mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	if (FAILED(hr))
		return hr;

	hr = mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	if (FAILED(hr))
		return hr;

	hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, mediaType);
	if (FAILED(hr))
		return hr;

	//
	// Get the wave format
	//
	COMPTR(IMFMediaType) outputMediaType;
	hr = reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &outputMediaType);
	if (FAILED(hr))
		return hr;

	UINT32 waveFormatSize = 0;
	WAVEFORMATEX* waveFormat = NULL;
	hr = MFCreateWaveFormatExFromMFMediaType(outputMediaType, &waveFormat, &waveFormatSize);
	if (FAILED(hr))
		return hr;

	memcpy_s(wfx, maxwfx, waveFormat, waveFormatSize);
	CoTaskMemFree(waveFormat);

	return S_OK;
}