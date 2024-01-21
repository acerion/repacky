# My experiments with rpcgen

Simple code that allows me to test some properties of rpcgen. Patches
welcome.

Developed with:

1. rpcgen version 1.4.3,
2. libtirpc3 (version of Debian package says 1.3.3, but `pkg-config --version
   libtirpc` says 1.8.1).


rpcgen is invoked with these two options:
```
-M generate MT-safe code
-N supports multiple arguments and call-by-value
```




# Organization of code

## rpcgen/

Code with .x file, defining RPC procedures.

## server/

Code of remote server providing some functionalities over RPC.

## client/

Client code that wants to call RPC procedures provided by RPC server.

## client_lib/

Functions called by client program. They are wrappers around RPC code, hiding
RPC from client.

Client can call simple API provided by the library, without worrying about
RPC too much.




# Working with the package

I have been using this code only on one machine: both client and server were
on the same computer. You should adjust the steps and the IP address when
using the code on two separate network hosts.

1. Install rpcgen
2. Install rpcbind
3. Enable RPC
   - `systemctl enable rpcbind.service`
   - `systemctl enable rpcbind.socket`
   - `systemctl start rpcbind.service`
   - `systemctl start rpcbind.socket`
4. Compile the package
   - `make`
5. Run the RPC server
   - `./server/rpc_server`
6. Run the RPC client (use server's IP as appropriate)
   - `./client/client 127.0.0.1`
7. Use the client's menu to call RPC procedures. Start with `help` command in
   client's prompt.




# Functionalities available in the client

The are three types of commands available in client through menu:

1. Commands for calling simple RPC procedures: `gettimeofday`, `add3`.
2. Commands that interact with a state machine running on RPC server,
   demonstrating that it's possible to keep, change and retrieve over RPC
   some persistent state. Those commands names start with `sm_`.
3. Commands that test transport layer of RPC mechanism, ensuring that 3
   parallel threads in client can use RPC's transport protocol without the
   data being mixed up between threads. The command is called
   `transport_test`.

In case of doubts, use `help` command in the client's menu.




# Threads

If you are interested in multi-threading in RPC code, be sure to read [this
reference](https://docs.oracle.com/cd/E19455-01/805-7224/6j6q44chc/index.html).




# Copyright


```
repacky - my toy RPC code
Copyright (C) 2024 Kamil Ignacak

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
```

