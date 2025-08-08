#include "brain_log.h"
#include "brain.h"
#include "utils/math.h"
#include "utils/print.h"

BrainLog::BrainLog(Brain *argBrain) : enabled(false), brain(argBrain)
{
    // Rerun logging disabled - always set enabled to false
    enabled = false;
}

void BrainLog::setTimeNow()
{
    // Stub - does nothing
}

void BrainLog::setTimeSeconds(double seconds)
{
    // Stub - does nothing
}

void BrainLog::logStatics()
{
    // Stub - does nothing
}

void BrainLog::prepare()
{
    // Stub - does nothing
}

void BrainLog::logToScreen(string logPath, string text, u_int32_t color, double padding)
{
    // Stub - does nothing
}