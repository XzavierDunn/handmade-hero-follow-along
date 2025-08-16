#include <windows.h>

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    MessageBoxA(0, "Body", "Handmade Hero",
                MB_OK | MB_ICONINFORMATION);

    return (0);
}
