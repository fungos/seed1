/*! \file Theora.cpp
	\author	Danny Angelo Carminati Grein
	\brief Theora decoder
*/

#include "Defines.h"

#if SEED_USE_THEORA == 1

#include "api/theora/Theora.h"
#include "Log.h"
#include "Timer.h"
#include "Screen.h"
#include "MemoryManager.h"


#define TAG "[Theora] "
#define OGGPLAY_BUFFER_SIZE		20


namespace Seed {


Theora::Theora()
	: pPlayer(NULL)
	, pTexData(NULL)
	, iDuration(0)
	, fFps(0.0f)
	, iFpsDenom(1000)
	, iFpsNum(25000)
	, fDelay(0.0f)
	, iFrameCount(0)
	, iUntilFrame(0)
	, iLastFrameTime(0)
	, iTrack(0)
	, iTotalFrames(0)
	, iWidth(0)
	, iHeight(0)
	, iUVWidth(0)
	, iUVHeight(0)
	, iTexWidth(0)
	, iTexHeight(0)
	, fTexScaleX(0.0f)
	, fTexScaleY(0.0f)
	, fElapsedTime(0.0f)
	, bLoaded(FALSE)
	, bPaused(FALSE)
	, bPlaying(FALSE)
	, bTerminateThread(FALSE)
	, sem(0)
	, iTextureId(0)
{
}

Theora::~Theora()
{
	this->Reset();
	SEM_CLOSE(sem);
}

INLINE void Theora::Reset()
{
	IRenderable::Reset();

	bFinished = FALSE;
	bPaused = FALSE;
	bPlaying = FALSE;
	bTerminateThread = FALSE;

	this->Unload();
}

INLINE BOOL Theora::Unload()
{
	if (pPlayer)
		oggplay_close(pPlayer);

	if (iTextureId)
		glDeleteTextures(1, &iTextureId);

	if (pTexData)
		pMemoryManager->Free(pTexData);

	iTextureId = 0;
	pTexData = NULL;
	pPlayer = NULL;
	bTerminateThread = TRUE;

	if (sem)
		SEM_CLOSE(sem);
	sem = 0;

	return TRUE;
}

BOOL Theora::Run()
{
	BOOL ret = Thread::Run();
	if (ret)
	{
		if (bPlaying && !bFinished)
		{
			OggPlayErrorCode r;
			SEM_WAIT(sem);

			r = E_OGGPLAY_TIMEOUT;
			while (r == E_OGGPLAY_TIMEOUT)
			{
				r = oggplay_step_decoding(pPlayer);
			}

			if (r != E_OGGPLAY_CONTINUE && r != E_OGGPLAY_USER_INTERRUPT)
			{
				//bPlaying = FALSE;
				bFinished = TRUE;
				pTimer->Sleep(10);
				SEM_WAIT(sem);
			}
		}
		
		ret = !bTerminateThread;
	}

	pTimer->Sleep(10);
	//Log("%d", bTerminateThread);
	return ret;
}

BOOL Theora::Load(const char *filename)
{
	OggPlayReader *reader = oggplay_file_reader_new(filename);
	pPlayer = oggplay_open_with_reader(reader);

	bLoaded = FALSE;

	if (pPlayer)
	{
		u32 tracks = oggplay_get_num_tracks(pPlayer);
		Log(TAG "There are %d tracks.", tracks);
		
		for (u32 i = 0; i < tracks; i++)
		{
			Log(TAG "Track %d is of type %s", i, oggplay_get_track_typename(pPlayer, i));
			if (oggplay_get_track_type(pPlayer, i) == OGGZ_CONTENT_THEORA)
			{
				oggplay_set_callback_num_frames(pPlayer, i, 1);

				oggplay_get_video_fps(pPlayer, i, (int*)&iFpsDenom, (int*)&iFpsNum);
				iDuration = (u32)oggplay_get_duration(pPlayer);
				fFps = static_cast<f32>(iFpsNum) / static_cast<f32>(iFpsDenom);
				fDelay = 1000 / fFps;
				iTotalFrames = static_cast<u32>(iDuration * fFps / 1000) + 1;
			}

			if (oggplay_set_track_active(pPlayer, i) < 0)
			{
				Log(TAG "Note: Could not set this track active!");
			}
			else
			{
				iTrack = i;
			}
		}

		oggplay_use_buffer(pPlayer, OGGPLAY_BUFFER_SIZE);

		bLoaded = TRUE;
		bPlaying = FALSE;
		bFinished = FALSE;

		SEM_CREATE(sem, OGGPLAY_BUFFER_SIZE);
		SEM_WAIT(sem);

		this->Create();
		this->Run();

		this->ConfigureRendering();
	}
	else
	{
		reader = NULL;
		Log(TAG "ERROR: could not initialise oggplay with '%s'", filename);
	}

	return bLoaded;
}

void Theora::Update()
{
	if (bPlaying)
	{
		if (iUntilFrame && iUntilFrame == iFrameCount)
		{
			this->Pause();
			return;
		}

		if (!this->WaitFrameRate())
			return;

		OggPlayDataHeader **headers;
		OggPlayVideoData *video_data;
		OggPlayDataType type;
		OggPlayCallbackInfo **track_info;

		u32 num_tracks = oggplay_get_num_tracks(pPlayer);
		u32 required = 0;

		track_info = oggplay_buffer_retrieve_next(pPlayer);
		if (track_info == NULL)
		{
			if (bFinished)
				this->Stop();
			// else?
			return;
		}

		for (u32 i = 0; i < num_tracks; i++)
		{
			type = oggplay_callback_info_get_type(track_info[i]);
			headers = oggplay_callback_info_get_headers(track_info[i]);
			
			switch (type)
			{
				case OGGPLAY_INACTIVE:
				break;

				case OGGPLAY_YUV_VIDEO:
					/*
					* there should only be one record
					*/
					required = oggplay_callback_info_get_required(track_info[i]);
					if (required == 0)
					{
						oggplay_buffer_release(pPlayer, track_info);
						goto next_frame;
					}
					video_data = oggplay_callback_info_get_video_data(headers[0]);
					this->ProcessVideoData(/*i,*/video_data);
				break;

				case OGGPLAY_RGBA_VIDEO:
					// Overlay
				break;

				case OGGPLAY_FLOATS_AUDIO:
				break;

				case OGGPLAY_CMML:
					if (oggplay_callback_info_get_required(track_info[i]) > 0)
						Info(TAG "%s", oggplay_callback_info_get_text_data(headers[0]));
				break;

				case OGGPLAY_KATE:
					required = oggplay_callback_info_get_required(track_info[i]);
					for (u32 j = 0; j < required; j++)
						Info(TAG "[%d] %s", j, oggplay_callback_info_get_text_data(headers[j]));
				break;

				default:
				break;
			}
		}

		iFrameCount++;
		oggplay_buffer_release(pPlayer, track_info);

	next_frame:
		SEM_SIGNAL(sem);
	}
}

INLINE BOOL Theora::GoToFrame(u32 frame)
{
	ASSERT(frame < iTotalFrames);

	BOOL ret = FALSE;
	iFrameCount = frame;
	ogg_int64_t pos = static_cast<ogg_int64_t>(frame * fDelay);
	
	if (oggplay_seek(pPlayer, pos) == E_OGGPLAY_CANT_SEEK)
	{
		Log(TAG "Cant seek forward.");
	}
	else
	{
		ret = TRUE;
		if (sem)
			SEM_SIGNAL(sem);
	}

	return ret;
}

INLINE void Theora::Rewind()
{
	iFrameCount = 0;
	if (oggplay_seek(pPlayer, 0) == E_OGGPLAY_CANT_SEEK)
	{
		Log(TAG "Cant seek backwards.");
	}

	if (sem)
		SEM_SIGNAL(sem);
}

INLINE u32 Theora::GetFrameCount() const
{
	return iFrameCount;
}

INLINE f32 Theora::GetFrameRate() const
{
	return fFps;
}

INLINE void Theora::PlayToFrame(u32 frame)
{
	iUntilFrame = frame;
	this->DoPlay();
}

INLINE void Theora::Play()
{
	iUntilFrame = 0;
	this->DoPlay();
}

INLINE void Theora::DoPlay()
{
	if (bPlaying)
		return;

	if (bLoaded)
	{
		if (!bPaused)
			this->Rewind();

		bFinished = FALSE;

		if (bLoaded)
			bPlaying = TRUE;

		bPaused = FALSE;
	}
}

INLINE void Theora::Stop()
{
	bPlaying = FALSE;
	bPaused = FALSE;
}

INLINE BOOL Theora::IsStopped() const
{
	return (!bPlaying || bFinished) && !bPaused;
}

INLINE void Theora::Pause()
{
	iLastFrameTime = 0;
	bPaused = TRUE;
	bPlaying = FALSE;
}

INLINE BOOL Theora::IsPaused() const
{
	return bPaused;
}

INLINE BOOL Theora::IsPlaying() const
{
	return bPlaying;
}

INLINE u32 Theora::Size() const
{
	return iTotalFrames;
}

INLINE void Theora::ProcessVideoData(OggPlayVideoData *data)
{
	OggPlayYUVChannels yuv;
	OggPlayRGBChannels rgb;
	
	yuv.ptry = data->y;
	yuv.ptru = data->u;
	yuv.ptrv = data->v;
	yuv.uv_width = iUVWidth;
	yuv.uv_height = iUVHeight;
	yuv.y_width = iWidth;
	yuv.y_height = iHeight;
	
	rgb.ptro = pTexData;
	rgb.rgb_width = iTexWidth;
	rgb.rgb_height = iTexHeight;
	
	oggplay_yuv2rgba(&yuv, &rgb);
}

INLINE BOOL Theora::WaitFrameRate()
{
	BOOL ret = FALSE;
	if (!iLastFrameTime)
		iLastFrameTime = pTimer->GetMilliseconds();

	//hold fps
	u64 iTime		= pTimer->GetMilliseconds();
	fElapsedTime	+= (iTime - iLastFrameTime);
	iLastFrameTime	= iTime;

	if (fElapsedTime >= fDelay)
	{
		fElapsedTime -= fDelay;
		ret = TRUE;
	}

	return ret;
}

void Theora::ConfigureRendering()
{
	u32 po2_width = 0;
	u32 po2_height = 0;

	glGenTextures(1, &iTextureId);
	glBindTexture(GL_TEXTURE_2D, iTextureId);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	oggplay_get_video_y_size(pPlayer, iTrack, reinterpret_cast<int *>(&iWidth), reinterpret_cast<int *>(&iHeight));
	oggplay_get_video_uv_size(pPlayer, iTrack, reinterpret_cast<int *>(&iUVWidth), reinterpret_cast<int *>(&iUVHeight));

	ASSERT_MSG((iUVWidth == (iWidth / 2)), "Invalid width ");
	ASSERT_MSG((iUVHeight == (iHeight / 2)), "Invalid height ");
	
	for (po2_width = 1; po2_width < iWidth; po2_width <<= 1);
	for (po2_height = 1; po2_height < iHeight; po2_height <<= 1);
	
	fTexScaleX = static_cast<f32>(iWidth) / static_cast<f32>(po2_width);
	fTexScaleY = static_cast<f32>(iHeight) / static_cast<f32>(po2_height);

	if (pTexData == NULL)
	{
		//pTexData = reinterpret_cast<u8 *>(calloc(1, po2_width * po2_height * 4));
		u32 size = po2_width * po2_height * 4;
		pTexData = reinterpret_cast<u8 *>(pMemoryManager->Alloc(size));
		MEMSET(pTexData, '\0', size);
		iTexWidth = po2_width;
		iTexHeight = po2_height;
	}
	/*
	else if (iTexWidth != po2_width || iTexHeight != po2_height) 
	{
		//free(pTexData);
		//pTexData = reinterpret_cast<u8 *>(calloc(1, po2_width * po2_height * 4));
		pMemoryManager->Free(pTexData);
		pTexData = reinterpret_cast<u8 *>(pMemoryManager->Alloc(po2_width * po2_height * 4));
		iTexWidth = po2_width;
		iTexHeight = po2_height;
	}
	*/

	coords[0] = 0.0f;
	coords[1] = 0.0f;
	coords[2] = fTexScaleX;
	coords[3] = 0.0f;
	coords[4] = fTexScaleX;
	coords[5] = fTexScaleY;
	coords[6] = 0.0f;
	coords[7] = fTexScaleY;

	#ifdef SEED_USE_REAL_COORDINATE_SYSTEM
		// A
		vertices[0] = 0.0f;
		vertices[1] = 0.0f;

		// B
		vertices[2] = (f32)pScreen->GetWidth();
		vertices[3] = 0.0f;

		// C
		vertices[4] = (f32)pScreen->GetWidth();
		vertices[5] = (f32)pScreen->GetHeight();

		// D
		vertices[6] = 0.0f;
		vertices[7] = (f32)pScreen->GetHeight();
	#else
		f32 y = 1.0f * pScreen->GetAspectRatio();

		// A
		vertices[0] = 0.0f;
		vertices[1] = 0.0f;

		// B
		vertices[2] = 1.0f;
		vertices[3] = 0.0f;

		// C
		vertices[4] = 1.0f;
		vertices[5] = y;

		// D
		vertices[6] = 0.0f;
		vertices[7] = y;
	#endif
}

void Theora::Render(f32 delta)
{
	UNUSED(delta);

	if (pTexData != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, iTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTexWidth, iTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexData);

		glTexCoordPointer(2, GL_FLOAT, 0, coords);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
	}
}


} // namespace


#endif // SEED_USE_THEORA
