#Welcome on our game app Casse-Brique
It was developped as part of a educative project, with a team of 2 junior developpers.
The language used is C.

It is a single/multiplayer game, where you try to be the last survivor by killing your opponents with bombs :bomb:. Numerous stats modificator  items :gift: will help you (or desserved you !) in this task.
The game can be played with 1 to 4 "human" players. In case of local play, you can play with your friends on the same computer, or with up to 3 bots (but dont expect them to be on your side :smiling_imp:)


##Prerequisite
As the programme is coded with C language, you need a compilator to compile and create the executable to play our game.
For linux users, please use GCC.
For windows users, use MinGW or Mingw32.
Regarding the IDE, the choice is yours as long as they support one of the previous compilators.
Install the program make.


##Installation
Just download the file Casse-Brique.tar provided, and extracted it in the folder of your choice. However, please do not change the global structure of the folders as it may create problems with the execution. Of course, you are free to do it, but remember that you would have to modify the filePath provided in the header of multiple files !

##Deployment
To play our game, it is mendatory to compile the game in an executable form.
As minGW provided the same command line than gcc, the deployment procedure for linux and windows user is the same.
Please follow the next steps:
- navigate to the root of the project named CasseBrick
- execute the command make
- execute the file:
	linux:
	./CasseBrick <opt: filePath for your custom map>
	windows:
	CasseBrick.exe <opt: filePath for your custom map>


##Game option

1) Local Game:
You can play solo or with friends on the same computer, on a turn by turn basis. If not enough players are available, bots will complete the line up.

2) Start a server:
Set up and start your server, where you can choose the map available for your futur game.
Be aware that max number of player is the number of needed player to be connected on your server to launch the game.

3) Join a server:
To join a server and play with your friend, the game will ask you to provide the IP adress of the server, and your pseudonym. When all players are connected, the game can start.

-------------
Options available on both local and server side:

* Maps provided by the game are for 2 or 4 players max. It is possible to choose to generate a random map for both number of players if you wish in the selection menu.

However, it is possible to load your own map to play on CasseBrick.
Be sure that it respects the following format:

1 <-- nb of starting bombs available per player
9 5 <-- nb of columns 'space' nb of rows
xxxxxxxxx <-- the map (representated by the previous informations) 
xp mmmmmx
x xmxmx x
xmmmmm px
xxxxxxxxx <-- the last character has to be the EOF, no more line after that.

Load it by adding it s filePath as argument when executing the game.
ie: ./CasseBrick ./AwesomeMap.txt
Be aware that the number of p (players) on the map will define the maximum number of players.

* At the end of a map, when 1 or 0 players are alive, you will have the option to continue and play another map at random in the initial map selection. If more that one map has been selected, the next map will be different.
In server mode, all players have to select 'continue' else the server will shut down and disconnect the players.

* Game is compatible windows and linux simultaneously on server mode.

##Game Structure

####Players
Each players are referenced by their number on the map.

####Map structure
Here is the list of the bricks building our game. The main frame is listed here:
	x : a undestructible wall. Stop the explosion of a bomb in its direction.
	m : a destructible wall (can generate item when exploded). A bomb can break hit, but stop the explosion of a bomb in its direction.
	'': space character. The tile is empty and can be moved on.
	
####Bombs
Each bombs are represented by a B. When a player launchs a bomb, it appears at the same coordonate as the player (under his foots), so be aware of it when you move as it migh block you later if you dont considerate it.
A bomb has a timer of 3 turns, bombs explode at the end of the player turn ! As there are no apparent timer shown and as a bomb can make another one exploded if it is in its explosion range, chain reactions must be expected!

A timer has been made to see the explosion and the map updated after that. So be patient as wonderful items might await for you once walls are broken !

####Items
Following is a brief descriptive list for the items dropped randomly after a wall is destroyed:
        '+': Bomb range explosion Up (max 9)
        '-': Bomb range explosion down (min 1)
        'O': Number of bombs Up (max 9)
        'o': Number of bombs Down (min 1)
	'P': PasseBomb - You can walk through bombs. When you are on a bomb, it is in an inactive state and the explosion timer is blocked until you move on another tile. Cannot be owned at the same time than KickBomb.
	'K': KickBomb - You can kick a bomb until it reach an obstacle. Cannot be owned at the same time than PasseBomb
	'*': Invincibility for the next 2 turns. A bomb cannot damage you in this state.
	'#': Heart. Provide a 1 time protection against explosion. Cannot be earned more than one time per map.
	'@': Life. Life counter go down each time a player is touched by an explosion. When timer is at 0, the player is dead and cannot play anymore on this map.
	'&': Fire MAX. Explosion range go to the max (max 9)

####Actions
A player turn ends when he makes an action:
	- launching a bomb (touch B)
	- waiting (touch W)
	- moving (touch Z: North, S: South, Q: West, D: East) Be carrefull as these controls where made for french keyboard !
Player turns finished only once a valid action has been done. 
	
When one or no more player are alive at the end of a turn, the game is over.


##Team organization
To make our organization smoother, we decided to use Git as a VCS, so we could keep track of each game features implemented, and make sure we could exhange and integrate each other codes efficiently.
One of our goals during this project was to experiment a more company oriented set up (IDE and VCS).


