# VNX Middleware

VNX stands for "Virtual Network Anything" which is the foundation of its architechture, borrowing existing concepts from real networks such as Ethernet and applying them to communication between processes and threads.

VNX is a light-weight middleware or software framework similar to ROS (Robot Operating System) and DDS (Data Distribution Service) with a focus on simplicity, ease of use, development efficiency and runtime performance. An important design feature is the absence of external dependencies, only a C++11 capable compiler and a POSIX compliant system is needed, as such it can be used on almost any modern hardware.

## Architecture

When working with VNX one has modules which run inside their own thread communicating via publish/subscribe and request/response with other modules running either in the same process or another process on the same machine or on a different machine. Routers are used to bridge the gap between processes (via UNIX domain sockets) and machines (via TCP/IP). Communication inside the same process is zero-copy and read-only.

The strict pairing of one thread for each module avoids having to write thread-safe code which avoids bugs like race conditions and dead locks by design. This is a key concept for safety critical systems.

Modules do not have to know where their data is coming from or where their output is going to, this is the job of the router design which is done by the user. In other words "discovery" is static, althought it can be made dynamic using the framework itself.

## VNI

The code generator is used to generate code for serializing and de-serializing of data based on interface type definitions. Type information is shared on the wire and thus for example automatically recorded and played back.

The code generator reads interface type definitions written in "VNI" (Virtual Network Interface) with a file extension of "*.vni". The language is similar to Java in its design, types are separated into packages and each type is contained in its own file with a matching file name.

## Modules

Modules can subscribe to topics to receive data, publish results as well as register themselves as a service and receive requests.

Each module has a main() function that is executed in its own thread. The only way to access a module is before it has been started and after that only by sending requests.

Modules are defined in the interface to generate a base class from which the user can inherit to implement callback functions. In addition code is generated to read configuration variables as specified in the module interface description. Also a client class is generated to send requests to said module and receive a response.

## Topics

A Topic is defined by its name which is a string that defines a leaf node inside a tree, just like a file path but with "." instead of "/" seperating the levels. For example: "sensors.raw\_data.Camera\_A".
Multiple different types can be published on the same topic without them being specified beforehand.

Modules can subscribe to specific topics as well as higher level nodes like "sensors.raw\_data" or even "sensors", it will then receive all samples published in that sub-tree.

Modules can subscribe to any number of topics as well as publish samples on any topic. Subscriptions are reference counted, which means if a module subscribes twice to the same topic it needs to unsubscribe twice to stop receiving samples on said topic.

Moreover, modules can subscribe and unsubscribe dynamically at any point in time.

Topics are global within a process but need to be imported and/or exported from/to other processes. This is achieved with the vnx::Proxy, vnx::Server and vnx::Router.

