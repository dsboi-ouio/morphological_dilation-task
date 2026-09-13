# 二值图像 5×5 区域膨胀

这是一个使用 C++17 编写的形态学膨胀练习项目。程序从文本文件读取二值图像矩阵，使用固定的 `5×5` 全 1 结构元素进行膨胀，并把结果输出到终端。

## 算法说明

输入矩阵中：

- `1` 表示前景像素；
- `0` 表示背景像素。

本项目使用的结构元素为：

```text
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
```

结构元素的中心是第 3 行、第 3 列。每遇到一个前景像素，程序就把它周围上下左右各 2 格范围内的有效位置设为 `1`。超出图像边界的位置直接忽略，输出矩阵尺寸与输入矩阵相同。

处理流程：

```text
文本文件
   ↓
读取并校验二值矩阵
   ↓
执行 5×5 膨胀
   ↓
将结果输出到终端
```

对于 `R` 行、`C` 列的图像，每个像素最多处理 25 个位置，因此时间复杂度为 `O(R × C × 25)`，可简写为 `O(R × C)`；输出图像占用的额外空间为 `O(R × C)`。

## 项目结构

```text
morphological_dilation-task/
├── CMakeLists.txt
├── README.md
├── examples/
│   ├── input_12x12.txt
│   └── tests/
│       ├── test_1.txt
│       ├── test_2.txt
│       ├── test_3.txt
│       ├── test_4.txt
│       ├── test_5.txt
│       └── test_6.txt
├── include/
│   ├── binary_image.hpp
│   ├── dilation.hpp
│   └── pipeline.hpp
└── src/
    ├── binary_image.cpp
    ├── dilation.cpp
    ├── pipeline.cpp
    └── main.cpp
```

主要模块：

- `BinaryImage`：保存矩阵并检查尺寸、行宽和像素值；
- `Dilation5x5`：实现固定 `5×5` 结构元素的膨胀；
- `DilationPipeline`：负责读取、处理和输出；
- `main.cpp`：提供命令行入口并处理错误。

## 构建项目

本项目已在 WSL/Linux 环境中验证。构建时需要支持 C++17 的编译器和 CMake 3.16 或更高版本。

在项目根目录运行：

```bash
cmake -B build
make -C build
```

出现 `Built target dilation_cli` 表示构建成功，可执行文件位于：

```text
build/dilation_cli
```

## 输入格式

输入文件由若干行空格分隔的 `0` 和 `1` 组成，例如：

```text
0 0 0 0
0 1 0 0
0 0 0 0
```

输入必须满足：

- 至少包含一行和一列；
- 每一行的像素数量相同；
- 每个像素只能是 `0` 或 `1`；
- 空行会被忽略。

## 运行程序

使用项目提供的 `12×12` 示例：

```bash
./build/dilation_cli examples/input_12x12.txt
```

如需把结果保存到文件，可以使用输出重定向：

```bash
./build/dilation_cli examples/input_12x12.txt > output.txt
```

查看命令帮助：

```bash
./build/dilation_cli --help
```

膨胀后的矩阵会直接输出到终端，不包含尺寸或说明文字。程序退出码的含义如下：

| 退出码 | 含义 |
| --- | --- |
| `0` | 处理成功，或成功显示帮助 |
| `1` | 文件无法打开或输入矩阵非法 |
| `2` | 命令行参数数量错误 |
