#!/bin/sh

ffmpeg -y \
 -i video1.mp4 \
 -c:v mpeg2video -qscale:v 10 \
 -c:a mp2 -b:a 192k \
 -vf scale=720x496 \
 -f segment \
 -segment_time 10 \
 -segment_list "playlist1.m3u8" \
 -segment_list_type m3u8 \
 "z_video1_cuhunk_%d.ts"

 ffmpeg -y \
 -i video2.mp4 \
 -c:v mpeg2video -qscale:v 10 \
 -c:a mp2 -b:a 192k \
 -vf scale=720x496 \
 -f segment \
 -segment_time 10 \
 -segment_list "playlist2.m3u8" \
 -segment_list_type m3u8 \
 "z_video2_cuhunk_%d.ts"
