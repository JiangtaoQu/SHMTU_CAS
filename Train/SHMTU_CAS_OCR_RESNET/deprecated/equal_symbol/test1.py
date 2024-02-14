import os
import shutil
from torchvision import transforms
from torch.utils.data import Dataset, DataLoader
from tqdm import tqdm
from PIL import Image
import torch
import torch.nn as nn
from torchvision import models

# 数据变换
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

# 定义测试数据集的加载方式
class CustomDataset(Dataset):
    def __init__(self, root, transform=None):
        self.root = root
        self.transform = transform
        self.image_paths = [os.path.join(root, filename) for filename in os.listdir(root) if filename.endswith(('.jpg', '.jpeg', '.png'))]

    def __len__(self):
        return len(self.image_paths)

    def __getitem__(self, idx):
        img_path = self.image_paths[idx]
        img = Image.open(img_path).convert('RGB')

        if self.transform:
            img = self.transform(img)

        return img, img_path  # 返回图像和路径

# 加载测试数据集
test_dataset = CustomDataset(root='../workdir/resnet18/equal_symbol/test_set1', transform=transform)
test_loader = DataLoader(test_dataset, batch_size=1, shuffle=False)

# 加载预训练的ResNet-18模型（假设已经训练好）
model = models.resnet18(pretrained=False)  # 不使用预训练的权重
model.fc = nn.Linear(model.fc.in_features, 2)  # 替换最后的全连接层

# 将模型和数据移动到GPU上
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model = model.to(device)

# 加载已训练的模型参数
model.load_state_dict(torch.load('trained_model.pth', map_location=device))

# 设置模型为评估模式
model.eval()

# 创建目标目录
output_dir = '../output_predictions'
os.makedirs(output_dir, exist_ok=True)

# 遍历测试集进行预测并保存图像
with torch.no_grad():
    for i, (inputs, img_path) in enumerate(tqdm(test_loader, desc='Predicting')):
        # 将数据移动到GPU
        inputs = inputs.to(device)

        # 进行预测
        outputs = model(inputs)
        _, predicted = torch.max(outputs, 1)

        # 获取原始图像文件名
        original_filename = os.path.basename(img_path[0])

        # 构建新的文件名，将预测结果添加到文件名开头
        new_filename = f'{str(predicted.item())}_{original_filename}'

        # 构建新的文件路径
        new_filepath = os.path.join(output_dir, new_filename)

        # 复制原始图像到目标目录，并使用新的文件名
        shutil.copy(img_path[0], new_filepath)

print('Prediction finished. Results are saved in the "output_predictions" directory.')
