<div align="center">
    <picture>
        <img src="resource/blaadpapers.svg" alt="BlaadPapersLogo" width="20%"/>
    </picture>
    <h1>BlaadPapers</h1>
</div>

> [!NOTE]
> The application is temporarily use [mpvpaper](https://github.com/GhostNaN/mpvpaper) as wallpaper renderer

## Description
Convenient wallpaper manager supporting both GUI and CLI usage. I will try to make it as fast and pleasing to the eye as possible.

## Preview

<div align="center">
    <picture>
        <img src="https://github.com/user-attachments/assets/66883352-e1d0-4389-9909-21a9c4b0dea9" alt="Main Screen" width="100%"/>
    </picture>
    <p><i>Main Screen</i></p>
</div>

## Installation
### Arch Linux
```bash
yay -S blaadpapers
```

### Windows
Expected after adding major features and implementing custom wallpaper renderer.

### Manual
Install deps: 
- `Qt6` (Base, Declarative, Svg)
- [`libvips`](https://github.com/libvips/libvips)
- [`FFmpeg`](https://ffmpeg.org/)
- [`nlohman-json`](https://github.com/nlohmann/json)
- [`imageinfo`](https://github.com/xiaozhuai/imageinfo)
- [`BStyle`](https://github.com/Blaadick/BStyle)

Then run it:
```bash
git clone https://github.com/Blaadick/BlaadPapers.git --depth=1
cd BlaadPapers
cmake -B cmake-build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --parallel
```

## Usage
Before use, run
`blaadpaperscli run-renderer`. You can add some [mpv arguments](https://mpv.io/manual/master/#command-interface) like
`blaadpaperscli run-renderer no-interpolation hwdec=vaapi`.

After that, you can run GUI with `blaadpapers`, or use CLI with `blaadpaperscli`.

Also, BlaadPapers supports deep linking, like this: [blaadpapers://shuffle?include=General](blaadpapers://shuffle?include=General).

## Afterword
Keep in mind that the application is being developed and tested on and for Linux systems. I will try to keep it running on other platforms as well, but this is not a priority.

## License
Code is licensed under GPL-3.0-only.
