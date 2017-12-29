#ifndef LINE_H
#define LINE_H

#include "cocos2d.h"

USING_NS_CC;

class Line
{
	public:
		Line(const CCPoint &p1, const CCPoint &p2);
		virtual ~Line();
		CCPoint getP1() const;
		CCPoint getP2() const;
		static float getCCPointDistance(const CCPoint &p1, const CCPoint &p2);
		static Line getPerpendicularLine(const CCPoint &p1, const CCPoint &p2, float thickness = 5.0f, float multiplier = 1.0f);
		static float getDistanceFromLine(const Line &line, const CCPoint &p1);
	private:
		CCPoint _p1;
		CCPoint _p2;
};

#endif