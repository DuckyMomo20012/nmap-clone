<div align="center">

  <h1>nmap Clone</h1>

  <p>
    Simulate nmap, ping command
  </p>

<!-- Badges -->
<p>
  <a href="https://github.com/DuckyMomo20012/nmap-clone/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/DuckyMomo20012/nmap-clone" alt="contributors" />
  </a>
  <a href="">
    <img src="https://img.shields.io/github/last-commit/DuckyMomo20012/nmap-clone" alt="last update" />
  </a>
  <a href="https://github.com/DuckyMomo20012/nmap-clone/network/members">
    <img src="https://img.shields.io/github/forks/DuckyMomo20012/nmap-clone" alt="forks" />
  </a>
  <a href="https://github.com/DuckyMomo20012/nmap-clone/stargazers">
    <img src="https://img.shields.io/github/stars/DuckyMomo20012/nmap-clone" alt="stars" />
  </a>
  <a href="https://github.com/DuckyMomo20012/nmap-clone/issues/">
    <img src="https://img.shields.io/github/issues/DuckyMomo20012/nmap-clone" alt="open issues" />
  </a>
  <a href="https://github.com/DuckyMomo20012/nmap-clone/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/DuckyMomo20012/nmap-clone.svg" alt="license" />
  </a>
</p>

<h4>
    <a href="https://github.com/DuckyMomo20012/nmap-clone/">View Demo</a>
  <span> · </span>
    <a href="https://github.com/DuckyMomo20012/nmap-clone">Documentation</a>
  <span> · </span>
    <a href="https://github.com/DuckyMomo20012/nmap-clone/issues/">Report Bug</a>
  <span> · </span>
    <a href="https://github.com/DuckyMomo20012/nmap-clone/issues/">Request Feature</a>
  </h4>
</div>

<br />

<!-- Table of Contents -->

# :notebook_with_decorative_cover: Table of Contents

- [About the Project](#star2-about-the-project)
  - [Features](#dart-features)
- [Getting Started](#toolbox-getting-started)
  - [Prerequisites](#bangbang-prerequisites)
  - [Run Locally](#running-run-locally)
- [Usage](#eyes-usage)
- [Roadmap](#compass-roadmap)
- [Contributing](#wave-contributing)
  - [Code of Conduct](#scroll-code-of-conduct)
- [FAQ](#grey_question-faq)
- [License](#warning-license)
- [Contact](#handshake-contact)
- [Acknowledgements](#gem-acknowledgements)

<!-- About the Project -->

## :star2: About the Project

<!-- Features -->

### :dart: Features

- Send ARP to host ID range from 1 to 254.
- Write host who response to ARP request to text file.
- Ping command.

<!-- Getting Started -->

## :toolbox: Getting Started

<!-- Prerequisites -->

### :bangbang: Prerequisites

This project requires the following prerequisites:

- GCC, G++ >= 10.2.1
- GDB
- make

```bash
sudo apt update
sudo apt install build-essential
```

<!-- Run Locally -->

### :running: Run Locally

Clone the project:

[unpv13e](https://github.com/k84d/unpv13e) is a submodule, so you have to clone
repo recursively:

```bash
git clone --recursive https://github.com/DuckyMomo20012/nmap-clone.git
```

Go to the `unpv13e` directory:

```bash
cd unpv13e
```

Build `unpv13e`:

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

Next, you will need file `Make.defines` to compile necessary library, you can
use sample file in folder `nmap` and `ping`.

OR:

You can create it manually:

<details>
<summary>Instructions</summary>

- After building `unpv13e`, you will have a file `Make.defines.in`.
- Copy file `Make.defines.in` from folder `unpv13e` to folder `nmap` and
  **rename to `Make.defines`**.
- In file `Make.defines`, replace @{EXAMPLE}@ with content from
  `unpv13e/config.status`:

  E.g:

  - In file `config.status` we have `s,@CC@,gcc,;t t`. So replace `@CC@` with
    `gcc`.
  - In file `config.status` we have `s,@CFLAGS@,-g -O2 -D_REENTRANT -Wall,;t t`.
    So replace `@CFLAGS@` with `-g -O2 -D_REENTRANT -Wall`.
  - In file `config.status` we have `s,@LIBUNP@,../libunp.a,;t t`.
    So replace `@LIBUNP@` with `../libunp.a`.

- Then, replace "`../`" with "`../unpv13e/`" in file `Makefile.defines`.

  E.g:

  ```
  CFLAGS = -I../unpv13e/lib -g -O2 -D_REENTRANT -Wall
  LIBS = ../unpv13e/libunp.a -lpthread
  LIBS_XTI = ../unpv13e/libunpxti.a ../unpv13e/libunp.a -lpthread

  LIBUNP_NAME = ../unpv13e/libunp.a

  LIBUNPXTI_NAME = ../unpv13e/libunpxti.a
  ```

</details>

Build `nmap` and `ping` program:

- **nmap:**

  - Go to the `nmap` directory:

    ```bash
    cd ../nmap
    ```

  - Build `nmap`:

    ```bash
    make
    ```

  - Run `nmap` program:

    ```bash
    sudo ./nmap 192.168.202.0
    ```

- **ping:**

  - Go to the `ping` directory:

    ```bash
    cd ../ping
    ```

  - Build `ping`:

    ```bash
    make
    ```

  - Run `ping` program:

    ```bash
    sudo ./ping 192.168.202.129
    ```

<!-- Usage -->

## :eyes: Usage

> NOTE: Program must run with `sudo` privileges

**nmap:**

> NOTE: Currently support class D (/24) addresses.

The idea is simple:

- When we ping, ICMP packets on some OS may be blocked, so we will send ARP packets, because
  everyone has to answer ARP packets.
- If the network is: 192.168.202.0/24, then we will send ARP requests to
  `192.168.202.1` -> `192.168.202.254`.

- To see more details:

  ```console
  sudo ./nmap 192.168.202.0 -v
  ```

**ping:**

> NOTE: Currently support IPv4 ping.

- Send ICMP request to target IP.

- To see more details:

  ```bash
  sudo ./ping 192.168.202.129 -v
  ```

---

Cleanup file:

```bash
make clean
```

<!-- Roadmap -->

## :compass: Roadmap

- [x] Stop after send complete.
- [x] Change compiled target name.
- [x] Handle ARP reply.

<!-- Contributing -->

## :wave: Contributing

<a href="https://github.com/DuckyMomo20012/nmap-clone/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=DuckyMomo20012/nmap-clone" />
</a>

Contributions are always welcome!

<!-- Code of Conduct -->

### :scroll: Code of Conduct

Please read the [Code of Conduct](https://github.com/DuckyMomo20012/nmap-clone/blob/main/CODE_OF_CONDUCT.md).

<!-- FAQ -->

## :grey_question: FAQ

- Is this project still maintained?

  - No, but I will only update documentation.

<!-- License -->

## :warning: License

Distributed under MIT license. See
[LICENSE](https://github.com/DuckyMomo20012/nmap-clone/blob/main/LICENSE)
for more information.

<!-- Contact -->

## :handshake: Contact

Duong Vinh - [@duckymomo20012](https://twitter.com/duckymomo20012) - tienvinh.duong4@gmail.com

Project Link: [https://github.com/DuckyMomo20012/nmap-clone](https://github.com/DuckyMomo20012/nmap-clone).

<!-- Acknowledgments -->

## :gem: Acknowledgements

Here are useful resources and libraries that we have used in our projects:

- [Awesome Readme Template](https://github.com/Louis3797/awesome-readme-template):
  A detailed template to bootstrap your README file quickly.
