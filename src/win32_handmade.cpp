#include <windows.h>

LRESULT CALLBACK
MainWindowCallback(
    HWND Window,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
    case WM_SIZE:
    {
        OutputDebugStringA("WM_SIZE\n");
        break;
    }

    case WM_DESTROY:
    {
        OutputDebugStringA("WM_DESTROY\n");
        break;
    }

    case WM_CLOSE:
    {
        OutputDebugStringA("WM_CLOSE\n");
        break;
    }

    case WM_ACTIVATEAPP:
    {
        OutputDebugStringA("WM_ACTIVATEAPP\n");
        break;
    }

    case WM_PAINT:
    {
        OutputDebugStringA("WM_PAINT\n");

        PAINTSTRUCT Paint;
        HDC DeviceContext = BeginPaint(Window, &Paint);

        int X = Paint.rcPaint.left;
        int Y = Paint.rcPaint.top;
        int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
        int Width = Paint.rcPaint.right - Paint.rcPaint.left;
        static DWORD Operation = WHITENESS;

        PatBlt(
            DeviceContext,
            X,
            Y,
            Width,
            Height,
            Operation);

        if (Operation == WHITENESS)
        {
            Operation = BLACKNESS;
        }
        else
        {
            Operation = WHITENESS;
        }

        EndPaint(Window, &Paint);
        break;
    }

    default:
    {
        //OutputDebugStringA("default\n");
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
    int ShowCode)
{
    WNDCLASS windowClass = {};

    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = Instance;
    windowClass.lpszClassName = "HandmadeHeroWindowClass";
    // windowClass.hIcon;

    ATOM res = RegisterClass(&windowClass);
    if (!res)
    {
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
        0);

    if (!WindowHandle)
    {
        // TODO: Handle Failure
    }

    MSG Message;
    for (;;)
    {
        BOOL msgRes = GetMessage(&Message, 0, 0, 0);
        if (msgRes < 0)
        {
            break;
        }

        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return (0);
}
