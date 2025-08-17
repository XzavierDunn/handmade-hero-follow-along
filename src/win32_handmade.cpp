#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

internal void Win32ResizeDIBSection(
    int Width,
    int Height
) {
    // Resize DIB (Device Independent Bitmap) Section based on new window width
    // and height

    // TODO: Check for success before freeing old section
    if (BitmapHandle) {
        DeleteObject(BitmapHandle);
    }

    if (!BitmapDeviceContext) {
        // TODO: Does this ever need recreated?
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(
        BitmapDeviceContext,
        &BitmapInfo,
        DIB_RGB_COLORS,
        &BitmapMemory,
        0,
        0
    );
}

internal void Win32UpdateWindow(
    HDC DeviceContext,
    int X,
    int Y,
    int Width,
    int Height
) {
    StretchDIBits(
        DeviceContext,
        X,
        Y,
        Width,
        Height,
        X,
        Y,
        Width,
        Height,
        BitmapMemory,
        &BitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

LRESULT CALLBACK Win32MainWindowCallback(
    HWND Window,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
) {
    LRESULT
    Result = 0;

    switch (Message) {
        case WM_SIZE: {
            OutputDebugStringA("WM_SIZE\n");

            RECT ClientRect;
            GetClientRect(Window, &ClientRect);

            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top;

            Win32ResizeDIBSection(Width, Height);
            break;
        }

        case WM_DESTROY: {
            OutputDebugStringA("WM_DESTROY\n");
            // TODO: handle as error - recreate window
            Running = false;
            break;
        }

        case WM_CLOSE: {
            OutputDebugStringA("WM_CLOSE\n");
            // TODO: prompt user to confirm action
            Running = false;
            break;
        }

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            break;
        }

        case WM_PAINT: {
            OutputDebugStringA("WM_PAINT\n");

            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);

            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            Win32UpdateWindow(DeviceContext, X, Y, Width, Height);

            EndPaint(Window, &Paint);
            break;
        }

        default: {
            // OutputDebugStringA("default\n");
            Result = DefWindowProc(Window, Message, wParam, lParam);
            break;
        }
    }

    return Result;
}

int WINAPI wWinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    PWSTR CommandLine,
    int ShowCode
) {
    WNDCLASS windowClass = {};

    windowClass.lpfnWndProc = Win32MainWindowCallback;
    windowClass.hInstance = Instance;
    windowClass.lpszClassName = "HandmadeHeroWindowClass";
    // windowClass.hIcon;

    ATOM res = RegisterClass(&windowClass);
    if (!res) {
        // TODO: Handle Failure
    }

    HWND WindowHandle = CreateWindowEx(
        0,
        windowClass.lpszClassName,
        "Handmade Hero",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        Instance,
        0
    );

    if (!WindowHandle) {
        // TODO: Handle Failure
    }

    Running = true;

    MSG Message;
    while (Running) {
        BOOL msgRes = GetMessage(&Message, 0, 0, 0);
        if (msgRes <= 0) {
            break;
        }

        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }

    return (0);
}
