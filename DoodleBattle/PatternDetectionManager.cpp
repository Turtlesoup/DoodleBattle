#include "PatternDetectionManager.h"
#include <cmath>
#include <climits>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>
#include "Line.h"
#include "EventManager.h"
#include "PatternDetectedEventData.h"

PatternDetectionManager::PatternDetectionManager()
{
	std::vector< IdToElement< std::vector<CCPoint> > > _patterns;
	std::vector< IdToElement< std::vector<float> > > _featureVectors;
	std::vector<float> _featureVectorNormalisationValues;
}

PatternDetectionManager::~PatternDetectionManager()
{
}

void PatternDetectionManager::loadPatternsFromFile(const char* filename)
{
	std::vector<CCPoint>* pattern = NULL;
	CCPoint point;

	const std::string breakPattern = "-";

	std::string patternID;
	std::string line;

	std::ifstream file(filename);
	if(file.is_open())
	{
		while(file.good())
		{
			std::getline(file, line);

			if(line == breakPattern)
			{
				if(pattern != NULL)
				{
					addPatternToDetectionList((unsigned int)std::atof(patternID.c_str()), *pattern);
				}

				std::getline(file, line);
				patternID = line;
				pattern = new std::vector<CCPoint>();
			}
			else
			{
				point = createPoint(line);
				pattern->push_back(point);
			}
		}

		if(pattern->size() > 0)
		{
			//add the last patter in the list
			addPatternToDetectionList((unsigned int)std::atof(patternID.c_str()), *pattern);
		}
		
		file.close();
	}

	//normalise the patterns
	updateFeatureVectorNormalisationValues();
	normaliseFeatureVectors();
}

CCPoint PatternDetectionManager::createPoint(const std::string line)
{
	const unsigned int splitIndex = line.find_first_of(" ");
	std::string xValueString = line.substr(0, splitIndex);
	std::string yValueString = line.substr(splitIndex+1, line.length());
	float xValue = (float)std::atof(xValueString.c_str());
	float yValue = (float)std::atof(yValueString.c_str());

	return ccp(xValue, yValue);
}

void PatternDetectionManager::writePatternsFromFile(const char* filename)
{
	std::ofstream file;
	file.open(filename);

	std::vector<CCPoint> points;
	unsigned int pointIndex;
	unsigned int numPoints;

	unsigned int numPatterns = _patterns.size();
	for(unsigned int patternIndex = 0; patternIndex < numPatterns; patternIndex++)
	{
		file << "-" << std::endl;
		file << _patterns[patternIndex].getID() << std::endl;
		points = _patterns[patternIndex].getElement();
		numPoints = points.size();
		for(pointIndex = 0; pointIndex < numPoints; pointIndex++)
		{
			file << points[pointIndex].x;
			file << " ";
			file << points[pointIndex].y;
			file << std::endl;
		}
	}

	file.close();
}

void PatternDetectionManager::normalisePatterns()
{
	updateFeatureVectorNormalisationValues();
	normaliseFeatureVectors();
}

