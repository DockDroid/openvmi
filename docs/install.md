#安装说明
## 系统安装
### 安装依赖软件
```
$ sudo apt install -y build-essential cmake cmake-data debhelper dbus google-mock \
    libboost-dev libboost-filesystem-dev libboost-log-dev libboost-iostreams-dev \
    libboost-program-options-dev libboost-system-dev libboost-test-dev \
    libboost-thread-dev libcap-dev libsystemd-dev libegl1-mesa-dev \
    libgles2-mesa-dev libglm-dev libgtest-dev liblxc1 \
    libproperties-cpp-dev libprotobuf-dev libsdl2-dev libsdl2-image-dev lxc-dev \
    pkg-config protobuf-compiler python-minimal libva-dev libx264-dev libfdk-aac-dev \
    linux-headers-`uname -r`
```
## K8S安装
## OPENVMI安装
- 下载源码
```
$ cd ~
$ git clone https://github.com/DockDroid/openvmi.git
```
- 编译与安装内核模块
```
$ sudo mkdir /opt/openvmi/driver/
$ cd ~/openvmi/kernel/binder/
$ make
$ sudo cp binder_linux.ko /opt/openvmi/driver/
$ cd ../ashmem/
$ make
$ sudo cp ashmem_linux.ko /opt/openvmi/driver/
```
- 编译与安装openvmi
```
$ cd ~/openvmi
$ mkdir build
$ cd build
$ cmake ..
$ make -j8 install
$ sudo mkdir /opt/openvmi/bin/ /opt/openvmi/libs/
$ sudo mkdir /opt/openvmi/libtranslator /opt/openvmi/libangle /opt/openvmi/libswiftshader
$ sudo cp bin/openvmi /opt/openvmi/bin/
$ sudo cp ../libEGL_so/swiftshader/* /opt/openvmi/libs/libswiftshader/
$ sudo cp ../libEGL_so/translator/* /opt/openvmi/libs/libtranslator/
$ sudo cp ../libs/angle/lib/* /opt/opt/openvmi/libangle/
```
## Android镜像编译