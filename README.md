# Game

I made a geme for my introduction to robotics course. It is some sort of space invaders game where the player uses two joysticks (one to move, one to shoot) in order to destroy some NPCs until the player runs out of lives.

## Stage 1
- Added message when the game stats;
- Added menu with
  -Start;
  -HighScore;
  -Settings;
  -About;
- The player can move through the matrix;
- The player can aim but not shoot in tha matrix with the second joystick;

## Stage 2
- Added option for player to shoot, the projectile will travel until it reaches the end of matrix
- Added the predefined places of NPCs, depending on difficulty

## Stage 3
- Added AI that controls the NPC that makes them move and shoot after the player, their behaviour, number, respawn speed and projectile speed depending on difficulty
- Added EEPROM functions to save and read the highest 3 highscores
- Added score
- Added in setings a way to change difficulty
