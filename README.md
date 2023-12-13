# SkywalkerGameServer

一套完整的服务器代码框架，除系统库之外，全部自己手写，性能不考虑，只是为了学习

## 目录说明

### Bin

执行目录

### SkywalkerGameClient

与**SkywalkerGameServer**配套的测试客户端

### SkywalkerGameServer

服务器

### SkywalkerScript

专用脚本编码工具

### SkywalkerServerEditor

服务器编辑器

### SkywalkerServerFramework

服务器框架

### SkywalkerTools

各种独立的工具

## 架构设想

### 关于多线程

分成两部分来设计

#### 单个 `CSkywalkerServerFramework`

每个 `CSkywalkerServerFramework`中本身存在多线程，线程只能在 `CSkywalkerServerFramework`中通信

线程根据功能暂且分

* 网络线程，只用来处理网络消息
* 资源管理线程，管理服务器的资源
* 场景线程，管理场景
* 主线程，负责游戏的逻辑

#### 多个 `CSkywalkerServerFramework`

可以使用多个 `CSkywalkerServerFramework`，将每个 `CSkywalkerServerFramework`当成一个线程，`CSkywalkerServerFramework`之间可以通信

为什么实现多个 `CSkywalkerServerFramework`设计

1. 方便一个编辑器管理全部服务器

### 关于 `CSkywalkerServerFramework`设计

分成三部分组成

#### Core

这中间的代码会生成一个静态链接库

#### SkywalkerServerFramework

会链接 `Core`生成的静态链接库，然后自身也会生成一个静态链接库

#### Plugin

里面存放了各种功能独立的插件(Plugin)，每个插件都会生成一个动态库，通过配置加载需要的动态库即可

## FAQ记录一些开发过程中遇到的问题

### 宏定义函数报错

* 是不是使用了单行注释，换成多行注释试下

### 枚举未声明

* 是不是它前面那个使用了中文注释，修改一下编码试试
