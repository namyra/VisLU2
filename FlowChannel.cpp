#include "FlowChannel.h"
#include <math.h>

float FlowChannel::getValue(vec3 pos)
{
    //IDs of cell vertices in the neighborhood of the sampled point
    int vtxID[4];
    //weighting coefficients for interpolation
    float coef[4];
    
    //let's determine the neighbouring geometry and get the interpolation coefficients
    //this is a general scheme... there can be various interpolation schemes used (nearest, linear) inside of the getInterpolationAt. It's up to you, what you implement.
    if (geom->getInterpolationAt(pos, vtxID, coef))
        return values[vtxID[0]]*coef[0] + values[vtxID[1]]*coef[1] + values[vtxID[2]]*coef[2] + values[vtxID[3]]*coef[3];
    else
    {
        std::cerr << "Outside of the dataset" << std::endl;
        return 0;
    }
}

float FlowChannel::getValue(int vtxID)
{
    return values[vtxID];
}

///returns the value at given position in normalized coordinates for each dimension <0..1>
float FlowChannel::getValueNormPos(vec3 pos)
{
	//first scale the coordinates back
    vec3 realPos = geom->unNormalizeCoords(pos);
	//now make a lookup
    return getValue(realPos);
}

///returns the value at given position in normalized coordinates for each dimension <0..1>
float FlowChannel::getValueNormPos(float x, float y)
{
	//calls the previous method
    return getValueNormPos(vec3(x,y));
}

float FlowChannel::normalizeValue(float val)
{
	//scales the value so that minimum will be 0 and maximum 1
    return (val-minimum)/(maximum-minimum);
}

FlowChannel::FlowChannel(FlowGeometry* g)
{
    geom = g;
    //create the appropriate storage
    values = new float[geom->getDimX()*geom->getDimY()];
    minimum = HUGE_VAL;
    maximum = -HUGE_VAL;
    std::cout << "ok" << std::endl;    
}

FlowChannel::~FlowChannel()
{
	//delete the value storage
    delete[] values;
    std::cout << "ok" << std::endl;
}

void FlowChannel::setValue(int vtxID, float val)
{
    values[vtxID] = val;
	//update the minimum and maximum
    minimum = (val < minimum) ? val : minimum;
    maximum = (val > maximum) ? val : maximum;    
}

//takes an array containing all attributes for a vertex and copies the attribute specified in offset to this channel
void FlowChannel::copyValues(float* rawdata, int vtxSize, int offset)
{
    for (int i = 0; i < geom->getDimX()*geom->getDimY(); i++)
    {
        values[i] = rawdata[(i*vtxSize) + offset];
		//update the minimum and maximum
        minimum = (values[i] < minimum) ? values[i] : minimum;
        maximum = (values[i] > maximum) ? values[i] : maximum;
    }
    std::cout << "Maximum value in channel: " << maximum << std::endl;
    std::cout << "Minimum value in channel: " << minimum << std::endl;    
}

float FlowChannel::getMin()
{
    return minimum;
}

float FlowChannel::getMax()
{
    return maximum;
}

float FlowChannel::getRange()
{
    return maximum - minimum;
}

float FlowChannel::getRawValue(int i)
{
	return values[i];
}