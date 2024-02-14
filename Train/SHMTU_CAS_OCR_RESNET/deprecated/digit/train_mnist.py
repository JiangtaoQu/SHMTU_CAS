import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from torchvision import datasets, transforms, models
from tqdm import tqdm
from time import sleep as time_sleep

max_degree = 15

transform_train = transforms.Compose([
    transforms.Grayscale(num_output_channels=3),  # 将MNIST图像转换为RGB格式
    transforms.Resize((240, 240)),
    transforms.RandomCrop(224),
    transforms.RandomRotation(degrees=(-max_degree, max_degree)),
    transforms.ToTensor(),
])

transform_val = transforms.Compose([
    transforms.Grayscale(num_output_channels=3),  # 将MNIST图像转换为RGB格式
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

# 下载MNIST数据集并将其转换为RGB格式
mnist_train = datasets.MNIST(root='../../workdir/Datasets/MNIST', train=True, download=True, transform=transform_train)
mnist_test = datasets.MNIST(root='../../workdir/Datasets/MNIST', train=False, download=True, transform=transform_val)

# 创建数据加载器
batch_size = 64
train_loader = DataLoader(mnist_train, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(mnist_test, batch_size=batch_size, shuffle=False)

# 使用PyTorch内部的ResNet模型
model = models.resnet34(weights=models.ResNet34_Weights.IMAGENET1K_V1)
model.fc = nn.Linear(model.fc.in_features, 10)

# 将模型和数据移动到GPU上
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model = model.to(device)

# 定义损失函数和优化器
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

# 训练模型
num_epochs = 3

for epoch in range(num_epochs):
    time_sleep(1)

    model.train()
    for inputs, labels in tqdm(train_loader, desc=f'Epoch {epoch + 1}/{num_epochs}'):
        inputs, labels = inputs.to(device), labels.to(device)
        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

    model.eval()
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
    print(f'Epoch [{epoch + 1}/{num_epochs}], Test Accuracy: {accuracy:.4f}')

    torch.save(model.state_dict(), f'../../workdir/Models/resnet34_digit_mnist_{epoch + 1}.pth')

print('Training finished.')
