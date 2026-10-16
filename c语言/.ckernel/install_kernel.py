"""
安装 C Kernel 到 Jupyter
运行此脚本将 C kernel 注册为 Jupyter 可用 kernel
"""
import json
import os
import sys

# kernel 目录
kernel_dir = os.path.join(
    os.path.expanduser("~"),
    "AppData", "Roaming", "jupyter", "kernels", "c_kernel"
)
os.makedirs(kernel_dir, exist_ok=True)

# 获取当前 Python 解释器路径
python_exe = sys.executable
kernel_script = os.path.join(os.path.dirname(os.path.abspath(__file__)), "c_kernel.py")

# kernel.json
kernel_json = {
    "argv": [
        python_exe,
        kernel_script,
        "-f",
        "{connection_file}"
    ],
    "display_name": "C (GCC)",
    "language": "c",
    "interrupt_mode": "signal"
}

kernel_json_path = os.path.join(kernel_dir, "kernel.json")
with open(kernel_json_path, 'w', encoding='utf-8') as f:
    json.dump(kernel_json, f, indent=4, ensure_ascii=False)

print(f"✅ C Kernel 安装成功！")
print(f"   Kernel 目录: {kernel_dir}")
print(f"   kernel.json: {kernel_json_path}")
print(f"   Python: {python_exe}")
print(f"   Kernel 脚本: {kernel_script}")
print()
print("可用命令查看已安装的 kernel:")
print("  jupyter kernelspec list")