# A clone of nmap.

Simulate nmap command

<table>
  <tr>
    <th>OS</th>
    <td>Debian 11</td>
  </tr>
  <tr>
    <th>gcc, g++</th>
    <td>10.2.1</td>
  </tr>
</table>

## Description:

The idea is simple:

- ICMP packets on some OS may be blocked, so will send ARP packets, because
  everyone has to answer ARP packets.
- If the network is: 192.168.202.0/24, then we will send ARP requests to
  `192.168.202.1` -> `192.168.202.254`.

## How to build:

unpv13e is a submodule:

```console
nmap-clone/src# git submodule add https://github.com/k84d/unpv13e.git unpv13e
```

- Execute the following from the src/unpv13e directory:

> After clone this nmap-clone project, you have to build unpv13e lib, you don't
> have to push commit to unpv13e

```
    ./configure    # try to figure out all implementation differences

    cd lib         # build the basic library that all programs need
    make           # use "gmake" everywhere on BSD/OS systems

    cd ../libfree  # continue building the basic library
    make

    cd ../libroute # only if your system supports 4.4BSD style routing sockets
    make           # only if your system supports 4.4BSD style routing sockets

    cd ../libxti   # only if your system supports XTI
    make           # only if your system supports XTI

    cd ../intro    # build and test a basic client program
    make daytimetcpcli
    ./daytimetcpcli 127.0.0.1
```

- After build unpv13e, you will have a file `Makefie.defines`.
- Copy it from unpv13e to `pnmap`.
- Change "`../`" to "`../unpv13e/`"

<details>
    <summary>Examples</summary>

CFLAGS = -I`../unpv13e/`lib -g -O2 -D_REENTRANT -Wall
LIBS = `../unpv13e/`libunp.a -lpthread
LIBS_XTI = `../unpv13e/`libunpxti.a `../unpv13e/`libunp.a -lpthread

LIBUNP_NAME = `../unpv13e/`libunp.a

LIBUNPXTI_NAME = `../unpv13e/`libunpxti.a

</details>

## How to use:

- Change directory to folder

```console
cd ./src/pnmap
```

- Build file:

> NOTE: This program is tested with `gcc, g++` version `10.2.1` and adapted to
> `extern` changes. So ping folder might have to change to work with new
> changes. Or you can downgrade `gcc, g++` to older version.

```console
make
```

- Run program:

> NOTE: Program must run with `sudo` privileges

```console
./ping 192.168.202.128
```

- Cleanup file:

```console
make clean
```

## TODOs:

- [ ] Change compiled target name
- [ ] Handle ARP reply