std::vector<float> PatternDetectionManager::createFeatureVector(std::vector<CCPoint>& pattern)
{
	std::vector<float> featureVector = std::vector<float>();

	//get the total number of points in the pattern
	unsigned int totalNumberOfPoints = pattern.size();

	//compute the line segments of the pattern
	std::vector<unsigned int> lineSegmentIndices = std::vector<unsigned int>();
	lineSegmentIndices.push_back(0);
	extractLineSegmentIndices(lineSegmentIndices, pattern, 0, totalNumberOfPoints-1, 0, 0.25);

	//add the number of line segments as the first feature in the feature vector
	featureVector.push_back(lineSegmentIndices.size());

	//compute the percentage of the total number of points of the larget line segment
	unsigned int numLines = lineSegmentIndices.size();
	unsigned int lineStartIndex;
	unsigned int lineEndIndex;
	unsigned int largestLineSize = 0;
	for(unsigned int lineIndex = 0; lineIndex < numLines; lineIndex++)
	{
		lineStartIndex = lineSegmentIndices[lineIndex];

		if((lineIndex + 1) >= numLines)
		{
			lineEndIndex = totalNumberOfPoints;
		}
		else
		{
			lineEndIndex = lineSegmentIndices[lineIndex+1];
		}

		if((lineEndIndex - lineStartIndex) > largestLineSize)
		{
			largestLineSize = (lineEndIndex - lineStartIndex);
		}
	}
	float linePercentageOfTotalPattern = ((float)largestLineSize/(float)totalNumberOfPoints);

	//add the largest line percentage to the featire vector
	featureVector.push_back(linePercentageOfTotalPattern);

	//compute various stats about the distribution of the points in regards to quadrants
	//of the given pattern. from this, we can compute the percentage of points which lie
	//within each quadrant. We must be careful of over-fitting here as some of the data
	//may not be useful for differenciating between the patterns.
	unsigned int bottomCount = 0;
	unsigned int rightCount = 0;
	unsigned int topLeftQuadrentCount = 0;
	unsigned int topRightQuadrentCount = 0;
	unsigned int bottomLeftQuadrentCount = 0;
	unsigned int bottomRightQuadrentCount = 0;
	CCPoint point;
	for(unsigned int pointIndex = 0; pointIndex < totalNumberOfPoints; pointIndex++)
	{
		point = pattern[pointIndex];
		if(point.x > 0.0 && point.y < 0.0)
		{
			bottomCount++;
			rightCount++;
			bottomRightQuadrentCount++;
		}
		else if(point.x > 0.0 && point.y > 0.0)
		{
			rightCount++;
			topRightQuadrentCount++;
		}
		else if(point.x < 0.0 && point.y < 0.0)
		{
			bottomCount++;
			bottomLeftQuadrentCount++;
		}
		else if(point.x < 0.0 && point.y > 0.0)
		{
			topLeftQuadrentCount++;
		}
		else
		{
			//center point - ignore
		}
	}

	float belowPointsPercentage = (float)bottomCount/(float)totalNumberOfPoints;
	float rightPointsPercentage = (float)rightCount/(float)totalNumberOfPoints;
	float bottomLeftPointsPercentage = (float)bottomLeftQuadrentCount/(float)totalNumberOfPoints;
	float bottomRightPointsPercentage = (float)bottomRightQuadrentCount/(float)totalNumberOfPoints;
	float topLeftPointsPercentage = (float)topLeftQuadrentCount/(float)totalNumberOfPoints;
	float topRightPointsPercentage = (float)topRightQuadrentCount/(float)totalNumberOfPoints;


	//add percentages to the feature vector
	featureVector.push_back(belowPointsPercentage);
	featureVector.push_back(rightPointsPercentage);
	featureVector.push_back(topLeftPointsPercentage);
	featureVector.push_back(bottomRightPointsPercentage);
	//featureVector.push_back(bottomLeftPointsPercentage);
	//featureVector.push_back(topRightPointsPercentage);

	return featureVector;
}

void PatternDetectionManager::extractLineSegmentIndices(std::vector<unsigned int>& lineSegmentIndices, std::vector<CCPoint>& pattern, const unsigned int firstIndex, const unsigned int secondIndex, const unsigned int minLineLength, const float threshold)
{
	if(secondIndex - firstIndex < minLineLength)
	{
		return;
	}

	CCPoint p1, p2;
	float distance;
	float maxDistance = INT_MIN;
	unsigned int futhestPointIndex;

	p1 = pattern[firstIndex];
	p2 = pattern[secondIndex];
	for(unsigned int pointIndex = firstIndex+1; pointIndex < secondIndex; pointIndex++)
	{
		distance = Line::getDistanceFromLine( Line(p1, p2), pattern[pointIndex] );
		if(distance > maxDistance)
		{
			maxDistance = distance;
			futhestPointIndex = pointIndex;
		}
	}

	//if the max distance of the furthest point P is greater than the allowed threshold,
	//split the line AB into two segments AP and PB and divide the problem until the threshold is matched
	if(maxDistance > threshold)
	{
		extractLineSegmentIndices(lineSegmentIndices, pattern, firstIndex, futhestPointIndex, minLineLength, threshold);
		
		if((futhestPointIndex - firstIndex) >= minLineLength &&
		   (secondIndex - futhestPointIndex) >= minLineLength)
		{
			lineSegmentIndices.push_back(futhestPointIndex);
		}

		extractLineSegmentIndices(lineSegmentIndices, pattern, futhestPointIndex, secondIndex, minLineLength, threshold);
	}
}

