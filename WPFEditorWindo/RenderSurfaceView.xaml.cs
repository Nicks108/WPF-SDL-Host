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

namespace WPFEditor
{
    /// <summary>
    /// Interaction logic for RenderSurfaceView.xaml
    /// </summary>
    public partial class RenderSurfaceView : UserControl, IDisposable
    {
        private RenderSurfaceHost _host = null;
        public RenderSurfaceView()
        {
            InitializeComponent();
            Loaded += OnRenderSurfaceViewLoaded;

            this.LostMouseCapture += OnLostMouseCapture;
            this.GotMouseCapture += OnGotMouseCapture;
            this.MouseEnter += OnMouseEnter;
            this.MouseLeave += OnMouseLeave;

            
        }

        private void OnMouseLeave(object sender, MouseEventArgs e)
        {
            Console.WriteLine("mouse Leave");
        }

        private void OnMouseEnter(object sender, MouseEventArgs e)
        {
            Console.WriteLine("mouse enter");
        }

        private void OnGotMouseCapture(object sender, MouseEventArgs e)
        {
            Console.WriteLine("got mouse");
        }

        private void OnLostMouseCapture(object sender, MouseEventArgs e)
        {
            Console.WriteLine("Lost mouse");
        }








        public void OnRenderSurfaceViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnRenderSurfaceViewLoaded;

            _host = new RenderSurfaceHost(ActualWidth, ActualHeight);
            Content = _host;
            

            //_host.MessageHook += new HwndSourceHook(ControlMsgFilter);
            
        }

        //protected override void OnKeyDown(KeyEventArgs e)
        //{
        //    //base.OnKeyDown(e);
        //    Console.WriteLine("Key down in view");
        //}


        private bool _isDisposed;
        protected virtual void Dispose(bool disposing)
        {
            if (!_isDisposed)
            {
                if (disposing)
                {
                    _host.Dispose();
                }

                _isDisposed = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        internal const int DLGC_WANTCHARS = 0x0080;
        internal const int WM_GETDLGCODE = 0x0087;
        internal const int WM_KEYDOWN = 0x0100;

        private IntPtr ControlMsgFilter(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            Console.WriteLine("controle message filter");
            //Console.WriteLine(Keyboard.FocusedElement.ToString());
            //Keyboard.Focus(this);
            //Console.WriteLine(Keyboard.FocusedElement.ToString());
            handled = false;

            if (msg == WM_KEYDOWN)
            {
                handled = true;
                Console.WriteLine("Key down in render view");
                return (IntPtr)DLGC_WANTCHARS;
            }

            return IntPtr.Zero;
        }


       

        
    }
}
