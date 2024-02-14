import os
import shutil
from torchvision import transforms
from torch.utils.data import DataLoader
from tqdm import tqdm
import torch
import torch.nn as nn
from torchvision import models

from src.classify.utils.dataloader import CustomDataset
from src.utils.files.dirs import create_dirs

# 数据变换
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

class_count = 10

# 加载测试数据集
# test_dataset = CustomDataset(root='../../workdir/Spilt/MainBody_symbol/2', transform=transform)
test_dataset = CustomDataset(root='../../workdir/Spilt/MainBody_symbol/2', transform=transform)
# test_dataset = CustomDataset(root='../../workdir/Spilt/MainBody_chs/0', transform=transform, include_subdir=False)
test_loader = DataLoader(test_dataset, batch_size=1, shuffle=False)

# 加载预训练的ResNet-18模型（假设已经训练好）
# model = models.resnet18(pretrained=False)  # 不使用预训练的权重
model = models.resnet34()
model.fc = nn.Linear(model.fc.in_features, class_count)  # 替换最后的全连接层

# 将模型和数据移动到GPU上
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model = model.to(device)

# 加载已训练的模型参数
model.load_state_dict(torch.load("../../workdir/Models/resnet34_digit_4.pth", map_location=device))

# 设置模型为评估模式
model.eval()

# 创建目标目录
output_dir = '../../workdir/Test/Digit_Predictions/Digit_Predictions_symbol_2'
# output_dir = '../../workdir/Test/Digit_Predictions/Digit_Predictions_chs_0'
os.makedirs(output_dir, exist_ok=True)

create_dirs(list(
    [
        str(os.path.join(output_dir, str(i)))
        for i in range(class_count)
    ]
))

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
        # new_filename = f'{str(predicted.item())}_{original_filename}'

        # 构建新的文件路径
        # new_filepath = os.path.join(output_dir, new_filename)
        new_filepath = os.path.join(output_dir, str(predicted.item()), original_filename)

        # 复制原始图像到目标目录，并使用新的文件名
        shutil.copy(img_path[0], new_filepath)

print('Prediction finished. Results are saved in the "output_predictions" directory.')
