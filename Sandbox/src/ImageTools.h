#pragma once

#include <vector>
#include <string>
#include <utility>

using width = uint32_t;
using height = uint32_t;
using length = uint32_t;
using data = uint8_t;

//std::tuple<std::vector<Sprite>, width, height> LoadBMP24ToTile(const std::string& filepath)
//{
//	HZ_TRACE("Loading bmp");
//	std::vector<Sprite> tiles;
//	std::ifstream ifs(filepath, std::ifstream::binary);
//
//	if (ifs.is_open())
//	{
//		// Read bmp header, make sure its bmp formatted
//		char bmpHeader[14 * 4];
//		// First 2 bytes should be "BM" if file is bmp.
//		char fileType[3];
//
//		ifs.read(bmpHeader, 14 * 4);
//		memcpy(fileType, bmpHeader, 2);
//		fileType[2] = '\0';
//
//		if (std::string("BM").compare(fileType) == 0)
//		{
//			HZ_TRACE("Good type");
//		}
//		else
//		{
//			HZ_TRACE(std::string("bad filetype ... "));
//			return {tiles, 0, 0};
//		}
//
//		uint64_t imageHeight = *(bmpHeader + 0x12);
//		uint64_t imageWidth = *(bmpHeader + 0x16);
//		uint32_t bmpDataOffset = *(bmpHeader + 0x0a);
//		uint64_t imageLength = imageWidth * imageHeight * 4;
//
//		// process bmp pixel matrix data
//		tiles.reserve(imageWidth * imageHeight);
//		char* buffer = new char[imageLength];
//		ifs.seekg(bmpDataOffset);
//		ifs.read(buffer, imageLength);
//		uint64_t offset = 0;
//
//		for (int iy = 0; iy < imageHeight; ++iy)
//		{
//			for (int ix = 0; ix < imageWidth; ++ix)
//			{
//				auto& tile = tiles.emplace_back(ix, iy);
//				tile.SetColor(buffer[offset + 2], buffer[offset + 1], buffer[offset + 0]);
//				offset += 3;
//			}
//			uint32_t aligment = offset % 4;
//			if (aligment != 0)
//				offset += (4 - aligment);
//		}
//		delete[] buffer;
//		return { tiles, imageWidth, imageHeight };
//	}
//	else
//	{
//		HZ_TRACE("Bad filename");
//	}
//
//	return {tiles, 0, 0};
//}


std::tuple<data*, width, height, length> LoadBMP24ToRGBA8(const std::string& filepath)
{
	HZ_TRACE("Loading bmp");
	std::ifstream ifs(filepath, std::ifstream::binary);

	if (ifs.is_open())
	{
		// Read bmp header, make sure its bmp formatted
		char bmpHeader[14 * 4];
		// First 2 bytes should be "BM" if file is bmp.
		char fileType[3];

		ifs.read(bmpHeader, 14 * 4);
		memcpy(fileType, bmpHeader, 2);
		fileType[2] = '\0';

		if (std::string("BM").compare(fileType) == 0)
		{
			HZ_TRACE("Good type");
		}
		else
		{
			HZ_TRACE(std::string("bad filetype ... "));
			return {nullptr, 0, 0, 0};
		}

		uint32_t imageHeight = *(bmpHeader + 0x12);
		uint32_t imageWidth = *(bmpHeader + 0x16);
		uint32_t bmpDataOffset = *(bmpHeader + 0x0a);
		uint32_t imageLength = imageWidth * imageHeight * 4;

		// process bmp pixel matrix data
		char* buffer = new char[imageLength];
		ifs.seekg(bmpDataOffset);
		ifs.read(buffer, imageLength);
		uint32_t offset = 0;
		uint32_t bufferOffset = 0;
		uint8_t* rgbaData = new uint8_t[imageLength];

		for (uint32_t iy = 0; iy < imageHeight; ++iy)
		{
			for (uint32_t ix = 0; ix < imageWidth; ++ix)
			{
				rgbaData[bufferOffset + 0] = buffer[offset + 2];
				rgbaData[bufferOffset + 1] = buffer[offset + 1];
				rgbaData[bufferOffset + 2] = buffer[offset + 0];
				rgbaData[bufferOffset + 3] = 0xFF;
				offset += 3;
				bufferOffset += 4;
			}
			uint32_t aligment = offset % 4;
			if (aligment != 0)
				offset += (4 - aligment);
		}
		delete[] buffer;
		return { rgbaData, imageWidth, imageHeight, imageLength };
	}
	else
	{
		HZ_TRACE("Bad filename");
	}

	return { nullptr, 0, 0, 0 };
}