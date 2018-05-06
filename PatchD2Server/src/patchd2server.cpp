#include <cstdlib>
#include <cstdio>
#include <iostream>


int main(int argc, char* argv[])
{
	if (argc == 2 && 
		(std::strcmp(argv[1], "/help") == 0 || std::strcmp(argv[1], "/h") == 0 || std::strcmp(argv[1], "/?") == 0))
	{
		std::cout << "PatchD2Server copies the patch code section into d2server.dll" << std::endl;
		std::cout << "Patch.exe and d2server.dll must be in the current directory and be available for reading and writing." << std::endl;
		std::cout << "Patch.exe filesize must be greater than 512 bytes and d2server.dll filesize must be greater than 64000 bytes." << std::endl;

		return EXIT_SUCCESS;
	}
	else if (argc >= 2)
	{
		std::cerr << "Unknown argument(s): ";

		for (int i = 1; i < argc; i++)
		{
			std::cerr << "\"" << argv[i] << "\" ";
		}

		std::cerr << std::endl;

		return EXIT_FAILURE;
	}

	FILE *patch_fp = std::fopen("Patch.exe", "rb");
	if (!patch_fp)
	{
		std::cerr << "Can't open Patch.exe to read" << std::endl;
		return EXIT_FAILURE;
	}

	char patch[40960];
	std::size_t patch_filesize = std::fread(patch, 1, sizeof(patch), patch_fp);
	std::fclose(patch_fp);

	FILE *d2server_fp = std::fopen("d2server.dll", "rb");
	if (!d2server_fp)
	{
		std::cerr << "Can't open d2server.dll to read" << std::endl;
		return EXIT_FAILURE;
	}

	void *targetbuf = std::malloc(1024 * 1024);
	std::size_t d2server_filesize = std::fread(targetbuf, 1, 1024 * 1024, d2server_fp);
	std::fclose(d2server_fp);

	d2server_fp = std::fopen("d2server.dll", "wb");
	if (!d2server_fp)
	{
		std::free(targetbuf);
		std::cerr << "Can't open d2server.dll to write" << std::endl;
		return EXIT_FAILURE;
	}

	if (patch_filesize > 512 && d2server_filesize > 0xFA00)
	{
		std::fseek(d2server_fp, 0, SEEK_SET);
		std::fwrite(targetbuf, 1, 0xFA00, d2server_fp);
		std::fwrite(&patch[512], 1, patch_filesize - 512, d2server_fp);
		std::memset(targetbuf, 0, 1024 * 100);
		std::fwrite(targetbuf, 1, 0x4000 - patch_filesize + 512, d2server_fp);
	}
	else
	{
		if (patch_filesize <= 512)
		{
			std::cerr << "Patch.exe filesize is equal to or less than 512 bytes" << std::endl;
		}

		if (d2server_filesize <= 0xFA00)
		{
			std::cerr << "d2server.dll filesize is equal to or less than 64000 bytes" << std::endl;
		}

		std::free(targetbuf);
		std::fclose(d2server_fp);

		return EXIT_FAILURE;
	}

	std::free(targetbuf);
	std::fclose(d2server_fp);

	std::cout << "Successfully patched d2server.dll" << std::endl;

	return EXIT_SUCCESS;
}
