# tcp-client-server

This project demonstrates TCP communication between client and server. 
The server listens for incoming connections and handles client requests. It responds with <i>world</i> when it receives a <i>hello</i> message, and it echoes back any other messages.
The **socket.h** library is used for network communication.
Server handles multiple clients concurrently using **pthread**.
Client connects to the server and allows the user to send messages. It receives responses from the server. Connection is active until the user sends a <i>disconnect</i> command.

<h2> Usage </h2>
To run this example, you'll need a Linux environment and a C compiler installed.
<ol>
  <li>
    Clone this repository:
    
    git clone https://github.com/kamilkotowicz/tcp-client-server.git
  </li>

  <li>
    Go to the project directory:
    
    cd tcp-client-server
  </li>

  <li>
    Compile the program:

    make hello
  </li>

  <li>
    Start the server:

    ./hello server
  </li>

  <li>
    In a separate terminal, start the client:

    ./hello client
  </li>

  <li>
    Enter messages in the client terminal to communicate with the server. Type <i>disconnect</i> to exit.
  </li>
</ol>
