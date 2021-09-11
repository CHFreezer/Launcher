#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <windows.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
#if _DEBUG
    boost::filesystem::wifstream wstream("Debug\\Launcher.ini");
#else
    boost::filesystem::wifstream wstream("Launcher.ini");  
#endif
    boost::locale::generator gen;
    wstream.imbue(gen("UTF-8"));
    std::wstring appPath;
    std::wstring line;
    while (std::getline(wstream, line))
    {
        if (boost::algorithm::find_first(line, TEXT("ApplicationPath")))
        {
            std::vector<std::wstring> result;
            boost::split(result, line, boost::is_any_of("="));
            appPath = boost::algorithm::trim_copy(result[1]);
            break;
        }
    }

#if _DEBUG
    MessageBox(NULL, appPath.c_str(), NULL, 0);
#endif

    boost::process::ipstream pipe_stream;
    boost::process::child process(appPath, boost::process::std_out > pipe_stream);
    process.detach();

    return 0;
}