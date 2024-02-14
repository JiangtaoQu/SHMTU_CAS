import cv2
import numpy as np


def find_and_save_last_contour(image_path, output_path):
    # 读取图像
    image = cv2.imread(image_path)
    if image is None:
        print("无法读取图像")
        return

    # 转换为灰度图
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # 二值化
    _, binary = cv2.threshold(gray, 128, 255, cv2.THRESH_BINARY)

    # 查找轮廓
    contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # 保存最后一个轮廓的部分
    if len(contours) > 0:
        last_contour = contours[-1]
        x, y, w, h = cv2.boundingRect(last_contour)
        last_part = image[y:y + h, x:x + w]
        cv2.imwrite(output_path, last_part)
        print(f"最后一个独立区域已保存至 {output_path}")
    else:
        print("未找到轮廓")


if __name__ == "__main__":
    input_image_path = "../workdir/ori_gray/20240102155959_server.png"
    output_image_path = "../workdir/last_part.png"

    find_and_save_last_contour(input_image_path, output_image_path)
