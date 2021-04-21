# Usage
To run the interactive test file GameTest.cpp
1. Change fake_main() in GameTest to main()
2. g++ *.cpp -o game.exe
3. game.exe

## Inheritance relationship
```
                  GridCompoenent
              /         |         \
            /           |           \
          /             |             \
    Obstacle          Space           Objective
       |                             /    |      \
       |                            |     |       |
      Rock                      Healing Evolve  Beacon

--------------------------------------------------------------

                    GamePlayer

      Fighter Monster Mage(TODO) Rogue(TODO) Cleric(TODO)
```
