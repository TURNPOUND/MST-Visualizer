# MST-Visualizer 🎄

最小生成树算法可视化工具 —— 卓软2501 黄仁海

基于 Qt 6 + C++ 开发，支持 Prim 和 Kruskal 算法的可视化演示。

## ✨ 功能

- **图生成** — 随机生成连通图，可调节节点数和边密度
- **Prim / Kruskal 算法** — 两种经典 MST 算法，带逐边动画演示
- **交互编辑** — 拖拽平移、缩放，支持加点、加边、右键编辑
- **查找节点** — 按序号或名称快速定位高亮
- **导入/导出图** — 文本格式保存和读取图数据
- **批量性能测试** — 自动对比不同规模下两种算法的运行时间

## 🛠 构建

需要 Qt 6.x + MinGW 或 MSVC：

```bash
qmake MST-Visualizer.pro
make          # Linux/macOS
mingw32-make  # Windows MinGW
```

## 📦 直接使用

从 [Releases](https://github.com/renhai-huang/MST-Visualizer/releases) 下载绿色便携版，解压后运行 `MST-Visualizer.exe`。

## 📄 许可

本项目仅供学习交流使用。
