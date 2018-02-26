#include "VideoReader.h"
#include <log4cpp/Category.hh>

//#define VIDEO_EXTRALOGGING

#undef min
#include <algorithm>
#include <cmath>

VideoReader::VideoReader(std::string filename, int maxwidth, int maxheight, bool keepaspectratio, bool usenativeresolution/*false*/)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    _filename = filename;
    _valid = false;
	_lengthMS = 0;
	_formatContext = nullptr;
	_codecContext = nullptr;
	_videoStream = nullptr;
	_dstFrame = nullptr;
    _srcFrame = nullptr;
	_pixelFmt = AVPixelFormat::AV_PIX_FMT_RGB24;
	_atEnd = false;
	_swsCtx = nullptr;
    _dtspersec = 1;
    _frames = 0;

	av_register_all();

	int res = avformat_open_input(&_formatContext, filename.c_str(), nullptr, nullptr);
	if (res != 0)
	{
        logger_base.error("Error opening the file " + filename);
		return;
	}

	if (avformat_find_stream_info(_formatContext, nullptr) < 0)
	{
        logger_base.error("VideoReader: Error finding the stream info in " + filename);
		return;
	}

	// Find the video stream
    AVCodec* cdc;
	_streamIndex = av_find_best_stream(_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &cdc, 0);
	if (_streamIndex < 0)
	{
        logger_base.error("VideoReader: Could not find any video stream in " + filename);
		return;
	}

	_videoStream = _formatContext->streams[_streamIndex];
    _videoStream->discard = AVDISCARD_NONE;
	_codecContext = _videoStream->codec;

    _codecContext->active_thread_type = FF_THREAD_FRAME;
    _codecContext->thread_count = 1;
	if (avcodec_open2(_codecContext, cdc, nullptr) != 0)
	{
        logger_base.error("VideoReader: Couldn't open the context with the decoder in " + filename);
		return;
	}

	// at this point it is open and ready
   if ( usenativeresolution )
   {
      _height = _codecContext->height;
      _width = _codecContext->width;
   }
   else
   {
      if ( keepaspectratio )
      {
         if ( _codecContext->width == 0 || _codecContext->height == 0 )
         {
            logger_base.error( "VideoReader: Invalid input reader dimensions (%d,%d) %s", _codecContext->width, _codecContext->height, (const char *)filename.c_str() );
            return;
         }

         // if > 0 then video will be shrunk
         // if < 0 then video will be stretched
         float shrink = std::min( (float)maxwidth / (float)_codecContext->width, (float)maxheight / (float)_codecContext->height );
         _height = (int)( (float)_codecContext->height * shrink );
         _width = (int)( (float)_codecContext->width * shrink );
      }
      else
      {
         _height = maxheight;
         _width = maxwidth;
      }
   }

	// get the video length in MS
	// Use the number of frames as the best possible way to calculate length
	_frames = (long)_videoStream->nb_frames;
    if (_frames == 0 || _videoStream->avg_frame_rate.den == 0)
    {
        logger_base.warn("VideoReader: dtspersec calc error _videoStream->nb_frames %d and _videoStream->avg_frame_rate.den %d cannot be zero. %s", (int)_videoStream->nb_frames, (int)_videoStream->avg_frame_rate.den, (const char *)filename.c_str());
        logger_base.warn("VideoReader: Video seeking will only work back to the start of the video.");
        _dtspersec = 1;
    }
    else
    {
        _dtspersec = (int)(((int64_t)_videoStream->duration * (int64_t)_videoStream->avg_frame_rate.num) / ((int64_t)_frames * (int64_t)_videoStream->avg_frame_rate.den));
    }

    if (_frames > 0)
	{
        if (_videoStream->avg_frame_rate.num != 0)
        {
            _lengthMS = (int)(((uint64_t)_frames * (uint64_t)_videoStream->avg_frame_rate.den * 1000) / (uint64_t)_videoStream->avg_frame_rate.num);
        }
        else
        {
            logger_base.info("VideoReader: _videoStream->avg_frame_rate.num = 0");
        }
    }

	// If it does not look right try to base if off the duration
	if (_lengthMS <= 0 || _frames <= 0)
	{
        if (_videoStream->avg_frame_rate.den != 0)
        {
            _lengthMS = (int)((uint64_t)_formatContext->duration * (uint64_t)_videoStream->avg_frame_rate.num / (uint64_t)_videoStream->avg_frame_rate.den);
            _frames = _lengthMS  * (uint64_t)_videoStream->avg_frame_rate.num / (uint64_t)(_videoStream->avg_frame_rate.den) / 1000;
        }
        else
        {
            logger_base.info("VideoReader: _videoStream->avg_frame_rate.den = 0");
        }
    }

	// If it still doesnt look right
	if (_lengthMS <= 0 || _frames <= 0)
	{
        if (_videoStream->avg_frame_rate.den != 0)
        {
            // This seems to work for .asf, .mkv, .flv
            _lengthMS = (long)(_formatContext->duration / 1000);
            _frames = _lengthMS  * (uint64_t)_videoStream->avg_frame_rate.num / (uint64_t)(_videoStream->avg_frame_rate.den) / 1000;
        }
    }

	if (_lengthMS <= 0 || _frames <= 0)
	{
		// This is bad ... it still does not look right
        logger_base.warn("Attempts to determine length of video have not been successful. Problems ahead.");
	}

	_dstFrame = av_frame_alloc();
	_dstFrame->width = _width;
	_dstFrame->height = _height;
	_dstFrame->linesize[0] = _width * 3;
	_dstFrame->data[0] = (uint8_t *)av_malloc(_width * _height * 3 * sizeof(uint8_t));

    _srcFrame = av_frame_alloc();
    _srcFrame->pkt_pts = 0;

    _swsCtx = sws_getContext(_codecContext->width, _codecContext->height,
                             _codecContext->pix_fmt, _width, _height, _pixelFmt, SWS_BICUBIC, nullptr,
                             nullptr, nullptr);

    av_init_packet(&_packet);
	_valid = true;

    logger_base.info("Video loaded: " + filename);
    logger_base.info("      Length MS: %d", _lengthMS);
    logger_base.info("      _videoStream->avg_frame_rate.num: %d", _videoStream->avg_frame_rate.num);
    logger_base.info("      _videoStream->avg_frame_rate.den: %d", _videoStream->avg_frame_rate.den);
    logger_base.info("      DTS per sec: %d", _dtspersec);
    logger_base.info("      _videoStream->nb_frames: %d", _videoStream->nb_frames);
    logger_base.info("      _frames: %d", _frames);
    logger_base.info("      Source size: %dx%d", _codecContext->width, _codecContext->height);
    logger_base.info("      Source coded size: %dx%d", _codecContext->coded_width, _codecContext->coded_height);
    logger_base.info("      Output size: %dx%d", _width, _height);
    if (_frames != 0)
    {
        logger_base.info("      Frame ms %d", _lengthMS / _frames);
    }
    else
    {
        logger_base.warn("      Frame ms <unknown as _frames is 0>");
    }
}

