#include "FlowGeometry.h"
#include "reverseBytes.h"

#include <QDebug>

bool FlowGeometry::readFromFile(char* header, FILE* fp, bool bigEndian)
{
	isFlipped = false;
	//determine the dimensions
    sscanf(header,"SN4DB %d %d %d",&dim[0],&dim[1],&dim[2]);
    std::cout << "Dimensions: " << dim[0] << " x " << dim[1] << " x " << dim[2] << std::endl;
    if (dim[2]!=1)
    {
        std::cerr << "Invalid Z dimension value." << std::endl;
        return false;
    }

	geometryData = new vec3[dim[0]*dim[1]];
	//read the data and check if everything went fine
    int result = fread(geometryData,sizeof(vec3),dim[0]*dim[1],fp);
	if (result != dim[0]*dim[1])
	{
		std::cerr << "+ Error reading grid file." << std::endl << std::endl;
		return false;
	}

	if (bigEndian)
		for (int j = 0; j < getDimX()*getDimY(); j++)
			for (int k = 0; k < 3; k++)
				geometryData[j][k] = reverseBytes<float>(geometryData[j][k]);

    //first vertex
	boundaryMin = vec3(getPos(0));
	//last vertex
	boundaryMax = vec3(getPos((dim[0]*dim[1]) - 1));
	boundarySize = boundaryMax - boundaryMin;

    //if X and Y are swapped... meaning that going in a row the y value increases and x stays the same
    if (getPosY(dim[0]-1)>(boundaryMin[1] + boundaryMax[1])*0.5)
    {
		//this flag will fix that, anywhere where it's neccerasy... watch it :)
        isFlipped = true;
        std::cout << "Flipped Y and X dimensions." << std::endl;
    }
    else isFlipped = false;        

    std::cout << "X Boundaries: " << boundaryMin[0] << " ... " << boundaryMax[0] << std::endl;
    std::cout << "Y Boundaries: " << boundaryMin[1] << " ... " << boundaryMax[1] << std::endl;
    
	qDebug() << "X Boundaries - Min: " << boundaryMin[0];
	qDebug() << "X Boundaries - Max: " << boundaryMax[0];
	qDebug() << "Y Boundaries - Min: " << boundaryMin[1];
	qDebug() << "Y Boundaries - Max: " << boundaryMax[1];

	for (int j = 0; j < getDimX()*getDimY(); j++) {
		geometryData[j][0] = (geometryData[j].v[0] - boundaryMin.v[0]) / boundarySize.v[0];
		geometryData[j][1] = (geometryData[j].v[1] - boundaryMin.v[1]) / boundarySize.v[1];
	}

	return true;
}

FlowGeometry::FlowGeometry()
{
    geometryData = NULL;
}

FlowGeometry::~FlowGeometry()
{
    if (geometryData)
        delete[] geometryData;
}

///returns X index of the last vertex lying left to the position x and the Y index of the last vertex lying under the position y 
int FlowGeometry::getXYvtx(vec3 pos)
{
    int i;
	int j;
	//search for the column left to the vertex
    for (i = 0; (i < dim[0])&&(getPosX(getVtx(i,0)) < pos[0]); i++);
	//search for the row under the vertex
	for (j = 0; (j < dim[1])&&(getPosY(getVtx(0,j)) < pos[1]); j++);
	
	//return the vertex ID of the found vertex
	return getVtx((i<dim[0]) ? i : dim[0]-1, (j<dim[1]) ? j : dim[1]-1);
}

inline vec3 FlowGeometry::getPos(int vtxID)
{
	return geometryData[vtxID];
}

inline float FlowGeometry::getPosX(int vtxID)
{
	return geometryData[vtxID][0];
}

inline float FlowGeometry::getPosY(int vtxID)
{
	return geometryData[vtxID][1];
}

