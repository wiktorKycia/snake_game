# Snake game
I wanted to learn pure C programming language, but I lacked the idea for a perfect starting project

Then [this video](https://www.youtube.com/watch?v=RIAxnxi-MCk) appeared on my youtube feed and inspired me of making a snake game in C

I never programmed in C before, apart from some [BroCode C tutorials](https://www.youtube.com/playlist?list=PLZPZq0r_RZOOzY_vR4zJM32SqsSInGMwe) also on youtube, but these were rather smaller ones - and I followed the tutorial line by line, so it was then nothing creative

I started on sathurday afternoon, finished in the late evening.
I had to debug it with [gdb](https://www.sourceware.org/gdb/), because it was the first time I've implemented ArrayList structure and I've done some errors like not incrementing `max_size` when reallocation or not decrementing `count` when removing the element from the list. 


## How to run the game?
First, you have to have [gcc](https://gcc.gnu.org/) installed and ready to use in terminal

Then you need the code to compile
```sh
git clone https://github.com/wiktorKycia/snake_game.git
```

Then you need to compile the code
```sh
gcc -o main main.c
```

Then run the binary file
```sh
./main
```

> [!IMPORTANT]
> If you use Windows, please chnage `system("clear")` to `system("cls")` and compile with command: `gcc -o main.exe main.c` and then run the `main.exe` file


## Acknowledgements
Thank you:
- [claude](https://claude.ai/) for teaching me how to pass 2d char array to funtion and for suggesting catching input with [termios](https://man7.org/linux/man-pages/man3/termios.3.html) 
- [LowLevel](https://www.youtube.com/watch?v=Dq8l1_-QgAc) for teaching me how to use [gdb](https://www.sourceware.org/gdb/)