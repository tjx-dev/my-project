# open.py —— OpenCV 入门示例
# 运行方式(使用虚拟环境里的 python):
#   d:\my-project\my-project\.venv\Scripts\python.exe open.py

import cv2
import numpy as np

# 1. 查看 OpenCV 版本
print("OpenCV 版本:", cv2.__version__)

# 2. 创建一张纯色图片(BGR 格式,3 个通道:蓝、绿、红)
img = np.zeros((300, 400, 3), dtype=np.uint8)  # 先建一张黑色的 300x400 画布
img[:] = (200, 180, 100)  # 整张图填充颜色(注意 OpenCV 是 BGR 顺序)

# 3. 在上面画一些基本图形
cv2.line(img, (0, 0), (400, 300), (0, 0, 255), 3)          # 画一条线(红色)
cv2.rectangle(img, (50, 50), (350, 250), (255, 0, 0), 2)   # 画一个矩形(蓝色)
cv2.circle(img, (200, 150), 80, (0, 255, 0), -1)           # 画一个实心圆(绿色)
cv2.putText(img, "Hello OpenCV", (70, 150),
            cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)  # 写文字(白色)

# 4. 保存图片到当前目录
cv2.imwrite("opencv_demo.jpg", img)
print("彩色图片已保存: opencv_demo.jpg")

# 5. 读取刚保存的图片
img2 = cv2.imread("opencv_demo.jpg")
print("读取到的图片尺寸(高, 宽, 通道):", img2.shape)

# 6. 转成灰度图并保存
gray = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
cv2.imwrite("opencv_demo_gray.jpg", gray)
print("灰度图片已保存: opencv_demo_gray.jpg")

print("全部完成,OpenCV 可以正常调用!")
