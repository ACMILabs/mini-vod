# ACMI Mini VOD

This is a simple application built in [OpenFrameworks](http://openframeworks.cc/) for running a touch screen Video on Demand, for use in exhibition spaces, or anywhere a VOD is required!

When the program starts, it trawls its `data/videos` directory looking for MP4 files to play, and corresponding JPG poster images, and text files for descriptions. It then displays a menu screen with a clickable tile for each video, which will then launch the video with a minimal interface (just play / pause, progress bar, and a back button).

The VOD works best with 3 to 5 videos, any more and the text on the menu screen will overflow. Future interfaces could easily extend this to scrollbars, pagination or other UI designs to accommodate more videos.

## Overview

To get up and running you'll need to install a few things and run a compiler before you have a working build. This might seem tedious at first, but it's fun! And it's the best way to learn OpenFrameworks.

## Getting started

OpenFrameworks is a cross-platform framework written in C++ that abstracts away the more complex bits of dealing with graphics libraries, video, input and other things to make it easier to build intensive visual and interactive applications. This particular application is fairly simple, but there's a few steps to get up and running.

Please note that although OpenFrameworks is cross-platform, we built this code targeting Windows PCs, as they're the ones we had readily available for spinning up touch screen kiosks and VODs. The code may require tweaking to get working on Mac or Linux.

1. Go to http://openframeworks.cc and follow the [setup guides](http://openframeworks.cc/download/) for your specific environment. If you're on Windows, [Visual Studio 2015 Community Edition](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx) is free for open source projects and small teams (even commercial work). Be sure to include `Common Tools for Visual C++ 2015` during the VS install process or you will not be able to compile OpenFrameworks projects. More info on the topic [can be found here](https://blogs.msdn.microsoft.com/vcblog/2015/07/24/setup-changes-in-visual-studio-2015-affecting-c-developers/).
2. If you're on Windows, make sure to install a codec pack for using MP4 / h.264 video. OpenFrameworks recommend the [K-Lite Codec Pack](http://openframeworks.cc/setup/vs/). If you're using this one, be sure to avoid an installer with adware!
3. Follow the [OpenFrameworks](http://openframeworks.cc/setup/vs/) setup guide to create a blank project. In the project creation steps, be sure to include the following Offical addons:
	- ofxGui
	- ofxXmlSettings
4. Next, replace the `main.cpp`, `ofApp.cpp`, and `ofApp.h` with the files in this repo. Copy all sub-directories from the repo (fonts, icons) to the project's `bin/data/` directory.
5. Add some MP4 videos to `bin/data/videos`. If you need a video for testing, [Big Buck Bunny](https://peach.blender.org/download/) has some good MP4s. For each video, add a JPEG poster image with the exact same filename (including case). E.g. for `my_video_01.mp4` create `my_video_01.jpg`.
6. Create a text file, also with the same name as the video, but with the file extension `.txt`. E.g. for `my_video_01.mp4` create `_my_video_01.txt`.
	- Within the text file, write a title as the first line.
	- Press enter and write a description in the second line.
	- Be sure not to press enter or create new lines in the description. The application only reads the first two lines of the text file, so any further lines will be ignored.
	- You can make particular words italic by wrapping the word in underscores. This is not quite like Markdown where you just add underscores or asterisks to the beginning and ends of a phrase or sentence. If you want an entire phrase or sentence to be italic, here you will need to wrap each word with underscores individually. This makes it slightly easier to parse the strings.
7. Optionally, add a 1920 x 1080 background image to `bin/data/backgrounds`, labelled `background_menu.jpg`. This will display behind the menu.
8. If you're using Visual Studio, make sure to select `Release Build`, and compile and run the application.

Once you have a successfully compiled version of the VOD, follow the end user notes for more details on fine-tuning settings and the 

To distribute the application to a target PC, be sure to bundle in the VC++ runtime DLLs before copying over your `bin` folder to the new PC. Alternately, install the VC++ redistributable packages on the new PC.

## Troubleshooting

This VOD attempts to run with VSync, but on same graphics cards the video may decode without VSync. If you're using Intel Integrated Graphics, follow these steps to force VSync: http://www.intel.com/content/www/us/en/support/graphics-drivers/000005552.html.

## Licence

The ACMI Mini VOD code itself is released under the [MIT License](https://en.wikipedia.org/wiki/MIT_License).

OpenFrameworks is distributed under the [MIT License](http://openframeworks.cc/about/license/).

Bundled with this application is the excellent [Open Sans](https://en.wikipedia.org/wiki/Open_Sans) font by [Steve Matteson](https://en.wikipedia.org/wiki/Steve_Matteson), which was released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0), included in this repo.