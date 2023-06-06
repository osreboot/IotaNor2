# What is this?
This project is a new take on my old Ludum Dare 38 [solo submission](https://ldjam.com/events/ludum-dare/38/iota-nor). Featuring an experimental glass tile rendering system, new visuals, a complete mechanical overhaul, new audio and more!

<p align="center">
  <img src="https://github.com/osreboot/IotaNor2/blob/master/iota_nor_2.gif">
</p>

A compiled executable is avaliable for download in the [releases section](https://github.com/osreboot/IotaNor2/releases).

# Controls

🖱️ + <kbd>LMB</kbd> = place piece

<kbd>A</kbd> + <kbd>D</kbd> = rotate piece

<kbd>Space</kbd> = hold piece

<kbd>M</kbd> = mute audio

<kbd>Escape</kbd> = close application

# Assets
This project uses the [Rubik](https://fonts.google.com/specimen/Rubik/about) font which is available under the [Open Font License](https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL). All other visuals and audio were created exclusively for this project.

To achieve the glass tile lighting effect, the refractive properties of a 3D tile model were simulated in [Blender](https://www.blender.org/) and baked into a series of what I'm calling "refraction maps". For every tile pixel, the GLSL fragment shader samples nine refraction maps (representing a 3x3 grid surrouding the current tile) and blends the light together to produce the final pixel color.

The other visuals were created using a combination of [GIMP](https://www.gimp.org/) and [Inkscape](https://inkscape.org/).

The sound effects were recorded using foley and my Antlion Audio ModMic 4, and edited in [Audacity](https://www.audacityteam.org/). One sound effect (the tile place sound) was synthesized using [JFXR](https://jfxr.frozenfractal.com/).

# Dependencies

This project depends on the following libraries:

- [stb](https://github.com/nothings/stb) for image and font loading.
- [GLFW](https://github.com/glfw/glfw) for display management and input polling.
- [GLEW-cmake](https://github.com/Perlmint/glew-cmake) (and [GLEW](https://github.com/nigels-com/glew)) for loading GLFW.
- [miniaudio](https://github.com/mackron/miniaudio) for audio loading and playback.