void PatternDetectionManager::updateFeatureVectorNormalisationValues()
{
	unsigned int numFeatureVectors = _featureVectors.size();

	if(numFeatureVectors < 1)
	{
		return;
	}

	float total, f1, f2;
	unsigned int numFields;
	unsigned int fieldIndex;
	float value;
	float maxValue;

	numFields = _featureVectors.front().getElement().size();

	for(unsigned int fieldIndex = 0; fieldIndex < numFields; fieldIndex++)
	{
		maxValue = INT_MIN;
		for(unsigned int featureVectorIndex = 0; featureVectorIndex < numFeatureVectors; featureVectorIndex++)
		{
			value = _featureVectors[featureVectorIndex].getElement()[fieldIndex];
			if(value > maxValue)
			{
				maxValue = value;
			}
		}
		_featureVectorNormalisationValues.push_back(maxValue);
	}
}

void PatternDetectionManager::normaliseFeatureVectors()
{
	unsigned int numFeatureVectors = _featureVectors.size();
	for(unsigned int featureVectorIndex = 0; featureVectorIndex < numFeatureVectors; featureVectorIndex++)
	{
		normaliseFeatureVector( _featureVectors[featureVectorIndex].getElement() );
	}
}

void PatternDetectionManager::normaliseFeatureVector(std::vector<float>& featureVector)
{
	unsigned int featureVectorSize = featureVector.size();

	float f1, maxValue;

	for(unsigned int featureIndex = 0; featureIndex < featureVectorSize; featureIndex++)
	{
		f1 = featureVector[featureIndex];
		maxValue = _featureVectorNormalisationValues[featureIndex];
		featureVector[featureIndex] = (f1/maxValue);
	}
}

float PatternDetectionManager::computeFeatureVectorDistance(std::vector<float>& featureVector1, std::vector<float>& featureVector2)
{
	unsigned int featureVectorSize = featureVector1.size();

	float total, f1, f2;

	total = 0.0f;
	for(unsigned int featureIndex = 0; featureIndex < featureVectorSize; featureIndex++)
	{
		f1 = featureVector1[featureIndex];
		f2 = featureVector2[featureIndex];
		total += pow(f1-f2 , 2);
	}

	return sqrt(total);
}

void PatternDetectionManager::normalisePattern(std::vector<CCPoint>& pattern)
{
	float maxX = INT_MIN;
	float minX = INT_MAX;

	float maxY = INT_MIN;
	float minY = INT_MAX;

	CCPoint* point;

	//get the maxX, minX, maxY, and minY values
	unsigned int numPoints = pattern.size();
	unsigned int pointIndex;
	for(pointIndex = 0; pointIndex < numPoints; pointIndex++)
	{
		point = &pattern[pointIndex];
		if(point->x < minX)
		{
			minX = point->x;
		}
		if(point->y < minY)
		{
			minY = point->y;
		}

		if(point->x > maxX)
		{
			maxX = point->x;
		}
		if(point->y > maxY)
		{
			maxY = point->y;
		}
	}

	//compute the width and height of the pattern
	float width = maxX - minX;
	float height = maxY - minY;

	//compute midpoint
	CCPoint midPoint = ccp(minX+(width/2), minY+(height/2));

	for(pointIndex = 0; pointIndex < numPoints; pointIndex++)
	{
		point = &pattern[pointIndex];
		point->x -= midPoint.x;
		point->x /= width;
		point->y -= midPoint.y;
		point->y /= height;
	}
}

void PatternDetectionManager::addPatternToDetectionList(unsigned int patternID, std::vector<CCPoint>& pattern)
{
	std::vector<CCPoint> clonedPattern = std::vector<CCPoint>(pattern);
	normalisePattern(clonedPattern);
	_patterns.push_back( IdToElement<std::vector<CCPoint>>( patternID, clonedPattern ) );
	_featureVectors.push_back( IdToElement<std::vector<float>>( patternID, createFeatureVector(clonedPattern) ) );
}

