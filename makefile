CasseBrick: ./Game/bomb.c ./Game/color.c ./Game/bot.c ./Game/collisionChecker.c ./Game/game.c ./Game/keyboardHandler.c ./Game/main.c ./Game/map.c ./Server/server.c ./Client/client.c ./Game/item.c
	gcc -o CasseBrick ./Game/bomb.c ./Game/bot.c ./Game/collisionChecker.c ./Game/game.c ./Game/keyboardHandler.c ./Game/main.c ./Game/map.c ./Server/server.c ./Client/client.c ./Game/color.c ./Game/item.c
