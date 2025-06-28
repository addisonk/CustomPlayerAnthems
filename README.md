# CustomPlayerAnthems - BakkesMod Plugin

A modern BakkesMod plugin for custom player anthems in Rocket League, built with optimized cross-platform development workflow.

## Development Workflow

```
Mac (development) → GitHub Actions (Windows build) → Download .dll + .set → PC (BakkesMod install)
```

This plugin is developed using the **optimized Mac-to-GitHub Actions workflow**, allowing full development on Mac while automatically building Windows-compatible `.dll` files via GitHub Actions.

## Features

- 🎵 Custom player anthems
- ⚡ Optimized build pipeline with 50%+ faster builds
- 🔄 Automatic version management
- 📋 Professional CI/CD workflow
- 🚀 Cross-platform development (Mac → Windows)

## Quick Start

### For Development
1. Clone this repository
2. Make changes to plugin source in `CustomPlayerAnthems/`
3. Push to trigger automatic Windows build
4. Download artifacts from GitHub Actions

### For Users
1. Download latest release from [Releases](https://github.com/addisonk/CustomPlayerAnthems-Fresh/releases)
2. Copy `CustomPlayerAnthems.dll` to `%APPDATA%/bakkesmod/bakkesmod/plugins/`
3. Copy `CustomPlayerAnthems.set` to the same directory
4. Load in Rocket League: `plugin load CustomPlayerAnthems`

## Build Status

![Build Status](https://github.com/addisonk/CustomPlayerAnthems-Fresh/workflows/Optimized%20BakkesMod%20Plugin%20Build/badge.svg)

## Documentation

- 📖 [Fresh Start Guide](docs/fresh-start-guide.md) - Complete setup for new projects
- 🖥️ [Mac Development Guide](docs/mac-bakkesmod-development-guide.md) - Cross-platform workflow details

## Architecture

Built using:
- **BakkesModPluginTemplate** by Martinii89 (gold standard template)
- **Optimized GitHub Actions** with intelligent caching
- **Modern C++17** with BakkesMod SDK
- **ImGui** for user interface
- **Cross-platform CI/CD** for seamless development

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Push and create a pull request
5. GitHub Actions will automatically build and test

## License

[Add your license here]

---

*This plugin was created using the [Fresh Start Guide](docs/fresh-start-guide.md) for optimized BakkesMod plugin development.*
