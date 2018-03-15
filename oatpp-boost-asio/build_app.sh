#!/bin/sh

echo "Building app. Please wait..."

g++ -std=gnu++11 \
-pthread \
-lboost_thread -lboost_system \
`find "../lib/oatpp-lib/" -type f -name *.cpp` \
`find "./src/" -type f -name *.cpp` \
-I "../lib/oatpp-lib/core/src/macro" \
-I "../lib/oatpp-lib/core/src/data/mapping/type/macro" \
-I "../lib/oatpp-lib/web/src/client/macro" \
-I "../lib/oatpp-lib/web/src/server/api/macro" \
-D BOOST_ERROR_CODE_HEADER_ONLY \
-D OATPP_USE_TARGET \
-D OATPP_TARGET_APP \
-D OATPP_DISABLE_ENV_OBJECT_COUNTERS \
-O2 \
-Wall \
-o run_app

chmod +x run_app

echo "Done. type './run_app' to run"
