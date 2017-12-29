#include "Line.h"

USING_NS_CC;

Line::Line(const CCPoint &p1, const CCPoint &p2) : _p1(p1),
												   _p2(p2)
{

}

Line::~Line()
{

}

CCPoint Line::getP1() const
{
	return _p1;
}

CCPoint Line::getP2() const
{
	return _p2;
}

float Line::getCCPointDistance(const CCPoint &p1, const CCPoint &p2)
{
	return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) );
}
	
Line Line::getPerpendicularLine(const CCPoint &p1, const CCPoint &p2, float thickness, float multiplier)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	float midX = p1.x + dx*0.5;
	float midY = p1.y + dy*0.5;

	float lineLength = sqrt( pow(dy, 2)+ pow(dx, 2) );

	float xV = abs(dx)/lineLength;
	float yV = abs(dy)/lineLength;

	float slope;
	float m;
	float c;

	float a1;
	float b1;
	float a2;
	float b2;

	if(yV > xV)
	{
		slope = (p2.y - p1.y)/(p2.x - p1.x);
		m = -1 / slope;
		c = midY - (m*midX);

		a1 = midX + multiplier*thickness*yV;
		b1 = (m*a1) + c;
		a2 = midX - multiplier*thickness*yV;
		b2 = (m*a2) + c;
	}
	else
	{
		slope = (p2.x - p1.x)/(p2.y - p1.y);
		m = -1 / slope;
		c = midX - (m*midY);

		b1 = midY + multiplier*thickness*xV;
		a1 = (m*b1) + c;
		b2 = midY - multiplier*thickness*xV;
		a2 = (m*b2) + c;
	}


	//y1-y2 = m(x1-x2)
	//y = mx + c
	//x = (y - c)/m

	return Line( ccp(a1,b1), ccp(a2,b2) );
}

float Line::getDistanceFromLine(const Line &line, const CCPoint &p)
{
	const CCPoint a = line.getP1();
	const CCPoint b = line.getP2();

	float numerator = abs(((b.x - a.x)*(b.y - p.y)) - ((a.x - p.x)*(b.y - a.y)));
	float denominator = sqrt( pow((b.x - a.x),2) + pow((b.y - a.y),2) );
	float distance = numerator / denominator;

	return distance;
}