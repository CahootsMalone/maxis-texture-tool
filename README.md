# Maxis Texture Tool

This program can view, export, and replace images contained in Maxis composite bitmap files matching the format of those used to store the textures for [SimCopter](https://en.wikipedia.org/wiki/SimCopter) and [Streets of SimCity](https://en.wikipedia.org/wiki/Streets_of_SimCity).

Recommended for use with [SimCopterX](http://simcopter.net) or [SimStreetsX](http://www.streetsofsimcity.com/), which not only patch both games to ensure reliable operation when run under modern versions of Windows but also ensure that they use local resource files (when installed 1990s-style, both games usually load resource files from their CDs even when the installer has copied them to the hard drive). This means you can modify textures using this program, save a Maxis composite bitmap file with your changes, replace the original file (first making a backup, of course), and see your changes in-game when running via SimCopterX or SimStreetsX.

![A screenshot of the Maxis Texture Tool viewing one of the tree textures from SimCopter.](screenshot1.png)

## Instructions

* Load a palette from a Maxis mesh geometry file.
  * Geometry files are stored in each game's `geo` folder.
  * The palette can be exported as a PNG image or a [GIMP](https://www.gimp.org/) palette file.
* Load a Maxis composite bitmap file.
  * These are stored in each game's `bmp` folder, along with many regular Windows bitmaps.
  * Textures can be exported as PNG images individually or all at once.
  * Textures can be replaced. If the replacement texture contains colours that aren't included in the palette, they will be mapped to the nearest palette colour.
    * It's preferable to export the palette and use it to limit the colours of the replacement texture in suitable image editing software before replacement. The mapping in the MTT is primitive (it doesn't account for the fact that [human colour sensitivity varies by wavelength](https://en.wikipedia.org/wiki/Luminosity_function)) and doesn't perform any [dithering](https://en.wikipedia.org/wiki/Dither).
    * See below for instructions on how to map an image's colours to a palette using GIMP.
* Save a Maxis composite bitmap file with your changes.
  * Copy your modified file to the game's `bmp` folder, make a copy of the original file, and rename your file appropriately.
    * For example, if you modified `sim3d.bmp`, move or rename it, copy the file with your changes into the `bmp` folder, and rename it `sim3d.bmp`.
  * Run the game via SimCopterX or SimStreetsX and you should see your changes.
* See the in-program help for additional information.

## Mapping an Image's Colours to a Palette Using GIMP

* Using the MTT, export the palette as a GIMP palette file.
* Import the palette into GIMP (see section 3.5.3 [here](https://docs.gimp.org/2.10/en/gimp-palette-dialog.html)).
* Open the image and convert it to indexed colour mode (see [here](https://docs.gimp.org/2.10/en/gimp-image-convert-indexed.html)), specifying the imported palette under "Use custom palette" and selecting a dithering method if desired.
* Export the image as a PNG and import it into the MTT using the "Replace Current" function.