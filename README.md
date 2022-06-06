# hello-grpc

Demo project of my talk [A gentle introduction to gRPC](https://www.youtube.com/watch?v=uZUJOsR8pYM).

* TOC
{:toc}

## Building

In theory, you can build the sources on any supported platforms.

For simplicity, I have assembled this demo on Visual Studio 2019, relying on [vcpkg](https://vcpkg.io/) for obtaining and building all the library dependencies. So, the first thing you need to do is to [install vcpkg](https://vcpkg.io/en/getting-started.html) and then get the required libraries.

I **didn't** use [vcpkg manifest](https://vcpkg.readthedocs.io/en/latest/users/manifests/) feature so let me tell you which libraries you have to install.

As you might image, all the projects need grpc:

```
vcpkg install grpc --triplet x64-windows
```

For the project [message-broker](https://github.com/ilpropheta/hello-grpc/tree/main/message-broker), I have made use of a few extra libraries:

```
vcpkg install sobjectizer --triplet x64-windows
vcpkg install spdlog --triplet x64-windows
```

For [hello-test](https://github.com/ilpropheta/hello-grpc/tree/main/hello-test) I have installed:

```
vcpkg install gtest --triplet x64-windows
```

Finally, remember that [message-broker-client](https://github.com/ilpropheta/hello-grpc/tree/main/message-broker-client) is a .Net Core project relying on NuGet official sources.

### Project settings

A few notes on the projects. 

0. Clearly, enable vcpkg on all the projects:
    - Open any project properties
    - navigate to **vcpkg**
    - then set **Yes** on **Use Vcpkg**
  
1. Since I am insane pirate, I have turned on the latest features: 
    - Open any project properties
    - navigate to **C/C++** 
    - then to **Language**
    - finally to **C++ Language Standard** and set the last option `/std:c++latest`.

2. grpc has some compilation issues with C++17 so I have just turned off some annoying warning messages by adding `_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS` to the Preprocessor Definitions: 
    - Open any project properties
    - navigate to **C/C++**
    - finally to **Preprocessor** and add `_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS`

3. Since grpc depends on protobuf that is linked dynamically, some warnings might be emitted due to standard types crossing the dll boundaries. For this reason, I have turned them all off:
    - Open any project properties
    - navigate to **C/C++**
    - then to **Command Line**
    - finally to **Additional Options** and add `/wd4251`

4. grpc needs Winsock2 library and this is not linked automatically. I usually do not pollute the source code with non-standard directives like `#pragma comment (lib, "Ws2_32.lib")` so I have added the library by hand on the linker options:
    - Open any project properties
    - navigate to **Linker**
    - then to **Input**
    - finally to **Additional Dependencies** and add `Ws2_32.lib` (do not remove `%(AdditionalDependencies)`)

## gRPCurl usage examples

[grpcurl](https://github.com/fullstorydev/grpcurl) is a command-line tool that lets you interact with gRPC servers. It's basically curl for gRPC servers.

Download the latest release from [here](https://github.com/fullstorydev/grpcurl/releases).

All the following examples use [grpc reflection](https://github.com/grpc/grpc/blob/master/doc/server-reflection.md) (but grpcurl supports also compiled protoset files and proto sources).

- List all the services exposed at a certain endpoint:

```
grpcurl.exe -plaintext localhost:50051 list
```

- List all the rpc methods of a certain service:

```
grpcurl.exe -plaintext localhost:50051 list NameOfTheService
```

- Describe a certain rpc method:

```
grpcurl.exe -plaintext localhost:50051 describe NameOfTheService.NameOfTheMethod
```

- Call a certain rpc method (on Windows we need to escape `"` and introduce json with `"` instead of `'`):

```
grpcurl --plaintext -d "{\"messages\": [ {\"topic\" : \"Channel1\", \"content\" : \"hello\" } ]}" localhost:50051 MessageBroker/Send
```

## ghz usage examples

[ghz](https://ghz.sh/docs/intro) is a command line utility and Go package for load testing and benchmarking gRPC services.

Download the latest release from [here](https://github.com/bojand/ghz/releases).

All the following examples use [grpc reflection](https://github.com/grpc/grpc/blob/master/doc/server-reflection.md) (but ghz supports also proto sources).

- Simple unary call test:

```
ghz --insecure --call MessageBroker/Send -d "{\"messages\": [ {\"topic\" : \"Channel1\", \"content\" : \"hello\" } ]}" localhost:50051
```

- Simple unary call with 20 threads:

```
ghz --insecure --call MessageBroker/Send -d "{\"messages\": [ {\"topic\" : \"Channel1\", \"content\" : \"hello\" } ]}" -c 20 localhost:50051
```

