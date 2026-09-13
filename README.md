# 二值图像 5×5 形态学处理

这是一个使用 C++17 编写的二值图像形态学练习项目。程序从文本文件读取二值矩阵，可选择 `5×5` 方形或圆盘结构元素，并执行膨胀或腐蚀，最后把处理结果输出到终端。

为了兼容项目原有用法，可执行文件仍命名为 `dilation_cli`。

## 功能概览

- 读取并校验由 `0`、`1` 组成的二值图像；
- 支持膨胀（`dilate`）和腐蚀（`erode`）；
- 支持方形（`square`）和圆盘（`disk`）结构元素；
- 使用面向对象和模块化设计，操作与结构元素可以独立选择；
- 保持输出矩阵与输入矩阵尺寸一致。

## 算法说明

输入矩阵中：

- `1` 表示前景像素；
- `0` 表示背景像素。

### 结构元素

方形结构元素包含 25 个有效位置：

```text
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
```

圆盘结构元素使用 `行偏移² + 列偏移² <= 2²` 生成，包含 13 个有效位置：

```text
0 0 1 0 0
0 1 1 1 0
1 1 1 1 1
0 1 1 1 0
0 0 1 0 0
```

两种结构元素的中心都是第 3 行、第 3 列。方形会处理最远的对角位置，圆盘不处理四个角附近的位置，因此圆盘通常产生更圆滑、较温和的效果。

结构元素作为独立对象传给膨胀或腐蚀类。以后如需增加其他 `5×5` 形状，只需构造新的掩膜并传入，膨胀和腐蚀算法本身不需要修改。

### 膨胀

程序遍历输入中的每个前景像素，把结构元素覆盖到该位置，并将所有有效且未越界的位置写为 `1`。

膨胀会扩大前景区域，可以填补小孔洞或裂缝，也可能连接距离较近的区域。结构元素超出图像边界的部分会被忽略。

### 腐蚀

程序把每个像素依次作为结构元素的中心，检查结构元素的所有有效位置：

- 如果全部位置都在图像内并且对应输入像素都是 `1`，中心像素保留为 `1`；
- 如果遇到输入像素 `0` 或超出图像边界，中心像素变为 `0`。

腐蚀会缩小前景、扩大孔洞、去除较小的前景噪点，并可能切断较细的连接。腐蚀不是对膨胀结果的无损还原，因为膨胀过程中丢失的形状信息无法自动恢复。

从膨胀改为腐蚀时，核心判断由“结构元素覆盖到的位置写成前景”变为“结构元素的所有有效位置都必须完全落在前景内”。本项目将两种算法分别封装在 `Dilation5x5` 和 `Erosion5x5` 中，并由管线选择执行哪一种操作。

### 处理流程

```text
文本文件
   ↓
读取并校验二值矩阵
   ↓
选择方形或圆盘结构元素
   ↓
选择膨胀或腐蚀
   ↓
执行处理并输出矩阵
```

对于 `R` 行、`C` 列的图像，两种操作对每个像素最多检查 25 个位置，因此时间复杂度为 `O(R × C × 25)`，可简写为 `O(R × C)`；输出图像占用的额外空间为 `O(R × C)`。

## 项目结构

```text
morphological_dilation-task/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── examples/
│   ├── input_12x12.txt
│   └── tests/
│       ├── test_1.txt
│       ├── test_2.txt
│       ├── test_3.txt
│       ├── test_4.txt
│       ├── test_5.txt
│       ├── test_6.txt
│       ├── test_7.txt
│       ├── test_8.txt
│       └── test_9.txt
├── include/
│   ├── binary_image.hpp
│   ├── structuring_element.hpp
│   ├── dilation.hpp
│   ├── erosion.hpp
│   └── pipeline.hpp
└── src/
    ├── binary_image.cpp
    ├── structuring_element.cpp
    ├── dilation.cpp
    ├── erosion.cpp
    ├── pipeline.cpp
    └── main.cpp
```

主要模块：

- `BinaryImage`：保存矩阵并检查尺寸、行宽和像素值；
- `StructuringElement5x5`：按值保存可替换的 `5×5` 掩膜，并通过 `square()`、`disk()` 创建结构元素；
- `Dilation5x5`：使用传入的结构元素执行膨胀；
- `Erosion5x5`：使用同一种结构元素类型执行腐蚀；
- `MorphologyOperation`：表示当前选择的是膨胀还是腐蚀；
- `MorphologyPipeline`：保存操作和结构元素，负责读取、处理和输出；
- `main.cpp`：解析命令行参数并处理错误。

`DilationPipeline` 作为旧名称的兼容别名保留，当前主要管线类是 `MorphologyPipeline`。

## 构建项目

本项目已在 WSL/Linux 环境中验证。构建时需要支持 C++17 的编译器和 CMake 3.16 或更高版本。

在项目根目录运行：

```bash
cmake -B build
make -C build
```

成功标志：终端出现 `Built target dilation_cli`。生成的可执行文件位于：

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

命令格式：

```text
./build/dilation_cli <输入文件> [square|disk] [dilate|erode]
```

两个可选参数必须按照“结构元素、操作”的顺序书写。默认结构元素是 `square`，默认操作是 `dilate`。

默认执行方形膨胀：

```bash
./build/dilation_cli examples/input_12x12.txt
```

执行圆盘膨胀：

```bash
./build/dilation_cli examples/input_12x12.txt disk
```

执行方形腐蚀：

```bash
./build/dilation_cli examples/tests/test_8.txt square erode
```

执行圆盘腐蚀：

```bash
./build/dilation_cli examples/tests/test_8.txt disk erode
```

注意：如果要执行方形腐蚀，必须写成 `square erode`，不能把 `erode` 放到结构元素参数的位置。

如需把结果保存到文件，可以使用输出重定向：

```bash
./build/dilation_cli examples/input_12x12.txt square dilate > output.txt
```

查看命令帮助：

```bash
./build/dilation_cli --help
```

处理后的矩阵会直接输出到终端，不包含尺寸或说明文字。程序退出码的含义如下：

| 退出码 | 含义 |
| --- | --- |
| `0` | 处理成功，或成功显示帮助 |
| `1` | 文件无法打开、输入矩阵非法或处理异常 |
| `2` | 参数数量错误、结构元素名称未知或操作名称未知 |