static int64_t MStoDTS(int ms, int dtspersec)
{
    return (((int64_t)ms * (int64_t)dtspersec) / (int64_t)1000);
}

static int DTStoMS(int64_t dts , int dtspersec)
{
    return (int)(((int64_t)1000 * dts) / (int64_t)dtspersec);
}

int VideoReader::GetPos()
{
    return DTStoMS(_srcFrame->pkt_dts, _dtspersec);
}

VideoReader::~VideoReader()
{
    if (_swsCtx != nullptr) {
        sws_freeContext(_swsCtx);
        _swsCtx = nullptr;
    }

    if (_srcFrame != nullptr) {
        av_free(_srcFrame);
        _srcFrame = nullptr;
    }
	if (_dstFrame != nullptr)
	{
		if (_dstFrame->data[0] != nullptr)
		{
			av_free(_dstFrame->data[0]);
		}
		av_free(_dstFrame);
		_dstFrame = nullptr;
	}
	if (_codecContext != nullptr)
	{
		avcodec_close(_codecContext);
		_codecContext = nullptr;
	}
	if (_formatContext != nullptr)
	{
		avformat_close_input(&_formatContext);
		_formatContext = nullptr;
	}
}

void VideoReader::Seek(int timestampMS)
{
	// we have to be valid
	if (_valid)
	{
        static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
        //logger_base.info("       VideoReader: Seeking to %d ms.", timestampMS);
        if (timestampMS < _lengthMS)
		{
			_atEnd = false;
		}
		else
		{
			// dont seek past the end of the file
			_atEnd = true;
            avcodec_flush_buffers(_codecContext);
            av_seek_frame(_formatContext, _streamIndex, MStoDTS(_lengthMS, _dtspersec), AVSEEK_FLAG_FRAME);
            return;
		}

        avcodec_flush_buffers(_codecContext);
        int f = av_seek_frame(_formatContext, _streamIndex, MStoDTS(timestampMS, _dtspersec), AVSEEK_FLAG_BACKWARD);
        if (f != 0)
        {
            logger_base.info("       VideoReader: Error seeking to %d.", timestampMS);
        }
		
        int currenttime = -999999;

        AVPacket pkt2;
        // Stop seeking 100ms before where we need ... that way we can read up to the frame we need
        while (currenttime < timestampMS - 100 && av_read_frame(_formatContext, &_packet) >= 0)
		{

			// Is this a packet from the video stream?
			if (_packet.stream_index == _streamIndex)
			{
				// Decode video frame
                pkt2 = _packet;
                while (pkt2.size) {
                    int frameFinished = 0;
                    int ret = avcodec_decode_video2(_codecContext, _srcFrame, &frameFinished,
                        &pkt2);

                    // Did we get a video frame?
                    if (frameFinished)
                    {
                        currenttime = GetPos();

                        // only prepare the image if we are close to the desired frame
                        if (currenttime / _frames >= (timestampMS / _frames) - 2)
                        {
#ifdef VIDEO_EXTRALOGGING
                            logger_base.debug("Seek video %s decoding frame %d.", (const char *)_filename.c_str(), currenttime);
#endif

                            sws_scale(_swsCtx, _srcFrame->data, _srcFrame->linesize, 0,
                                _codecContext->height, _dstFrame->data,
                                _dstFrame->linesize);
                        }
                    }
                    if (ret >= 0) {
                        ret = FFMIN(ret, pkt2.size); /* guard against bogus return values */
                        pkt2.data += ret;
                        pkt2.size -= ret;
                    }
                    else {
                        pkt2.size = 0;
                    }
                }
            }

			// Free the packet that was allocated by av_read_frame
			av_packet_unref(&_packet);
		}
	}
}

