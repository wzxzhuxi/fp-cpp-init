# fp-cpp-init

[![CI](https://github.com/wzxzhuxi/fp-cpp-init/actions/workflows/ci.yml/badge.svg)](https://github.com/wzxzhuxi/fp-cpp-init/actions/workflows/ci.yml)
[![Release](https://github.com/wzxzhuxi/fp-cpp-init/actions/workflows/release.yml/badge.svg)](https://github.com/wzxzhuxi/fp-cpp-init/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

C++ Project Scaffolding Tool - 快速初始化 C++ 项目。

## 特性

- **跨平台支持**：Linux、macOS、Windows
- **函数式架构**：纯函数设计，副作用隔离
- **三种项目类型**：
  - `exe` - 可执行文件项目
  - `lib` - 静态库项目
  - `header` - 头文件库项目
- **多种开源许可证**：MIT、Apache 2.0、GPL v3、BSD 3-Clause
- **自动配置**：生成 `.clang-format` 和 `.clang-tidy`
- **Git 集成**：自动读取 git config 中的作者信息
- **CI/CD**：自动化构建和发布 CI/CD

## 安装

### 从 Release 下载

前往 [Releases](https://github.com/wzxzhuxi/fp-cpp-init/releases) 下载对应平台的预编译二进制文件。

### 从源码编译

```bash
git clone https://github.com/wzxzhuxi/fp-cpp-init.git
cd fp-cpp-init

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# 安装（可选）
sudo cmake --install build
```

**依赖要求**：
- CMake 3.20+
- 支持 C++20 的编译器（GCC 10+, Clang 12+, MSVC 2019+）

## 使用

```bash
# 创建可执行文件项目（默认）
fp-cpp-init new myapp

# 创建静态库项目
fp-cpp-init new mylib --type=lib

# 创建头文件库项目
fp-cpp-init new myheader --type=header

# 指定 C++ 标准和许可证
fp-cpp-init new myproject --std=23 --license=apache2

# 指定作者和描述
fp-cpp-init new myproject --author="John Doe" --desc="My awesome project"

# 查看帮助
fp-cpp-init --help
fp-cpp-init new --help
```

## 参数

| 参数 | 短选项 | 默认值 | 说明 |
|------|--------|--------|------|
| `--type` | `-t` | `exe` | 项目类型：exe, lib, header |
| `--license` | `-l` | `mit` | 许可证：mit, apache2, gpl3, bsd3, none |
| `--std` | `-s` | `20` | C++ 标准：17, 20, 23 |
| `--author` | `-a` | git config | 作者名 |
| `--desc` | `-d` | 空 | 项目描述 |

## 生成的项目结构

### 可执行文件项目 (exe)

```
myapp/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── include/
│   └── myapp/
├── tests/
├── .gitignore
├── .clang-format
├── .clang-tidy
├── LICENSE
└── README.md
```

### 静态库项目 (lib)

```
mylib/
├── CMakeLists.txt
├── src/
│   └── mylib.cpp
├── include/
│   └── mylib/
│       └── mylib.hpp
├── tests/
│   ├── CMakeLists.txt
│   └── test_main.cpp
├── .gitignore
├── .clang-format
├── .clang-tidy
├── LICENSE
└── README.md
```

### 头文件库项目 (header)

```
myheader/
├── CMakeLists.txt
├── include/
│   └── myheader/
│       └── myheader.hpp
├── examples/
│   ├── CMakeLists.txt
│   └── example.cpp
├── tests/
│   ├── CMakeLists.txt
│   └── test_main.cpp
├── .gitignore
├── .clang-format
├── .clang-tidy
├── LICENSE
└── README.md
```

## 架构

本项目采用函数式编程风格：

```
┌─────────────────────────────────────────────────────────┐
│                      main.cpp                           │
│                  (Side Effect Boundary)                 │
│    ┌─────────┐  ┌─────────┐  ┌─────────┐                │
│    │ 读取CLI │→ │ 生成项目│→ │ 写入磁盘│                │
│    └─────────┘  └─────────┘  └─────────┘                │
└─────────────────────────────────────────────────────────┘
         ↓              ↓
    ┌─────────┐    ┌──────────────────────────┐
    │ cli.cpp │    │     Pure Functions       │
    │(纯函数) │    │ ┌────────┐ ┌─────────┐   │
    └─────────┘    │ │render  │ │ project │   │
                   │ └────────┘ └─────────┘   │
                   └──────────────────────────┘
```

- **Result<T>**：用于错误处理的 Monad
- **纯函数**：`parse_args`、`render_*`、`generate_project` 无副作用
- **副作用边界**：所有 IO 操作集中在 `main.cpp`

## CI/CD

本项目使用 GitHub Actions 实现自动化构建和发布。

### 什么是 CI/CD？

- **CI（Continuous Integration，持续集成）**：每次代码变更时自动构建和测试，尽早发现问题
- **CD（Continuous Delivery，持续交付）**：自动将通过测试的代码打包发布，用户可直接下载使用

### 工作流程

```
开发者 push 代码
       ↓
┌──────────────────────────────────────────────────────┐
│              GitHub Actions (CI)                     │
│                                                      │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐     │
│  │   Linux     │ │   macOS     │ │  Windows    │     │
│  │  ubuntu-    │ │  macos-     │ │  windows-   │     │
│  │  latest     │ │  latest     │ │  latest     │     │
│  └──────┬──────┘ └──────┬──────┘ └──────┬──────┘     │
│         │               │               │            │
│         ↓               ↓               ↓            │
│     cmake build     cmake build     cmake build      │
│         │               │               │            │
│         ↓               ↓               ↓            │
│     运行测试         运行测试        运行测试        │
│         │               │               │            │
│         └───────────────┼───────────────┘            │
│                         ↓                            │
│               全部通过 → ✅ 绿色                     │
│               任一失败 → ❌ 红色                     │
└──────────────────────────────────────────────────────┘
```

### CI 配置 (`.github/workflows/ci.yml`)

**触发条件**：
- 向 `main` 分支 push 代码
- 向 `main` 分支提交 Pull Request

**执行步骤**：
1. 检出代码 (`actions/checkout`)
2. 配置 CMake (`cmake -B build`)
3. 编译项目 (`cmake --build build`)
4. 运行测试 (`--help`, `--version`, 创建项目)

### Release 配置 (`.github/workflows/release.yml`)

**触发条件**：
推送以 `v` 开头的 Git 标签（如 `v0.1.0`, `v1.0.0`）

**执行步骤**：
1. 三平台并行编译 Release 版本
2. 重命名二进制文件（添加平台后缀）
3. 自动上传到 GitHub Releases 页面

**产出文件**：
- `fp-cpp-init-linux-x86_64` - Linux 版本
- `fp-cpp-init-macos-x86_64` - macOS 版本
- `fp-cpp-init-windows-x86_64.exe` - Windows 版本

### 如何发布新版本

```bash
# 1. 确保代码已提交
git add .
git commit -m "Release v0.1.0"

# 2. 创建版本标签
git tag v0.1.0

# 3. 推送标签到 GitHub（这会触发 Release 工作流）
git push origin v0.1.0

# 4. 等待几分钟，GitHub Actions 会自动：
#    - 在三个平台上编译
#    - 创建 Release 页面
#    - 上传预编译的二进制文件
```

### 查看构建状态

- 在 GitHub 仓库页面点击 **Actions** 标签页
- 绿色 ✅ 表示构建成功
- 红色 ❌ 表示构建失败，点击可查看详细日志

### 配置文件位置

```
.github/
└── workflows/
    ├── ci.yml      # CI 配置：自动构建测试
    └── release.yml # Release 配置：自动发布
```

## License

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！

- **Issue**：[https://github.com/wzxzhuxi/fp-cpp-init/issues](https://github.com/wzxzhuxi/fp-cpp-init/issues)
- **PR**：Fork 本仓库，创建分支，提交 PR
