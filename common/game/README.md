# Usage
To run the interactive test file GameTest.cpp
1. Change fake_main() in GameTest to main()
2. g++ *.cpp -o game.exe
3. game.exe

## game module inheritance relationship
```
            GameComponent
       /        |         \      
     /          |          \
    |           |           |
  Space      GamePlayer   Obstacle
                |
              /   \
        Monster   Fighter
```
