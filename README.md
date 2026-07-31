# hoshi

**hoshi** is a modloader designed for Kirby Air Ride (NTSC). 

---

## Features

- **Mod File Loading**  
  **hoshi** detects and loads all compatible mod binaries placed in the `mods` folder on the game disc. Initialization is handled via callbacks defined in each mod.

- **Code Hooking**  
  Inject C code from mods into the dol at runtime.

- **Save File System**  
  **hoshi** creates and manages its own save file, automatically handling save data for mods.

- **Custom Settings Menu**  
  Add your own toggles and menus to a shared in-game settings menu, allowing users to easily configure your mod.

- **Scene Injection Support**  
  Add new major and minor scenes to the game.

---

## Installation

1. Place the [hoshi.bin](https://github.com/UnclePunch/hoshi/raw/refs/heads/main/out/release/hoshi.bin) in the root directory of your NTSC Kirby Air RIde iso.

2. Install the entrypoint [gecko code](https://github.com/UnclePunch/hoshi/blob/main/entrypoint/out/codes.txt) or [.gct file](https://github.com/UnclePunch/hoshi/blob/main/entrypoint/out/codes.gct) by your preferred means. This will kickstart the hoshi framework. I recommend using the .gct with [@JoshuaMKW](https://github.com/JoshuaMKW)'s [GeckoLoader](https://github.com/JoshuaMKW/GeckoLoader/releases), as it is a reliable and permanent installation within your .dol file.

---

## Create your own module

To develop your own module, clone the mod template repo [here](https://github.com/UnclePunch/hoshi-mod) and follow the instructions provided.

---

## Build Instructions

To build the hoshi modloader binary, see [BUILD](BUILD.md) file for details.

---

## Special Thanks
[@Ploaj](https://github.com/ploaj) - Symbol generation tooling.

---

## License

This project is licensed under the [GPLv3](./LICENSE).

---
