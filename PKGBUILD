# Maintainer: vals <valsodarg@gmail.com>
pkgname=krunner-notion-tasks-git
pkgver=1.0.1
pkgrel=1
pkgdesc="KRunner plugin (Plasma 6) to add tasks to a Notion database"
arch=('x86_64')
url="https://gitlab.com/PLACEHOLDER/krunner-notion-tasks"
license=('GPL-2.0-or-later')
depends=('krunner' 'kcmutils' 'knotifications')
makedepends=('cmake' 'extra-cmake-modules' 'git')
provides=('krunner-notion-tasks')
conflicts=('krunner-notion-tasks')
source=("$pkgname::git+${url}.git")
sha256sums=('SKIP')

pkgver() {
    cd "$pkgname"
    printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
    cmake -B build -S "$pkgname" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build --parallel
}

package() {
    DESTDIR="$pkgdir" cmake --install build
}
