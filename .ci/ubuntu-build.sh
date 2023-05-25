#!/bin/bash

# Build
.ci/common/build.sh linux &&

# Build linuxdeployqt
sudo apt install -y zsync patchelf desktop-file-utils libxcb-cursor0
sudo apt install -y qtbase5-dev
git clone https://github.com/Open-Typer/linuxdeployqt linuxdeployqt-build
cd linuxdeployqt-build
/usr/bin/qmake
make -j$(nproc --all)
mv bin/linuxdeployqt ..
cd ..
rm -rf linuxdeployqt-build
wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool*.AppImage
sudo mv appimagetool*.AppImage /usr/bin/appimagetool

# Build AppImage
mkdir -p AppDir/usr/bin &&
mkdir -p AppDir/usr/lib &&
mkdir -p AppDir/usr/share/applications &&
cp -r res/linux-release/usr/share/pixmaps AppDir/usr/share/ &&
cp res/linux-release/usr/share/applications/open-typer-appimage.desktop AppDir/usr/share/applications/open-typer.desktop &&
cp "$executable_name" AppDir/usr/bin/
cp *.so* AppDir/usr/lib/
./linuxdeployqt AppDir/usr/share/applications/*.desktop -qmldir=src -updateinformation="zsync|${appimage_zsync_url}" -appimage &&
rm linuxdeployqt
