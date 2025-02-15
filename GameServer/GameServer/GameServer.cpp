#include "pch.h"
#include <iostream>

std::shared_ptr<GameSession> CreateGameSession()
{
	return std::make_shared<GameSession>();
}

int main()
{
	HelloWorld(); // IOCPCore 함수 가져오기 테스트

    //std::shared_ptr<ServerService> service = std::make_shared<ServerService>(
    //    NetAddress(L"127.0.0.1", 7777),
    //    std::make_shared<IocpCore>(),
    //    CreateGameSession, // 함수 포인터 전달
    //    100);

}