#pragma once

/**
A class for measuring the seconds after the program started.
*/
class Timer {
public:
	Timer();
	double GetDeltaTime();
private:
	double m_prevTime;
};