import socket
import cv2
import numpy as np

from predict_file import *

device = get_recommended_device()
print("device:", device)

print("Load Model")
(
    model_equal_symbol,
    model_operator,
    model_digit
) = load_model(device)
print("Load Model Success")


def handle_pic(image_data) -> str:
    image_array = np.frombuffer(image_data, dtype=np.uint8)
    image = cv2.imdecode(image_array, cv2.IMREAD_COLOR)
    # cv2.imshow("image", image)
    # cv2.waitKey(0)

    result, expr, _, _, _, _ = \
        predict_validate_code(
            image,
            device,
            model_equal_symbol,
            model_operator,
            model_digit,
            True
        )

    return expr


# 创建TCP/IP套接字
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 监听端口5000
server_socket.bind(('localhost', 5000))
server_socket.listen(1)

print("等待连接...")

while True:
    # 等待客户端连接
    client_socket, client_address = server_socket.accept()
    print(f"连接来自: {client_address}")

    # 接收图像数据
    image_data = b""
    while True:
        data = client_socket.recv(1024)
        if data == b"<END>":  # 检查是否收到特殊标记
            break
        if not data:
            break
        image_data += data

    print("Received!")

    result: str = handle_pic(image_data)

    print(result)

    client_socket.sendall(result.encode(encoding="utf-8"))

    # 关闭连接
    client_socket.close()
    print("连接已关闭")
