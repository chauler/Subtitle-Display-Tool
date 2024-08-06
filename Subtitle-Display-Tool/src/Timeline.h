#pragma once
#include <vector>
#include <memory>

/**
* Class which may contain nested instances. Used to hold Window instances
* and draw them with proper timing information. Timing is done outside of
* the Timeline, then a point on the timeline should be passed to Play().
*/
class Timeline {
public:
	Timeline(double timestamp);
	Timeline();
	virtual void Play(double timestamp);
	double GetStartTime() { return m_startTime; }
	double GetDuration();
	double GetEndTime();
	void AddChild(std::unique_ptr<Timeline>);
protected:
	std::vector<std::unique_ptr<Timeline>> m_children;
private:
	double m_playhead;
	double m_startTime;
};
