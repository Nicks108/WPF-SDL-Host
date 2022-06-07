using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WPFEditor;

namespace WPFEditorWindo
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            //IntPtr i =EngineAPI.GetWindowHandle(new IntPtr());


            
            bool b = this.IsKeyboardFocused;
            int q = 0;
            
            
            //HwndSource source = HwndSource.FromVisual(this) as HwndSource;
            
        }
        
        
    }
}
