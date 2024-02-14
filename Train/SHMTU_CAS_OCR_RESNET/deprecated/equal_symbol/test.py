import os
import shutil
from torchvision import transforms
from torchvision import datasets, models
from torch.utils.data import DataLoader
from torch import nn
import torch
from tqdm import tqdm
from PIL import Image

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

# 数据变换
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

# 加载测试数据集
test_dataset = datasets.ImageFolder(
    root='../workdir/resnet18/equal_symbol/test_set',
    transform=transform
)
test_loader = DataLoader(test_dataset, batch_size=1, shuffle=False)  # 使用batch_size=1，每次加载一个图像

# 加载预训练的ResNet-18模型（假设已经训练好）
model = models.resnet18(pretrained=False)  # 不使用预训练的权重
model.fc = nn.Linear(model.fc.in_features, 2)  # 替换最后的全连接层

# 加载已训练的模型参数
model.load_state_dict(torch.load('trained_model.pth', map_location=device))
model = model.to(device)

# 设置模型为评估模式
model.eval()

# 创建目标目录
output_dir = '../output_predictions'
os.makedirs(output_dir, exist_ok=True)

# 遍历测试集进行预测并保存图像
with torch.no_grad():
    for i, (inputs, _) in enumerate(tqdm(test_loader, desc='Predicting')):
        # 将数据移动到GPU
        inputs = inputs.to(device)

        # 进行预测
        outputs = model(inputs)
        _, predicted = torch.max(outputs, 1)

        # 获取原始图像路径
        original_path = test_loader.dataset.samples[i][0]

        # 获取原始图像文件名
        original_filename = os.path.basename(original_path)

        # 构建新的文件名，将预测结果添加到文件名开头
        new_filename = f'{str(predicted.item())}_{original_filename}'

        # 构建新的文件路径
        new_filepath = os.path.join(output_dir, new_filename)

        # 复制原始图像到目标目录，并使用新的文件名
        shutil.copy(original_path, new_filepath)

print('Prediction finished. Results are saved in the "output_predictions" directory.')
