#!/usr/bin/env bash
# Build and install the krunner-notion-tasks plugin (Plasma 6, C++/Qt)
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
PREFIX="$HOME/.local"

# ── Build dependency check ────────────────────────────────────────────────────
echo "==> Checking build dependencies..."
missing=()
command -v cmake  >/dev/null 2>&1 || missing+=("cmake")
command -v g++    >/dev/null 2>&1 || missing+=("gcc")
command -v make   >/dev/null 2>&1 || missing+=("make")
pacman -Q extra-cmake-modules                                       >/dev/null 2>&1 || missing+=("extra-cmake-modules")
pkg-config --exists KF6Runner   2>/dev/null || pacman -Q krunner   >/dev/null 2>&1 || missing+=("krunner")
pkg-config --exists KF6KCMUtils 2>/dev/null || pacman -Q kcmutils  >/dev/null 2>&1 || missing+=("kcmutils")

if [[ ${#missing[@]} -gt 0 ]]; then
    echo "ERROR: Missing: ${missing[*]}"
    echo "  sudo pacman -S cmake gcc make krunner kcmutils extra-cmake-modules"
    exit 1
fi
echo "    OK"

# ── Build ─────────────────────────────────────────────────────────────────────
echo "==> Configuring..."
cmake -B "$BUILD_DIR" -S "$SCRIPT_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DKDE_INSTALL_QTPLUGINDIR="$PREFIX/lib/qt6/plugins" \
    -DKDE_INSTALL_DATADIR="$PREFIX/share"

echo "==> Building..."
cmake --build "$BUILD_DIR" --parallel

# ── Install ───────────────────────────────────────────────────────────────────
echo "==> Installing to $PREFIX ..."
cmake --install "$BUILD_DIR"

# ── Qt plugin path (Arch: ~/.local/lib/qt6/plugins not scanned by default) ───
ENV_DIR="$HOME/.config/plasma-workspace/env"
ENV_FILE="$ENV_DIR/qt-user-plugins.sh"
mkdir -p "$ENV_DIR"
if ! grep -q "qt6/plugins" "$ENV_FILE" 2>/dev/null; then
    echo "==> Writing $ENV_FILE ..."
    echo 'export QT_PLUGIN_PATH="$HOME/.local/lib/qt6/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}"' >> "$ENV_FILE"
fi
export QT_PLUGIN_PATH="$PREFIX/lib/qt6/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}"

# ── Flush KDE plugin cache ────────────────────────────────────────────────────
echo "==> Updating KDE plugin cache..."
kbuildsycoca6 --noincremental 2>/dev/null || true

# ── Restart KRunner ───────────────────────────────────────────────────────────
echo "==> Restarting KRunner ..."
kquitapp6 krunner 2>/dev/null || killall krunner 2>/dev/null || true
sleep 1
krunner &
disown

echo ""
echo "✓ Installation complete!"
echo "  Open KRunner (Alt+Space) and type:"
echo "    todo buy milk !high @today"
echo ""
echo "  Configure: KRunner settings → Notion Tasks → wrench icon"
echo "  Or type: todo :config"
