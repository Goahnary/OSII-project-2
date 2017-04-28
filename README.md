# CS445 Project 2 - Key Distribution Center Emulation
#### Noah Gary and Jared Prince

You must run our program with gcc version 4.4 You can install it on a debian using: 

```
sudo apt-get install python-software-properties
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
```

Be sure to compile our program with this command:

`gcc-4.4 -lpthread KDC.c`

### Requirements of this project:

* Have A request a session key from the KDC with both IDs for each client
* Return session key to A and B
* Request response from B
* Respond to A

We met all of the above requirements in our program.
