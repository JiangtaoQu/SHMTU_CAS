import socket

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


def handle_pic(image_byte_data: bytes) -> str:
    image_array = np.frombuffer(image_byte_data, dtype=np.uint8)

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

# 监听端口
server_socket.bind(('0.0.0.0', 21601))
server_socket.listen(1)

print("等待连接...")

end_marker = "<END>".encode("utf-8")

while True:
    # 等待客户端连接
    client_socket, client_address = server_socket.accept()
    print(f"连接来自: {client_address}")

    # 接收图像数据
    image_data = b""
    receive_error = False
    while True:
        try:
            data = client_socket.recv(1024)
        except:
            receive_error = True
            break

        if not data:
            break

        image_data += data

        if image_data.endswith(end_marker):  # 检查是否收到特殊标记
            image_data = image_data[:-len(end_marker)]
            break

    if receive_error:
        print("接收数据错误！")
        continue
    print("Received!")

    result: str
    try:
        result = handle_pic(image_data)
    except:
        print("处理图像错误！")
        result = ""

    print(result)

    try:
        client_socket.sendall(
            result.encode(encoding="utf-8")
        )
    except:
        print("结果发送错误！")

    # 关闭连接
    client_socket.close()
    print("连接已关闭")
