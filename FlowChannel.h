#ifndef FLOWCHANNEL_H
#define FLOWCHANNEL_H

#include "FlowGeometry.h"
#include <iostream>
///Handles one scalar field of floats defined for each cell.
/**
* More dimensional vectors are split into components. E.g. a 3D velocity vector gets stored in three FlowChannels. A FlowChannel stores data only from one time step, it is not aware of any time related information.
*/
class FlowChannel{
    private:
        ///reference to the geometry structure
        FlowGeometry* geom;    
        ///channel data storage
        float* values;
        ///minimum value (of all cells in a single time step)
        float minimum;
        ///maximum value (of all cells in a single time step)
        float maximum;
    public:
        FlowChannel(FlowGeometry* g);
        ~FlowChannel();
        ///sets the value of the given vertex
        void setValue(int vtxID, float val);
        ///takes an array containing all attributes for a vertex and copies the j-th attribute to this channel
		/**
		* This methos is used by the loading of data sets.
		* @param rawdata data gained directly from the file, without any processing. It contains all channels for all cells. Please note, there is no time information considered here.
		* @param vtxSize number of channels per cell (incl. velocity vector size)
		* @param offset offset of the parameter loaded into this channel
		*/
        void copyValues(float* rawdata, int vtxSize, int offset);
        
		///returns the value at given position in data set coordinates (from 0 to dimX or dimY)
        float getValue(vec3 pos);
		///returns the value of the given vertex
        float getValue(int vtxID);
        ///returns the value at given position in normalized coordinates for each dimension <0..1>
        float getValueNormPos(vec3 pos);
        ///returns the value at given position in normalized coordinates for each dimension <0..1>
		float getValueNormPos(float x, float y);
        
		///scales the value according to the channel minimim and maximum, so that it lies inside of <0,1>
        float normalizeValue(float val);
        
		///returns the minimum value found in the channel
        float getMin();
        ///returns the maximum value found in the channel
		float getMax();
		///returns the range = max - min
		float getRange();
};
#endif
