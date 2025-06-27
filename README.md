# MSIX 稀疏包注册器

一个 GUI 版 MSIX 稀疏包注册器，纯粹就是拿来辅助注册稀疏包的小工具，做成 UWP 不过就是为了个现代化界面。  
（为什么不选择 Windows App SDK？因为它一直都不稳定，一不小心，文字、图标全部消失，看你怎么用？）

## 使用方法

访问 [Releases 页面](https://github.com/IInspectable-Informal/SparsePackageRegisterUWP/releases)，下载最新版 (`bin_SparsePackageRegisterUWP.zip`)，解压后按照提示安装。

## 编译

建议不要自己编译，但如果一定要编译，请按照以下步骤操作：

### 0. 检查电脑配置

确保使用 **Windows 操作系统**，且 **Windows 版本号 ≥ 10.0.19041.0**，然后根据想要安装的 Visual Studio 版本参考以下内容：

- **Visual Studio 2019**：[系统要求](https://learn.microsoft.com/visualstudio/releases/2019/system-requirements)
- **Visual Studio 2022**：[系统要求](https://learn.microsoft.com/visualstudio/releases/2022/system-requirements)

### 1. 下载 Visual Studio

- **Visual Studio 2019**（已停止主流支持，未来可能下架）
- **Visual Studio 2022**（目前可用）

访问 [Visual Studio 官网](https://visualstudio.microsoft.com/zh-hans/downloads/)，下载社区版 (Community) 或其他版本。

### 2. 安装 Visual Studio

1. 运行安装程序，按照提示完成初始设置
2. 工作负荷选择：
   - **必选**：`使用 C++ 的桌面开发`
   - **Visual Studio 2019**：
     - 勾选 `通用 Windows 平台开发` 工作负荷
     - 然后在右侧的“**安装详细信息**”中，展开“**通用 Windows 平台开发**”，勾选 `C++ (v142) 通用 Windows 平台工具` 可选项
   - **Visual Studio 2022**：
     - 勾选 `WinUI 应用程序开发` 工作负荷
     - 然后在右侧的“**安装详细信息**”中，展开“**WinUI 应用程序开发**”，勾选 `通用 Windows 平台工具` 和 `C++ (v143) 通用 Windows 平台工具` 可选项
3. 建议安装 [Windows SDK 10.0.26100.0](https://developer.microsoft.com/windows/downloads/windows-sdk/)（本项目引用该 SDK）  
   或者修改项目目标 Windows 版本（不得低于 `10.0.19041.0`）

### 3. 下载并打开项目

1. 下载 [项目源代码](https://github.com/IInspectable-Informal/SparsePackageRegisterUWP/archive/refs/heads/main.zip)
2. 解压全部文件
3. 打开 `.sln` 解决方案文件
4. 等待 Visual Studio 加载完成

### 4. 编译项目

1. **还原 NuGet 包**
2. 选择目标架构 (`x86`/`x64`/`arm`/`arm64`)
3. 依次点击：`项目(P)` → `发布(P)` → `创建应用程序包(P)...`

#### 打包步骤：

1. **选择分发方法**：
   - 选择"旁加载"
   - 取消勾选"启用自动更新"（按需选择）
   - 点击"下一步"

2. **选择签名方法**：
   - 选择"是，选择证书"
   - 点击"创建(C)..."按钮
   - 设置发布者公用名称（必填）
   - 设置密码（可选）
   - 点击"确定" → "下一步"

   > ⚠️ **注意**：自签名证书安装时可能触发系统警告，需先启用开发人员模式
   >
   > **启用方法**：
   > - **Windows 10 2004+**：`设置` → `更新与安全` → `开发者选项`
   > - **Windows 11 21H2-22H2**：`设置` → `隐私和安全性` → `开发者选项`
   > - **Windows 11 23H2+**：`设置` → `系统` → `开发者选项`
   >
   > **替代方法**：
   > 1. 按 `Win + R`
   > 2. 输入：
   >    ```cmd
   >    start ms-settings:developers
   >    ```
   > 3. 启用"开发人员模式"
   >
   > **添加证书到受信任存储**：
   > 1. 双击生成的证书
   > 2. 选择"本地计算机"
   > 3. 按照向导完成导入
   > 4. 选择存储位置："受信任的证书颁发机构"或"受信任人"

3. **选择和配置包**：
   - 按需调整配置
   - 点击"创建(R)"按钮

4. 等待编译完成，即可安装应用程序包。
