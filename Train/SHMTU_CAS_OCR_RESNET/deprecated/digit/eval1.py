import torch
import torch.nn as nn
from torch.utils.data import DataLoader
from torchvision import datasets, transforms, models
from tqdm import tqdm

# pth_path = "../../workdir/Models/resnet18_digit_mnist.pth"
pth_path = "../../workdir/Models/resnet18_digit.pth"

# 数据变换
transform = transforms.Compose([
    transforms.Grayscale(num_output_channels=3),  # 将图像转换为RGB格式
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

# 下载MNIST数据集并将其转换为RGB格式
# mnist_test = datasets.MNIST(root='../../workdir/Datasets/MNIST', train=False, download=True, transform=transform)
path = "../../workdir/Classify/Digit/Dataset_Full"
dataset = datasets.ImageFolder(root=path, transform=transform)

# 创建数据加载器
batch_size = 64
test_loader = DataLoader(dataset, batch_size=batch_size, shuffle=False)

# 使用PyTorch内部的ResNet-18模型
model = models.resnet18()
model.fc = nn.Linear(model.fc.in_features, 10)  # 10类别的分类任务

# 将模型移动到GPU上
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model = model.to(device)

# 加载之前保存的模型参数
model.load_state_dict(torch.load(pth_path))
model.eval()

# 进行推理
correct = 0
total = 0

with torch.no_grad():
    for inputs, labels in tqdm(test_loader, desc='Testing'):
        inputs, labels = inputs.to(device), labels.to(device)
        outputs = model(inputs)
        _, predicted = torch.max(outputs, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()

accuracy = correct / total
print(f'Test Accuracy: {accuracy:.4f}')
