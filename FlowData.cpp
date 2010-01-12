#include "FlowData.h"
#include <math.h>
#include "reverseBytes.h"

#include <qgl.h>
#include <QDebug>

FlowData::FlowData()
{
    //mark all the channel slots as free
    for(int i = 0; i < max_channels; i++)
    {
		channels[i] = NULL;
		freeChannel[i] = true;   
    }
}

FlowData::~FlowData()
{
	//delete all the channels
	for(int i = 0; i < max_channels; i++)
		if (!freeChannel[i])
			deleteChannel(i);
}

bool FlowData::loadDataset(string filename, bool bigEndian)
{
	FILE* griFile = NULL;
	FILE* datFile = NULL;	
    char header[40];

	//localize the last dot in the filename
	int lastdot = filename.find_last_of(".",filename.length()-1);
	if (lastdot != string::npos)
		//if there is a dot, remove everything behind it
		filename = filename.substr(0,lastdot);	

	/////////////
	// GRID FILE
	/////////////
	string griName = filename+".gri";

	qDebug() << "- Loading grid file '" << griName.c_str();
	std::cout << "- Loading grid file '" << griName << "' ... " << std::endl;
	
    //open the grid file
	griFile = fopen(griName.c_str(),"rb");

	if (!griFile)
	{
		std::cerr << "+ Error loading grid file:" << griName << std::endl << std::endl;
		qDebug() << "+ Error loading grid file:" << griName.c_str();
		return false;
	}
	//save the header
	fread(header,40,1,griFile);
	//pass the grid file to the geometry class to process it
	if (!geometry.readFromFile(header,griFile,bigEndian))
		return false;
	//close the file
	fclose(griFile);
	 
	int dimX,dimY,dimZ,numChannels;
	float DT;	 
	//read some neceassry data from the header
	sscanf(header,"SN4DB %d %d %d %d %d %f",&dimX,&dimY,&dimZ,&numChannels,&timesteps,&DT);
	printf("Channels: %d\nTimesteps: %d\n",numChannels,timesteps);

	qDebug() << "Channels: " << numChannels;
	qDebug() << "Timesteps: " << timesteps;

	/////////////
	// DAT FILE
	/////////////
	//since this framework handles only one timestep, we use only a single dat file
	char suffix[16];
	sprintf(suffix,".%.5u.dat",0); //the second dot and the following 5 specify that a minimum of 5 numbers will be written
	string datName = filename.append(suffix);
	std::cout << "- Loading grid file '" << datName << "' ... " << std::endl;
	qDebug() << "- Loading grid file '" << datName.c_str();
	//open the dat file
	datFile = fopen(datName.c_str(),"rb");
	if (!datFile)
	{
		std::cerr << "+ Error loading dat file:" << datName << std::endl << std::endl;
		qDebug() << "+ Error loading dat file:" << datName.c_str();
		return false;
	}
	//let's prepare the channels
	numChannels += 3; //add the 3 components of the velocity vector to the number of additional chanenls
	int* ch = new int[numChannels]; //create a storage for addresses our channels

	//because reading big chunks of data is much faster than single values, 
	//we read the data into a temporary array and then copy it to the channels
	float* tmpArray = new float[numChannels*geometry.getDimX()*geometry.getDimY()]; //create temporary storage
	int result = fread(tmpArray,sizeof(float),numChannels*geometry.getDimX()*geometry.getDimY(),datFile); //read the data
	//have we read the whole data file?
	if (result != numChannels*geometry.getDimX()*geometry.getDimY())
	{
		std::cerr << "+ Error reading dat file:" << datName << std::endl << std::endl;
		qDebug() << "+ Error reading dat file:" << datName.c_str();
		return false;
	}
	//close the file, it is no longer needed
	fclose(datFile);

	//if swap the byte order, if the file is encoded big-endian
	if (bigEndian)
		for(int j = 0; j < numChannels*geometry.getDimX()*geometry.getDimY(); j++)
			tmpArray[j] = reverseBytes<float>(tmpArray[j]);
	//assign the data to the appropriate channels
	for (int j = 0; j < (numChannels); j++)
	{
		//create the new channel
		ch[j] = createChannel();
		//copy the values of the jth channel from tmpArray, which carries numChannels    
		channels[ch[j]]->copyValues(tmpArray,(numChannels),j);
	}

	int vel = createChannelVectorLength(0,1,2);

	qDebug() << "TEST: " << getChannel(vel)->getValueNormPos(vec3(0.5,0.5));
	qDebug() << "TEST2: " << getChannel(3)->getValueNormPos(vec3(0.5,0.5));
	qDebug() << "TEST3: " << getChannel(4)->getValueNormPos(vec3(0.5,0.5));

	delete[] ch;
	delete[] tmpArray;
	return true;
}

int FlowData::createChannel()
{
    //find the first unused channel slot
    int i = 0;
    while ((!freeChannel[i])&&(i < max_channels)) i++;
    //if there is a free slot
	if (i < max_channels) 
    {
        std::cout << "Creating channel at " << i << " ... ";
		qDebug() << "Creating channel at " << i;
        //create a new channel
        channels[i] = new FlowChannel(&geometry);
        //remember the slot
        freeChannel[i] = false;
        //return the adress of the new channel
        return i;
    }
    //there is no free channel
    else 
    {
        return -1;
        std::cerr << "There is no free channel slot!" << std::endl;
    }
}

void FlowData::deleteChannel(int i)
{
	//if the address is really occupied
    if (!freeChannel[i])
    {
        std::cout << "Deleting channel at " << i << " ... ";
        //delete the channel instance
        delete channels[i];
        channels[i] = NULL;
        //free the slot
        freeChannel[i] = true;
    }
    else std::cout << "Tried to delete a non-existing channel at " << i << "." << std::endl;
}

FlowChannel* FlowData::getChannel(int i)
{
    return channels[i];
}

int FlowData::createChannelGeometry(int dimension)
{
    int result = createChannel();
	//just take the dimension as if it was an offset to the geometryData array
    channels[result]->copyValues((float*)geometry.geometryData, 3, dimension);
    return result;
}

int FlowData::createChannelVectorLength(FlowChannel* chX, FlowChannel* chY, FlowChannel* chZ)
{
    int result = createChannel();
    //check whether we deal with 2D or 3D vectors
	if (chZ)
        for (int i = 0; i < geometry.getDimX()*geometry.getDimY(); i++)
			//save the vector length
            channels[result]->setValue(i,sqrt(chX->getValue(i)*chX->getValue(i) + chY->getValue(i)*chY->getValue(i) + chZ->getValue(i)*chZ->getValue(i)));
    else
        for (int i = 0; i < geometry.getDimX()*geometry.getDimY(); i++)
            channels[result]->setValue(i,sqrt(chX->getValue(i)*chX->getValue(i) + chY->getValue(i)*chY->getValue(i)));
 
    return result;
}

int FlowData::createChannelVectorLength(int chX, int chY, int chZ)
{
	//just a wrapper for the method above
	if (chZ >= 0)
		return createChannelVectorLength(getChannel(chX),getChannel(chY),getChannel(chZ));
	else return createChannelVectorLength(getChannel(chX),getChannel(chY));
}

int FlowData::getNumTimesteps()
{
	return timesteps;
}

FlowGeometry* FlowData::getGeometry() {
	return &geometry;
}