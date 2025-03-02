protoc.exe -I=./ --cpp_out=./ --csharp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ --csharp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ --csharp_out=./ ./Protocol.proto
PAUSE