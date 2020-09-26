#pragma once
#include <intrin.h>
#include <winternl.h>
#include "xorstr.h"
#include <winnt.h>
#include "crt.h"

template <typename T>
T Read(DWORD64 address) {
	if (address > 0x40000 && (address + sizeof(T)) < 0x7FFFFFFF0000) {
		return *(T*)(address);
	}
	else {
		return T{};
	}
}
template <typename T>
void Write(DWORD64 address, T data) {
	if (address > 0x40000 && (address + sizeof(T)) < 0x7FFFFFFF0000) {
		*(T*)(address) = data;
	}
}
#define safe_read(Addr, Type) Read<Type>((DWORD64)Addr)
#define safe_write(Addr, Data, Type) Write<Type>((DWORD64)Addr, Data);

ULONG64 GetExport(ULONG64 hDll, const char* Name)
{
	PIMAGE_NT_HEADERS NT_Head = (PIMAGE_NT_HEADERS)(hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(hDll + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

	for (DWORD i = 0; i < ExportDir->NumberOfNames; i++)
	{
		USHORT Ordinal = ((USHORT*)(hDll + ExportDir->AddressOfNameOrdinals))[i];
		const char* ExpName = (const char*)hDll + ((DWORD*)(hDll + ExportDir->AddressOfNames))[i];

		for (int i = 0; ExpName[i] == Name[i]; i++) if (!ExpName[i])
			return hDll + ((DWORD*)(hDll + ExportDir->AddressOfFunctions))[Ordinal];
	} return 0;
}

DWORD64 g_Base = 0;
ULONG64 GetModBase(const wchar_t* Name = xorstr(L"GameAssembly.dll")) 
{
	bool ga = m_wcsicmp((wchar_t*)Name, xorstr(L"GameAssembly.dll"));
	if (g_Base && ga) { 
		return g_Base;
	}
	if (ga)
		g_Base = (ULONG64)(GetModuleHandleW(Name));
	//PPEB_LDR_DATA Ldr = ((PTEB)__readgsqword(FIELD_OFFSET(NT_TIB, Self)))->ProcessEnvironmentBlock->Ldr;
	//for (PLIST_ENTRY CurEnt = Ldr->InMemoryOrderModuleList.Flink; CurEnt != &Ldr->InMemoryOrderModuleList; CurEnt = CurEnt->Flink) {
	//	PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(CurEnt, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
	//	if (((ULONG64)Name > 1 && m_wcsicmp((wchar_t*)Name, (wchar_t*)pEntry->BaseDllName.Buffer)) ||
	//		((ULONG64)Name == 1 && pEntry->BaseDllName.Length == 18 && *(ULONG64*)pEntry->BaseDllName.Buffer == 0x6C00640074006E) || !Name) {
	//		if (ga) g_Base = (ULONG64)pEntry->DllBase;
	//		return (ULONG64)pEntry->DllBase;
	//	}

	//}
	return (ULONG64)(GetModuleHandleW(Name));
}
