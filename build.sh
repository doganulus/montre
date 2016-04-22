cd ../montre-zone
make clean
make
cd ../montre-offline
rm libmontre.so
ln -s ../montre-zone/libmontre.so
pure -c main.pure -o montre
