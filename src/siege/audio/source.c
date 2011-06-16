/*
    Copyright (c) 2007 SIEGE Development Team
    All rights reserved.
    
    This file is part of libSIEGE.
    
    This software is copyrighted work licensed under the terms of the
    2-clause BSD license. Please consult the file "license.txt" for
    details.
    
    If you did not recieve the file with this program, please email
    Tim Chas <darkuranium@gmail.com>.
*/

#define SG_BUILD_LIBRARY
#include <siege/audio/source.h>
#include <siege/modules/audio.h>

#include <stdlib.h>
#include <math.h>

SGbool SG_EXPORT _sgAudioSourceInit(void)
{
    _sg_srcDisLength = 16;
    if(sgmAudioSourceMaxSources != NULL)
        sgmAudioSourceMaxSources(&_sg_srcDisLength);
    _sg_srcDisList = malloc(_sg_srcDisLength * sizeof(SGAudioSourceDispatch));

    SGuint i;
    for(i = 0; i < _sg_srcDisLength; i++)
    {
        _sg_srcDisList[i].source = NULL;
        if(sgmAudioSourceCreate != NULL)
            sgmAudioSourceCreate(&_sg_srcDisList[i].handle);
    }

    return SG_TRUE;
}
SGbool SG_EXPORT _sgAudioSourceDeinit(void)
{
    SGuint i;
    for(i = 0; i <_sg_srcDisLength; i++)
    {
        if(sgmAudioSourceDestroy != NULL)
            sgmAudioSourceDestroy(_sg_srcDisList[i].handle);
    }

    free(_sg_srcDisList);

    return SG_TRUE;
}

SGAudioSourceDispatch* SG_EXPORT _sgAudioSourceGetFreeDispatch(SGAudioSource* source)
{
    SGlong mini = -1;
    float minf = HUGE_VAL;

    SGlong blanki = -1;
    SGuint processed;
    SGuint queued;

    SGlong i;
    for(i = 0; i <_sg_srcDisLength; i++)
    {
        if(_sg_srcDisList[i].source == NULL)
        {
            _sg_srcDisList[i].source = source;
            return &_sg_srcDisList[i];
        }
        if((sgmAudioSourceGetNumProcessedBuffers != NULL) && (sgmAudioSourceGetNumQueuedBuffers != NULL))
        {
            sgmAudioSourceGetNumProcessedBuffers(_sg_srcDisList[i].handle, &processed);
            sgmAudioSourceGetNumQueuedBuffers(_sg_srcDisList[i].handle, &queued);
            if(processed == queued)
                blanki = i;
        }
        if(_sg_srcDisList[i].source->priority < minf)
        {
            mini = i;
            minf = _sg_srcDisList[i].source->priority;
        }
    }

    // failed to find, so now we revert to used ones
    if((blanki >= 0) || (mini >= 0))
    {
        if(blanki >= 0)
            i = blanki; // try to use a source that's finished playing
        else if(mini >= 0)
            i = mini; // all sources playing, so try to find one with a smaller priority

        _sg_srcDisList[i].source->dispatch = NULL;
        _sg_srcDisList[i].source = source;
        return &_sg_srcDisList[i];
    }

    // nothing found, so give up
    return NULL;
}

SGAudioSource* SG_EXPORT sgAudioSourceCreate(float priority, float volume, float pitch, SGbool looping)
{
    SGAudioSource* source = malloc(sizeof(SGAudioSource));
    source->priority = priority;

    source->dispatch = _sgAudioSourceGetFreeDispatch(source);
    // todo: enqueue?
    if(source->dispatch == NULL)
    {
        free(source);
        return NULL;
    }
    
    sgAudioSourceSetVolume(source, volume);
    sgAudioSourceSetPitch(source, pitch);
    sgAudioSourceSetLooping(source, looping);

    return source;
}
void SG_EXPORT sgAudioSourceDestroy(SGAudioSource* source)
{
    if(source == NULL)
        return;

    // tell dispatch that the source is available
    if(source->dispatch != NULL)
        source->dispatch->source = NULL;
    free(source);
}

