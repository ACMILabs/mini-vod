**ACMI Mini VOD** version 0.0.1.1

# ACMI Mini VOD - End user docs

The ACMI Mini VOD is a small program built on OpenFrameworks for running a touch screen Video on Demand. When you start the program, it takes a folder full of .MP4 files, displays a simple menu and lightweight player interface, ideal for use in a gallery environment.

## System Requirements

The exact system requirements haven't yet been defined. You will need a system that can comfortably decode 1920 x 1080 h.264 video, and handle basic OpenGL. It has been tested with the following systems, all running Windows 7 64-bit:

***
- Intel i5-2500K CPU @ 3.30Ghz
- 16gb RAM
- NVidia GTX 560

***
- Intel i7-4790S CPU @ 3.20GHz
- 8.00 GB RAM
- AMD Radeon R7 A265

***
- Intel i7-4790S CPU @ 3.20GHz
- 8.00 GB RAM
- Intel HD Graphics 4600

## Getting Started

These instructions assume that you already have a compiled `.exe` version of the ACMI Mini VOD for Windows. It assumes a Windows 7+ environment.

1.	Ensure that the Microsoft Visual C++ Redistributable DLLs have been installed on your computer (or are included in the ACMI mini VoD build). If not, these can be downloaded here: https://www.microsoft.com/en-au/download/details.aspx?id=48145.
2.	Ensure you have codecs installed to decompress the MP4 files. OpenFrameworks recommend the K-Lite Codec Pack, via the Ninite installer, which removes any adware from the installer before download: https://ninite.com/klitecodecs/
3.	Ensure VSync is enabled / enforced in the computer's video card drivers. If you're using Intel Integrated Graphics, follow these steps to force VSync: http://www.intel.com/content/www/us/en/support/graphics-drivers/000005552.html.

## Get some video files to play!

The player is optimised for 16:9 video files, in MP4 containers. Videos with aspect ratios other than 16:9 should be re-encoded as 16:9 videos, letterboxed or pillarboxed to preserve the original aspect ratio.

Source video files should be encoded to Youtube's standard Full HD encoding specs. More info here: https://support.google.com/youtube/answer/1722171?hl=en

Broadly these encoding settings are:

-	Pixel resolution: 1920 x 1080
-	Container: MP4
-	Audio codec: AAC-LC
-	Video codec: H.264
-	Video Bitrate (standard): 8 Mbps
-	Audio Bitrate (stereo): 384 kbps

### Steps to get content into the VoD

1. Copy encoded 16:9 MP4 videos into the following directory, relative to where the ACMI Mini VOD is saved: `data/videos/`
2. Create a poster image 16:9 JPEG image for each video. This should be 1920 x 1080, or the full resolution of the video file. The application scales the poster images at start-up, so there's no need to create thumbnail sized versions.
3. Give the poster image the exact the same filename as the `.mp4` file, but with a `.jpg` extension.
4. Create a plain text file for the title and description of the video, named exactly the same as the `.mp4` file, but with a `.txt` extension.
5. Within the text file, in the first line, write the title of the video.
6. Press enter, and write a description in the second line. Note that the application will only read the first two lines of this file, treating them as the Title and Description respectively. Any further lines will be ignored.
7. To italicise a word, wrap the word with underscore like this: `My _Very_ Good Movie`.
8. To italicise a phrase, each word must be italicised separately, like this: `_The_ _Lord_ _of_ _the_ _Rings_ was released in 2001`.

For each video, there will be three files. For example:

- `01_Dracula.mp4`
- `01_Dracula.jpg`
- `01_Dracula.txt`

The VoD requires at least one video to be present. Any more than 6 will likely break the current layout. 3 to 5 videos is ideal for this format.

## Interface

The Video on Demand has a menu screen with touch / clickable tiles that when pressed will launch a video. While the video plays, a user can click or touch anywhere on the screen to pause the video and display a controlbar. The back button in the top left of the screen will return to the menu screen. There is a single play / pause button in the controlbar at the bottom of the screen, and a progress bar which can be pressed to skip to a different part of the current clip.

There is no volume control within the application. Volume should be set at the operating system level to 100%.

## Keyboard shortcuts

- `SPACE` --- start / stop current video
- `s` --- toggle display of current settings and version number
- `f` --- toggle full screen
- `ESCAPE` --- exit out of application

## Customising settings

In the `data` folder, you will find a file named `settings.xml`. Particular application settings are defined here. As versions progress, more of the touch screen will be customisable through these parameters. At launch here are the settings that can be changed:

-	`controbar_show_length` - number of milliseconds the controlbar will remain after video starts playing (default 3000 - 3 seconds).
-	`controlbar_anim_length` - number of milliseconds for animation to hide controlbar (default `500` - half a second).
-	`icon_padding` - number of pixels surrounding play and back icons (default `10`).
-	`fonts_space_between_words` - number of pixels separating each word on menu screen (default `8.5`).
-	`menu_margin_x` - percent horizontal margin around menu tiles expressed as value between zero and one (default `0.15` - which means 15 percent).
-	`menu_margin_y` - percent vertical margin around menu tiles expressed as value between zero and one (default `0.1` - which means 10 percent).
-	`font_title_color` - colour for menu titles in comma separated RGB (default `252, 69, 19`).
-	`icon_highlight_color` - background color for back icon and hover state of play icon in comma separated RGB (default `252, 69, 19`).
-	`menu_background_filename` - background image for menu, relative to data folder. (default `background/background_menu.jpg`). Image should be 1920 x 1080.
-	`font_main_normal_filename` - TTF font file, relative to data folder for titles and descriptions. (default `fonts/opensans-regular.ttf`).
-	`font_main_italic_filename` - TTF font file, relative to data folder for titles and descriptions when text is italicised. (default `fonts/opensans-italic.ttf`).
-	`font_stats_filename` - TTF font file, relative to data folder used for stats overlay. (default `fonts/opensans-regular.ttf`).

## Troubleshooting

### Video takes a while to load at launch
When you start the application, the first time you go to play a video, there will likely be a delay as the codec software is launched. After this is ready, the application should launch additional videos almost immediately.

### Colours, icons or fonts not displaying correctly
If the colours, icons or fonts have changed or are not displaying correctly, double check that the appropriate files are all in their correct places in the `/data/` sub-folders. Next, double-check the `settings.xml` file has not been changed, and recreate the file from a backup.

### Video will not play
If videos will not play, double check that they have the `.mp4` file extension in lowercase, and that you have the correct codecs installed. Attempt to play the video using Windows Media Player, or any other video player that does _not_ come bundled with the codecs (i.e. don't check with VLC).

### Restore settings.xml
To restore the player to application default settings, delete the `settings.xml` file, then launch the player. Press `s` to display the stats, and click on the disk icon at the top left of the screen (and top right of the stats panel). This will create a new `settings.xml` file. Note: if the player has been customised for a particular exhibition or installation, this will reset to _Application_ defaults, not to the defaults for the installation. In this case, you will need to copy your settings.xml file from a backup, or recreate the settings.

### Check version number
While the application is running, press `s` show the stats. In the top-right hand corner of the screen you should see current version number e.g. `ACMI Video on Demand v 0.0.1.0`.

### Change fonts
To change fonts, copy the new TTF fonts to the `/fonts/` directory, and update the appropriate parts of the `settings.xml` file. Note that they must be in TTF format, not OTF.
