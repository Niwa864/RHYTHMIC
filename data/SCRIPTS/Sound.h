// CSoundクラス定義
//
// ※予めWinMainあるいはmainで、
//   CoInitializeEx(NULL, COINIT_MULTITHREADED)
//   が実行済であることが前提。
#pragma once

// BGM/SE定義
enum eBGM {
	BGM_TITLE = 0,	// メインテーマ
	BGM_SELECT,
	BGM_CHARSELECT,
	BGM_MAIN,

	MAX_BGM
};

enum eSE {
	SE_NEXT,
	SE_MARACAS1,
	SE_MARACAS2,
	SE_ENEMYFOUND,
	MAX_SE
};

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include <objbase.h>
#include <vector>
#include <mmreg.h>

#include <comip.h>
#include <comdef.h>
#define COMPTR(type) _com_ptr_t<_com_IIID<type,&__uuidof(type)>>

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include <initguid.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mfplat.lib")
#else
#include <comdecl.h>
#include <xaudio2.h>

#define MF_SDK_VERSION 0x0001
#define MF_API_VERSION 0x0070
#define MF_VERSION (MF_SDK_VERSION << 16 | MF_API_VERSION)
#include <initguid.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mfplat_vista.lib")
#endif
#pragma comment(lib, "mfreadwrite.lib")

#include "BaseObj.h"
#include "define.h"

//-------- マクロ
#ifndef SAFE_DESTROYVOICE
#define SAFE_DESTROYVOICE(x)	if(x){ x->DestroyVoice(); x=NULL; }
#endif
#define MAX_BUFFER_COUNT		3	// ストリーム再生用バッファ数
#define MAX_DUP					16	// 最大多重再生数

enum SoundStatus {
	SS_STOP = 0,
	SS_PLAY,
	SS_PAUSE,

	MAX_SOUNDSTATUS
};

// BGMクラス
struct CSoundStream
{
private:
	IXAudio2SourceVoice*	m_pSourceVoice;
	COMPTR(IMFSourceReader)	m_reader;
	DWORD					m_currentStreamBuffer;
	std::vector<BYTE>		m_buffers[MAX_BUFFER_COUNT];
	bool					m_endOfStream;
	SoundStatus				m_status;

public:
	CSoundStream();
	virtual ~CSoundStream();
	HRESULT Open(IXAudio2* pXAudio2, LPCWSTR pszPath);
	void Update();
	void Close();
	void Play();
	void Stop();
	void Pause();
	bool IsPlaying();
	void SetVolume(float fVol);
	float GetVolume();
};

// SEクラス
struct CSoundEffect
{
public:
	IXAudio2SourceVoice*	m_pSourceVoice[MAX_DUP];
	SoundStatus				m_status[MAX_DUP];
	std::vector<BYTE>		m_buffer;
	DWORD					m_bufferLength;

public:
	CSoundEffect();
	virtual ~CSoundEffect();
	HRESULT Open(IXAudio2* pXAudio2, LPCWSTR pszPath);
	void Close();
	void Play();
	void Stop(int n = -1);
	void Pause();
	bool IsPlaying();
	void SetVolume(float fVol);
	float GetVolume();
};

// サウンド マネージャ クラス
class CSound : public CBaseObj
{
private:
	static IXAudio2*				m_pXAudio2;
	static IXAudio2MasteringVoice*	m_pMasteringVoice;
	static CSoundStream*			m_pBgm;
	static CSoundEffect*			m_pSe;

public:
	CSound(CScene* pScene);
	virtual ~CSound(void);

	virtual bool Init(void);
	virtual void Update(void);
	virtual void Fin(void);
	static void Play(eBGM bgm);
	static void Play(eSE se);
	static bool IsPlaying(eBGM bgm);
	static void Stop(eBGM bgm);
	static void Stop(eSE se);
	static void SetVolume(float fVol);
	static float GetVolume();
	static void SetVolume(eBGM bgm, float fVol);
	static float GetVolume(eBGM bgm);
	static void SetVolume(eSE se, float fVol);
	static float GetVolume(eSE se);
};