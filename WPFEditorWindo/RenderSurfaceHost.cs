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
        private HandleRef _hwnd;
        private HandleRef _hwndParent;

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
        internal const int WM_NCHITTEST = 0x0084;
        internal const int HTCLIENT = 1;
        internal const int WM_CAPTURECHANGED = 0x0215;
        internal const int WM_MOUSELEAVE = 0x02A3;
        internal const int WM_MOUSEHOVER = 0x02A1;
        internal const int WM_NCMOUSEHOVER = 0x02A0;
        internal const int WM_NCMOUSELEAVE = 0x02A2;
        internal bool _isMouseInwindow = false;

        internal bool IsMouseInWindow
        {
            set
            {_isMouseInwindow = value;
                if (value == true)
                    SetFocus(_hwnd.Handle);
                else
                {
                    SetFocus(IntPtr.Zero);
                }
            }
            get
            {
                return _isMouseInwindow;
            }
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            _hwndParent = hwndParent;
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

            _hwnd = new HandleRef(this, _renderWindowHandle);

            _renderThread = new Thread(new ParameterizedThreadStart(Render));
            _renderThread.Start();

            //did this do anything?
            //const int GWL_STYLE = (-16);
            //const int WS_CHILD = 0x40000000;
            //SetWindowLong(_hwnd.Handle, GWL_STYLE, WS_CHILD);
            //SetParent(_hwnd.Handle, hwndParent.Handle);
            SetFocus(_hwnd.Handle);

            this.MouseEnter+= OnMouseEnter;
            this.MouseLeave += OnMouseLeave;
            
            return _hwnd;
        }

        private void OnMouseLeave(object sender, MouseEventArgs e)
        {
            Console.WriteLine("leaveing render surface");
        }

        private void OnMouseEnter(object sender, MouseEventArgs e)
        {
            Console.WriteLine("entering render surface");
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
            
            //Console.WriteLine(IsKeyboardFocused);
            //Console.WriteLine(IsKeyboardFocusWithin);
            
            switch (msg)
            {
                case WM_KEYDOWN:
                    
                    Console.WriteLine("keydoen in render host");
                    Console.WriteLine(this.IsMouseCaptured);
                    System.Windows.Point p = Mouse.GetPosition(this);
                    
                    handled = !IsMouseInWindow;
                    break;
                case WmErasebkgnd:
                    //_view.update();
                    handled = true;
                    break;
                case WM_NCHITTEST:
                {
                    Console.WriteLine("mouse in window");
                    IsMouseInWindow = true;
                    handled = true;
                    return (IntPtr)(HTCLIENT);
                }
                break;
                case WM_CAPTURECHANGED:
                    //Console.WriteLine("mouse capture change");
                    break;
                case WM_MOUSELEAVE:
                    Console.WriteLine("mouse leaving");
                    IsMouseInWindow = false;
                    handled = true;
                    break;
                case WM_MOUSEHOVER:
                    Console.WriteLine("mouse hover render host");
                    break;
                case WM_NCMOUSEHOVER:
                    Console.WriteLine("mouse hover render host NC");
                    break;
                case WM_NCMOUSELEAVE :
                    Console.WriteLine("mouse leave render host NC");
                    break;
                default:
                    handled = false;
                    //SendMessage(_renderWindowHandle, msg, wParam, lParam);
                    break;
            }

            return IntPtr.Zero;
        }
        //protected override void OnKeyDown(KeyEventArgs e)
        //{
        //    //base.OnKeyDown(e);
        //    Console.WriteLine("Key down in render host");
        //    //SendMessage(_renderWindowHandle, WM_KEYDOWN, (IntPtr)KeyInterop.VirtualKeyFromKey(e.Key), IntPtr.Zero);

        //    MSG msg = ComponentDispatcher.CurrentKeyboardMessage;
            
            

        //    ModifierKeys modifiers = Keyboard.Modifiers; // HwndKeyboardInputProvider.GetSystemModifierKeys();

        //    bool handled = ((IKeyboardInputSink)this).TranslateAccelerator(ref msg, modifiers);

        //    if (handled)
        //        e.Handled = handled;

        //    base.OnKeyDown(e);

        //}
        

       




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


        [DllImport("user32.dll")]
        static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);
        [DllImport("user32.dll")]
        static extern int SetWindowLong(IntPtr hWnd, int nIndex, UInt32 dwNewLong);

        [DllImport("user32.dll")]
        static extern IntPtr SetFocus(IntPtr hwnd);



    }
}
