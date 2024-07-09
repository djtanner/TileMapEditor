***Tilemap Editor for 2d games***

<img width="600" alt="Screenshot 2024-07-08 at 10 55 14 PM" src="https://github.com/djtanner/MapTool/assets/3960256/f7aeba11-b61a-44c7-873f-9d02e66afc83">

Created using C++ and SDL.

**Instructions**

Put your spritesheet in the assets folder. (See desert.png as an example.)

To run it, use the make file. ('make', followed by 'make run') Select a tile from the source in the bottom window (the selected tile will show a dark overlay) and click where to place it in the map. Delete by right clicking on the map.

Save using Ctrl + S, and you will get an output file map.txt in the following format:


```
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 2 2 2 0 0 0 0 0 0 0 0 0 0 
0 0 0 3 4 4 2 5 5 2 2 2 2 0 0 0 0 0 0 0 
10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 
22 22 22 22 22 22 22 22 22 22 22 22 22 22 22 22 22 22 22 22
```
