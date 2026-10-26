<div align="center">
    <picture>
        <img src="shared/resource/blaadpapers.svg" alt="BlaadPapersLogo" width="20%"/>
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
        <source media="(prefers-color-scheme: dark)" srcset="https://github.com/user-attachments/assets/1c084af2-1a25-4363-90f6-b14ebd89eb8f">
        <source media="(prefers-color-scheme: light)" srcset="https://github.com/user-attachments/assets/e3ecf412-e560-4379-928d-cb49f93e8590">
        <img src="https://github.com/user-attachments/assets/e3ecf412-e560-4379-928d-cb49f93e8590" alt="Main Screen" width="100%"/>
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
- [`curl`](https://curl.se/)
- [`yyjson`](https://ibireme.github.io/yyjson/)
- [`imageinfo`](https://github.com/xiaozhuai/imageinfo)
- [`Qt6`](https://www.qt.io) (Base, Declarative, Svg, Multimedia)
- [`libvips`](https://github.com/libvips/libvips)
- [`FFmpeg`](https://ffmpeg.org/)
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
Code is licensed under GPL-3.0-or-later.
