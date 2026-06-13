# Changelog

## 1.2.0

### Added

- **Configurable due date field** — the Notion property used for due dates is now configurable via the KRunner settings UI ("Due date field") and the `due_field` key in `~/.config/krunner-notion-tasks.conf`. Defaults to `Due`.
- **`@field:value` syntax** — arbitrary Notion text fields can now be set inline. Examples:
  - `task Fix bug @tag:backend` — sets the "tag" field to "backend"
  - `task Fix bug @tag:backend @tag:critical` — sets the "tag" field to "backend, critical"
  - `task Buy milk @Due:today` — equivalent to `@today`

  Multiple values for the same field are joined with `, ` and sent as `rich_text`. The configured due field is still sent as a Notion `date` property with `today`/`tomorrow`/`tmr`/`YYYY-MM-DD` parsing.

## 1.0.1

- Initial public release
