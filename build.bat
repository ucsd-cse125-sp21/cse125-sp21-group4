@ECHO OFF
ECHO Building client...
g++ .\tests\test_windows_client.cpp .\client\CommunicationClient.cpp .\common\game\*.cpp -o .\client\client.exe -lWs2_32
if NOT ["%errorlevel%"]==["0"] pause
ECHO Building client with optimized network...
g++ .\tests\test_windows_client_with_updates.cpp .\client\CommunicationClient.cpp .\common\game\*.cpp -o .\client\client_with_updates.exe -lWs2_32
if NOT ["%errorlevel%"]==["0"] pause
ECHO Building server...
g++ .\server\*.cpp .\common\game\*.cpp -o .\server\server.exe -lWs2_32
if NOT ["%errorlevel%"]==["0"] pause
ECHO Done!