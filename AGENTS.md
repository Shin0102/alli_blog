# AGENTS.md

## Overview
- This repository is a personal blog built with Hugo.
- The active theme is `zzo`.
- Primary content is Korean (`defaultContentLanguage = "ko"`).

## Project Layout
- `content/`: blog posts and page content.
- `config/`: Hugo site configuration.
- `layouts/`: site layout overrides and custom templates.
- `assets/`: processed assets managed by Hugo Pipes.
- `static/`: static files copied as-is.
- `themes/zzo/`: upstream theme source. Prefer overrides in the root project before editing the theme directly.
- `public/`: generated site output. Do not hand-edit generated files unless the task is explicitly about build artifacts.

## Working Rules
- Prefer minimal, targeted changes that fit the existing Hugo and theme structure.
- For presentation changes, check whether the right place is `layouts/`, `assets/`, or `config/` before editing theme files.
- Avoid modifying `public/` by hand. Rebuild it from source changes instead.
- Preserve existing Korean copy unless the task explicitly asks for content edits or translation.

## Local Commands
- Start local server: `hugo server -D`
- Production build: `hugo`
- Deploy script: `./deploy.sh`

## Validation
- After template, config, or content changes, run `hugo` to catch build errors.
- If working on local preview behavior, use `hugo server -D`.

## Notes For Agents
- Check for existing overrides before introducing new partials, templates, or assets.
- Keep theme customizations easy to maintain across future `zzo` updates.