//TODO students: improve this
///a very slow and dumb routine, that finds the nearest vertex to the given position
int FlowGeometry::getNearestVtx(vec3 pos)
{
	//take the norm to the first vertex
	float dist = pos.dist2(getPos(0));
	//mark the vertex 0 as the closest one
	int closest = 0;
	float newd;
	//Iterate through all vertices and search for the nearest one. I know, this is so inefficient, that you'll have to rewrite it :P
	for (int i = 1; i < dim[0]*dim[1]; i++)
	{
		newd = pos.dist2(getPos(i));
		if (newd < dist)
		{
			dist = newd;
			closest = i;
		}
	}
	return closest;
}

bool FlowGeometry::getInterpolationAt(vec3 pos, int* vtxID, float* coef)
{
    //if we are outside of the dataset, return false
	//please note, that this test is valid only for rectangular datasets (block, hurricane), but not for tube
    if ((pos[0]<boundaryMin[0])||(pos[1]<boundaryMin[1])||(pos[0]>boundaryMax[0])||(pos[1]>boundaryMax[1]))
        return false;

	//example of a low-quality lookup with no interpolation
	vtxID[0] = getNearestVtx(pos); //finds the nearest vertex to the given position and returns it's value as the dominanting one. The getNearestVtx needs to be improved to gain some speed.
    vtxID[1] = 0;
    vtxID[2] = 0;
    vtxID[3] = 0;        
    coef[0] = 1.0f;
    coef[1] = 0.0f;
    coef[2] = 0.0f;
    coef[3] = 0.0f;
    return true;
}

float FlowGeometry::getMinX()
{
    return boundaryMin[0];
}

float FlowGeometry::getMaxX()
{
    return boundaryMax[0];
}

float FlowGeometry::getMinY()
{
    return boundaryMin[1];
}

float FlowGeometry::getMaxY()
{
    return boundaryMax[1];
}

inline int FlowGeometry::getDimX()
{
	return (isFlipped) ? dim[1] : dim[0];
}

inline int FlowGeometry::getDimY()
{
	return (isFlipped) ? dim[0] : dim[1];
}

inline int FlowGeometry::getDimZ()
{
    return dim[2];
}

int FlowGeometry::getVtx(int x, int y)
{
	//if we need to flip the rows and columns, we do it here 
	return (isFlipped)? (x*dim[1]) + y : (y*dim[0]) + x;
}

int FlowGeometry::getVtxX(int vtxID)
{
	//if we need to flip the rows and columns, we do it here
	return (isFlipped)? vtxID / dim[1] : vtxID % dim[0];
}

int FlowGeometry::getVtxY(int vtxID)
{
	//if we need to flip the rows and columns, we do it here
	return (isFlipped)? vtxID % dim[1] : vtxID / dim[0];
}

int FlowGeometry::getRightNeigh(int vtxID)
{
    int x = getVtxX(vtxID);
    return (x+1 < dim[0]) ? getVtx(x+1,getVtxY(vtxID)) : -1;
}

int FlowGeometry::getTopNeigh(int vtxID)
{
	//remember that the data is structured with (0,0) in the upper-left corner and (1,1) in the lower-right
	//that's why we are subtracting 1 to find the top neighbour
    int y = getVtxY(vtxID);
    return (y+1 < dim[1]) ? getVtx(getVtxX(vtxID), y-1) : -1;
}

int FlowGeometry::getLeftNeigh(int vtxID)
{
    int x = getVtxX(vtxID);
    return (x > 1) ? getVtx(x-1,getVtxY(vtxID)) : -1;
}

int FlowGeometry::getBottomNeigh(int vtxID)
{
	//remember that the data is structured with (0,0) in the upper-left corner and (1,1) in the lower-right
    int y = getVtxY(vtxID);
    return (y < 1) ? getVtx(getVtxX(vtxID), y+1) : -1;
}

vec3 FlowGeometry::normalizeCoords(vec3 pos)
{
	vec3 u(pos - boundaryMin);
	//scale each component according to the side length
	u[0] /= boundarySize[0];
	u[1] /= boundarySize[1];

	return u;
}

vec3 FlowGeometry::unNormalizeCoords(vec3 pos)
{
	vec3 u(pos);
	//multiply each component according to the side length
	u[0] *= boundarySize[0];
	u[1] *= boundarySize[1];

	return u += boundaryMin;
}
