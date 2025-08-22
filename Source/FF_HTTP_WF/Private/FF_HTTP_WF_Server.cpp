// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_HTTP_WF_Server.h"

// Sets default values
AHTTP_Server_WF::AHTTP_Server_WF()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned.
void AHTTP_Server_WF::BeginPlay()
{
	Super::BeginPlay();
	this->HTTP_Server_Start();
}

// Called when the game end or when destroyed.
void AHTTP_Server_WF::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	this->HTTP_Server_Stop();
	Super::EndPlay(EndPlayReason);
}

// Called every frame.
void AHTTP_Server_WF::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AHTTP_Server_WF::HTTP_Server_Start()
{
	auto Callback = [this](WFHttpTask* Task)
		{
			UHttpRequestWf* Request = NewObject<UHttpRequestWf>();
			Request->Task = Task;

			this->OnHttWf_Request(Request);
			this->Delegate_Http_Request.Broadcast(Request);
		};

	try
	{
		this->WF_Server = MakeShared<WFHttpServer, ESPMode::ThreadSafe>(Callback);
		const int Result = this->WF_Server->start(this->Port_HTTP);

		if (Result != 0)
		{
			return false;
		}

		this->bIsServerStarted = true;
		this->Delegate_HTTP_Start.Broadcast();
		this->OnHttWf_Start();
		
		return true;
	}

	catch (const std::exception& Exception)
	{
		FString ExceptionString = Exception.what();
		UE_LOG(LogTemp, Warning, TEXT("FF HTTP WF : Thread->Callback_HTTP_Start : %s"), *ExceptionString);

		return false;
	}
}

void AHTTP_Server_WF::HTTP_Server_Stop()
{
	if (this->WF_Server.IsValid() && this->bIsServerStarted)
	{
		this->WF_Server->stop();
		this->Delegate_HTTP_Stop.Broadcast();
		this->OnHttWf_Stop();
	}
}

FString AHTTP_Server_WF::GetListenAddress()
{
	if (!this->WF_Server.IsValid() && !this->bIsServerStarted)
	{
		return "";
	}

	sockaddr SocketAddress;
	memset(&SocketAddress, 0, sizeof(SocketAddress));
	socklen_t SocketLenght = sizeof(SocketAddress);
	this->WF_Server->get_listen_addr(&SocketAddress, &SocketLenght);

	char IP_Address[NI_MAXHOST];
	char Port[NI_MAXSERV];
	const int ErrorCode = getnameinfo(&SocketAddress, sizeof(SocketAddress), IP_Address, sizeof(IP_Address), Port, sizeof(Port), NI_NUMERICHOST | NI_NUMERICSERV);

	if (ErrorCode != 0)
	{
		return "";
	}

	FString IpString;
	FString PortString;

	IpString.AppendChars(IP_Address, sizeof(IP_Address));
	PortString.AppendChars(Port, sizeof(Port));
	const FString TempAddress = FString::Printf(TEXT("%s:%s"), *IpString, *PortString);

	return TempAddress;
}