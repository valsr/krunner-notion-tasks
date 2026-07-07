# krunner-notion-tasks

A KRunner plugin (Plasma 6 / systemd) that lets you add tasks directly to your Notion Tasks database from the launcher.

```text
todo buy milk !high @today
todo fix the bug @2026-05-15 !medium
todo call dentist
```

Press **Enter** to create the task. A desktop notification confirms success.

---

## Syntax

```text
<keyword> <task name> [!priority] [@due]
```

| Token       | Values                                          | Example       |
| ----------- | ----------------------------------------------- | ------------- |
| `!priority` | `!high` / `!h`, `!medium` / `!m`, `!low` / `!l` | `!high`       |
| `@due`      | `@today`, `@tomorrow` / `@tmr`, `@YYYY-MM-DD`   | `@2026-05-10` |

The keyword defaults to `todo` and is configurable — see [Step 4](#step-4--configure).

---

## Step 1 — Create a Notion Integration Token

1. Go to **[notion.com/my-integrations](https://www.notion.com/my-integrations)**
2. Click **"+ New integration"**
3. Give it a name (e.g. *KRunner Tasks*), select your workspace, leave type as *Internal*
4. Click **Submit** — copy the **Internal Integration Token** (`secret_…`)

## Step 2 — Share your Tasks database with the integration

1. Open your **Tasks** database in Notion
2. Click the **⋯** menu → **Connections** → **Add connections**
3. Search for your integration name and click it

## Step 3 — Install

**Dependencies** (Arch / Manjaro):

```bash
sudo pacman -S python-dbus python-gobject
```

**Install:**

```bash
git clone https://github.com/valsr/krunner-notion-tasks.git
cd krunner-notion-tasks
./install.sh
```

The installer registers and starts a systemd user service (`krunner-notion-tasks.service`) that keeps the plugin running. It starts automatically at login.

## Step 4 — Configure

Type the config trigger in KRunner:

```text
todo :config
```

This shows a **Configure Notion Tasks** match — press Enter to open the settings dialog.
The dialog has a **Test connection** and **Test sprint lookup** button to verify your setup.

You can also launch the dialog directly from a terminal:

```bash
python3 ~/.local/bin/krunner_notion_tasks_config.py
```

> Requires PyQt6: `sudo pacman -S python-pyqt6`. Falls back to `kdialog` if unavailable.

### Config file

`~/.config/krunner-notion-tasks/config.ini` — changes take effect immediately, no restart needed.

---

## Uninstall

```bash
./uninstall.sh
```

---

## How it works

The plugin exposes the `org.kde.krunner1` DBus interface. KRunner auto-starts it via DBus
session activation; an autostart entry keeps it running from login so it is always available.

Tasks are created via the [Notion REST API](https://developers.notion.com/reference/post-page) with:

- **Status** → *Not Started*
- **Priority** → from `!priority` token (optional)
- **Due** → from `@date` token (optional)
- **Project** → default project from config (optional)
- **Template** → page body copied from configured template (optional)
