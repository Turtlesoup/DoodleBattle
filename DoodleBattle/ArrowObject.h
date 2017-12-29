#ifndef ARROW_OBJECT_H
#define ARROW_OBJECT_H

#include "PatternObject.h"

class ArrowObject : public PatternObject
{
	public:
		ArrowObject(std::vector<CCPoint>& pattern);
		~ArrowObject();
		virtual void update(float deltaTime);
	private:
		const float SPEED;
		const float DESTROY_THRESHOLD;
};

#endif