void SG_EXPORT sgAudioSourcePlay(SGAudioSource* source)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourcePlay != NULL)
        sgmAudioSourcePlay(source->dispatch->handle);
}
SGbool SG_EXPORT sgAudioSourceIsPlaying(SGAudioSource* source)
{
    if(source == NULL)
        return SG_FALSE;
    if(source->dispatch == NULL)
        return SG_FALSE;

    SGbool playing = SG_FALSE;
    if(sgmAudioSourceIsPlaying != NULL)
        sgmAudioSourceIsPlaying(source->dispatch->handle, &playing);
    return playing;
}
void SG_EXPORT sgAudioSourcePause(SGAudioSource* source)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourcePause != NULL)
        sgmAudioSourcePause(source->dispatch->handle);
}
SGbool SG_EXPORT sgAudioSourceIsPaused(SGAudioSource* source)
{
    if(source == NULL)
        return SG_FALSE;
    if(source->dispatch == NULL)
        return SG_FALSE;

    SGbool paused = SG_FALSE;
    if(sgmAudioSourceIsPaused != NULL)
        sgmAudioSourceIsPaused(source->dispatch->handle, &paused);
    return paused;
}
void SG_EXPORT sgAudioSourceRewind(SGAudioSource* source)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourceRewind != NULL)
        sgmAudioSourceRewind(source->dispatch->handle);
}
/*SGbool SG_EXPORT sgAudioSourceIsRewinded(SGAudioSource* source)
{
    if(source == NULL)
        return SG_FALSE;
    if(source->dispatch == NULL)
        return SG_FALSE;

    SGbool rewinded = SG_FALSE;
    if(sgmAudioSourceIsRewinded != NULL)
        sgmAudioSourceIsRewinded(source->dispatch->handle, &rewinded);
    return rewinded;
}*/
void SG_EXPORT sgAudioSourceStop(SGAudioSource* source)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourceStop != NULL)
        sgmAudioSourceStop(source->dispatch->handle);
}
SGbool SG_EXPORT sgAudioSourceIsStopped(SGAudioSource* source)
{
    if(source == NULL)
        return SG_FALSE;
    if(source->dispatch == NULL)
        return SG_FALSE;

    SGbool stopped = SG_FALSE;
    if(sgmAudioSourceIsStopped != NULL)
        sgmAudioSourceIsStopped(source->dispatch->handle, &stopped);
    return stopped;
}
SGbool SG_EXPORT sgAudioSourceIsActive(SGAudioSource* source)
{
    if(source == NULL)
        return SG_FALSE;
    if(source->dispatch == NULL)
        return SG_FALSE;

    SGuint processed = 0;
    SGuint queued = 0;
    if(sgmAudioSourceGetNumProcessedBuffers != NULL)
        sgmAudioSourceGetNumProcessedBuffers(source->dispatch->handle, &processed);
    if(sgmAudioSourceGetNumQueuedBuffers != NULL)
        sgmAudioSourceGetNumQueuedBuffers(source->dispatch->handle, &queued);

    return processed != queued;
}
void SG_EXPORT sgAudioSourceQueueBuffers(SGAudioSource* source, SGAudioBuffer** buffers, size_t numbuffers)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    size_t i;
    for(i = 0; i < numbuffers; i++)
        sgAudioSourceQueueBuffer(source, buffers[i]);
}
void SG_EXPORT sgAudioSourceQueueBuffer(SGAudioSource* source, SGAudioBuffer* buffer)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    //if(sgmAudioSourceSetBuffer != NULL)
    //    sgmAudioSourceSetBuffer(source->dispatch->handle, buffer->handle);
    if(sgmAudioSourceQueueBuffers != NULL)
        sgmAudioSourceQueueBuffers(source->dispatch->handle, &buffer->handle, 1);
}

