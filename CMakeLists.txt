cmake_minimum_required(VERSION 3.10)
project(SpaceInvaders)

set(CMAKE_CXX_STANDARD 17)

add_executable(SpaceInvaders
    main.cpp
    Game.cpp
    GameObject.cpp
    Player.cpp
    Bullet.cpp
    Enemy.cpp
)