# cse125-sp21-group4

## Running the clients and the server

### Prerequisites:
- Visual Studio ("Desktop Development C++" workload + Windows SDK)
- MingW64 for g++
- Windows machine

### How to run the Client (with Graphics)
1. Follow http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20 to install Visual Studio (w/ nupengl and glm)
2. Make sure when you create a new project from existing code, choose the client subfolder, not the entire repo folder

### How to run the Test Client (No Graphics / test_windows_client.cpp)
1. cd client
2. g++ test_windows_client.cpp CommunicationClient.cpp -o client.exe -lWs2_32
3. ./client.exe  

### How to run the Server
1. cd server
2. g++ *.cpp -o server.exe -lWs2_32
3. ./server.exe

Note: requires 4 clients to connect for server to start the game
