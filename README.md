# scivis

Get `fftw-2.1.5`

`curl -o fftw-2.1.5 http://www.fftw.org/fftw-2.1.5.tar.gz
tar -xvf fftw-2.1.5.tar.gz
rm fftw-2.1.5.tar.gz
cd fftw-2.1.5
./configure
make
sudo make install
cd ..
cp /usr/local/include/{fftw.h,rfftw.h} include/
sudo cp /usr/local/lib/{libfftw.a,libfftw.la,librfftw.a,librfftw.la} lib/
rm -rf fftw-2.1.5
`

Install GLUT

`sudo apt install freeglut3`