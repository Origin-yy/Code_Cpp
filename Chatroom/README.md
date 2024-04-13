# Chatroom

这是来自另一个存储库的子目录 Chatroom 的迁移，[原项目位置](https://github.com/Yuanmxc/Code_cc/tree/main/Chatroom)

安装依赖（以Arch Linux为例）:

```bash
sudo pacman -S gcc cmake# 安装gcc,cmak
sudo pacman -S nlohmann-json # 安装 nlohmann-json 库
sudo pacman -S hiredis # 安装 hiredis库
sudo pacman -S redis # 安装 redis
sudo systemctl start redis # 启动 redis 服务
```

编译运行：

```bash
mkdir build
cd build
cmake ..
make
./server # 启动服务器
./client # 启动客户端
```

