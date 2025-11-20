#include "TimeSystem.h"
#include <chrono>

// Variable globale pour stocker le temps actuel
auto TimeNow = std::chrono::steady_clock::now();

// Implémentation de GetElapseTime
float GetElapseTime() {
    std::chrono::steady_clock::time_point NewTimeNow = std::chrono::steady_clock::now();
    std::chrono::duration<float> ElapsedTime = NewTimeNow - TimeNow;
    TimeNow = NewTimeNow;
    return ElapsedTime.count();
}


