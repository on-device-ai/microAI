# How to build microAI for CAM-C1126S2U  
The following instructions are for software building under the Ubuntu 16.04 operating system. For more information, please refer to the manufacturer's wiki page: [Source Compilation](https://wiki.t-firefly.com/en/CAM-C11262U/Source_code.html).  
Please follow the steps below to build the software of MicroAI for CAM-C1126S2U :  

1. Install the necessary software on the Ubuntu 16.04 operating system :  
`sudo apt-get install repo git-core gitk git-gui gcc-arm-linux-gnueabihf u-boot-tools device-tree-compiler gcc-aarch64-linux-gnu mtools parted libudev-dev libusb-1.0-0-dev python-linaro-image-tools linaro-image-tools autoconf autotools-dev libsigsegv2 m4 intltool libdrm-dev curl sed make binutils build-essential gcc g++ bash patch gzip gawk bzip2 perl tar cpio python unzip rsync file bc wget libncurses5 libqt4-dev libglib2.0-dev libgtk2.0-dev libglade2-dev cvs git mercurial rsync openssh-client subversion asciidoc w3m dblatex graphviz python-matplotlib libc6:i386 libssl-dev expect fakeroot cmake flex bison liblz4-tool libtool keychain`  
2. Download rv1126\_rv1109\_linux\_ai\_camera\_release\_20210306.tgz from [Google Drive](https://drive.google.com/drive/folders/1j_-YrJEy0ldmyujr14jKK97WGYq9SR3n)  
3. Decompression rv1126\_rv1109\_linux\_ai\_camera\_release\_20210306.tgz :  
`tar xvf rv1126_rv1109_linux_ai_camera_release_20210306.tgz`  
4. Change the decompressed folder name :  
`mv rv1126_rv1109_linux_ai_camera_release_20210306 rv1126_rv1109_linux_ai_camera`
5. Enter the folder to complete the synchronization work :  
`cd rv1126_rv1109_linux_ai_camera`  
`.repo/repo/repo sync -l`  
`.repo/repo/repo sync -c --no-tag`  
`.repo/repo/repo start firefly --all`  
6. Follow the micoAI's [rv1126\_rv1109\_linux\_a\_camera](https://github.com/on-device-ai/microAI/blob/main/rv1126_rv1109_linux_ai_camera) modification content, add and modify the source code files of CAM-C1126S2U. The complete source code is here: [https://tinyurl.com/uy9a8krw](https://tinyurl.com/uy9a8krw).  
7. Select the configuration file cam-crv1126s2u-uvcc.mk to configure :  
`./build.sh cam-crv1126s2u-uvcc.mk`  
8. Building the software :  
`./build.sh`  
