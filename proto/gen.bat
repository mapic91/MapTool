@echo off
cls
protoc --cpp_out="../" TmxMapMessage.proto
protoc --csharp_out="../TmxReadHelper/Proto" TmxMapMessage.proto
pause
