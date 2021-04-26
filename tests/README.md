# Usage
To run the interactive test file GameTest.cpp
1. Change fake_main() in GameTest to main()
2. g++ GameTest.cpp ../common/game/*.cpp -o game.exe
3. game.exe

To run test_windows_client.cpp
1. cd client
2. g++ test_windows_client.cpp ../client/CommunicationClient.cpp ../common/game/*.cpp -o client.exe -lWs2_32
3. ./client.exe