using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace SHMTU_CAS_OCR_CLR_Connector_ClassLibrary
{
    public class ImageUtils
    {
        public static BitmapImage ConvertBitmapToBitmapImage(Bitmap bitmap)
        {
            // 将 Bitmap 转换为 BitmapImage
            using (var memoryStream = new MemoryStream())
            {
                // 可以根据需要更改格式
                bitmap.Save(memoryStream, ImageFormat.Png);
                memoryStream.Position = 0;

                var bitmapImage = new BitmapImage();
                bitmapImage.BeginInit();
                bitmapImage.StreamSource = memoryStream;
                bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                bitmapImage.EndInit();
                // 冻结图像以提高性能和避免资源泄漏
                bitmapImage.Freeze();

                return bitmapImage;
            }
        }

        public static async Task<Bitmap> DownloadImageAsync(string imageUrl)
        {
            using (var webClient = new WebClient())
            {
                // 异步下载图片
                var imageData =
                    await webClient.DownloadDataTaskAsync(new Uri(imageUrl));

                // 将字节数组转换为 Bitmap
                using (var memoryStream = new MemoryStream(imageData))
                {
                    return (Bitmap)Image.FromStream(memoryStream);
                }
            }
        }

        public static Bitmap ConvertTo32BppArgb(Bitmap bitmap)
        {
            var newBitmap = new Bitmap(
                bitmap.Width, bitmap.Height,
                PixelFormat.Format32bppArgb
            );
            using (Graphics g = Graphics.FromImage(newBitmap))
            {
                g.DrawImage(bitmap, 0, 0);
            }

            return newBitmap;
        }
    }
}