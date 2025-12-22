Conan Directory
===============

Conan package manager configuration.

Contents
--------
- **profiles/**: Platform-specific Conan profiles (iOS, Android, etc.)

Create custom profiles here for cross-compilation targets.

Example
-------
To use a custom profile:
```bash
conan install . --profile=conan/profiles/ios_release.profile --build=missing
```
