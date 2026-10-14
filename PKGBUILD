_repoowner="Blaadick"
_reponame="BlaadPapers"

pkgname="blaadpapers"
pkgdesc="Fast & clean wallpaper manager"
license=("GPL-3.0-or-later")
pkgver="0.4.3"
pkgrel=1
arch=("any")
depends=(
    "qt6-base"
    "qt6-declarative"
    "qt6-svg"
    "qt6-imageformats"
    "curl"
    "libvips"
    "ffmpeg"
    "mpvpaper"
    "bstyle-qml-theme"
)
makedepends=(
    "cmake"
    "ninja"
    "yyjson"
    "imageinfo"
)
provides=("blaadpapers", "blaadpaperscli")
source=("$pkgname-$pkgver.tar.gz::https://github.com/$_repoowner/$_reponame/archive/refs/tags/v$pkgver.tar.gz")
sha512sums=("b032c722281e2259c90cdfa59cb1a819aaf845b4651b7b0d87b513fdc68c147869b913ca6ecccfa31e8fd7e6d317afd1e0ee3de8406c08f3811d3922add72dcb")
url="https://github.com/$_repoowner/$_reponame"

build() {
    cd "$_reponame-$pkgver"

    cmake -B cmake-build-release-linux -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
    cmake --build cmake-build-release-linux --parallel
}

package() {
    cd "$_reponame-$pkgver"

    install -Dm755 "cmake-build-release-linux/gui/$pkgname" "$pkgdir/usr/bin/$pkgname"
    install -Dm755 "cmake-build-release-linux/cli/${pkgname}cli" "$pkgdir/usr/bin/${pkgname}cli"
    install -Dm644 "LICENSE" "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
    install -Dm644 "gui/resource/$pkgname.desktop" "$pkgdir/usr/share/applications/$pkgname.desktop"
    install -Dm644 "cli/resource/$pkgname-install.desktop" "$pkgdir/usr/share/applications/$pkgname-install.desktop"
    install -Dm644 "cli/resource/$pkgname-url.desktop" "$pkgdir/usr/share/applications/$pkgname-url.desktop"
    install -Dm644 "cli/resource/${pkgname}cli.fish" "$pkgdir/usr/share/fish/vendor_completions.d/${pkgname}cli.fish"
    install -Dm644 "cli/resource/${pkgname}cli.zsh" "$pkgdir/usr/share/zsh/site-functions/_${pkgname}cli"
    install -Dm644 "shared/resource/$pkgname.svg" "$pkgdir/usr/share/icons/hicolor/scalable/apps/$pkgname.svg"

    for size in 8 16 18 22 24 32 36 42 44 48 64 72 84 96 128 256 512 1024; do
        install -d "$pkgdir/usr/share/icons/hicolor/${size}x${size}/apps"
        vips thumbnail "shared/resource/$pkgname.svg" "$pkgdir/usr/share/icons/hicolor/${size}x${size}/apps/$pkgname.png" $size
    done
}
