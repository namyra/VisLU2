#ifndef FLOWGEOMETRY_H
#define FLOWGEOMETRY_H

#include <stdio.h>
#include <iostream>
#include "vec3.h"
///class for handling the geometry == rectangular grids organized in vertices and cells
class FlowGeometry{
		friend class FlowData;
	private:
		///resolution of the data for the dimensions X, Y
		int dim[2]; 
		///minimum boundary values for the dataset geometry sotred as {minX, minY)
		vec3 boundaryMin;
		///maximum boundary values for the dataset geometry sotred as (maxX, maxY)
		vec3 boundaryMax;
		///boundary sizes for the dataset geometry sotred as (maX - minX, maxY - minY)
		vec3 boundarySize;
	    
		///Storage for the geometry
		vec3* geometryData;

		///returns general vtxID for the vertex array indexes
		int getVtx(int x, int y);
		///returns X index for the general vtxID
		int getVtxX(int vtxID);
		///returns Y index for the general vtxID
		int getVtxY(int vtxID);
	    
		///returns X index of the last vertex lying left to the position x and the Y index of the last vertex lying under the position y 
		int getXYvtx(vec3 pos);

		///returns the position of the vertex
		vec3 getPos(int vtxID);
		///returns the x position of the vertex
		float getPosX(int vtxID);
		///returns the y position of the vertex
		float getPosY(int vtxID);    
	    
		///indicates whether the x and y axes have to be swaped
		bool isFlipped;

		//TODO for students: improve this
		///a very slow and dumb routine, that finds the nearest vertex to the given position
		int getNearestVtx(vec3 pos);
		
	public:
		FlowGeometry();
		///deletes the allocated geometry storage
		~FlowGeometry();
	    
		///Returns true if inside. Stores the vertex indices and interpolation weights for the given position in the arrays
		/**
		* Stores the indexes and weights of vertices surrounding the given position. This information can be used later on for interpolation of channel values.
		* @param pos geometrical position for the lookup
		* @param vtxID list of surrounding vertices (given by vertex ID)
		* @param coef list of surrounding vertex weights (sum == 1.0)
		* @return true if the given position is inside of the geometrical boundaries
		*/
		bool getInterpolationAt(vec3 pos, int* vtxID, float* coef);
	        
		///reads the geometry gris data from a file
		bool readFromFile(char* header, FILE* fp, bool bigEndian);
	    
		//remember that our grids are curvilinear and only 2D
		///returns the number of vertices in X dimension
		int getDimX();
		///returns the number of vertices in Y dimension
		int getDimY();
		///returns the number of vertices in Z dimension, is always 1
		int getDimZ();
	    
		///returns the minimum in the X dimension
		float getMinX();
		///returns the maximum in the X dimension
		float getMaxX();
		///returns the minimum in the Y dimension
		float getMinY();
		///returns the maximum in the Y dimension
		float getMaxY();
	    
		///returns the vertex ID of the neighbour on its right
		int getRightNeigh(int vtxID);
		///returns the vertex ID of the neighbour on its top
		int getTopNeigh(int vtxID);
		///returns the vertex ID of the neighbour on its left
		int getLeftNeigh(int vtxID);
		///returns the vertex ID of the neighbour on its bottom
		int getBottomNeigh(int vtxID);
	    
		///compression of coordinates in each dimension separately, returns values scaled to <0,1>
		vec3 normalizeCoords(vec3 pos);
		///inverts the compression. From values of <0,1> it restores the real geometrical coordinates
		vec3 unNormalizeCoords(vec3 pos);
};

#endif
