/*
 * ntimage.h — NT DDK/SDK Compatibility Header
 *
 * Part of the NetCog (NT-OC) AGI-OS unified build.
 * Provides cross-platform compatibility definitions for the NT4 kernel source tree.
 * Referenced by 15 source files.
 *
 * Source references:
 *   - phnt (winsiderss/phnt) for NT Native API types
 *   - ReactOS (reactos/reactos) for DDK structure layouts
 *   - NDK (arizvisa/ndk) for kernel-internal structures
 */
#ifndef _NT_COMPAT_NTIMAGE_H_H_
#define _NT_COMPAT_NTIMAGE_H_H_

#include "ntdef.h"

#ifdef __cplusplus
extern "C" {
#endif


/* NT PE Image format definitions */
#ifndef _NTIMAGE_H_COMPAT
#define _NTIMAGE_H_COMPAT
#define IMAGE_DOS_SIGNATURE     0x5A4D
#define IMAGE_NT_SIGNATURE      0x00004550
#define IMAGE_FILE_MACHINE_I386     0x014c
#define IMAGE_FILE_MACHINE_ALPHA    0x0184
#define IMAGE_FILE_MACHINE_MIPS16   0x0266
#define IMAGE_FILE_MACHINE_POWERPC  0x01F0
typedef struct _IMAGE_DOS_HEADER {
    USHORT e_magic; USHORT e_cblp; USHORT e_cp; USHORT e_crlc;
    USHORT e_cparhdr; USHORT e_minalloc; USHORT e_maxalloc; USHORT e_ss;
    USHORT e_sp; USHORT e_csum; USHORT e_ip; USHORT e_cs;
    USHORT e_lfarlc; USHORT e_ovno; USHORT e_res[4]; USHORT e_oemid;
    USHORT e_oeminfo; USHORT e_res2[10]; LONG e_lfanew;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
typedef struct _IMAGE_FILE_HEADER {
    USHORT Machine; USHORT NumberOfSections; ULONG TimeDateStamp;
    ULONG PointerToSymbolTable; ULONG NumberOfSymbols;
    USHORT SizeOfOptionalHeader; USHORT Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;
typedef struct _IMAGE_DATA_DIRECTORY { ULONG VirtualAddress; ULONG Size; } IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
typedef struct _IMAGE_OPTIONAL_HEADER {
    USHORT Magic; UCHAR MajorLinkerVersion; UCHAR MinorLinkerVersion;
    ULONG SizeOfCode; ULONG SizeOfInitializedData; ULONG SizeOfUninitializedData;
    ULONG AddressOfEntryPoint; ULONG BaseOfCode; ULONG BaseOfData;
    ULONG ImageBase; ULONG SectionAlignment; ULONG FileAlignment;
    USHORT MajorOperatingSystemVersion; USHORT MinorOperatingSystemVersion;
    USHORT MajorImageVersion; USHORT MinorImageVersion;
    USHORT MajorSubsystemVersion; USHORT MinorSubsystemVersion;
    ULONG Win32VersionValue; ULONG SizeOfImage; ULONG SizeOfHeaders;
    ULONG CheckSum; USHORT Subsystem; USHORT DllCharacteristics;
    ULONG SizeOfStackReserve; ULONG SizeOfStackCommit;
    ULONG SizeOfHeapReserve; ULONG SizeOfHeapCommit;
    ULONG LoaderFlags; ULONG NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;
typedef struct _IMAGE_NT_HEADERS {
    ULONG Signature; IMAGE_FILE_HEADER FileHeader; IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;
#define IMAGE_SIZEOF_SHORT_NAME 8
typedef struct _IMAGE_SECTION_HEADER {
    UCHAR Name[IMAGE_SIZEOF_SHORT_NAME];
    union { ULONG PhysicalAddress; ULONG VirtualSize; } Misc;
    ULONG VirtualAddress; ULONG SizeOfRawData; ULONG PointerToRawData;
    ULONG PointerToRelocations; ULONG PointerToLinenumbers;
    USHORT NumberOfRelocations; USHORT NumberOfLinenumbers; ULONG Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
typedef struct _IMAGE_EXPORT_DIRECTORY {
    ULONG Characteristics; ULONG TimeDateStamp;
    USHORT MajorVersion; USHORT MinorVersion;
    ULONG Name; ULONG Base; ULONG NumberOfFunctions;
    ULONG NumberOfNames; ULONG AddressOfFunctions;
    ULONG AddressOfNames; ULONG AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;
#endif /* _NTIMAGE_H_COMPAT */


#ifdef __cplusplus
}
#endif

#endif /* _NT_COMPAT_NTIMAGE_H_H_ */
