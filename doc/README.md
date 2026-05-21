UniversalMolecule Core
======================

Setup
---------------------
UniversalMolecule Core is the UniversalMolecule full-node and wallet codebase.
It downloads and, by default, stores the UniversalMolecule blockchain. Depending
on the speed of your computer and network connection, the initial
synchronization process can take a while.

Project source and release materials are maintained at
[BlueDragon747/universalmol](https://github.com/BlueDragon747/universalmol).

Running
---------------------
The following are some helpful notes on how to run UniversalMolecule Core on
your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/universalmolecule-qt` (GUI) or
- `bin/universalmoleculed` (headless)

### Windows

Unpack the files into a directory, and then run `universalmolecule-qt.exe`.

### macOS

Drag UniversalMolecule Core to your applications folder, and then run
UniversalMolecule Core.

### Need Help?

* Review the top-level [UniversalMolecule README](/README.md).
* Use the project issue tracker for project-specific support.

Building
---------------------
The following are developer notes on how to build UniversalMolecule Core on your
native platform. They are not complete guides, but include notes on the
necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [FreeBSD Build Notes](build-freebsd.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [NetBSD Build Notes](build-netbsd.md)
- [Android Build Notes](build-android.md)

Development
---------------------
The UniversalMolecule [root README](/README.md) contains relevant information on
the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Productivity Notes](productivity.md)
- [Release Process](release-process.md)
- [Source Code Documentation](Doxyfile.in)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [JSON-RPC Interface](JSON-RPC-interface.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)
- [Internal Design Docs](design/)

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [universalmolecule.conf Configuration File](universalmolecule-conf.md)
- [CJDNS Support](cjdns.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [I2P Support](i2p.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [Managing Wallets](managing-wallets.md)
- [Multisig Tutorial](multisig-tutorial.md)
- [P2P bad ports definition and list](p2p-bad-ports.md)
- [PSBT support](psbt.md)
- [Reduce Memory](reduce-memory.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Transaction Relay Policy](policy/README.md)
- [ZMQ](zmq.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
