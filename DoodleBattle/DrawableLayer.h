#ifndef DRAWABLE_LAYER_H
#define DRAWABLE_LAYER_H

#include <vector>
#include "cocos2d.h"
#include "PatternDetectionManager.h"

USING_NS_CC;

class DrawableLayer
{
	public:
		DrawableLayer();
		virtual ~DrawableLayer();
		virtual void draw();
		virtual void update(float deltaTime);
		virtual void onTouchBegin(const CCPoint &touchPosition);
		virtual void onTouchMoved(const CCPoint &touchPosition);
		virtual void onTouchEnd(const CCPoint &touchPosition);
		virtual void onTouchCancel();
		std::vector<CCPoint>& getPatternVertices();
		void clear();
	private:
		void fadeAndRemoveShapes(float deltaTime);
		std::vector<CCPoint> _verts;
		std::vector<CCPoint> _points;
		CCPoint _initialPoition;
		PatternDetectionManager* _patternDetection;
		bool _touchingJustEnded;
		bool _isTouching;
		bool _fading;
		float _currentFadeTime;
		const float LINE_THICKNESS;
};

#endif