#include <windows.h>

namespace {

constexpr wchar_t windowClassName[] = L"HelloWorldWindow";
constexpr int languageButtonId = 1;
constexpr int languageButtonWidth = 180;
constexpr int languageButtonHeight = 40;

bool showJapanese = false;

void LayoutLanguageButton(HWND window) {
    HWND button = GetDlgItem(window, languageButtonId);
    if (button == nullptr) {
        return;
    }

    RECT clientArea;
    GetClientRect(window, &clientArea);
    const int x = (clientArea.right - languageButtonWidth) / 2;
    const int y = clientArea.bottom - languageButtonHeight - 20;
    MoveWindow(button, x, y, languageButtonWidth, languageButtonHeight, TRUE);
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        HWND button = CreateWindowExW(
            0, L"BUTTON", L"日本語に切り替え",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            0, 0, languageButtonWidth, languageButtonHeight,
            window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(languageButtonId)),
            GetModuleHandleW(nullptr), nullptr);
        if (button == nullptr) {
            return -1;
        }
        LayoutLanguageButton(window);
        return 0;
    }
    case WM_SIZE:
        LayoutLanguageButton(window);
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == languageButtonId && HIWORD(wParam) == BN_CLICKED) {
            const bool nextLanguage = !showJapanese;
            const wchar_t* buttonText = nextLanguage
                ? L"英語に切り替え"
                : L"日本語に切り替え";
            if (!SetWindowTextW(GetDlgItem(window, languageButtonId), buttonText)) {
                MessageBoxW(window, L"Failed to update the language button.",
                            L"Hello World", MB_OK | MB_ICONERROR);
                return 0;
            }
            showJapanese = nextLanguage;
            InvalidateRect(window, nullptr, TRUE);
            return 0;
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT paint;
        HDC deviceContext = BeginPaint(window, &paint);

        RECT clientArea;
        GetClientRect(window, &clientArea);
        clientArea.bottom -= languageButtonHeight + 40;
        const wchar_t* greeting = showJapanese
            ? L"こんにちは！"
            : L"Hello, World!";
        DrawTextW(deviceContext, greeting, -1, &clientArea,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(window, &paint);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(window, message, wParam, lParam);
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

}  // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand) {
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.lpszClassName = windowClassName;
    windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    if (RegisterClassExW(&windowClass) == 0) {
        MessageBoxW(nullptr, L"Failed to register the application window.",
                    L"Hello World", MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND window = CreateWindowExW(
        0, windowClassName, L"Hello World", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 400,
        nullptr, nullptr, instance, nullptr);
    if (window == nullptr) {
        MessageBoxW(nullptr, L"Failed to create the application window.",
                    L"Hello World", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(window, showCommand);
    UpdateWindow(window);

    MSG message{};
    int result;
    while ((result = GetMessageW(&message, nullptr, 0, 0)) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return result == -1 ? 1 : static_cast<int>(message.wParam);
}
