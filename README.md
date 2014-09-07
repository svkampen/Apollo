# Apollo
Apollo is an IRC bot written in C.

#### Compiling
After `git clone`ing this repository, you will first need to initialize the submodules: `git submodule init` and `git submodule update`.

Now, you can compile apollo using `make`.  
Apollo also requires the `common` plugin, which is supplied and which you can make using `make plugs`.

Apollo requires a hashmap implementation which is supplied as a submodule. It has already been compiled and put in bin, but the dynamic linking loader (ld.so) might complain with the following:  

`./apollo: error while loading shared libraries: libhashmap.so: cannot open shared object file: No such file or directory`

To fix this, set `LD_LIBRARY_PATH` to `.` or move libhashmap.so to your normal LD library path (most likely `/lib` or `/lib64`)

After this, you can run apollo by changing directory to bin and running `./apollo <server>`
