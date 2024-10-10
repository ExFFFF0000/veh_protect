#include <iostream>
#include <windows.h>
#include <format>

extern "C" uint64_t ReadPointer(uint64_t ptr);

long __stdcall handler(EXCEPTION_POINTERS* info)
{
	if (info->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		return EXCEPTION_CONTINUE_SEARCH;

	std::cout << "Handling EXCEPTION_ACCESS_VIOLATION" << std::endl;

	// decrypt
	info->ContextRecord->Rcx -= 0x8000000000000000;

	return EXCEPTION_CONTINUE_EXECUTION;
}

uint64_t old_ptr = 0;

int main()
{
	int a = 0;
	old_ptr = reinterpret_cast<uint64_t>(&a);
	std::cout << "old_ptr: " << std::hex << reinterpret_cast<uint64_t>(&old_ptr) << std::endl;
	std::cin.get();

	// encrypt
	uint64_t invalid_ptr = 0x8000000000000000 + old_ptr;
	old_ptr = invalid_ptr;

	//add exception handler
	AddVectoredExceptionHandler(false, handler);
	std::cin.get();

	ReadPointer(old_ptr);

	std::cout << "Done";
	std::cin.get();
}
