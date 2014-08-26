#ifndef _startKinect_H_
#define _startKinect_H_

//*********************************System headers**********************************//
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


//*******************************ROS specific Headers******************************//
#include "ros/ros.h"
//***********************************Other Headers*********************************//

#include "cv_bridge/cv_bridge.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image_transport/image_transport.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/image_encodings.h"
#include <json/json.h>
#include <k2_client/Audio.h>
#include <k2_client/BodyArray.h>
#include <camera_info_manager/camera_info_manager.h>

class Socket
{
public:
	struct sockaddr_in inAddress;				//IP and port container for easy readability
	struct sockaddr socketAddress;				//IP and port container for the code
	struct addrinfo hints, *response, *iterator;//hint specifies what options to look for
												//response is a linked list that getaddrinfo fills for us
												//iterator is later on used to go through each of the response
	int mSocket;
	int mBufferSize;
	char *mBuffer;
	Socket(char address[],char portNum[],int bufferSize);
	void readData();
private:

};


Socket::Socket(char address[],char portNum[],int bufferSize)
{
	memset(&(this->hints), 0, sizeof (this->hints));
	this->hints.ai_family = AF_INET;
	this->hints.ai_socktype = SOCK_STREAM;
	this->mBufferSize = bufferSize;
	this->mBuffer = new char[this->mBufferSize];
	getaddrinfo(address,portNum,&(this->hints),&(this->response));
	for(this->iterator = this->response;this->iterator!=NULL;this->iterator = this->iterator->ai_next)
	{
		if(iterator->ai_family != AF_INET)
		{
			std::cout<<"Please disable IPv6"<<std::endl;
			return;
		}
		struct sockaddr_in *address = (struct sockaddr_in *)(iterator->ai_addr);
		char ipString[INET_ADDRSTRLEN];
		inet_ntop(iterator->ai_family,&(address->sin_addr),ipString,sizeof ipString);
		this->mSocket = socket(this->response->ai_family,this->response->ai_socktype,this->response->ai_protocol);
		if(this->mSocket !=-1)
		connect(this->mSocket,this->response->ai_addr,this->response->ai_addrlen);
	}
}

void Socket::readData()
{
	memset(this->mBuffer,0,this->mBufferSize);
	recv(this->mSocket,this->mBuffer,this->mBufferSize,MSG_WAITALL);
}

#endif