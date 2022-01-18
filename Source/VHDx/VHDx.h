#pragma once
#ifndef _H_VHD_X_H_
#define _H_VHD_X_H_

#include "FormatVHDx.h"
#include "IVHDx.h"
#include <fstream>
#include <string>


/*
*	是VHDx, 不是VHDX, 他俩真不是一个东西
*	当前是对Fixed Hard Disk Image以
*	LBA(Logical Block Addressing)的方式读写操作
*/
//class IVHDx;
class VHDx : public IVHDx
{
public:
	VHDx(const char* szFile);
	~VHDx();

public:
	/*
	*	打开VHD文件
	*/
	int OpenVHD(const char* szFile = nullptr);
	/*
	*	关闭VHD文件
	*/
	int CloseVHD();

	/*
	*	读取Footer信息
	*/
	long long ReadFooterInfo();

	/*
	*	读数据
	*	LBA(Logical Block Addressing)索引
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	long long Read(unsigned int uiLBA, char* szDataBuffer, long long llDataSize);
	/*
	*	写数据
	*	LBA(Logical Block Addressing)索引
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	long long Write(unsigned int uiLBA, const char* szDataBuffer, long long llDataSize);

	/*
	*	Checksum计算
	*/
	unsigned int Checksum();

	/*
	*	VHD文件信息
	*/
	const char* ToString();

	/*
	*	释放资源
	*/
	void Dispose();


public:
	/*
	*	LBA转换CHS
	*	LBA(Logical Block Addressing)索引
	*	C = LBA / (HPC * SPT) + CS
	*	H = LBA / SPT - (C - CS) * HPC + HS
	*	S = LBA - (C - CS) * HPC * SPT - (H - HS) * SPT + SS
	*	C, H and S are the cylinder number, the head number, and the sector number
	*	LBA is the logical block address
	*	HPC is the number of heads per cylinder
	*	SPT is the number of sectors per track
	*/
	static size_t LBA2CHS(DiskGeometry* pDiskGeometry, unsigned int uiLBA);
	/*
	*	CHS转换LBA
	*	LBA = (C - CS) * HPC * SPT + (H - HS) * SPT + (S - SS)
	*	C, H and S are the cylinder number, the head number, and the sector number
	*	LBA is the logical block address
	*	HPC is the number of heads per cylinder
	*	SPT is the number of sectors per track
	*/
	static unsigned int CHS2LBA(const DiskGeometry* pDiskGeometry);

	/*
	*	CHS获取
	*	VirtualSize is the current size of the fixed disk in bytes.
	*/
	static size_t CHS(DiskGeometry* pDiskGeometry, unsigned long long ullVirtualSize);

	/*
	*	UUID字符信息
	*	UUID 数据
	*/
	static const std::string UUID(const unsigned char* pUUID);


private:
	/*
	*	读数据
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	std::streamsize Read(char* szDataBuffer, std::streamsize llDataSize);
	/*
	*	写数据
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	std::streamsize Write(const char* szDataBuffer, std::streamsize llDataSize);


private:
	//VHD文件Footer结构信息
	HardDiskFooter m_stFooter;
	//VHD文件Footer文本信息
	std::string m_strFooter;

private:
	//VHD文件操作流
	std::fstream m_oFile;
	std::string m_strFile;

};


#endif // !_H_VHD_X_H_
