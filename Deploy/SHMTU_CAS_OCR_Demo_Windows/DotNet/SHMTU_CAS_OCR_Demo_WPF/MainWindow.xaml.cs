using System;
using System.Windows;

namespace SHMTU_CAS_OCR_Demo_WPF
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Initialized(object sender, EventArgs e)
        {
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            ReleaseModel();
        }
    }
}