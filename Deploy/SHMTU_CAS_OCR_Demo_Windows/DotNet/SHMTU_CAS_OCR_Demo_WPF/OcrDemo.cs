using System.Diagnostics;
using System.Drawing;
using System.Windows;
using SHMTU_CAS_OCR_CLR_Connector_ClassLibrary;

namespace SHMTU_CAS_OCR_Demo_WPF
{
    public partial class MainWindow : Window
    {
        private static Bitmap _innerBitmap;

        private static string GetCheckpointSaveDirPath(string path)
        {
            path = path.Trim();
            if (!string.IsNullOrEmpty(path))
            {
                return path;
            }

            if (Debugger.IsAttached)
            {
                return "../../../../../checkpoint/";
            }
            else
            {
                return "../../checkpoint/";
            }
        }

        private void ButtonOcr_Click(object sender, RoutedEventArgs e)
        {
            if (_innerBitmap == null)
            {
                MessageBox.Show("Please download or select image first.");
                return;
            }

            var useGpu =
                CheckBoxGpu.IsChecked.GetValueOrDefault(false);
            CAS_OCR.set_model_gpu_support(useGpu);

            CAS_OCR.init_model(GetCheckpointSaveDirPath(""), "fp16");

            var result = CAS_OCR.predict_validate_code(_innerBitmap);

            var resultExprStr = result.Item2;

            LabelInfo.Content = resultExprStr;
            MessageBox.Show(
                resultExprStr, "Result",
                MessageBoxButton.OK, MessageBoxImage.Information
            );

            // CAS_OCR.release_model();
        }

        // GC相关
        private static void ReleaseModel()
        {
            CAS_OCR.release_model();
        }

        private void ButtonReleaseModel_Click(object sender, RoutedEventArgs e)
        {
            CAS_OCR.release_model();
            MessageBox.Show("Model released.", "Tip");
        }
    }
}