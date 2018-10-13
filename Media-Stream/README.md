# Media-Stream (Http-Live-Streaming) Example
Example project of how-to build HLS-streaming server using oat++ Async-API.

Live stream tested with Safari-Browser and VLC-player

More about oat++:
- Website: [https://oatpp.io](https://oatpp.io)
- Docs: [https://oatpp.io/docs/start](https://oatpp.io/docs/start)

## Overview

Server generates infinite .m3u8 playlist from the pre-generated 
```playlist_live.m3u8``` file in the ```video``` folder.  
File ```video/playlist_live.m3u8``` together with video chunks is generated using ```ffmpeg```.  

Server is built using oat++ Async-Api and has following endpoints:

- ```("GET", "/", Root)``` - Starting html-page aka index.html
- ```("GET", "video/*", Video)``` - Embedded video html-page 
- ```("GET", "media/live", Live)``` - Infinite HLS playlist for live-stream describing video chunks
- ```("GET", "media/*", Static)``` - Endpoint serving media files with range requests support

### Screenshots
<img src="https://github.com/oatpp/oatpp-examples/blob/master/Media-Stream/screenshots/screen-2.png" width="33%"><img src="https://github.com/oatpp/oatpp-examples/blob/master/Media-Stream/screenshots/screen-3.png" width="33%"><img src="https://github.com/oatpp/oatpp-examples/blob/master/Media-Stream/screenshots/screen-4.png" width="33%">



## Build and run

1) Git-Clone examples repo:
```
$ git clone --recurse-submodules https://github.com/oatpp/oatpp-examples
```

2) CD to this example 
```
$ cd oatpp-examples/Media-Stream/
```
3) Build project
```
$ ./build_app.sh
```
This will just call g++

4) Run app
```
./run_app
```
5) Read 'Documentation' section of README

enjoy!

## More
If you can't build app.- probably you need to [install build-essential](https://www.google.com.ua/search?q=install+build-essentials)

## Xcode, MAC
Xcode project included

## Documentation

### Urls
- localhost:8000 - Index page
- localhost:8000/media/live - Live stream made from {repo}/video/video1.mp4 and {repo}/video/video2.mp4 played in the loop

### Files
- {repo}/video/playlist_live.m3u8 - playlist used for live HLS streaming
- {repo}/video/generate_pls.sh - example script used to generate sub-playlists and video fragmets using ffmpeg tool. #EXTINF sections of sub-playlists then have to be manualy moved to playlist_live.m3u8.

### Note
If app can't find playlist of video files then specify full-file-paths for {repo}/video/playlist_live.m3u8 file and {repo}/video folder in AppComponent.hpp
