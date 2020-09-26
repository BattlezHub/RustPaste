enum class AimBone : int {
    leg = 2,
    pelvis = 4,
    spine = 8,
    neck = 16,
    head = 32,
    none = 64
};
enum class DistancePreference : int {
    none,
    mininal,
    max_damage,
};

//
//typedef struct _IMAGE_DATA_DIRECTORY {
//    DWORD   VirtualAddress;
//    DWORD   Size;
//} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;
//typedef struct _IMAGE_OPTIONAL_HEADER64 {
//    WORD        Magic;
//    BYTE        MajorLinkerVersion;
//    BYTE        MinorLinkerVersion;
//    DWORD       SizeOfCode;
//    DWORD       SizeOfInitializedData;
//    DWORD       SizeOfUninitializedData;
//    DWORD       AddressOfEntryPoint;
//    DWORD       BaseOfCode;
//    ULONGLONG   ImageBase;
//    DWORD       SectionAlignment;
//    DWORD       FileAlignment;
//    WORD        MajorOperatingSystemVersion;
//    WORD        MinorOperatingSystemVersion;
//    WORD        MajorImageVersion;
//    WORD        MinorImageVersion;
//    WORD        MajorSubsystemVersion;
//    WORD        MinorSubsystemVersion;
//    DWORD       Win32VersionValue;
//    DWORD       SizeOfImage;
//    DWORD       SizeOfHeaders;
//    DWORD       CheckSum;
//    WORD        Subsystem;
//    WORD        DllCharacteristics;
//    ULONGLONG   SizeOfStackReserve;
//    ULONGLONG   SizeOfStackCommit;
//    ULONGLONG   SizeOfHeapReserve;
//    ULONGLONG   SizeOfHeapCommit;
//    DWORD       LoaderFlags;
//    DWORD       NumberOfRvaAndSizes;
//    IMAGE_DATA_DIRECTORY DataDirectory[16];
//} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;
//typedef struct _IMAGE_FILE_HEADER {
//    WORD    Machine;
//    WORD    NumberOfSections;
//    DWORD   TimeDateStamp;
//    DWORD   PointerToSymbolTable;
//    DWORD   NumberOfSymbols;
//    WORD    SizeOfOptionalHeader;
//    WORD    Characteristics;
//} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;
//typedef struct _IMAGE_NT_HEADERS64 {
//    DWORD Signature;
//    IMAGE_FILE_HEADER FileHeader;
//    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
//} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64;
//typedef PIMAGE_NT_HEADERS64 PIMAGE_NT_HEADERS;
//typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
//    WORD   e_magic;                     // Magic number
//    WORD   e_cblp;                      // Bytes on last page of file
//    WORD   e_cp;                        // Pages in file
//    WORD   e_crlc;                      // Relocations
//    WORD   e_cparhdr;                   // Size of header in paragraphs
//    WORD   e_minalloc;                  // Minimum extra paragraphs needed
//    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
//    WORD   e_ss;                        // Initial (relative) SS value
//    WORD   e_sp;                        // Initial SP value
//    WORD   e_csum;                      // Checksum
//    WORD   e_ip;                        // Initial IP value
//    WORD   e_cs;                        // Initial (relative) CS value
//    WORD   e_lfarlc;                    // File address of relocation table
//    WORD   e_ovno;                      // Overlay number
//    WORD   e_res[4];                    // Reserved words
//    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
//    WORD   e_oeminfo;                   // OEM information; e_oemid specific
//    WORD   e_res2[10];                  // Reserved words
//    LONG   e_lfanew;                    // File address of new exe header
//} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;
//typedef struct _IMAGE_EXPORT_DIRECTORY {
//    DWORD   Characteristics;
//    DWORD   TimeDateStamp;
//    WORD    MajorVersion;
//    WORD    MinorVersion;
//    DWORD   Name;
//    DWORD   Base;
//    DWORD   NumberOfFunctions;
//    DWORD   NumberOfNames;
//    DWORD   AddressOfFunctions;     // RVA from base of image
//    DWORD   AddressOfNames;         // RVA from base of image
//    DWORD   AddressOfNameOrdinals;  // RVA from base of image
//} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;
//
//typedef struct _UNICODE_STRING {
//    USHORT Length;
//    USHORT MaximumLength;
//    PWSTR  Buffer;
//} UNICODE_STRING;
//typedef UNICODE_STRING* PUNICODE_STRING;
//
//typedef struct LDR_DATA_TABLE_ENTRY;
//
//typedef struct _NT_TIB {
//    struct _EXCEPTION_REGIxorstrTION_RECORD* ExceptionList;
//    PVOID StackBase;
//    PVOID StackLimit;
//    PVOID SubSystemTib;
//#if defined(_MSC_EXTENSIONS)
//    union {
//        PVOID FiberData;
//        DWORD Version;
//    };
//#else
//    PVOID FiberData;
//#endif
//    PVOID ArbitraryUserPointer;
//    struct _NT_TIB* Self;
//} NT_TIB;
#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))
#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))