// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core.h"
//#include "UObject/NoExportTypes.h"
#include <mutex>
#include <thread>
#include <memory>
#include <queue>
#include <chrono>
#include <condition_variable>
#include <flatbuffers/flatbuffers.h>
#include "./flatbuffers/FBHeader.h"



//#include "NetworkConnectMNG.generated.h"

/**
 * 
 */
//UCLASS()
#define BUFFERSIZE 2048

union RecvBuffer
{
	uint8 buffer[BUFFERSIZE];
	struct RecvPacket
	{
		unsigned short size;
		PS symbol;
		uint8 buffer;
	} packet;
};

struct SendBuffer
{
	SendBuffer(const PS& symbol, std::shared_ptr<flatbuffers::FlatBufferBuilder> fbb)
	{
		com_sendSize = 0;
		buffer.packet.size = fbb->GetSize() + sizeof(unsigned short) + sizeof(PS);

		buffer.packet.symbol = symbol;
		memcpy(buffer.packet.data_buffer, fbb->GetBufferPointer(), fbb->GetSize());
	}

	SendBuffer(const PS& symbol, void* buffer_pointer, short buffersize)
	{
		com_sendSize = 0;
		buffer.packet.size = buffersize + sizeof(unsigned short) + sizeof(PS);

		buffer.packet.symbol = symbol;
		memcpy(buffer.packet.data_buffer, buffer_pointer, buffersize);
	}

	SendBuffer(const PS& symbol)
	{
		com_sendSize = 0;
		buffer.packet.size = sizeof(unsigned short) + sizeof(PS);
		buffer.packet.symbol = symbol;
	}
	union SendPacket
	{
		uint8 start_pointer;
		struct Packet
		{
			unsigned short  size;
			PS symbol;
			uint8 data_buffer[BUFFERSIZE];
		} packet;
	} buffer;

	unsigned short com_sendSize;
};

class NETWORKMNG_API FNetworkConnectMNG //: public UObject
{
	//GENERATED_BODY()
public:
	FNetworkConnectMNG();
	~FNetworkConnectMNG();

	bool Connect(FString _ip=TEXT("127.0.0.1"), int _port=8888);
	void Run();


	bool IsRecvDataEmpty();
	//Pool<SRecvPack>* GetRecvData();
	std::shared_ptr<RecvBuffer> GetRecvData();
	std::shared_ptr<flatbuffers::FlatBufferBuilder> FBB_Make_Shared();

	bool GetConnect() { return connect; }
	void SendReq(const PS& symbol, std::shared_ptr<flatbuffers::FlatBufferBuilder> fbb);
	void SendReq(const PS& symbol, void* buffer_pointer, short buffersize);
	void SendReq(const PS& symbol);
private:
	class FSocket* socket;

	//std::queue<Pool<SRecvPack>*> recvQueue;
	std::queue<std::shared_ptr<RecvBuffer>> recvQueue;
	std::queue<std::shared_ptr<SendBuffer>> sendQueue;

	bool connect;
	FString ip;
	int port;

	std::condition_variable cv;
	std::mutex m;
	std::thread* recvThread;
	std::thread* sendThread;
	bool threadRun;	 

public:
	static int Count;
};
