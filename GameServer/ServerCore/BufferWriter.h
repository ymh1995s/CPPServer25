#pragma once

/*----------------
	BufferWriter
-----------------*/

class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE*			Buffer() { return _buffer; }
	uint32			Size() { return _size; }
	uint32			WriteSize() { return _pos; }
	uint32			FreeSize() { return _size - _pos; }

	template<typename T>
	bool			Write(T* src) { return Write(src, sizeof(T)); }
	bool			Write(void* src, uint32 len);

	template<typename T>
	T*				Reserve();

	/* 강의 사용 예시
		BufferWriter bw(sendBuffer->Buffer(), 4096);

		PacketHeader* header = bw.Reserve<PacketHeader>();
		// id(uint64), 체력(uint32), 공격력(uint16)
		bw << (uint64)1001 << (uint32)100 << (uint16)10;
		// bw.operator<<((uint64)1001).operator<<((uint32)100).operator<<((uint16)10);와 같다.
		bw.Write(sendData, sizeof(sendData));
	*/
	template<typename T>
	BufferWriter&	operator<<(T&& src);

private:
	BYTE*			_buffer = nullptr;
	uint32			_size = 0;
	uint32			_pos = 0;
};

template<typename T>
T* BufferWriter::Reserve()
{
	if (FreeSize() < sizeof(T))
		return nullptr;

	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return ret;
}

// {T&& + forward}완벽 전달 : 왼값이면 왼값 참조로, 오른값이면 오른값 참조로.
// T&&만 사용하면 오른값 참조
template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	using DataType = std::remove_reference_t<T>; // 참조 제거(ex. int& -> int) 버퍼에 기록할 때는 참조를 넘기지 않음
	*reinterpret_cast<DataType*>(&_buffer[_pos]) = std::forward<DataType>(src); // 완벽 전달과 함께 쓰는 forward
	_pos += sizeof(T);
	return *this;
}