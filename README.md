# Bewof (1 kilobyte roguelike)
![alt text](Example.png "Logo Title Text 1")
### Overview
Bewof is a simple roguelike which fits in one kilobyte (1024 bytes) of C source code.
This project was inspired by [ZigClimb](http://flend.net/zigclimb/index.html)
### Compile
Bewof should compile an any Unix based system (tested an MacOS)
Bewof only requires [ncurses](https://www.gnu.org/software/ncurses/ncurses.html) to build, so make sure you have it installed.
```
$git clone https://github.com/dragonsploder/Bewof.git
$cd Bewof
$gcc -w -lncurses -obewof bewof.c
$./bewof
```
### Play
In Bewof, you are the at sign (@).
The base game of Bewof uses the wasd keys to move, Q to quit, and any other key to wait.
You have 20 health and the goal of the game is to clear the screen of monsters before your health hits 0.
The four directions each do something different when you attempt to walk on a monster tile.
* W will attack a monster for 1 health
```
.b.         .a.
.@. - w - > .@.
...         ...
```
* A will move a monster up two tiles and over one
```
...         .f.
... - a - > ...
f@.         .@.
```
* S will push a monster down one tile
```
.@.         .@.
.a. - s - > ...
...         .a.
```
* D will swap you with a monster
```
...         ...
c@. - d - > @c.
...         ...
```
If a monster is next two you at the end of your turn, it will attack you for one health

Tips:
* There is no randomness in the base game, so plan ahead
* S and A can send monsters on top of each other, killing them in one quick blow
* Only 'a' can be killed without harming yourself
### Extra
If you grow tired of playing the base game, you can change your class.
* Invoking Bewof with a 'T' will alow you to teleport once every twenty turns. To do so, press the '^' key.
* Invoking Bewof with a 'W' will alow will give you a 2d3 weapon, giving your game a little more chance.
* Invoking Bewof with a 'H' will cause monsters more then 5 tiles away from you to not notice your presence.

This can create some interesting variations:
To play as a Wizard:
```
$./bewof T
```

To play as a Rogue:
```
$./bewof H
```

To play as a Paladin
```
$./bewof T W
```

Etc.