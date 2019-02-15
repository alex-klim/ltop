# ltop
ltop is a task manager inspired by top and htop. It is based on [termbox] tui library.

### Features that implemented:
- general info about cores and memory
- uptime, number of threads and average load
- process list
- sending SIGTERM signal to highlighted process

### Installation
```sh
git clone https://github.com/alex-klim/ltop
cd ltop
mkdir build && cd build
cmake ..
make
```

### TODO:
- fix bug with cursor after resize
- calculating shared memory for process
- calculating MEM% and CPU% for process


[termbox]: https://github.com/nsf/termbox
