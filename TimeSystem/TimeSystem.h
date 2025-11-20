#pragma once
#include <functional>
#include <vector>

// Macro pour créer une structure FTimedFunction avec un type de retour et des paramètres spécifiques
#define TIMED_FUNC(ReturnType , ...)\
    struct FTimedFunction { \
    FTimedFunction(): Time(-1.f), MaxTime(-1.0f), CallBack(nullptr) {}; \
    FTimedFunction(float DesiredTime, const std::function<ReturnType(__VA_ARGS__)>& DesiredCallBack) : Time(DesiredTime), MaxTime(DesiredTime) , CallBack(DesiredCallBack) {}\
    \
    void SubstractTime(float SubstractedTime) {\
        Time = Time - SubstractedTime;\
    }\
    \
    bool IsFinished() const {\
        return Time <= 0;\
    }\
    \
    void Restart() {\
        Time = MaxTime;\
    }\
    \
    auto GetCallBack() const {\
        return CallBack;\
    }\
private:\
    float Time;\
    float MaxTime;\
    std::function<ReturnType(__VA_ARGS__)> CallBack;\
};\

// Macro pour déclarer une fonction temporisée
#define DECLARE_TIMED_FUNC(FuncName, Time, Func)\
    FTimedFunction FuncName = FTimedFunction(Time, Func)\

// Instanciation du type FTimedFunction pour le type void avec paramètres vector<int>&, int, int
TIMED_FUNC(void, std::vector<int>&, int, int);

// Fonction pour obtenir le temps écoulé depuis le dernier appel
float GetElapseTime();

