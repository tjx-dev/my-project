#!/usr/bin/env python
"""
C Language Jupyter Kernel
使用 MinGW GCC (C:/msys64/mingw64/bin/gcc.exe) 编译并运行 C 代码
"""

import sys
import os
import subprocess
import tempfile
import shutil

GCC_PATH = "C:/msys64/mingw64/bin/gcc.exe"

from ipykernel.kernelbase import Kernel

class CKernel(Kernel):
    implementation = 'c_kernel'
    implementation_version = '1.0'
    language = 'c'
    language_version = 'c17'
    banner = "C Kernel with GCC 16.1.0 - Compile and run C code interactively"
    language_info = {
        'name': 'c',
        'mimetype': 'text/plain',
        'file_extension': '.c',
    }

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._code = ""
        self._tmpdir = tempfile.mkdtemp(prefix="c_kernel_")

    def do_execute(self, code, silent, store_history=True, user_expressions=None, allow_stdin=False):
        """执行 C 代码"""
        self._code = code.strip()
        
        if not self._code:
            return {
                'status': 'ok',
                'execution_count': self.execution_count,
                'payload': [],
                'user_expressions': {},
            }

        # 将代码写入临时 .c 文件
        c_file = os.path.join(self._tmpdir, f"cell_{self.execution_count}.c")
        exe_file = os.path.join(self._tmpdir, f"cell_{self.execution_count}.exe")

        with open(c_file, 'w', encoding='utf-8') as f:
            f.write(self._code)

        # 编译
        compile_cmd = [GCC_PATH, "-Wall", "-std=c17", "-o", exe_file, c_file]
        try:
            compile_result = subprocess.run(
                compile_cmd,
                capture_output=True,
                text=True,
                timeout=30
            )
        except subprocess.TimeoutExpired:
            self.send_response(
                self.iopub_socket,
                'stream',
                {'name': 'stderr', 'text': '[ERROR] 编译超时（30秒）\n'}
            )
            return {
                'status': 'ok',
                'execution_count': self.execution_count,
                'payload': [],
                'user_expressions': {},
            }

        if compile_result.stderr:
            self.send_response(
                self.iopub_socket,
                'stream',
                {'name': 'stderr', 'text': compile_result.stderr}
            )

        if compile_result.returncode != 0:
            self.send_response(
                self.iopub_socket,
                'stream',
                {'name': 'stderr', 'text': f'[编译失败] 返回码: {compile_result.returncode}\n'}
            )
            return {
                'status': 'ok',
                'execution_count': self.execution_count,
                'payload': [],
                'user_expressions': {},
            }

        # 编译成功，执行
        try:
            run_result = subprocess.run(
                [exe_file],
                capture_output=True,
                text=True,
                timeout=10
            )
        except subprocess.TimeoutExpired:
            self.send_response(
                self.iopub_socket,
                'stream',
                {'name': 'stderr', 'text': '[ERROR] 程序运行超时（10秒）\n'}
            )
            return {
                'status': 'ok',
                'execution_count': self.execution_count,
                'payload': [],
                'user_expressions': {},
            }

        # 输出 stdout
        if run_result.stdout:
            self.send_response(
                self.iopub_socket,
                'stream',
                {'name': 'stdout', 'text': run_result.stdout}
            )

        # 输出 stderr（运行时错误）
        if run_result.stderr:
            self.send_response(
                self.iopub_socket,
                'stream',
                {'name': 'stderr', 'text': run_result.stderr}
            )

        # 清理临时文件
        try:
            os.remove(c_file)
            if os.path.exists(exe_file):
                os.remove(exe_file)
        except OSError:
            pass

        return {
            'status': 'ok',
            'execution_count': self.execution_count,
            'payload': [],
            'user_expressions': {},
        }

    def do_complete(self, code, cursor_pos):
        """简单的 C 代码补全"""
        c_keywords = [
            'auto', 'break', 'case', 'char', 'const', 'continue', 'default',
            'do', 'double', 'else', 'enum', 'extern', 'float', 'for', 'goto',
            'if', 'int', 'long', 'register', 'return', 'short', 'signed',
            'sizeof', 'static', 'struct', 'switch', 'typedef', 'union',
            'unsigned', 'void', 'volatile', 'while',
            'printf', 'scanf', 'malloc', 'free', 'include', 'define',
            'main', 'NULL', 'FILE', 'stdin', 'stdout', 'stderr'
        ]
        
        # 获取当前单词
        line = code[:cursor_pos]
        word_start = cursor_pos
        for i, ch in enumerate(reversed(line)):
            if not ch.isalnum() and ch != '_':
                word_start = cursor_pos - i
                break
        else:
            word_start = 0
        
        prefix = code[word_start:cursor_pos]
        matches = [kw for kw in c_keywords if kw.startswith(prefix)]
        
        cursor_end = cursor_pos + len(prefix)
        
        return {
            'status': 'ok',
            'matches': matches,
            'cursor_start': word_start,
            'cursor_end': cursor_end,
            'metadata': {},
        }

    def do_shutdown(self, restart):
        """清理临时目录"""
        try:
            shutil.rmtree(self._tmpdir)
        except OSError:
            pass
        super().do_shutdown(restart)


if __name__ == '__main__':
    from ipykernel.kernelapp import IPKernelApp
    IPKernelApp.launch_instance(kernel_class=CKernel)