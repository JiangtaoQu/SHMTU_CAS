import torch

from src.classify.utils.devices_selector import get_recommended_device
from src.config import config
from src.classify.digit.train_mnist import train_mnist
from src.classify.digit.train_digit import train_digit


def train_digit_all(device: torch.device):

    print("Training digit...")

    if config.pretrain_on_mnist:
        train_mnist(device)
    else:
        print("Skip pretrain on mnist")

    train_digit(device)


if __name__ == "__main__":
    device = get_recommended_device()
    train_digit_all(device)
