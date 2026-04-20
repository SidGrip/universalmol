<p align="center">
  <img src="src/qt/res/icons/bitcoin.png" alt="UniversalMolecule" width="95">
</p>

# UniversalMolecule 0.15.21

> SegWit signaling schedule: UniversalMolecule mainnet versionbits signaling starts on May 11, 2026 00:00:00 UTC (`1778457600`) and times out on May 11, 2027 00:00:00 UTC (`1809993600`). Activation still depends on BIP9 lock-in.

## About UniversalMolecule

UniversalMolecule is a Blake-256 cryptocurrency in the BlakeStream family. This repository carries the UniversalMolecule Core `0.15.21` update and ships build/release packaging for native Ubuntu `20.04`, `22.04`, `24.04`, `25.10`, Windows, native macOS, and Ubuntu `22.04+` AppImage.

- Uses the **Blake-256** hashing algorithm
- Based on **Bitcoin Core 0.15.2**
- Uses the autotools build system (`./configure` + `make`)
- Ships release packages for Ubuntu 20.04, 22.04, 24.04, 25.10, Windows, macOS, and Ubuntu 22+ AppImage

| Network Info | |
|---|---|
| Algorithm | Blake-256 (8 rounds) |
| Block time | 120 seconds (2 minutes) |
| Block reward | 1 UMO genesis, 0.001 UMO launch, then 0.1 or 2 UMO based on difficulty trend |
| Difficulty retarget | Every 5 blocks (10 minutes) |
| Default port | 24785 |
| RPC port | 5921 |
| Max supply | 105,120,001.44 UMO |

---

## Quick Start

```bash
git clone https://github.com/SidGrip/universalmol.git
cd universalmol
bash ./build.sh --help
```

For most users, downloading a prebuilt release from GitHub Releases is the simplest path.
Use build.sh to build the release artifacts locally.

---

## Build Options

```bash
bash ./build.sh [PLATFORM] [TARGET] [OPTIONS]

Platforms:
  --native          Build natively on this machine (Linux or macOS)
  --appimage        Build portable Linux AppImage (requires Docker)
  --windows         Cross-compile for Windows from Linux (requires Docker)
  --macos           Cross-compile for macOS from Linux (requires Docker)

Targets:
  --daemon          Build daemon only
  --qt              Build Qt wallet only
  --both            Build daemon and Qt wallet (default)

Docker options:
  --pull-docker     Pull prebuilt Docker images from Docker Hub
  --build-docker    Build Docker images locally from repo Dockerfiles
  --no-docker       For --native on Linux: build directly on the host

Other options:
  --jobs N          Parallel make jobs
```

---

## Platform Build Instructions

### Native Linux

```bash
bash ./build.sh --native --both
```

- On supported Ubuntu hosts, `build.sh` auto-detects the OS version and installs missing packages automatically
- Native Linux release packaging targets Ubuntu `20.04`, `22.04`, `24.04`, and `25.10`
- Native Linux builds write directly to `outputs/`
- `--both` refreshes the full Ubuntu-native wallet files directly in `outputs/`
- `--daemon` refreshes the daemon-side Linux files directly in `outputs/`
- `--qt` refreshes the Qt wallet files directly in `outputs/`
- Native Ubuntu outputs are bare same-Ubuntu binaries that rely on host-installed native packages
- Native Ubuntu builds bootstrap Berkeley DB `4.8.30.NC` into a local repo cache and always link wallet builds against that copy
- Each Ubuntu output folder gets its own `install-deps.sh` and `README.md` for the non-BDB host runtime packages

### Linux (Docker)

Use `--pull-docker` to pull prebuilt images from Docker Hub, or `--build-docker` to build them locally from the Dockerfiles in `docker/`.

```bash
bash ./build.sh --native --both --pull-docker
bash ./build.sh --native --qt --pull-docker
bash ./build.sh --native --daemon --pull-docker
bash ./build.sh --native --both --build-docker
```

### AppImage

```bash
bash ./build.sh --appimage --pull-docker
```

- Uses `sidgrip/appimage-base:22.04`
- Produces a self-contained AppImage in `outputs/AppImage/`
- The output folder keeps `UniversalMolecule-0.15.21-x86_64.AppImage`, `README.md`, and `build-info.txt`
- Intended for Ubuntu `22.04+`
- Direct launch on Ubuntu `22.04.5` needs `sudo apt install libfuse2`
- Direct launch on Ubuntu `24.04.4` and `25.10` needs `sudo apt install libfuse2t64`
- If the host is missing that package, AppImage runtime startup fails with `dlopen(): error loading libfuse.so.2`
- Fallback launch remains `--appimage-extract-and-run`

