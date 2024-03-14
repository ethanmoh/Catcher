# Catcher Game Project

This project involves creating a game called "Catcher" using VGA tools and C logic on the DE1-SOC board. 
The goal of the game is to catch as many falling balls as possible with the paddle.

## Features:
- Graphics generated using VGA tools (VGA_box, VGA_line, etc.)
- Scripts ran using putty with COM3 connection to DE1-SOC
- Logic processed in C
- Switch 0 controls the paddle, flipping the switch moves the paddle in one direction or the other
- Switch 1 controls the difficulty setting, increasing the speed of the ball and paddle at higher difficulty levels

## Gameplay:
- Balls spawn randomly onto the screen and begin falling
- Catching a ball with the paddle increases the score and spawns another ball
- Failing to catch a ball ends the game

## How to Play:
- Use switch 0 to move the paddle left or right to catch falling balls
- Use switch 1 to increase the difficulty for a greater challenge
