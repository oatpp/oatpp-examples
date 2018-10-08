#!/bin/sh

echo "Building app. Please wait..."

g++ -std=gnu++11 \
-pthread \
-I "/usr/local/Cellar/libressl/2.7.4/include" \
-L "/usr/local/Cellar/libressl/2.7.4/lib" \
-ltls -lssl -lcrypto \
`find "../lib/oatpp/" -type f -name *.cpp` \
`find "../lib/oatpp-libressl/" -type f -name *.cpp` \
`find "./src/" -type f -name *.cpp` \
-I "../lib" \
-D OATPP_USE_TARGET \
-D OATPP_TARGET_APP \
-D OATPP_DISABLE_ENV_OBJECT_COUNTERS \
-O2 \
-Wall \
-o run_app

chmod +x run_app

echo "Done. type './run_app' to run"
