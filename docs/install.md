# 安装说明
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
## K8S云平台相关安装
### 1. Docker运行环境安装
本项目开发环境所使用的Docker版本为18.09.9，安装步骤如下：<br><br>
**Master(Worker)节点：**
- 创建Docker配置文件
```bash
    $ sudo mkdir -p /etc/docker
    $ sudo tee /etc/docker/daemon.json <<-'EOF'
    {
      "exec-opts": ["native.cgroupdriver=systemd"],
      "log-driver": "json-file",
      "log-opts": {
                       "max-size": "100m"
                   },
      "storage-driver": "overlay2"
    }
    EOF
```
- 添加Docker源
```bash
    $ sudo add-apt-repository \
      "deb [arch=arm64] https://download.docker.com/linux/ubuntu \
      $(lsb_release -cs) \
      stable"
```
- 安装Docker
```bash
    $ sudo apt-get install docker-ce=5:18.09.9~3-0~ubuntu-bionic
```
- 为当前用户增加docker命令操作权限(可选)
```bash
    $ sudo groupadd docker
    $ sudo usermod -aG docker $USER
    $ sudo service docker restart
    $ newgrp - docker
```
### 2. K8S运行环境安装
本项目开发环境所使用的K8S版本为1.14.2，安装步骤如下：<br><br>
**Master节点：**
- 永久关闭系统的swap功能 
```bash
    $ sudo swapoff -a
    $ sudo vi /etc/fstab (以#注释swapfile打头的那一行)
```
- 安装K8S相关命令行工具
```bash
    $ sudo apt-get install kubeadm kubernetes-cni kubectl kubelet
```
- 下载K8S Master节点运行组件的Docker镜像
```bash
    $ docker pull docker.io/mirrorgooglecontainers/kube-apiserver-arm64:v1.14.2 
    $ docker pull docker.io/mirrorgooglecontainers/kube-controller-manager-arm64:v1.14.2 
    $ docker pull docker.io/mirrorgooglecontainers/kube-scheduler-arm64:v1.14.2 
    $ docker pull docker.io/mirrorgooglecontainers/kube-proxy-arm64:v1.14.2 
    $ docker pull docker.io/mirrorgooglecontainers/pause-arm64:3.1 
    $ docker pull docker.io/mirrorgooglecontainers/etcd-arm64:3.3.10 
    $ docker pull docker.io/coredns/coredns:1.3.1
```
- 修改下载镜像的Tag
```bash
    $ docker tag docker.io/mirrorgooglecontainers/kube-apiserver-arm64:v1.14.2 k8s.gcr.io/kube-apiserver:v1.14.2 
    $ docker tag docker.io/mirrorgooglecontainers/kube-controller-manager-arm64:v1.14.2 k8s.gcr.io/kube-controller-manager:v1.14.2 
    $ docker tag docker.io/mirrorgooglecontainers/kube-scheduler-arm64:v1.14.2 k8s.gcr.io/kube-scheduler:v1.14.2 
    $ docker tag docker.io/mirrorgooglecontainers/kube-proxy-arm64:v1.14.2 k8s.gcr.io/kube-proxy:v1.14.2 
    $ docker tag docker.io/mirrorgooglecontainers/pause-arm64:3.1 k8s.gcr.io/pause:3.1 
    $ docker tag docker.io/mirrorgooglecontainers/etcd-arm64:3.3.10 k8s.gcr.io/etcd:3.3.10 
    $ docker tag docker.io/coredns/coredns:1.3.1 k8s.gcr.io/coredns:1.3.1
```
- 删除下载镜像的原Tag
```bash
    $ docker rmi docker.io/mirrorgooglecontainers/kube-apiserver-arm64:v1.14.2 
    $ docker rmi docker.io/mirrorgooglecontainers/kube-controller-manager-arm64:v1.14.2 
    $ docker rmi docker.io/mirrorgooglecontainers/kube-scheduler-arm64:v1.14.2 
    $ docker rmi docker.io/mirrorgooglecontainers/kube-proxy-arm64:v1.14.2 
    $ docker rmi docker.io/mirrorgooglecontainers/pause-arm64:3.1 
    $ docker rmi docker.io/mirrorgooglecontainers/etcd-arm64:3.3.10 
    $ docker rmi docker.io/coredns/coredns:1.3.1
```
- 安装K8S
```bash
    $ sudo kubeadm init --pod-network-cidr=10.244.0.0/16 --kubernetes-version=v1.14.2  
````
- 安装K8S网络插件
```bash
    $ kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml
