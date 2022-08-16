<div align="center">
  <div style="
      z-index: 0; 
      top: 0; 
      left: 0;
      width: 1006px;
      height: 580px;
    ">
    <img src=".github/header.svg?raw=true" />
  </div>
  <div
    style="
      z-index: 1;
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      background-image: url(.github/UI.png?raw=true);
      width: 1006px;
      height: 580px;
      background-repeat: no-repeat;
      margin-left: auto;
      margin-right: auto;
      text-align: center;
    "></div>
</div>


## Quick links

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Installation/Uninstallation](#install)
- [Usage](#usage)
- [Gallery](#gallery)
- [Help](#help)
- [Acknowledgements](#acknowledgements)

<!----><a name="about"></a>
## About
B.A.M. adds an in-game interface that lets you add new things to the game (e.g., Blur powerups) and edit existing things (e.g., control time of day). See ['Gallery' below](#gallery) for screenshots.

<!----><a name="features"></a>
## Features

**Need for Speed Most Wanted 2005** v1.3 (RELOADED, English):

- Blur Powerups
- Bouncy Things
- Custom Car Scale
- Custom Time of Day
- D3D Point/Wireframe View
- Disable Game Pause
- LOD Control
- Mirrored and Upside Down World
- Various game feature togglers/cheats

**Need for Speed Carbon (Collector's Edition)** v1.4 (RELOADED, English):

- Blur Powerups
- Bouncy Things
- Custom Car Scale
- Custom Time of Day
- Disable Game Pause
- LOD Control
- Mirrored and Upside Down World
- Various game feature togglers/cheats

**Need for Speed ProStreet** v1.1 (BATTERY/ViTALiTY, English):

- Blur Powerups
- Bouncy Things
- Custom Time of Day
- Disable Game Pause
- LOD Control
- Various game feature togglers/cheats (including god mode)

<!----><a name="requirements"></a>
## Requirements
- Windows 7 SP1 or newer.
    - Windows 11 has difficulty running the games/B.A.M. as of now, see ['Help'](#help) if you have issues.
- The latest Visual C++ Redistributables. **BOTH [x86](https://aka.ms/vs/17/release/vc_redist.x86.exe) AND [x64](https://aka.ms/vs/17/release/vc_redist.x64.exe)!**
- [DirectX End-User Runtimes (June 2010)](https://www.microsoft.com/en-us/download/details.aspx?id=8109).
- The latest [DirectX End-User Runtimes](https://www.microsoft.com/en-us/download/details.aspx?id=35).

<!----><a name="install"></a>
## Installation
1. [Click here](https://github.com/berkayylmao/BerkaysAssortedMods/releases/latest).
2. Download the "BerkaysAssortedMods.zip" file.
3. Extract the zip file into the game folder.
4. Say yes to overwrite if asked.

### Uninstallation
Simply delete BAM.asi from your scripts folder (`<Game Folder>/scripts/BAM.asi`).

<!----><a name="usage"></a>
## Usage
1. Start the game.
2. Follow the tutorial.
3. Enjoy!

<!----><a name="gallery"></a>
## Gallery
<p align="center">
<img style="border-radius:10px" src=".github/MW05.png?raw=true" />
<img style="border-radius:10px" src=".github/Carbon.png?raw=true" />
<img style="border-radius:10px" src=".github/ProStreet.png?raw=true" />
</p>

<!----><a name="help"></a>
## Help
Is the game _'appearing'_ to start but then suddenly crash/free? Try **all** of these first:

1. Make sure you meet the [requirements](#requirements). Reboot your computer after installing all of them.

2. Make sure you are running one of [the supported games](#features).

3. Reset your B.A.M. settings (delete `<Game Folder>/scripts/BAM/Config.json`).

4. Set game executable compatibility to `Windows XP (Service Pack 3)` and try again.
<p align="center">
<img src=".github/WindowsContextMenu.png?raw=true" />
<img src=".github/WindowsCompatibilityWindow.png?raw=true" />
</p>

5. Set game executable to run as an administrator and try again.
<p align="center">
<img src=".github/WindowsRunAsAdminWindow.png?raw=true" />
</p>

6. Install a fresh copy of the game to a different folder and test it **_only_** with B.A.M.

7. If the problem still persists, create an issue on GitHub with as much information as you can give.

## Incompatibilities
None that I know of. This is a script mod so file mods (car mods, VLT scripts, Binary modules etc.) do not affect this.
