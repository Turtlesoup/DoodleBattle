#ifndef PATTER_DETECTION_MANAGER_H
#define PATTER_DETECTION_MANAGER_H

#include <vector>
#include <map>
#include "cocos2d.h"
#include "IdToElement.h"

USING_NS_CC;

class PatternDetectionManager
{
	public:
		PatternDetectionManager();
		virtual ~PatternDetectionManager();
		void addPatternToDetectionList(unsigned int patternID, std::vector<CCPoint>& pattern);
		void loadPatternsFromFile(const char* filename);
		void writePatternsFromFile(const char* filename);
		CCPoint createPoint(const std::string line);
		void normalisePatterns();
		void getClosestPatternID(std::vector<CCPoint>& pattern);
	private:
		//feature extraction methods
		void extractLineSegmentIndices(std::vector<unsigned int>& lineSegmentIndices, std::vector<CCPoint>& pattern, const unsigned int firstIndex, const unsigned int secondIndex, const unsigned int minLineLength, const float threshold);

		//feature vector creation
		float computeFeatureVectorDistance(std::vector<float>& featureVector1, std::vector<float>& featureVector2);
		std::vector<float> createFeatureVector(std::vector<CCPoint>& pattern);
		void updateFeatureVectorNormalisationValues();
		void normaliseFeatureVector(std::vector<float>& featureVector);
		void normaliseFeatureVectors();
		
		//pattern detection
		void normalisePattern(std::vector<CCPoint>& pattern);
		float computeDTWDistance(std::vector<CCPoint>& pattern1, std::vector<CCPoint>& pattern2);
		
		void computeDTWDistanceForIndex(unsigned int patternID, unsigned int index, std::vector<CCPoint>& pattern1, std::vector<CCPoint>& pattern2);
		unsigned int numberOfPatternDTWDistancesToCompute;
		std::map<unsigned int, float> _indexToDistances;

		//pattern data stores
		std::vector< IdToElement< std::vector<CCPoint> > > _patterns;
		std::vector< IdToElement< std::vector<float> > > _featureVectors;
		std::vector<float> _featureVectorNormalisationValues;
};

#endif