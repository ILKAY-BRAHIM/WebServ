# WEBSERV-1.0

# HTTP server

At its simplest, when a browser wants a file from a web server, it asks for the file using HTTP. Once the request reaches the appropriate hardware web server, the software-based HTTP server acknowledges the request, locates the requested document, and then sends it back to the browser, all using the HTTP protocol.

HTTP servers commonly utilize TCP (Transmission Control Protocol) for communications. TCP provides a reliable and connection-oriented communication channel, ensuring the secure and orderly transfer of data between the server and clients in the HTTP ecosystem.

## TCP socket programming

Socket : mechanism to give programs access to the network

### **socket -- create a socket**

The **`socket()`** function is part of the [Berkeley](https://www.notion.so/Berkeley-28a34ab345ec4b9bb8dc5d3321ddd39d?pvs=21) sockets API and is used to create a new socket. The Berkeley sockets API is commonly used for network programming in both C and C++.

```cpp
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
```

✅  `domain`: Specifies the address family used by the socket (e.g., `AF_INET` for IPv4, `AF_INET6`                                  for IPv6, `AF_LOCAL` local).


✅ **`type`**: Specifies the socket type, such as **`SOCK_STREAM`** for a stream socket (TCP) or **`SOCK_DGRAM`** for a datagram socket (UDP).


✅ **`protocol`**: Specifies the specific protocol to be used. Typically set to 0 to use the default protocol for the chosen address family and socket type, you can see protocol's values in `/etc/protocols`


### **setsockopt -- change option in bhaver of socket**

The **`setsockopt`** function is a system call in C and C++ that is used to set options on a socket. It allows you to configure various aspects of socket behavior. Here is the basic syntax:

```cpp
#include <[sys/socket.h](https://pubs.opengroup.org/onlinepubs/000095399/basedefs/sys/socket.h.html)>
int setsockopt(int *socket*, int *level*, int *option_name*, const void **option_value*, socklen_t *option_len*);
```

✅ **`socket`**: The file descriptor of the socket.


✅ **`option_name`**: The name of the option you want to set.


✅ **`option_len`**: The size of the option value.


✅ **`level`**: The protocol level at which the option resides. Common values include **`SOL_SOCKET`** for socket-level options and other values such as **`IPPROTO_TCP`** for TCP-specific options.


✅ **`option_value`**: A pointer to the value you want to set for the option.


### **fcntl - - manipulate file descriptors**

The **`fcntl`** (file control) function is a system call in Unix and Unix-like operating systems, including Linux. It is used to perform various operations on open files or file descriptors. The **`fcntl`** function is quite versatile and can be used for a range of purposes, including file descriptor duplication, setting file descriptor flags, and advisory file locking.

```cpp
#include <fcntl.h>
int fcntl(int fd, int cmd, ... /* arg */ );
```

✅ The **`F_SETFL`** command is used to set the **`O_NONBLOCK`** flag is used to set a file descriptor to non-blocking mode. When a file descriptor is in non-blocking mode, certain operations on it, such as reading or writing, won't block


✅ The **`F_GETFL`** command is used to retrieve the current file status flags (**`flags`**)


✅ The **`F_SETFD`** command is used to set the **`FD_CLOEXEC`** flag.


### bind -- identify a socket

The **`bind`** function in the context of socket programming is used to associate a socket with a specific local address and port number.

```cpp
int bind(int sockfd, const struct sockaddr *address, socklen_t address_len);

struct sockaddr_in
{
    __uint8_t         sin_len;
    sa_family_t       sin_family;
    in_port_t         sin_port;
    struct in_addr    sin_addr;
    char              sin_zero[8];
};
```

`sin_family` = `domain`

`sin_port` = a port number in banery (use `htons`)

`sin_addr` = address for the socket in banery (use `inet_pton`)

### htons, htonl, ntohs, ntohl -- convert values between host and network byte order

**way need to use htons, htonl, ntohs, ntohl ??**

> "All integer fields are sent in network byte order which is always
most significant byte first." (RFC 793, Section 3.1 - Introduction)
> 

In the context of networking protocols, especially those standardized by the Internet Engineering Task Force (IETF), big-endian byte order is used to ensure consistent interpretation of multi-byte fields across different computer architectures. In big-endian format, the most significant byte (the one with the higher memory address) is transmitted first.

For example, a 16-bit integer with a value of 0x1234 would be transmitted as 0x12 followed by 0x34. In little-endian format, it would be the other way around (0x34 followed by 0x12).

This adherence to network byte order helps in interoperability between systems with different architectures. Most network protocols, including TCP and UDP, follow this convention to ensure that data can be correctly interpreted by systems with varying byte orders.

```c
#include <arpa/inet.h>uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
```

- `htons()` : converts a short integer (e.g. address) to a network representation
- etc.

### select -- synchronous I/O multiplexing

The **`select`** function in C is used for performing input/output multiplexing. It allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some kind of I/O operation (read, write, or exception).

```cpp
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```

- **`nfds`**: This is the highest-numbered file descriptor in any of the sets, plus 1.
- **`readfds`**, **`writefds`**, **`exceptfds`**: These are pointers to sets of file descriptors that you want to monitor for read, write, or exception conditions respectively. You can set them to **`NULL`** if you're not interested in a particular set.
- **`timeout`**: This parameter specifies the maximum time the **`select`** call should wait for an event to occur. It can be set to **`NULL`** for blocking indefinitely, or it can be a pointer to a **`struct timeval`** structure specifying a maximum time interval.
1. **"The `select` function modifies the sets to indicate which file descriptors are ready."**
    
    When you call the **`select`** function, you provide sets of file descriptors that you are interested in monitoring for specific events, such as read, write, or exceptional conditions. These sets are passed as arguments (**`readfds`**, **`writefds`**, **`exceptfds`**). The **`select`** function examines these sets and modifies them to indicate which file descriptors are ready for the specified operations.
    
    - For example, if you are monitoring a set of file descriptors for read readiness, after the **`select`** call, the **`readfds`** set may be modified to only include the file descriptors that are ready for reading.
2. **"It returns the total number of ready descriptors, or -1 on error."**
    
    The **`select`** function returns an integer value that represents the total number of file descriptors that are ready for the specified operations across all sets. This value can help you determine which sets have file descriptors that are ready. Here's what the return value means:
    
    - If the return value is greater than 0, it indicates the number of file descriptors that are ready for the specified operation (read, write, or exception). The sets passed to **`select`** are modified accordingly to include only the ready descriptors.
    - If the return value is 0, it means that the timeout specified in the **`struct timeval`** parameter has expired, and no file descriptors are ready. The sets remain unchanged in this case.

### accept -- accept a connection on a socket

```c
int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
```

`accept` grabs the first connection request and create a new socket for communication (the listening socket should be used only for listening purpose). `addr` and `addrlen` are filled by the function.

### listen -- listen for connections on a socket

```c
int listen(int sockfd, int backlog);
```

marks the socket `sockfd` as a listening socket. The `backlog` argument defines the maximum lenght of the queue of pending connection requests.

### send -- send a message on a socket

```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

The only difference between `write()` and `send()` is the presence of flags.

### recv -- receive a message from a socket

```c
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

The only difference between `read()` and `recv()` is the presence of flags.

---

# HTTP Request & Response :


## HTTP Protocole:

HTTP is an application-layer protocol. That means it is the format that applications use to interpret the message from their remote peer. While HTTP is not the only application-layer protocol, it’s the one that browsers use to access a website. Hence any web application that needs to be reachable by a browser needs to talk HTTP. If the application will only be accessed by other applications, they can choose to talk via whatever protocol they wish.

HTTP has URLs that are used to identify resources. It has methods/verbs that can be used to represent the action one wants to take on a resource. It has headers for all kinds of metadata. There is status code to represent the success/error scenarios. And finally, there is the body that can hold the actual data represented by the resource.

## **HTTP request:**

The request message is structured into different parts: the request line, headers, and sometimes a message body. Between these parts, there's a need for a clear separation to ensure proper interpretation by the server.

Example of an HTTP request and an explanation of each line:

```plan/text
Request Line:
GET /example/path HTTP/1.1

Headers:
Host: www.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.71 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8

```

1. **Request Line**:
    - `GET`: The HTTP method used in this request, indicating that the client is asking the server to retrieve a resource.
        
        📌 HTTP defines a set of request methods(or *verbs*) to indicate the desired action to be performed for a given resource. The HTTP/1.0 specification defined the GET, HEAD, and POST methods, and the HTTP/1.1 specification added five new methods: PUT, DELETE, CONNECT, OPTIONS, and TRACE.
        
        ✅ `GET` = requests that the target resource transfer a representation of its state (**HTTP status codes**). Requests using GET should only retrieve data without making changes.
        
        
        ✅ `POST` = submits an entity to the specified resource, often causing a change in state or side effects on the server.
        
        
        ✅ `PUT` = requests that the target resource create or update its state with the state defined by the submitted request. A distinction from POST is that the client specifies the target location on the server.
        
        
        ✅ `HEAD` = asks for a response identical to a GET request, but without the response body (only the header).
        
        
        ✅ `DELETE` = deletes the specified resource.
        
        
        
    - `/example/path`: The path to the resource being requested on the server.
    - `HTTP/1.1`: The version of the HTTP protocol being used.
    
    the request line finishes with Carriage Return Line Feed (CRLF) which are end-of-line markers for all protocol elements, except the entity-body. Refers to a fundamental aspect of the HTTP protocol regarding how these parts are delimited.
    
    ✅ **Carriage Return (CR)**: Represented by `\r`, it's a control character that signifies the end of a line in ASCII or Unicode text files.
    
    
    ✅ **Line Feed (LF)**: Represented by `\n`, it's another control character used to denote a new line in a text file.
    
    Together, they form the sequence `CRLF`, indicating the end of a line in HTTP. In the context of an HTTP request:
    
    
    It's important because this consistent use of `CRLF` as an end-of-line marker ensures that different components of the HTTP request message are correctly distinguished by the server. The presence of `CRLF` is a part of the HTTP protocol's syntax and ensures the proper parsing and interpretation of the request by both the client and the server.
    
2. **Headers**:
    
    HTTP headers are essential components of the Hypertext Transfer Protocol (HTTP), which is the foundation of data communication on the World Wide Web. HTTP headers are used to transmit additional information between the client (usually a web browser) and the server during a request-response cycle. They play a crucial role in defining the parameters of the communication and ensuring that both the client and server understand each other's requirements.
    
    - `Host`: Indicates the domain name of the server to which the request is being sent. This header helps the server understand which domain the client is requesting.
    - `User-Agent`: Provides information about the client making the request. In this case, it specifies that the request is coming from a browser (Chrome) running on Windows 10 (64-bit).
    - `Accept`: Informs the server about the types of content the client can understand or accept. This header tells the server that the client prefers HTML (`text/html`) and other formats like XML and images.

## HTTP Response

A response message is sent by a server to a client as a reply to its former request message. It contains information about the status of the request, along with optional data such as the requested content or instructions for further action. HTTP responses are composed of a status line, headers, and an optional body.

```
Status Lines:
HTTP/1.1 200 OK

Headers:
Content-Type: text/html
Content-Length: 1234
Server: nginx
```

1. **Status Lines**:
    
    The first line of an HTTP response is the status line, which includes:
    
    - HTTP Version: The version of the HTTP protocol being used (e.g., HTTP/1.1).
    - Status Code: A three-digit numerical code that indicates the outcome of the server's attempt to process the request.
    - Reason Phrase: A brief textual description associated with the status code, providing more details about the status.
2. **Headers:**
    
    Similar to request headers, response headers provide additional information about the server's response. Common response headers include:
    
    - Content-Type: Specifies the media type of the content in the response body.
    - Content-Length: Indicates the size of the response body in octets (bytes).
    - Server: Identifies the software or server type that generated the response.
    - Date: Specifies the date and time at which the response was generated.
    - Set-Cookie: Sets cookies on the client's browser for session management or other purposes.
3. **Body:**

The response body contains the actual data being sent from the server to the client. The presence and format of the body depend on the request type and the status code. For example:

- In a successful response (status code 200 OK), the body might contain HTML, JSON, or other types of data.
- In the case of an error (e.g., status code 404 Not Found), the body may contain an error message or additional details.

## CGI

CIG (Common Gateway Interface) enables web servers to execute an external program, for example to process user request.

### **CGI Script Execution:**

Our CGI scripts are implemented using the **`execve`** system call. This low-level Unix system call facilitates the seamless execution of CGI programs, ensuring effective integration within the web server environment.

```c
execve(CGI_PATH, args, env);
```

1. `CGI_PATH` : Refers to the correct path derived from the request URL, ensuring that it has the necessary permissions to execute. This is crucial for the proper functioning of CGI (Common Gateway Interface) scripts.
2. `env` : CGI environment includes a set of variables and values that provide information to the CGI script during its execution. These variables typically include details such as:
    
    Server specific variables :
    
    - `SERVER_SOFTWARE` : name/version of HTTP server.
    - `SERVER_NAME` : host name of the server, may be dot-decimal IP address.
    - `GATEWAY_INTERFACE` : CGI/version.
    
    Request specific variables :
    
    - `SERVER_PROTOCOL` : HTTP/version.
    - `SERVER_PORT` : TCP port (decimal).
    - `REQUEST_METHOD` : name of HTTP method (see above)
    - `PATH_INFO` : path suffix, if appended to URL after program name and a slash
    - `SCRIPT_NAME` : relative path to the program, like `/cgi-bin/script.cgi`.
    - `QUERY_STRING` : the part of URL after `?` character
    - `REMOTE_HOST` : host name of the client, unset if server did not perform such lookup
    - `REMOTE_ADDR` : IP address of the client (dot-decimal)
    - `AUTH_TYPE` : identification type, if applicable
    - `REMOTE_USER` : used for certain AUTH_TYPEs
    - `REMOTE_IDENT` : see ident, only if server performed such lookup.
    - `CONTENT_TYPE` : Internet media type of input data if `PUT` or `POST` method are used, as provided via HTTP header
    - `CONTENT_LENGTH` : similarly, size of input data (decimal, in octets) if provided via HTTP header

![request_response](https://github.com/RajaeRs/philosopher/assets/75434277/f7b5d271-c2a6-4cf7-a427-405ec8511829)

# Configuration file

Example of a complete configuration file (`nginx.conf` inspiration):

```
server {
	listen			    HOST;
	port            PORT;
	server_name		  SERVER_NAME;
	root			      ROOT;
	index			      INDEX;
	client_body		  MAX_CLIENT_BODY;
	methods			    METHOD1 METHOD2 ...;
	error_page		  NUM_ERROR \ ERROR_FILE;

	location /DIRECTORY1 {
		root			    ROOT;
		index			    INDEX;
		client_body		MAX_CLIENT_BODY;
		methods			  METHOD1 METHOD2 ...;
	}

	location /DIRECTORY2 {
		root			    ROOT;
		methods			  METHOD1 METHOD2 ...;
		cgi_index	    CGI;
		redirect      REDIR_URL;
		autoindex     on;
	}

	location /INTERNAL_DIR {
		internal;
	}
}
```
