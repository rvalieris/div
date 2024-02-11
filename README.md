# div

a dry image viewer

## build
```bash
# qt5
qmake div.pro && make

# qt6
mkdir build && cd build
cmake -B. -S..
make
```
## dependencies
 * C++11
 * [Qt5 or Qt6](http://www.qt.io)
 * [libarchive](http://libarchive.org)

# 3rd party
 * [icons by Breeze project](https://projects.kde.org/projects/kde/workspace/breeze-icons)

# License
MIT License

### TODO
 * detect changes to directory/files
 * f5 refresh image/directory
 * on-the-fly zip file browsing

