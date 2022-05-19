@echo off

REM Example of usage: generate c:\vcpkg ../protos hello.proto

REM Path to your vcpkg repository
set VCPKG_PATH="%1"
REM Path to the proto directory (e.g. ../protos)
set PROTOS_PATH="%2"
REM Name of the proto to generate code for (e.g. hello.proto)
set PROTO_TO_GENERATE=%3
REM Path to destination directory
set OUT_PATH=..\generated

set PATH="%PATH%";%VCPKG_PATH%\installed\x64-windows\tools\protobuf;%VCPKG_PATH%\installed\x64-windows\tools\grpc

REM First, we generate code for gRPC stuff
protoc -I %PROTOS_PATH% --grpc_out=generate_mock_code=true:%OUT_PATH% --plugin=protoc-gen-grpc=%VCPKG_PATH%\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe %PROTO_TO_GENERATE%

REM Then, we generate code for ordinary protobuf-only messages
protoc -I %PROTOS_PATH% --cpp_out=%OUT_PATH% %PROTO_TO_GENERATE%