using System;
using System.Drawing;
using SHMTU_CAS_CLR_ClassLibrary;

namespace SHMTU_CAS_OCR_CLR_Connector_ClassLibrary
{
    // ReSharper disable once InconsistentNaming
    public static class CAS_OCR
    {
#pragma warning disable IDE1006
        public static bool init_model(string checkpointDirPath, string typeName)
#pragma warning restore IDE1006
        {
            return CAS_OCR_CLR.init_model_clr(checkpointDirPath, typeName);
        }

        public static void set_model_gpu_support(bool useGpu)
        {
            CAS_OCR_CLR.set_model_gpu_support_clr(useGpu);
        }

#pragma warning disable IDE1006
        public static Tuple<int, string, int, int, int, int>
            predict_validate_code(
                Bitmap bitmap,
                bool useGpu = false
            )
#pragma warning restore IDE1006
        {
            if (bitmap == null)
            {
                throw new ArgumentNullException(nameof(bitmap));
            }

            var clrImage = (Bitmap)bitmap.Clone();

            // 判断图像是否为 32 位 ARGB 格式
            if (clrImage.PixelFormat != System.Drawing.Imaging.PixelFormat.Format32bppArgb)
            {
                // 如果不是，转换为 32 位 ARGB 格式
                clrImage = ImageUtils.ConvertTo32BppArgb(bitmap);
            }

            var result = CAS_OCR_CLR.predict_validate_code_clr(clrImage, useGpu);

            return result;
        }

#pragma warning disable IDE1006
        public static void release_model()
#pragma warning restore IDE1006
        {
            CAS_OCR_CLR.release_model_clr();
        }
    }
}