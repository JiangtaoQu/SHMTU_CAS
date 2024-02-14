import os.path

import torch
import torch.nn as nn
import torchvision.transforms as transforms
from torchvision import datasets, models
from torch.utils.data import DataLoader
from tqdm import tqdm  # 导入tqdm
from time import sleep as time_sleep

path = "../../workdir/Datasets/Operator"

# 检查是否有可用的GPU，如果有，则使用它，否则使用CPU
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

# 数据变换
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

# 加载数据集
dataset_train = datasets.ImageFolder(root=os.path.join(path, "train"), transform=transform)
dataset_val = datasets.ImageFolder(root=os.path.join(path, "val"), transform=transform)

# 创建数据加载器
batch_size = 64
data_loader_train = DataLoader(dataset_train, batch_size=batch_size, shuffle=True)
data_loader_val = DataLoader(dataset_val, batch_size=batch_size, shuffle=False)

# 加载预训练的ResNet-18模型
# model = models.resnet18()
model = models.resnet18(weights=models.ResNet18_Weights.IMAGENET1K_V1)
model.fc = nn.Linear(model.fc.in_features, 6)  # 替换最后的全连接层

# 将模型和数据移动到GPU上
model = model.to(device)
criterion = nn.CrossEntropyLoss().to(device)

# 定义优化器
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

# 训练模型
num_epochs = 3

for epoch in range(num_epochs):
    time_sleep(1)

    total_loss = 0.0
    model.train()
    # 使用tqdm显示进度条
    for inputs, labels in tqdm(data_loader_train, desc=f'Epoch {epoch + 1}/{num_epochs}'):
        # 将数据移动到GPU
        inputs, labels = inputs.to(device), labels.to(device)

        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        total_loss += loss.item()

    average_loss = total_loss / len(data_loader_train)

    model.eval()
    count_correct = 0
    count_total = 0
    with torch.no_grad():
        for inputs, labels in tqdm(data_loader_val, desc='Testing'):
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = model(inputs)
            _, predicted = torch.max(outputs, 1)
            count_total += labels.size(0)
            count_correct += (predicted == labels).sum().item()

    accuracy = count_correct / count_total
    print(f'Epoch [{epoch + 1}/{num_epochs}], Average Loss: {average_loss}, Accuracy: {(accuracy * 100):.4f}%')

    # 保存模型
    torch.save(model.state_dict(), f"../../workdir/Models/resnet18_operator_{epoch + 1}.pth")

print('Training finished.')
