#!/bin/bash

cd build

emcc -c ../src/JSON.cpp ../src/Input.cpp ../src/Output.cpp ../src/Value.cpp ../src/Object.cpp ../src/Variant.cpp ../src/ToObject.cpp ../src/Visitor.cpp ../src/Type.cpp ../src/CRC64.cpp ../src/Buffer.cpp ../src/Binary.cpp ../src/Memory.cpp ../src/Topic.cpp ../src/Endpoint.cpp ../src/Exception.cpp ../src/Generic.cpp ../src/InternalError.cpp ../src/JRPC_Request.cpp ../src/JRPC_Response.cpp ../src/LogMsg.cpp ../src/ModuleInfo.cpp ../src/Return.cpp ../src/Session.cpp ../src/SHA256.cpp ../src/Time.cpp ../src/Util.cpp ../src/linux/Endpoint.cpp ../src/linux/Directory.cpp ../generated/src/*.cpp -I ../include/ -I ../include/wasm/ -I ../generated/include/

emar rcs libvnx_base.a *.o

