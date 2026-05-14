namespace LibraryLoader {

    // Simple dynamic library loader for Windows v 0.2
    typedef void * HMODULE;
    typedef void * HWND;
    typedef unsigned int UINT;
    typedef long long int (*FARPROC)();
    typedef const char * LPCSTR;
    typedef bool BOOL;
    
    extern "C" {
        FARPROC GetProcAddress(HMODULE hModule, LPCSTR  lpProcName);
        HMODULE LoadLibraryA(LPCSTR lpLibFileName);
        int MessageBoxA(HWND   hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT   uType);
        void exit(int);
        constexpr unsigned int MB_OK = 0;
        BOOL FreeLibrary(HMODULE hLibModule);
    }
  
    class Loader {

        HMODULE hModule;
        FARPROC proc;

    public:
        Loader(const char * dllPath) {
            hModule = LoadLibraryA(dllPath);
            if (!hModule) {
                MessageBoxA(nullptr, "Failed to load library!", dllPath, MB_OK);
                exit(1);
            }

        }

        ~Loader() {
            if (hModule) FreeLibrary(hModule);
        }

     
        void LoadFunction(const char * funcName) {
            
            proc = GetProcAddress(hModule, funcName);
            if (!proc) {
                MessageBoxA(nullptr, "Failed to get function!", funcName, MB_OK);    
                exit(1);
            }
          
        }

        // This template mimics the "CFFI" feel
        template <typename Ret, typename... Args>
        void Call(Args... args) {
           // Define the function pointer type based on template arguments
            
           Ret (__cdecl *FuncPtr)(Args... args) = nullptr; 
           FuncPtr = (Ret (__cdecl *)(Args... args))proc; 
           FuncPtr(args...);    
            
        } 

    };





}
