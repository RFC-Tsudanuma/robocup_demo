#pragma once

#include <string>
// #include <rerun.hpp>  // Removed rerun dependency

class Brain; // Forward declaration

using namespace std;

/**
 * Operations related to logging are handled in this library.
 * Rerun dependency has been removed - this is now a stub for compatibility.
 */
class BrainLog
{
public:
    BrainLog(Brain *argBrain);
    
    void prepare();

    void logStatics();

    void setTimeNow();

    void setTimeSeconds(double seconds);

    // Stub for compatibility - does nothing
    template <typename... Ts>
    inline void log(string_view entity_path, const Ts &...archetypes_or_collections) const
    {
        // Rerun logging disabled - this is a stub
    }

    void logToScreen(string logPath, string text, u_int32_t color, double padding = 0.0);

    inline bool isEnabled()
    {
        return enabled;
    }

private:
    bool enabled;
    Brain *brain;
};