### Windows

```bash
bash ./build.sh --windows --both --pull-docker
```

- Runs on Linux with Docker using `sidgrip/mxe-base:latest`
- Writes loose cross-built outputs to `outputs/Windows/`

### macOS

There are two macOS paths in this repo:

#### Cross-build from Linux

```bash
bash ./build.sh --macos --both --pull-docker
```

- Runs on Linux with Docker using `sidgrip/osxcross-base:sdk-26.2`
- Produces artifacts in `outputs/Macosx/`

#### Native build on macOS

```bash
bash ./build.sh --native --both
```

- Uses Homebrew on the Mac host
- `build.sh` installs missing Homebrew dependencies automatically
- Native macOS builds write to `outputs/Macosx/`

---

## Output Structure

```text
outputs/
├── AppImage/
│   ├── UniversalMolecule-0.15.21-x86_64.AppImage
│   ├── README.md
│   └── build-info.txt
├── Macosx/
│   ├── UniversalMolecule-Qt.app
│   ├── universalmolecule-cli-0.15.21
│   ├── universalmolecule-qt-0.15.21
│   ├── universalmolecule-tx-0.15.21
│   ├── universalmoleculed-0.15.21
│   └── build-info.txt
├── Ubuntu-20/
│   ├── README.md
│   ├── universalmolecule-256.png
│   ├── universalmolecule-cli
│   ├── universalmolecule.conf
│   ├── universalmolecule.desktop
│   ├── universalmolecule-qt
│   ├── universalmolecule-qt-bin
│   ├── universalmolecule-tx
│   ├── universalmoleculed
│   └── install-deps.sh
├── Ubuntu-22/
├── Ubuntu-24/
├── Ubuntu-25/
└── Windows/
    ├── universalmolecule-cli-0.15.21.exe
    ├── universalmolecule-qt-0.15.21.exe
    ├── universalmolecule-tx-0.15.21.exe
    ├── universalmoleculed-0.15.21.exe
    └── build-info.txt
```

For Ubuntu native builds, the current host's final wallet files land in `outputs/Ubuntu-20/`, `outputs/Ubuntu-22/`, `outputs/Ubuntu-24/`, or `outputs/Ubuntu-25/` depending on the detected Ubuntu release. These are bare Ubuntu-native binaries, so each Ubuntu folder gets its own `install-deps.sh`, `README.md`, and `universalmolecule.conf`. Berkeley DB `4.8.30.NC` is bootstrapped into a local repo cache by the builder rather than being installed from apt.

For Windows cross-builds from Linux, the output bundle lands in `outputs/Windows/` and contains the four `.exe` binaries plus `build-info.txt`.

For native macOS builds, the current host's daemon tools, `UniversalMolecule-Qt.app`, and the raw `universalmolecule-qt-0.15.21` binary all land in `outputs/Macosx/`.

For AppImage builds, `outputs/AppImage/` keeps `UniversalMolecule-0.15.21-x86_64.AppImage`, `README.md`, and `build-info.txt`.

---

## Docker Images

When using `--pull-docker`, the build script uses these prebuilt images:

| Image | Purpose |
|---|---|
| `sidgrip/native-base:20.04` | Native Linux Ubuntu 20.04 build |
| `sidgrip/native-base:22.04` | Native Linux Ubuntu 22.04 build |
| `sidgrip/native-base:24.04` | Native Linux Ubuntu 24.04 build |
| `sidgrip/native-base:25.10` | Native Linux Ubuntu 25.10 build |
| `sidgrip/appimage-base:22.04` | Ubuntu 22+ AppImage build |
| `sidgrip/mxe-base:latest` | Windows cross-compile |
| `sidgrip/osxcross-base:sdk-26.2` | macOS cross-compile |

---

## Multi-Coin Builder

For building wallets for all Blake-family coins [Blakecoin](https://github.com/SidGrip/Blakecoin), [Photon](https://github.com/SidGrip/photon), [BlakeBitcoin](https://github.com/SidGrip/BlakeBitcoin), [Electron-ELT](https://github.com/SidGrip/Electron-ELT), [UniversalMolecule](https://github.com/SidGrip/universalmol), and [Lithium](https://github.com/SidGrip/lithium), see the [Blakestream Installer](https://github.com/SidGrip/Blakestream-Installer).

## License

UniversalMolecule is released under the terms of the MIT license. See `COPYING` for more information.
