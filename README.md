An interface to the Battlefield 2142 rcon protocol

This module provides an interface to interact with a Battlefield 2142 game server over the rcon protocol.
There are three functions which are exposed:

connect
    : creates a socket and connects to the game server

login
    : authenticates the connection with the server

invoke
    : sends commands and receives their results
