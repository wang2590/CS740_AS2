# CS740_AS2
a small program to demonstrate the key idea of Chord w/ DHT

### For Chord-Node join demonstration: 
For build: require Cmake 3.15, crypto and openssl library installed \
cd /Chord  \
mkdir build && cd build \
cmake .. && make  \
./chord \
OR  \
g++ src/Node.cpp main.cpp -L/usr/lib -lssl -lcrypto && ./a.out

### For DHT-Virtual Node demonstration:
cd /DHT \
g++ DHT.cpp main.cpp -L/usr/lib -lssl -lcrypto && ./a.out
