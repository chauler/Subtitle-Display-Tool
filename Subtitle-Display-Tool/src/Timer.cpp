#include "Timer.h"
#include "raylib.h"

Timer::Timer(): m_prevTime(GetTime()) {

}

/**
Returns the time since the last call to GetDeltaTime.
*/
double Timer::GetDeltaTime() {
	double currentTime = GetTime();
	double delta = currentTime - m_prevTime;
	m_prevTime = currentTime;
	return delta;
}