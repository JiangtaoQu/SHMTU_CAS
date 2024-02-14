import torch
import platform


def get_recommended_device():
    """
    获取推荐的设备
    :return:
    """
    print("Checking device...")
    if torch.cuda.is_available():
        # 如果CUDA可用，则检查操作系统是否为macOS
        if 'Darwin' in platform.system():
            # 如果是macOS系统，则启用Metal后端
            if torch.backends.metal.available:
                torch.backends.metal.enabled = True
                print("Metal backend is enabled.")
            else:
                print("Metal backend is not available, falling back to CUDA.")
        else:
            # 如果不是macOS系统，则使用CUDA
            print("Using CUDA.")
            return torch.device("cuda:0")
    else:
        # 如果CUDA不可用，则使用CPU
        print("CUDA is not available, using CPU.")
    return torch.device("cpu")


if __name__ == '__main__':
    device = get_recommended_device()
    print("Device:", device)
