import torch
import torch.nn as nn
import torchvision.transforms as transforms
from torchvision import datasets, models
from torch.utils.data import DataLoader
from tqdm import tqdm  # 导入tqdm
from time import sleep as time_sleep

path = "../../workdir/Classify/Digit/Dataset_Full"

# 检查是否有可用的GPU，如果有，则使用它，否则使用CPU
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

max_degree = 15

# 数据变换
transform_train = transforms.Compose([
    transforms.Grayscale(num_output_channels=3),
    transforms.Resize((240, 240)),
    transforms.RandomCrop(224),
    transforms.RandomRotation(degrees=(-max_degree, max_degree)),
    transforms.ToTensor(),
])

# 加载数据集
dataset = datasets.ImageFolder(root=path, transform=transform_train)

# 创建数据加载器
batch_size = 64
data_loader = DataLoader(dataset, batch_size=batch_size, shuffle=True)

# 加载预训练的ResNet-18模型
# model = models.resnet18(pretrained=True)
# model = models.resnet34(weights=models.ResNet34_Weights.IMAGENET1K_V1)
model = models.resnet34()
model.fc = nn.Linear(model.fc.in_features, 10)  # 替换最后的全连接层

# 将模型和数据移动到GPU上
model = model.to(device)
model.load_state_dict(torch.load('../../workdir/Models/resnet34_digit_mnist_3.pth', map_location=device))

criterion = nn.CrossEntropyLoss().to(device)

# 定义优化器
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

# 训练模型
num_epochs = 5

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

    torch.save(model.state_dict(), f'../../workdir/Models/resnet34_digit_{epoch + 1}.pth')

print('Training finished.')