```
- 为当前用户增加kubectl命令操作权限
```bash
    $ sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
    $ sudo chown $(id -u):$(id -g) $HOME/.kube/config
```
- 创建用于在Worker节点上执行将其加入到集群的命令
```bash
    $ kubeadm token create --print-join-command	
```
<br>**Worker节点：**
- 永久关闭系统的swap功能 
```bash
    $ sudo swapoff -a
    $ sudo vi /etc/fstab (以#注释swapfile打头的那一行)
```
- 安装K8S相关命令行工具 
```bash
    $ sudo apt-get install kubeadm kubernetes-cni kubectl kubelet
```
- 下载K8S Worker节点运行组件的Docker镜像
```bash
    $ docker pull docker.io/mirrorgooglecontainers/kube-proxy-arm64:v1.14.2 
    $ docker pull docker.io/mirrorgooglecontainers/pause-arm64:3.1 
```
- 修改下载镜像的Tag
```bash
    $ docker tag docker.io/mirrorgooglecontainers/kube-proxy-arm64:v1.14.2 k8s.gcr.io/kube-proxy:v1.14.2 
    $ docker tag docker.io/mirrorgooglecontainers/pause-arm64:3.1 k8s.gcr.io/pause:3.1 
```
- 删除下载镜像的原Tag
```bash
    $ docker rmi docker.io/mirrorgooglecontainers/kube-proxy-arm64:v1.14.2 
    $ docker rmi docker.io/mirrorgooglecontainers/pause-arm64:3.1 
```
- 执行Master节点安装流程最后一步中产生的加入到集群的命令

### 3. K8S设备插件服务安装
**Master(Worker)节点：**<br><br>
在安装该服务前请确保K8S运行环境已经安装成功，否则服务安装后将无法成功启动，安装步骤如下：
- 进入项目工程根目录下的“cloud-platform/k8s-dev-plugin-service"文件夹
```bash
    $ cd cloud-platform/k8s-dev-plugin-service
```
- 执行服务安装脚本
```bash
    $ sudo ./install.sh
```

### 4. K8S安卓运行环境管理服务安装
**Master(Worker)节点：**<br><br>
- 进入项目工程根目录下的“cloud-platform/android-env-service"文件夹
```bash
    $ cd cloud-platform/android-env-service
```
- 执行服务安装脚本
```bash
    $ sudo ./install.sh
```
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
$ sudo cp ../libs/angle/lib/* /opt/opt/openvmi/libs/libangle/
```
## Android镜像编译
### 搭建Android镜像编译环境
本部分介绍了如何设置本地工作环境来编译 Android 源文件。您需要使用 Linux 或 Mac OS。目前不支持在 Windows 环境下进行编译。

#### 设置 Linux 编译环境
Android系统版本是以Android7.1.1版本AOSP为基础源码开发的，通常是在 GNU/Linux 或 Mac OS 操作系统中进行编译。您也可以使用虚拟机在不支持的系统（例如 Windows）中编译 Android。但我们推荐使用 Ubuntu LTS (16.04、18.04) 乃至更高版本系统对 Android系统进行编译。欢迎向我们报告在其他分发版本中的测试结果（无论结果是成功还是失败）。

- 注意：要查看完整的硬件和软件要求列表，请参阅[AOSP编译软硬件要求](https://source.android.google.cn/source/requirements?hl=zh-cn)。然后，请按照下方适用于 Ubuntu 和 Mac OS 的详细说明进行操作。

##### 安装 JDK
在 Ubuntu 上，请使用 [OpenJDK](http://openjdk.java.net/install/)。要了解确切版本，请参阅 [JDK 要求](https://source.android.google.cn/source/requirements?hl=zh-cn#jdk)；要了解相关说明，请参阅以下各个部分。

###### 安装OpenJDK（如果 Ubuntu >= 16.04）
请运行以下命令：

```bash
sudo apt-get update
sudo apt-get install openjdk-8-jdk
```
###### 更新默认的 Java 版本 - 可选
（可选）对于以上 Ubuntu 版本，您可以通过运行以下命令来更新默认的 Java 版本：

```bash
sudo update-alternatives --config java
sudo update-alternatives --config javac
```
在编译过程中，如果您遇到 [Java 版本错误](https://source.android.google.cn/source/building?hl=zh-cn#wrong-java-version)，请按照错误的 Java 版本部分中的说明设置其路径。

##### 安装依赖库
执行以下命令，安装编译所需的依赖库：

```bash
sudo apt-get install libx11-dev:i386 libreadline6-dev:i386 libgl1-mesa-dev g++-multilib 
sudo apt-get install -y git flex bison gperf build-essential libncurses5-dev:i386 
sudo apt-get install tofrodos python-markdown libxml2-utils xsltproc zlib1g-dev:i386 
sudo apt-get install dpkg-dev libsdl1.2-dev libesd0-dev
sudo apt-get install git-core gnupg flex bison gperf build-essential  
sudo apt-get install zip curl zlib1g-dev gcc-multilib g++-multilib 
sudo apt-get install libc6-dev-i386 
sudo apt-get install lib32ncurses5-dev x11proto-core-dev libx11-dev 
sudo apt-get install libgl1-mesa-dev libxml2-utils xsltproc unzip m4
sudo apt-get install lib32z-dev ccache
```

##### 使用单独的输出目录
默认情况下，每次编译的输出都会存储在相应源代码树的 out/ 子目录下。

在一些拥有多个存储设备的计算机上，如果将源文件和输出存储在单独的存储卷中，编译速度会更快。若要进一步提高编译速度，可以将输出存储在已针对速度（而非崩溃稳定性）进行优化的文件系统中，这是因为在文件系统损坏时可以重新生成所有文件。
要进行这项设置，请导出 OUT_DIR_COMMON_BASE 变量，使其指向将存储输出目录的位置。

```bash
export OUT_DIR_COMMON_BASE=<path-to-your-out-directory>
```

#### 优化编译环境（可选）
##### 设置 ccache
您可以视需要指示编译过程使用 ccache 编译工具，ccache 是适用于 C 和 C++ 的编译器缓存，有助于提高编译速度。这对于编译服务器和其他高容量生产环境来说尤其有用。ccache 可用作用于加快重新编译速度的编译器缓存。如果您经常使用 make clean，或者经常在不同的编译产品之间切换，则非常适合使用 ccache。

    注意：如果您是在执行增量编译（例如个人开发者而非编译服务器），ccache 可能会让您为缓存未命中埋单，从而减慢您的编译速度。

要使用 ccache，请在源代码树的根目录下执行以下命令：

```bash
export USE_CCACHE=1
export CCACHE_DIR=/<path_of_your_choice>/.ccache
prebuilts/misc/linux-x86/ccache/ccache -M 50G
```
建议的缓存大小为 50G 到 100G。

请将以下内容添加到 .bashrc（或等同文件）中：

```bash
export USE_CCACHE=1
```
默认情况下，缓存将存储在 ~/.ccache 下。如果主目录位于 NFS 或一些其他的非本地文件系统中，您还需要在 .bashrc 文件中指定目录。

在 Mac OS 中，您应将 linux-x86 替换成 darwin-x86：

```bash
prebuilts/misc/darwin-x86/ccache/ccache -M 50G
```
在编译 Ice Cream Sandwich (4.0.x) 或更低版本时，ccache 位于其他位置：

```bash
prebuilt/linux-x86/ccache/ccache -M 50G
```
该设置会存储在 CCACHE_DIR 中，并且为永久设置。

在 Linux 中，您可以运行以下命令来观看使用 ccache 时的情况：

```bash
watch -n1 -d prebuilts/misc/linux-x86/ccache/ccache -s
```

