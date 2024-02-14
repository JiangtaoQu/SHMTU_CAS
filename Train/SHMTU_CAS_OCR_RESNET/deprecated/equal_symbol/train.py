import torch
import torch.nn as nn
import torchvision.transforms as transforms
from torchvision import datasets, models
from torch.utils.data import DataLoader
from tqdm import tqdm  # 导入tqdm
from time import sleep as time_sleep

path = "../../workdir/Datasets/EqualSymbol/DatasetFull"

# 检查是否有可用的GPU，如果有，则使用它，否则使用CPU
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

# 数据变换
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

# 加载数据集
dataset = datasets.ImageFolder(root=path, transform=transform)

# 创建数据加载器
batch_size = 64
data_loader = DataLoader(dataset, batch_size=batch_size, shuffle=True)

# 加载预训练的ResNet-18模型
model = models.resnet18(pretrained=True)
model.fc = nn.Linear(model.fc.in_features, 2)  # 替换最后的全连接层

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

    # 使用tqdm显示进度条
    for inputs, labels in tqdm(data_loader, desc=f'Epoch {epoch + 1}/{num_epochs}'):
        # 将数据移动到GPU
        inputs, labels = inputs.to(device), labels.to(device)

        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        total_loss += loss.item()

    average_loss = total_loss / len(data_loader)
    print(f'Epoch [{epoch + 1}/{num_epochs}], Average Loss: {average_loss}')

# 保存模型
torch.save(model.state_dict(), "../../workdir/Models/resnet18_equal.pth")
print('Training finished.')