AVFrame* VideoReader::GetNextFrame(int timestampMS, int gracetime)
{
    if (!_valid)
    {
        return nullptr;
    }

    if (timestampMS > _lengthMS)
    {
        _atEnd = true;
        return nullptr;
    }

#ifdef VIDEO_EXTRALOGGING
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.debug("Video %s getting frame %d.", (const char *)_filename.c_str(), timestampMS);
#endif

    // If the caller is after an old frame we have to seek first
    int currenttime = GetPos();
    if (currenttime > timestampMS + gracetime)
    {
#ifdef VIDEO_EXTRALOGGING
        logger_base.debug("Video %s seeking back from %d to %d.", (const char *)_filename.c_str(), currenttime, timestampMS);
#endif
        Seek(timestampMS);
        currenttime = GetPos();
    }

	if (timestampMS <= _lengthMS)
	{
		AVPacket pkt2;

        int rc;
		while (currenttime <= timestampMS && (rc = av_read_frame(_formatContext, &_packet)) >= 0 &&  currenttime <= _lengthMS)
		{
            // Is this a packet from the video stream?
			if (_packet.stream_index == _streamIndex)
			{
				// Decode video frame
                pkt2 = _packet;
                while (pkt2.size) {
                    int frameFinished = 0;
                    int ret = avcodec_decode_video2(_codecContext, _srcFrame, &frameFinished,
                                                &pkt2);

                    // Did we get a video frame?
                    if (frameFinished)
                    {
                        currenttime = GetPos();
                        // only prepare the image if we are close to the desired frame
                        if (currenttime / _frames >= (timestampMS / _frames) - 2)
                        {
#ifdef VIDEO_EXTRALOGGING
                            logger_base.debug("Video %s decoding frame %d.", (const char *)_filename.c_str(), currenttime);
#endif

                            sws_scale(_swsCtx, _srcFrame->data, _srcFrame->linesize, 0,
                                _codecContext->height, _dstFrame->data,
                                _dstFrame->linesize);
                        }
                    }

                    if (ret >= 0) {
                        ret = FFMIN(ret, pkt2.size); /* guard against bogus return values */
                        pkt2.data += ret;
                        pkt2.size -= ret;
                    } else {
                        pkt2.size = 0;
                    }
                }
			}

			// Free the packet that was allocated by av_read_frame
			av_packet_unref(&_packet);
		}
	}
	else
	{
		_atEnd = true;
		return nullptr;
	}

	if (_dstFrame->data[0] == nullptr || currenttime > _lengthMS)
	{
		_atEnd = true;
		return nullptr;
	}
	else
	{
		return _dstFrame;
	}
}
