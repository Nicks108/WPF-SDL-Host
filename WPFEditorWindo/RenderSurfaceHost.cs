using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Interop;

namespace WPFEditor
{
    class RenderSurfaceHost : HwndHost
    {
        private int _width = 800;
        private int _height = 600;
        private IntPtr _renderWindowHandle = IntPtr.Zero;

        private static int INVALID_ID = -1;
        public int SurfaceID { get; private set; } = INVALID_ID;

        private Thread _renderThread;
        private bool isRunning = true;

        public RenderSurfaceHost(double width, double height)
        {
            _width = (int) width;
            _height = (int) height;
        }

        //private IntPtr _hwndHost;
        //private IntPtr _hwndChiled;
        internal const int WsChild = 0x40000000;
        internal const int WsVisible = 0x10000000;
        internal const int HostId = 0x00000002;
        internal const int WmErasebkgnd = 0x0014;
        internal const int WM_KEYDOWN = 0x0100;

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            // SurfaceID =
            _renderWindowHandle = CreateWindowEx(0, "static", "",
                WsChild | WsVisible,
                0, 0,
                (int)ActualWidth,
                (int)ActualHeight,
                hwndParent.Handle,
                (IntPtr) HostId,
                IntPtr.Zero,
                0);

            // _renderWindowHandle = EngineAPI.GetWindowHandle(_hwndHost);
            EngineAPI.CreatNewChildGameWindow(_renderWindowHandle, _width, _height);

            HandleRef h = new HandleRef(this, _renderWindowHandle);

            _renderThread = new Thread(new ParameterizedThreadStart(Render));
            _renderThread.Start();

            
            return h;
        }

        private void Render(object data)
        {
            try
            {
                //init open gl

                while (isRunning)
                {
                    if (this.IsMouseOver)
                    {
                        byte[] array = new byte[256];
                        GetKeyboardState(array);
                        int i = 0;
                    }

                    //handle resize
                    EngineAPI.dllRender();

                    
                }
            }
            catch (Exception e)
            {
                //Dispatcher.BeginInvoke(Error, e.Message);
            }
        }

        private void HandleResize()
        {
            // viewport resize?
            int newWidth = (int)ActualWidth;
            int newHeight = (int)ActualHeight;

            if (_width != newWidth || _height != newHeight)
            {
                _width = newWidth;
                _height = newHeight;
                EngineAPI.ResizeGameWindow(_width, _height);
            }
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
        isRunning = false;
            //   Remove renderer surface(SurfaceID);

            SurfaceID = INVALID_ID;
            //dfstrow surface in c++
            _renderWindowHandle = IntPtr.Zero;
        }

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            //Console.WriteLine(Keyboard.FocusedElement.ToString());
            //Keyboard.Focus(this);
            //Console.WriteLine(Keyboard.FocusedElement.ToString());

            switch (msg)
            {
                case WM_KEYDOWN:
                    handled = false;
                    break;
                case WmErasebkgnd:
                    //_view.update();
                    handled = true;
                    break;
                default:
                    handled = false;
                    //SendMessage(_renderWindowHandle, msg, wParam, lParam);
                    break;
            }

            return IntPtr.Zero;
        }
        protected override void OnKeyDown(KeyEventArgs e)
        {
            //base.OnKeyDown(e);
            Console.WriteLine("Key down in render host");
            SendMessage(_renderWindowHandle, WM_KEYDOWN, (IntPtr)KeyInterop.VirtualKeyFromKey(e.Key), IntPtr.Zero);
        }


        [DllImport("user32.dll", EntryPoint = "SendMessage", CharSet = CharSet.Unicode)]
        internal static extern int SendMessage(IntPtr hwnd,
            int msg,
            IntPtr wParam,
            IntPtr lParam);



        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        internal static extern IntPtr CreateWindowEx(int dwExStyle,
            string lpszClassName,
            string lpszWindowName,
            int style,
            int x, int y,
            int width, int height,
            IntPtr hwndParent,
            IntPtr hMenu,
            IntPtr hInst,
            [MarshalAs(UnmanagedType.AsAny)] object pvParam);

        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);


        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GetKeyboardState(byte[] lpKeyState);
    }
}
