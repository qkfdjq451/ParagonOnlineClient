// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkConnectMNG.h"
#include "Runtime/Networking/Public/Networking.h"

int FNetworkConnectMNG::Count = 0;

FNetworkConnectMNG::FNetworkConnectMNG()
{
	threadRun = false;
	recvThread = nullptr;
	sendThread = nullptr;
	connect = false;
	//recvBufPool = Pooling<SRecvPack>::Create(100);
	Count++;
	UE_LOG(LogTemp, Warning, TEXT("Network Manager Count: %d"), Count);
}

FNetworkConnectMNG::~FNetworkConnectMNG()
{
	socket->Close();
	threadRun = false;
	connect = false;

	while (!sendQueue.empty())
	{
		sendQueue.pop();
	}

	while (!recvQueue.empty())
	{
		recvQueue.pop();
	}


	if (recvThread)
	{
		if (recvThread->joinable())
			recvThread->join();

		if (recvThread != nullptr)
		{
			delete recvThread;
		}
	}
	if (sendThread)
	{
		if (sendThread->joinable())
		{
			cv.notify_all();
			sendThread->join();
		}
		if (sendThread != nullptr)
		{
			delete sendThread;
		}
	}
	
	
	
	
	Count--;

	UE_LOG(LogTemp, Warning, TEXT("Network Manager Count: %d"), Count);
}


bool FNetworkConnectMNG::Connect(FString _ip,int _port)
{
	UE_LOG(LogTemp, Warning, TEXT("Connect Start"));
	ip = _ip;
	port = _port;

	auto addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	TArray<FString> parseIP;
	TArray<uint8> parseIP_int;

	ip.ParseIntoArray(parseIP, TEXT("."), false);

	for (auto str : parseIP)
	{
		parseIP_int.Push(FCString::Atoi(*str));
	}

	addr->SetPort(port);
	addr->SetIp(FIPv4Address(parseIP_int[0], parseIP_int[1], parseIP_int[2], parseIP_int[3]).Value);
	socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"));

	connect = socket->Connect(*addr);

	////테스트
	//{
	//	int32 sendSize=10;
	//	RecvBuffer rb = { 0 };		
	//	rb.packet.symbol = PS::TEST;
	//	rb.packet.size = 4 + 4;
	//	memcpy(rb.packet.buffer, &sendSize, sizeof(int32));
	//	bool result = socket->Send(rb.buffer, rb.packet.size, sendSize);
	//}
	
	if (connect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connect Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Connect False"));		
	}

	return connect;
}

void FNetworkConnectMNG::Run()
{
	using namespace std;

	threadRun = true;

	{
		unique_lock<mutex> lock(m);
		while (!sendQueue.empty())
		{
			sendQueue.pop();
		}
	}
	
	while (!recvQueue.empty())
	{
		recvQueue.pop();
	}


	if (recvThread != nullptr)
	{
		delete recvThread;
	}
	if (sendThread != nullptr)
	{
		delete sendThread;
	}

	recvThread =new thread([&]
	{
		static thread_local int RecvThreadCount = 0;
		RecvThreadCount++;
		UE_LOG(LogTemp, Warning, TEXT("RecvThreadCount : %d"), RecvThreadCount);
		while (threadRun)
		{
			int recvSize=0;
			//auto buffer = recvBufPool->GetObject();			
			auto buffer = make_shared<RecvBuffer>();
			bool result = socket->Recv(buffer->buffer, 1024, recvSize);

			//결과를 저장하고 queue에 넣는다.
			if (!result)
			{
				connect = false;
				threadRun = false;
			}
			recvQueue.push(std::move(buffer));
		}		
		RecvThreadCount--;
		UE_LOG(LogTemp, Warning, TEXT("RecvThreadCount : %d"), RecvThreadCount);
		return;
	});

	sendThread =new thread([&]
	{
		static thread_local int sendThreadCount = 0;
		sendThreadCount++;
		UE_LOG(LogTemp, Warning, TEXT("SendThreadCount : %d"), sendThreadCount);
		std::shared_ptr<SendBuffer> sb = nullptr;
		while (threadRun)
		{
			static int sendTry = 0;
			//보낼 데이터가 없으면 Queue에 데이터가 있는지 체크한다.
			if(sb==nullptr)
			{
				unique_lock<mutex> lock(m);				
				cv.wait(lock, [this] {return !sendQueue.empty() || connect==false; });

				if (connect)
				{
					sb = sendQueue.front();
					sendQueue.pop();
				}
				//연결이 끊겼으면 queue 비우기
				else
				{
					while (!sendQueue.empty())
					{
						sendQueue.pop();
					}
				}
			}

			if (connect)
			{
				int32 sendSize = 0;

				UE_LOG(LogTemp, Warning, TEXT("Send Size : %d"), sb->buffer.packet.size);
				bool result = socket->Send(&sb->buffer.start_pointer, sb->buffer.packet.size, sendSize);
				UE_LOG(LogTemp, Warning, TEXT("Complete Send : %d"), sendSize);
				//정상적으로 보내지지 않았을 때
				if (result == false)
				{
					sendTry++;
					if (sendTry >= 10)
					{
						connect = false;
						threadRun = false;
						UE_LOG(LogTemp, Warning, TEXT("Thread Close"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Send Try : %d"), sendTry);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));					
				}
				//정상적으로 보내졌을 때
				else
				{
					sb->com_sendSize += sendSize;
					if (sb->com_sendSize == sendSize)
						sb = nullptr;
					sendTry = 0;
					UE_LOG(LogTemp, Warning, TEXT("Send Success"));
				}
			}			
		}	
		sendThreadCount--;
		UE_LOG(LogTemp, Warning, TEXT("SendThreadCount : %d"), sendThreadCount);
		return;
	});
}

std::shared_ptr<flatbuffers::FlatBufferBuilder> FNetworkConnectMNG::FBB_Make_Shared()
{
	return std::make_shared<flatbuffers::FlatBufferBuilder>();
}

void FNetworkConnectMNG::SendReq(const PS & symbol, std::shared_ptr<flatbuffers::FlatBufferBuilder> fbb)
{	
	std::unique_lock<std::mutex> lock(m);
	sendQueue.push(std::make_shared<SendBuffer>(symbol,std::move(fbb)));
	cv.notify_all();
}

void FNetworkConnectMNG::SendReq(const PS & symbol, void * buffer_pointer, short buffersize)
{
	std::unique_lock<std::mutex> lock(m);
	sendQueue.push(std::make_shared<SendBuffer>(symbol, buffer_pointer,buffersize));
	cv.notify_all();
}

void FNetworkConnectMNG::SendReq(const PS & symbol)
{
	std::unique_lock<std::mutex> lock(m);
	sendQueue.push(std::make_shared<SendBuffer>(symbol));
	cv.notify_all();
}

bool FNetworkConnectMNG::IsRecvDataEmpty()
{
	return recvQueue.empty();
}

std::shared_ptr<RecvBuffer> FNetworkConnectMNG::GetRecvData()
{
	auto retval = recvQueue.front();
	recvQueue.pop();
	return retval;
}
