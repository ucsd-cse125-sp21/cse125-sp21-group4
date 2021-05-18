# cse125-sp21-group4

## Running the clients and the server

### Prerequisites:
- Visual Studio ("Desktop Development C++" workload + Windows SDK)
- MingW64 for g++
- Windows machine

### How to run the Client (with Graphics) (New Instructions with Solution file)
1. Open up Visual Studio -> Open a project or solution -> Goto repository/client and pick CSE125Client.sln
2. Right click the Project in the Solution Explorer (not the Solution) -> Properties -> Make sure All Configurations is chosen
3. Linker -> Input
4. Prepend "opengl32.lib;" to the Additional Dependencies (if not already prepended).
5. Apply -> Ok -> Press Ctrl - F5 to run.

### How to run the Client (with Graphics) (Legacy Instructions)
1. Follow http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20 to install Visual Studio (w/ nupengl and glm)
2. Make sure when you create a new project from existing code, choose the client subfolder, not the entire repo folder
3. If you are running the client AND server, you must uncomment #define SERVER_ENABLED in client/Main.cpp, otherwise it will not connect to the server.
4. If you having issues with external dependencies with the common folder (LNK errors), try the following:
  - a) Goto Project -> Add Existing Item -> Add all the files in common 
  - b) If a) doesn't work, then right click your project then goto: Properties -> C/C++ -> General -> Additional Include Directories, then add the common folder to your Additional Include Directories via absolute path


### How to build and run the Test Client (Console Game No Graphics / test_windows_client.cpp) and Server
Execute `build.bat` within the root directory of the project to generate `client.exe` and `server.exe` within the `client` and `server` directory, respectively - the build was succesful if the terminal automatically closes! You can also build the client and server individually by `cd`'ing into their respective directories:
- Client
  - `cd client`
  - `g++ ../tests/test_windows_client.cpp CommunicationClient.cpp ../common/game/*.cpp -o client.exe -lWs2_32`
- Server
  - `cd server`
  - `g++ *.cpp ../common/game/*.cpp -o server.exe -lWs2_32`

To run the client or server, execute their respective `exe` files.

### Notes on running the server and client
- If you are running the client AND server, you must uncomment #define SERVER_ENABLED in client/Window.h , otherwise clients will not connect to the server.
- If your server does not see output even after client connects or has bind() errors: check if another server is running OR change SERVER_PORT to some random port like 31524.
- Requires 4 clients to connect for server to start the game. You may edit PLAYER_NUM in common/game/GameConstants.h to lower the player count for testing.
