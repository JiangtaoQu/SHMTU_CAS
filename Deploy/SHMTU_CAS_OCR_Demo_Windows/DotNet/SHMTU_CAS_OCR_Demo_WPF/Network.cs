using System;
using System.Drawing;
using System.Windows;
using Microsoft.Win32;
using SHMTU_CAS_OCR_CLR_Connector_ClassLibrary;

namespace SHMTU_CAS_OCR_Demo_WPF
{
    public partial class MainWindow : Window
    {
        private async void ButtonDownloadUrl_Click(object sender, RoutedEventArgs e)
        {
            const string imageUrl = @"https://cas.shmtu.edu.cn/cas/captcha";

            try
            {
                // 异步下载图片并保存为 Bitmap
                var bitmap = await ImageUtils.DownloadImageAsync(imageUrl);

                _innerBitmap = bitmap;

                // 将 Bitmap 转换为 BitmapImage
                var bitmapImage =
                    ImageUtils.ConvertBitmapToBitmapImage(bitmap);

                // 将 BitmapImage 设置为 Image 控件的源
                imageCode.Source = bitmapImage;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error downloading image: {ex.Message}");
            }
        }

        private void ButtonOpenLocal_Click(object sender, RoutedEventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter =
                    "Image Files (*.png, *.jpeg, *.bmp)|*.png;*.jpeg;*.bmp"
                    + "|PNG files (*.png)|*.png"
                    + "|JPEG files (*.jpg, *.jpeg)|*.jpg;*.jpeg"
                    + "|BMP files (*.bmp)|*.bmp"
                    + "|All files (*.*)|*.*",
                FilterIndex = 1,
                Multiselect = false
            };

            if (openFileDialog.ShowDialog() != true) return;

            // 用户选择了文件
            var selectedFile = openFileDialog.FileName;

            try
            {
                var bitmap = new Bitmap(selectedFile);

                _innerBitmap = bitmap;

                // 将 Bitmap 转换为 BitmapImage
                var bitmapImage =
                    ImageUtils.ConvertBitmapToBitmapImage(bitmap);

                // 将 BitmapImage 设置为 Image 控件的源
                imageCode.Source = bitmapImage;
            }
            catch
            {
                ;
            }
        }
    }
}