#pragma once

struct MemoryUnit
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;

	const bool isRecv;	// MemoryUnit은 항상 false이기 때문에 False. true == recv, false == send

	char* dataBuf;

public:
	MemoryUnit(const bool InIsRecv = false);
	~MemoryUnit();

	//MemoryUnit(const MemoryUnit& other);
	//MemoryUnit(MemoryUnit&& other) noexcept;
	//MemoryUnit& operator=(MemoryUnit&& other) noexcept;
};