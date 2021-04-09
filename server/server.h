void cleanUpSocket(SOCKET* clientSocketPtr);
DWORD WINAPI handlePlayerThread(LPVOID lpParam); 
int checkBounds(int pos, int lower, int upper);
void processInput(int assigned_player_id, CLIENT_INPUT input);