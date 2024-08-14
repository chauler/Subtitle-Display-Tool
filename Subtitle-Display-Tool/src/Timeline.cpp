#include <algorithm>
#include "Timeline.h"
#include "raylib.h"

Timeline::Timeline(): 
	m_playhead(0),
	m_startTime(0)
{

}

Timeline::Timeline(double timestamp):
	m_playhead(0),
	m_startTime(timestamp)
{

}

/**
* Render the timeline at a given timestamp.
* @param timestamp A double representing the position in the timeline, in seconds
*/
void Timeline::Play(double timestamp)
{
	//m_children.erase(std::remove_if(m_children.begin(),
	//	m_children.end(),
	//	[]() {}));
	for (const auto& child : m_children) {
		if (child->GetStartTime() < timestamp && child->GetEndTime() > timestamp) {

		}
		child->Play(timestamp);
	}
}

double Timeline::GetDuration()
{
	return 0.0;
}

double Timeline::GetEndTime()
{
	return 0.0;
}

/**
* @param child A ptr to the Timeline object to be added to the timeline.
* Claims ownership of the passed Timeline and adds it to the list of children.
*/
void Timeline::AddChild(std::unique_ptr<Timeline> child)
{
	m_children.push_back(std::move(child));
}
