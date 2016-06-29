#ifndef _FPS_H_
#define _FPS_H_

class Fps
{
public:
    Fps();
    Fps(unsigned int fps);
    virtual ~Fps();

    virtual void setFPS(unsigned int fps);
    virtual void sleep();
    virtual void reset();

private:
    unsigned int  _fps;
    double        _frameMillisec;

    unsigned long _startTime;
    unsigned long _endTime;
    double        _delta; // åÎç∑

#if defined(_DEBUG) || defined(DEBUG)
public:
    virtual double getFPS() const;
    virtual double getCpuUsage() const;
private:
    unsigned int  _dFrameCount;
    unsigned int  _dSleepTime;
    double        _dFps;
    double        _dCpuUsage;
    unsigned long _dLastFlameTime;
#endif

};

#endif // _FPS_H_
