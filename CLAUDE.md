# krunner-notion-tasks

A KRunner plugin (KDE Plasma 6) that lets you add tasks to a Notion database directly from the KRunner launcher bar.

## Architecture

Two compiled shared libraries, both installed as KDE plugins:

- **`krunner_notion_tasks`** — the runner (`src/notionrunner.*`). Loaded by `krunner` at startup. `match()` does pure string parsing (no network); `run()` makes the Notion API call after the user presses Enter.
- **`kcm_notion_tasks`** — the KCM config widget (`src/config/notionconfig.*`). Loaded in-process by KRunner settings when the user clicks the wrench icon next to the plugin.

The runner's JSON metadata (`src/notionrunner.json`) contains `"X-KDE-ConfigModule": "kf6/krunner/kcms/kcm_notion_tasks"` at the top level — this is what makes the wrench icon appear in KRunner settings.

Both plugins read/write `~/.config/krunner-notion-tasks.conf` via `KSharedConfig`.

## Build & install

```bash
./install.sh        # cmake build + install to ~/.local + restart krunner
./uninstall.sh      # removes installed files, optionally removes config
```

Dependencies (Arch/Manjaro): `cmake gcc make krunner kcmutils`

## Usage syntax

Open KRunner (`Alt+Space`) and type any configured trigger keyword followed by the task:

```
task Buy milk !high @today
todo Fix the bug !med @2026-05-10
```

Modifiers:
- `!high` / `!h` / `!medium` / `!med` / `!m` / `!low` / `!l` — priority
- `@today` / `@tomorrow` / `@tmr` / `@YYYY-MM-DD` — due date

## Config

Config file: `~/.config/krunner-notion-tasks.conf`

```ini
[notion]
# Your Notion internal integration token (starts with "ntn") See README.md for how to create one and share your
# database with it.
token=ntn_...

# Tasks database ID (default is pre-filled)
database_id=<32-char tasks DB ID>

# Projects database ID — used to look up your default project by name
projects_database_id=<32-char projects DB ID>

[runner]
# Trigger keyword in KRunner — e.g. "todo buy milk !high @today"
keyword=task,todo

# Name of the project to assign new tasks to (leave blank to skip)
default_project=Todos

# Page ID of the Notion template to apply to new tasks (leave blank to skip). Default is the "Todo" task template.
template_id=<32-char template page ID>
```

Sprint assignment is handled by a Notion automation — the plugin does not assign sprints.

## Key implementation notes

- `match()` iterates `cfg.keywords` (split on `,`) — first match wins, determines where task text starts
- `run()` is the only place network calls happen; `match()` must stay synchronous
- `QNetworkAccessManager + QEventLoop` pattern gives synchronous-style HTTP on worker threads
- Template cache uses `QMutex`-protected `QHash<QString, CacheEntry>` with TTLs: project 30 min, template blocks 1 hr
- `K_PLUGIN_CLASS_WITH_JSON` required for both plugins (not `K_PLUGIN_CLASS`)
- Runner installs to `kf6/krunner`, KCM to `kf6/krunner/kcms`
- After install, must run `kbuildsycoca6 --noincremental` to flush KDE plugin cache