void PatternDetectionManager::getClosestPatternID(std::vector<CCPoint>& pattern)
{
	
	//normalise the pattern
	normalisePattern(pattern);

	//compute the feature vector for the pattern
	std::vector<float> featureVector = createFeatureVector(pattern);
	normaliseFeatureVector(featureVector);

	//get the number of patterns in the samples
	unsigned int numPatterns = _featureVectors.size();

	//find the stored pattern with the lowest feature vector distance from the given pattern
	float featureVectorDistance;
	float lowestFeatureVectorDistance = INT_MAX;
	unsigned int closestPatternIndex = INT_MAX;

	for(unsigned int featureVectorIndex = 0; featureVectorIndex < numPatterns; featureVectorIndex++)
	{
		featureVectorDistance = computeFeatureVectorDistance( _featureVectors[featureVectorIndex].getElement(), featureVector);

		if(featureVectorDistance < lowestFeatureVectorDistance)
		{
			lowestFeatureVectorDistance = featureVectorDistance;
			closestPatternIndex = featureVectorIndex;
		}
	}

	//get the id of the closest pattern
	unsigned int closestPatternID = _featureVectors[closestPatternIndex].getID();

	//for all pattern samples with the same pattern id as the closest feature vector, compute the
	//dtw distance from the given pattern and return the lowest dtw distance
	_indexToDistances.clear();
	std::vector<std::thread*> workerThreads;
	for(unsigned int patternIndex = 0; patternIndex < numPatterns; patternIndex++)
	{
		if(_patterns[patternIndex].getID() == closestPatternID)
		{
			std::thread* workerThread = new std::thread(&PatternDetectionManager::computeDTWDistanceForIndex, this, closestPatternID, workerThreads.size(), _patterns[patternIndex].getElement(), pattern);
			workerThreads.push_back(workerThread);
		}
	}

	//join all the threads
	numberOfPatternDTWDistancesToCompute = workerThreads.size();
	for (unsigned int threadIndex = 0; threadIndex < numberOfPatternDTWDistancesToCompute; threadIndex++)
	{
		workerThreads[threadIndex]->join();
	}
}




void PatternDetectionManager::computeDTWDistanceForIndex(unsigned int patternID, unsigned int index, std::vector<CCPoint>& pattern1, std::vector<CCPoint>& pattern2)
{
	float dtwDistance = computeDTWDistance(pattern1, pattern2);

	_indexToDistances[index] = dtwDistance;

	if(_indexToDistances.size() == numberOfPatternDTWDistancesToCompute)
	{
		float lowestDTWDistance = INT_MAX;
		std::map<unsigned int, float>::iterator indexToDistanceIterator;
		for(indexToDistanceIterator = _indexToDistances.begin(); indexToDistanceIterator != _indexToDistances.end(); indexToDistanceIterator++)
		{
			if(indexToDistanceIterator->second < lowestDTWDistance)
			{
				lowestDTWDistance = indexToDistanceIterator->second;
			}
		}

		EventManager::Get()->dispatchEvent(ON_PATTERN_DETECTION, new PatternDetectedEventData(patternID, lowestDTWDistance));
	}
}

float PatternDetectionManager::computeDTWDistance(std::vector<CCPoint>& pattern1, std::vector<CCPoint>& pattern2)
{
	const unsigned int n = pattern1.size();
	const unsigned int m = pattern2.size();
	std::vector<std::vector<float>> dtw;

	//create the DTW table
	unsigned int nIndex;
	unsigned int mIndex;
	for(nIndex = 0; nIndex < n; nIndex++)
	{
		dtw.push_back( std::vector<float>() );
		for(mIndex = 0; mIndex < m; mIndex++)
		{
			dtw[nIndex].push_back(INT_MAX);
		}
	}

	//initialise the first value
	dtw[0][0] = 0;

	//compute the dtw table dynamically, filling the table
	//with the lowest distance values computed so far
	float cost;
	float minLastElement;
	for(nIndex = 1; nIndex < n; nIndex++)
	{
		for(mIndex = 1; mIndex < m; mIndex++)
		{
			cost = Line::getCCPointDistance(pattern1[nIndex], pattern2[mIndex]);
			float values[3] = {dtw[nIndex-1][mIndex],		// insertion
							   dtw[nIndex][mIndex-1],		// deletion
							   dtw[nIndex-1][mIndex-1]};	// match
			minLastElement = *std::min_element(values, values+3);
            dtw[nIndex][mIndex] = cost + minLastElement;
		}
	}

    return dtw[n-1][m-1];
}