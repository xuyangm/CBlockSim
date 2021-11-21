ls | grep -v start.sh | xargs rm -rf
cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=RELEASE ..
make -j8
./CBlockSim