void SG_EXPORT sgAudioSourceSetPosition3f(SGAudioSource* source, float x, float y, float z)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourceSetPosition != NULL)
        sgmAudioSourceSetPosition(source->dispatch->handle, x, y, z);
}
void SG_EXPORT sgAudioSourceSetPosition2f(SGAudioSource* source, float x, float y)
{
    sgAudioSourceSetPosition3f(source, x, y, 0.0f);
}
void SG_EXPORT sgAudioSourceGetPosition3f(SGAudioSource* source, float* x, float* y, float* z)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    float buf;
    if(x == NULL)
        x = &buf;
    if(y == NULL)
        y = &buf;
    if(z == NULL)
        z = &buf;

    if(sgmAudioSourceGetPosition != NULL)
        sgmAudioSourceGetPosition(source->dispatch->handle, x, y, z);
}
void SG_EXPORT sgAudioSourceGetPosition2f(SGAudioSource* source, float* x, float* y)
{
    sgAudioSourceGetPosition3f(source, x, y, NULL);
}

void SG_EXPORT sgAudioSourceSetVelocity3f(SGAudioSource* source, float x, float y, float z)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourceSetVelocity != NULL)
        sgmAudioSourceSetVelocity(source->dispatch->handle, x, y, z);
}
void SG_EXPORT sgAudioSourceSetVelocity2f(SGAudioSource* source, float x, float y)
{
    sgAudioSourceSetVelocity3f(source, x, y, 0.0f);
}
void SG_EXPORT sgAudioSourceGetVelocity3f(SGAudioSource* source, float* x, float* y, float* z)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    float buf;
    if(x == NULL)
        x = &buf;
    if(y == NULL)
        y = &buf;
    if(z == NULL)
        z = &buf;

    if(sgmAudioSourceGetVelocity != NULL)
        sgmAudioSourceGetVelocity(source->dispatch->handle, x, y, z);
}
void SG_EXPORT sgAudioSourceGetVelocity2f(SGAudioSource* source, float* x, float* y)
{
    sgAudioSourceGetVelocity3f(source, x, y, NULL);
}

//void SG_EXPORT sgAudioSourceSetFalloff(SGAudioSource* source, float falloff);
//float SG_EXPORT sgAudioSourceGetFalloff(SGAudioSource* source);

void SG_EXPORT sgAudioSourceSetPitch(SGAudioSource* source, float pitch)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourceSetPitch != NULL)
        sgmAudioSourceSetPitch(source->dispatch->handle, pitch);
}
float SG_EXPORT sgAudioSourceGetPitch(SGAudioSource* source)
{
    if(source == NULL)
        return SG_NAN;
    if(source->dispatch == NULL)
        return SG_NAN;

    float pitch = 1.0f;
    if(sgmAudioSourceGetPitch != NULL)
        sgmAudioSourceGetPitch(source->dispatch->handle, &pitch);
    return pitch;
}

void SG_EXPORT sgAudioSourceSetVolume(SGAudioSource* source, float volume)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourceSetVolume != NULL)
        sgmAudioSourceSetVolume(source->dispatch->handle, volume);
}
float SG_EXPORT sgAudioSourceGetVolume(SGAudioSource* source)
{
    if(source == NULL)
        return SG_NAN;
    if(source->dispatch == NULL)
        return SG_NAN;

    float volume = 1.0f;
    if(sgmAudioSourceGetVolume != NULL)
        sgmAudioSourceGetVolume(source->dispatch->handle, &volume);
    return volume;
}

void SG_EXPORT sgAudioSourceSetLooping(SGAudioSource* source, SGbool looping)
{
    if(source == NULL)
        return;
    if(source->dispatch == NULL)
        return;

    if(sgmAudioSourceSetLooping != NULL)
        sgmAudioSourceSetLooping(source->dispatch->handle, looping);
}
SGbool SG_EXPORT sgAudioSourceGetLooping(SGAudioSource* source)
{
    if(source == NULL)
        return SG_FALSE;
    if(source->dispatch == NULL)
        return SG_FALSE;

    SGbool looping = SG_FALSE;
    if(sgmAudioSourceGetLooping != NULL)
        sgmAudioSourceGetLooping(source->dispatch->handle, &looping);
    return looping;
}
