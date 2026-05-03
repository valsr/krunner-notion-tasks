#!/usr/bin/env bash
# Uninstall the krunner-notion-tasks plugin (Plasma 6, C++/Qt)
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MANIFEST="$SCRIPT_DIR/build/install_manifest.txt"

if [[ ! -f "$MANIFEST" ]]; then
    echo "ERROR: No install manifest found at $MANIFEST"
    echo "       Run ./install.sh first, then uninstall."
    exit 1
fi

echo "==> Removing installed files..."
while IFS= read -r file; do
    if [[ -f "$file" ]]; then
        rm -f "$file"
        echo "    Removed: $file"
    fi
done < "$MANIFEST"

CFG="$HOME/.config/krunner-notion-tasksrc"
if [[ -f "$CFG" ]]; then
    read -r -p "Remove config file $CFG? [y/N] " confirm
    if [[ "${confirm,,}" == "y" ]]; then
        rm -f "$CFG"
        echo "    Config removed."
    else
        echo "    Config kept."
    fi
fi

echo "==> Updating KDE plugin cache..."
kbuildsycoca6 --noincremental 2>/dev/null || true

echo "==> Restarting KRunner ..."
kquitapp6 krunner 2>/dev/null || killall krunner 2>/dev/null || true
krunner &
disown

echo ""
echo "✓ Uninstall complete."
