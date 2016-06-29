#include "Fps.h"

#include <windows.h>
#include <mmsystem.h>

#include "debug.h"

Fps::Fps()
    : _startTime(0), _endTime(0), _delta(.0)
{
    setFPS(60);
}

Fps::Fps(unsigned int fps)
    : _startTime(0), _endTime(0), _delta(.0)
{
    setFPS(fps);
}

Fps::~Fps()
{
}

void Fps::setFPS(unsigned int fps)
{
    _fps = fps;
    _frameMillisec = 1000.0 / (double)fps;
    reset();
}

void Fps::sleep()
{
    _endTime = timeGetTime();
    int sleep = (int)(_frameMillisec + _delta - (double)((long long)_endTime - (long long)_startTime));
    if (sleep <= 0)
    {
        sleep = 1;
        //TRACE("%lld, %d, %d\n", (long long)_endTime - (long long)_startTime, sleep, frameCount);
        TRACE("%lld, %f, %d\n", (long long)_endTime - (long long)_startTime, _delta, sleep);
    }
    else if (sleep > ((int)_frameMillisec * 2))
    {
        sleep = (int)_frameMillisec;
        //TRACE("%lld, %d, %d\n", (long long)_endTime - (long long)_startTime, sleep, frameCount);
        TRACE("%lld, %f, %d\n", (long long)_endTime - (long long)_startTime, _delta, sleep);
    }

    Sleep(sleep);

    DWORD startTime = timeGetTime();
    _delta += _frameMillisec - (double)((long long)startTime - (long long)_startTime);
    if (_delta >= _frameMillisec || _delta <= -_frameMillisec)
    {
        _delta = 0;
    }
    _startTime = startTime;

    // FPS デバッグ用
#if defined(_DEBUG) || defined(DEBUG)
    ++_dFrameCount;
    _dSleepTime += sleep;
    if (_dFrameCount >= _fps)
    {
        _dFps      = (double)_fps * 1000.0 / (double)((long long)_startTime - (long long)_dLastFlameTime);
        _dCpuUsage = (double)(((long long)_startTime - (long long)_dLastFlameTime - _dSleepTime) * 100)
                        / (double)((long long)_startTime - (long long)_dLastFlameTime);
        TRACE("%.2f, CPU: %.2f %%\n", _dFps, _dCpuUsage);
        _dLastFlameTime = _startTime;
        _dFrameCount = _dSleepTime = 0;
    }
    /*
    if ((long long)_startTime - (long long)_dLastFlameTime >= 1000)
    {
        _dCpuUsage = (double)(((long long)_startTime - (long long)_dLastFlameTime - _dSleepTime) * 100)
                        / (double)((long long)_startTime - (long long)_dLastFlameTime);
        TRACE("%d, CPU: %.2f %%\n", _dFrameCount, _dCpuUsage);
        _dFps = _dFrameCount;
        _dLastFlameTime = _startTime;
        _dFrameCount = _dSleepTime = 0;
    }
    */
#endif
}

void Fps::reset()
{
    _startTime = timeGetTime();

#if defined(_DEBUG) || defined(DEBUG)
    _dLastFlameTime = _startTime;
    _dFrameCount = _dSleepTime = 0;
#endif
}

#if defined(_DEBUG) || defined(DEBUG)

double Fps::getFPS() const
{
    return _dFps;
}

double Fps::getCpuUsage() const
{
    return _dCpuUsage;
}

#endif
