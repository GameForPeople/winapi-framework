#include "stdafx.h"

#include "PacketType.h"
#include "NetworkManager.h"

NetworkManager::NetworkManager(const std::string_view& inIPAddress)
	: ipAddress(inIPAddress)
{
	InitNetwork();
}

void NetworkManager::InitNetwork()
{
	using namespace GLOBAL_UTIL::ERROR_HANDLING;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) ERROR_QUIT(L"WSAStartup()");

	if (this->socket = ::socket(AF_INET, SOCK_STREAM, 0)
		; socket == INVALID_SOCKET) ERROR_QUIT(L"socket()");

	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	serverAddr.sin_port = htons(SERVER_PORT);

	if(int retVal = connect(socket, (SOCKADDR *)&serverAddr, sizeof(serverAddr))
		; retVal == SOCKET_ERROR) ERROR_QUIT(L"bind()");
}

std::pair<UINT8, UINT8> NetworkManager::SendMoveData(const DIRECTION inDirection)
{
	dataBuffer[0] = static_cast<BYTE>(PACKET_TYPE::MOVE);
	dataBuffer[1] = static_cast<BYTE>(inDirection);

	if (int retVal = send(socket, dataBuffer, 2, NULL)
		; retVal == SOCKET_ERROR)	GLOBAL_UTIL::ERROR_HANDLING::ERROR_QUIT(L"Send에 실패하였습니다. 클라이언트를 종료합니다. \n");

	std::cout << "[Send] Move Packet를 전송했습니다. : " << [/*void*/](const DIRECTION dir) noexcept -> std::string
	{
		if (dir == DIRECTION::LEFT) return "LEFT";
		if (dir == DIRECTION::RIGHT) return "RIGHT";
		if (dir == DIRECTION::UP) return "UP";
		if (dir == DIRECTION::DOWN) return "DOWN";

	}(inDirection)
		<< "\n";

	//BLOCK...!
	// 아 바로 받는 이런거...너무 멍청한데...
	if (int retVal = recv(socket, dataBuffer, 2, NULL)
		; retVal == SOCKET_ERROR)	GLOBAL_UTIL::ERROR_HANDLING::ERROR_QUIT(L"Recv에 실패하였습니다. 클라이언트를 종료합니다. \n");

	std::cout << "[RECV] Packet를 전송받았습니다. : " << (int)(dataBuffer[0]) <<" X : " << (int)(GLOBAL_UTIL::BIT_CONVERTER::GetLeft4Bit(dataBuffer[1])) << " , y : " << (int)(GLOBAL_UTIL::BIT_CONVERTER::GetRight4Bit(dataBuffer[1])) << "\n";

	return std::make_pair(GLOBAL_UTIL::BIT_CONVERTER::GetLeft4Bit(dataBuffer[1]) , GLOBAL_UTIL::BIT_CONVERTER::GetRight4Bit(dataBuffer[1]));
}