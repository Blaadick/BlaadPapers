<div align="center">
    <img src="resource/blaadpapers.svg" alt="BlaadPapersLogo" width="135"/>
    <h1>BlaadPapers</h1>
</div>

> [!NOTE]
> The application is temporarily using hyprpaper on Hyprland, plasma-apply-wallpaperimage on KDE, and gsettings on Gnome

> [!NOTE]
> At the moment, with ~2500 wallpapers, the app is not particularly pleasant to use

## Description
Convenient wallpaper manager supporting both GUI and CLI usage. I will try to make it as fast and pleasing to the eye as possible.

Keep in mind that the application is being developed and tested on and for Linux systems. I will try to keep it running on other platforms as well, but this is not a priority.

## Installation
### Arch Linux
```bash
yay -S blaadpapers
```

### Manual
Install [BStyle](https://github.com/Blaadick/BStyle), `Qt6 Base`, `Qt6 QML` and `FFmpeg`. Then run it:

```bash
git clone https://github.com/Blaadick/BlaadPapers.git
cd BlaadPapers
cmake -B cmake-build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --parallel
```

### Windows
Expected after adding major features and implementing own wallpaper rendering.

## License
Code is licensed under GPL-3.0-only
