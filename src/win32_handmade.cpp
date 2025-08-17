#include <stdint.h>
#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable int BitmapWidth;
global_variable int BitmapHeight;
global_variable int BytesPerPixel = 4;

internal void RenderGradient(
    int xOffset,
    int yOffset
) {
    int Pitch = BitmapWidth * BytesPerPixel;
    uint8_t *Row = (uint8_t *)BitmapMemory;

    for (int y = 0; y < BitmapHeight; ++y) {
        uint32_t *Pixel = (uint32_t *)Row;
        for (int x = 0; x < BitmapWidth; ++x) {
            uint8_t Blue = (x + xOffset);
            uint8_t Green = (y + yOffset);

            *Pixel++ = ((Green << 8) | Blue);
        }

        Row += Pitch;
    }
}

internal void Win32ResizeDIBSection(
    int Width,
    int Height
) {
    // Resize DIB (Device Independent Bitmap) Section based on new window width
    // and height

    if (BitmapMemory) {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE);
    }

    BitmapWidth = Width;
    BitmapHeight = Height;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = BytesPerPixel * (BitmapWidth * BitmapHeight);
    BitmapMemory =
        VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

internal void Win32UpdateWindow(
    HDC DeviceContext,
    RECT *ClientRect
) {
    int WindowWidth = ClientRect->right - ClientRect->left;
    int WindowHeight = ClientRect->bottom - ClientRect->top;

    StretchDIBits(
        DeviceContext,
        0,
        0,
        BitmapWidth,
        BitmapHeight,
        0,
        0,
        WindowWidth,
        WindowHeight,
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

            Win32UpdateWindow(DeviceContext, &Paint.rcPaint);

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

    HWND Window = CreateWindowEx(
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

    if (!Window) {
        // TODO: Handle Failure
    }

    Running = true;
    int xOffset = 0;
    int yOffset = 0;

    while (Running) {
        MSG Message;
        while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) {
            if (Message.message == WM_QUIT) {
                Running = false;
            }

            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        };

        RenderGradient(xOffset, yOffset);

        RECT ClientRect;
        GetClientRect(Window, &ClientRect);
        HDC DeviceContext = GetDC(Window);
        Win32UpdateWindow(DeviceContext, &ClientRect);
        ReleaseDC(Window, DeviceContext);

        xOffset++;
    }

    return (0);
}
