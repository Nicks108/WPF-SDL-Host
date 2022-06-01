using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;


namespace WPFEditor
{
    static class EngineAPI
    {
        //private const string _engineDLL = @"C:\Users\ndt59\OneDrive - University of Suffolk\UOS\Units\Tools For Game Development\06 - im gui 1\SDLGameNetworking - C# editor test\Debug\SDLGameNetworking.dll";


        private const string _engineDLL =
            @"D:\One Drive - UOS\OneDrive - University of Suffolk\UOS\Units\Tools For Game Development\06 - im gui 1\SDLGameNetworking - C# editor test\Debug\SDLGameNetworking.dll";

        //C:\Users\ndt59\OneDrive - University of Suffolk\UOS\Units\Tools For Game Development\06 - im gui 1\SDLGameNetworking - C# editor test\Debug

        //[DllImport(@"D:\One Drive - UOS\OneDrive - University of Suffolk\UOS\Units\Tools For Game Development\06 - im gui 1\SDLGameNetworking - C# editor test\Debug\SDLGameNetworking.dll",
        //    CallingConvention = CallingConvention.Cdecl)]
        //public static extern IntPtr GetWindowHandle(); //IntPtr


        [DllImport(_engineDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void dllRender();

        [DllImport(_engineDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreatNewChildGameWindow(IntPtr handle, int width, int height);

        [DllImport(_engineDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ResizeGameWindow(int width, int height);
        
        
    }
}
