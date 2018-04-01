#!/bin/sh

echo "Building app. Please wait..."

g++ -std=gnu++11 \
-pthread \
`find "../lib/oatpp/" -type f -name *.cpp` \
`find "./src/" -type f -name *.cpp` \
-I "../lib" \
-I "../lib/oatpp/core/macro" \
-I "../lib/oatpp/core/data/mapping/type/macro" \
-I "../lib/oatpp/web/client/macro" \
-I "../lib/oatpp/web/server/api/macro" \
-D OATPP_USE_TARGET \
-D OATPP_TARGET_APP \
-D OATPP_DISABLE_ENV_OBJECT_COUNTERS \
-O2 \
-Wall \
-o run_app

chmod +x run_app

echo "Done. type './run_app' to run"
