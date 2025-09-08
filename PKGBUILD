# Maintainer: Blaadick <null>

_reponame="BlaadPapers"
pkgname="blaadpapers"
pkgdesc="Fast & clean wallpaper manager"
pkgver="0.1.0"
pkgrel=1
arch=("any")
url="https://github.com/Blaadick/BlaadPapers"
license=("MIT")
depends=("qt6-base" "hyprpaper")
makedepends=("cmake" "ninja" "librsvg")
options=("!debug")
source=("blaadpapers-0.1.0.tar.gz::https://github.com/Blaadick/BlaadPapers/archive/refs/tags/v0.1.0.tar.gz")
sha256sums=("fe6df0ca2933fa1bd78c9854ac6979fa2a5050933362a37fcf18bdaca231bf26")

build() {
    cd "$_reponame-$pkgver"
    cmake -B cmake-build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build cmake-build-release -- -j 14
}

package() {
    cd "$_reponame-$pkgver"

    install -Dm755 "./cmake-build-release/$pkgname" "$pkgdir/usr/bin/$pkgname"
    install -Dm644 "./res/$pkgname.desktop" "$pkgdir/usr/share/applications/$pkgname.desktop"
    install -Dm644 "./res/$pkgname.fish" "$pkgdir/usr/share/fish/vendor_completions.d/$pkgname.fish"
    install -Dm644 "./res/$pkgname.svg" "$pkgdir/usr/share/icons/hicolor/scalable/apps/$pkgname.svg"

    for size in 16 22 24 32 36 48 64 72 96 128 192 256 384 512; do
        install -d "$pkgdir/usr/share/icons/hicolor/${size}x${size}/apps"
        rsvg-convert -w $size -h $size -o "$pkgdir/usr/share/icons/hicolor/${size}x${size}/apps/$pkgname.png" "./res/$pkgname.svg"
    done
}
