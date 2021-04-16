# cse125-sp21-group4

## Running the clients and the server

### Prerequisites:
- Visual Studio ("Desktop Development C++" workload + Windows SDK)
- MingW64 for g++
- Windows machine

### How to run the Client (with Graphics) (New Instructions with Solution file)
1. Open up Visual Studio -> Open a project or solution -> Goto repository/client and pick CSE125Proj.sln
2. Right click the Project in the Solution Explorer (not the Solution) -> Properties -> Make sure All Configurations is chosen
3. Linker -> Input
4. Prepend "opengl32.lib;" to the Additional Dependencies
5. Apply -> Ok -> Press Ctrl - F5 to run.

### How to run the Client (with Graphics) (Legacy Instructions)
1. Follow http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20 to install Visual Studio (w/ nupengl and glm)
2. Make sure when you create a new project from existing code, choose the client subfolder, not the entire repo folder
3. If you are running the client AND server, you must uncomment #define SERVER_ENABLED in client/Main.cpp, otherwise it will not connect to the server.
4. If you having issues with external dependencies with the common folder (LNK errors), try the following:
  - a) Goto Project -> Add Existing Item -> Add all the files in common 
  - b) If a) doesn't work, then right click your project then goto: Properties -> C/C++ -> General -> Additional Include Directories, then add the common folder to your Additional Include Directories via absolute path

### How to run the Test Client (Console Game No Graphics / test_windows_client.cpp)
1. `cd client`
2. `g++ test_windows_client.cpp CommunicationClient.cpp ..\common\game\*.cpp -o client.exe -lWs2_32`
3. `./client.exe  `

### How to run the Server
1. `cd server`
2. `g++ *.cpp ..\common\game\*.cpp -o server.exe -lWs2_32`
3. `./server.exe`
4. If you are running the client AND server, you must uncomment #define SERVER_ENABLED in client/Main.cpp , otherwise clients will not connect to the server.

Note: Requires 4 clients to connect for server to start the game, you may edit MAX_PLAYERS in common/constants.h to lower the maximum for testing.
