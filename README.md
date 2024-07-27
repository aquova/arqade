# Arqade

WIP libretro frontend.

This program will eventually be a Qt frontend for libretro cores. It is very fragile, but has been shown to work on some cores (mgba, bsnes, gambatte) with audio and save games.

Currently only works on Linux, and requires SDL2 and libretro cores to be installed.

## Core Status

### Working

Video, Input, and Audio works (as well as anything does so far)

- bsnes
- Gambatte
- mGBA
- Picodrive

### Starts

Gameplay begins, but either has corruption issues or crashes quickly

- Blastem - Video working, but scanlines are not properly aligned. Game crashes after a few seconds due to an audio driver bug.
- Nestopia - Game appears to be running, but there is no video or audio.
- Sameboy - Video works correctly, no audio

### Crashes Immediately

- Genesis Plus GX

All other cores are untested

## Credits

I owe much to understanding how libretro interfaces from this blog post - https://www.retroreversing.com/CreateALibRetroFrontEndInRust
