# Media-Stream (Http-Live-Streaming) Example
Example project of how-to build HLS-streaming server using oat++ Async-API.

Live stream tested with Safari-Browser and VLC-player

## Build and run

1) Git-Clone examples repo:
```
git clone https://github.com/oatpp/oatpp-examples
```

2) Get oatpp-lib (it is included as git submodule)
```
git submodule init
git submodule update --remote --merge
```

3) CD to crud example-project
```
cd crud/
```
4) Build project
```
./build_app.sh
```
or (same as in build_app.sh)
```
g++ -std=gnu++11 \
-pthread \
`find "../lib/oatpp-lib/" -type f -name *.cpp` \
`find "./src/" -type f -name *.cpp` \
-D OATPP_USE_TARGET \
-D OATPP_TARGET_APP \
-D OATPP_DISABLE_ENV_OBJECT_COUNTERS \
-O2 \
-Wall \
-o run_app

chmod +x run_app
```
5) Run app
```
./run_app
```
6) Read 'Documentation' section of README

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
