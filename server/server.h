void cleanUpSocket(SOCKET* clientSocketPtr);
DWORD WINAPI handlePlayerThread(LPVOID lpParam); 
int checkBounds(int pos, int lower, int upper);