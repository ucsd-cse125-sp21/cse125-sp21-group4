I had to install Visual Studio ("Desktop Development with C++" workload) + Windows SDK + MingW64 so I can spin up a TCP server.

# Usage
To run the tcp server using a .cpp file called "server.cpp":

Using command prompt:
1. g++ *.cpp ..\common\game\*.cpp -o run_server.exe -lWs2_32
2. run_server.exe