### Simple Client and Server app with C++

**GitHubcode**
[](https://www.google.com/)

**How Client-Server Communication Works in Socket Programming:**
Socket programming allows two nodes on a network to communicate with each other. One node sends a message, and the other node receives it. 

1. Server creates a `socket` using the `socket()` method.
2. Server `binds` the socket to an IP address and a port using the `bind()` method.
3. Server `listens` for incoming connections using the `listen()` method.
4. Client creates a `socket`.
5. Client attempts to `connect` to the server's IP and port.
6. If the server accepts the connection, communication can occur via `read()` and `send()` methods.

**Difference Explained: API Call vs Networking:**
An API call is a request that is made to an application's interface, allowing for communication between different software systems. Networking, on the other hand, is the practice of linking computing devices together. When you make an API call over the internet, you are utilizing networking to send the request to the server where the application's interface resides.

**Why Messages Can Be Seen as Plain Text on the Network:**
If encryption is not used, data transmitted over a network, including messages, can be intercepted and read as plain text by anyone with the necessary tools and access to the network. This is why it's crucial to use encryption techniques, like SSL/TLS, especially for sensitive data, to ensure the data is scrambled and can only be read by the intended recipient.

**Important Lines from the Given Code:**

***Server Code:***
1. Socket creation: 
   ```cpp
   server_fd = socket(AF_INET, SOCK_STREAM, 0)
   ```
2. Binding socket:
   ```cpp
   bind(server_fd, (struct sockaddr *)&address, sizeof(address))
   ```
3. Server listening:
   ```cpp
   listen(server_fd, 3)
   ```
4. Accepting client:
   ```cpp
   client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)
   ```
5. Reading from client:
   ```cpp
   valread = read(client_fd, buffer, 1024)
   ```
6. Sending message to client:
   ```cpp
   send(client_fd, message, strlen(message), 0)
   ```

***Client Code:***
1. Socket creation: 
   ```cpp
   sock = socket(AF_INET, SOCK_STREAM, 0)
   ```
2. Getting server IP from user:
   ```cpp
   std::cin >> ipAddress;
   ```
3. Connecting to server:
   ```cpp
   connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))
   ```
4. Sending message to server:
   ```cpp
   send(sock, message, strlen(message), 0)
   ```
5. Reading from server:
   ```cpp
   valread = read(sock, buffer, 1024)
   ```