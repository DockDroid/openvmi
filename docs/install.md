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
- [搭建Android镜像编译环境](docs/搭建Android镜像编译环境.md)
