#!/bin/bash

# 读取a.ini文件中的扩展名列表
extensions=$(cat ./Template/clean_up_ext.ini)

# 循环遍历扩展名列表
for ext in $extensions
do
    # 删除当前目录下具有该扩展名的文件
    rm -f *.$ext
done

find . -type f -name "._wordcount_selection.tex" -delete

