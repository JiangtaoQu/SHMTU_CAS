# 读取 clean_up_ext.ini 文件中的扩展名列表
$extensions = Get-Content ".\Template\clean_up_ext.ini" | ForEach-Object { $_.Trim() }

# 循环遍历扩展名列表
foreach ($ext in $extensions) {
    # 删除当前目录以及子目录下具有该扩展名的文件
    Get-ChildItem -Filter "*.$ext" -Recurse | Remove-Item -Force
}