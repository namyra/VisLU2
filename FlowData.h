#ifndef FLOATDATA_H
#define FLOATDATA_H

#include "FlowGeometry.h"
#include "FlowChannel.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
//maximum nuber of channels. For VisLU data sets we have only 5 anyways
#define max_channels 16
///class managing the data sets and related stuff like data loading, channels creation etc.
class FlowData{
private:
    ///Is there any data loaded?
    bool loaded;
   
    ///Number of timesteps
    int timesteps;

    ///Stores the underlying geometry
    FlowGeometry geometry;
    
    ///is the channel slot free?
    bool freeChannel[max_channels];    
    ///stores the values of data channels for one time step. For time-dependent data, the best solution is to create a separate class handling channels in one timestep and to instanciate this class for all timesteps.
    FlowChannel* channels[max_channels];

public:
	///initializes the channel storage
    FlowData();
    ///destoys all created channels
    ~FlowData();

    ///Loads a dataset, returns true if everything successful. You have to specify the byte order used in the data
    bool loadDataset(string filename, bool bigEndian);
    
    ///Returns the number of timesteps
    int getNumTimesteps();
    
    //channels stuff
	///creates a new channel and returns it's address in the channels array (line 28)
	int createChannel();
	///deletes the channel and all it's data at given adress
    void deleteChannel(int i);
	///returns a pointer to the instance of channel at given adress. This is the only way to access the channels storage (at line 28)
	FlowChannel* getChannel(int i);
    
    //special channels creation
	///creates a new channel containing the geometrical information of the given dimension (x = 0, y = 1). Returns address of the created channel in the channels array (line 28)
    int createChannelGeometry(int dimension);
	///creates a new channel containing the vector lengths for the given channels (channels given by IDs). Returns address of the created channel in the channels array (line 28)
    int createChannelVectorLength(int chX, int chY, int chZ = -1);
	///creates a new channel containing the vector lengths for the given channels (channels given by reference). Returns address of the created channel
	int createChannelVectorLength(FlowChannel* chX, FlowChannel* chY, FlowChannel* chZ = NULL);

	FlowGeometry* getGeometry();
};
#endif
