#pragma once
#ifndef _H_VHD_X_H_
#define _H_VHD_X_H_

#include "FormatVHDx.h"
#include "IVHDx.h"
#include <fstream>
#include <string>


/*
*	��VHDx, ����VHDX, �����治��һ������
*	��ǰ�Ƕ�Fixed Hard Disk Image��
*	LBA(Logical Block Addressing)�ķ�ʽ��д����
*/
//class IVHDx;
class VHDx : public IVHDx
{
public:
	VHDx(const char* szFile);
	~VHDx();

public:
	/*
	*	��VHD�ļ�
	*/
	int OpenVHD(const char* szFile = nullptr);
	/*
	*	�ر�VHD�ļ�
	*/
	int CloseVHD();

	/*
	*	��ȡFooter��Ϣ
	*/
	long long ReadFooterInfo();

	/*
	*	������
	*	LBA(Logical Block Addressing)����
	*	DataBuffer ����
	*	DataSize ���ݴ�С
	*/
	long long Read(unsigned int uiLBA, char* szDataBuffer, long long llDataSize);
	/*
	*	д����
	*	LBA(Logical Block Addressing)����
	*	DataBuffer ����
	*	DataSize ���ݴ�С
	*/
	long long Write(unsigned int uiLBA, const char* szDataBuffer, long long llDataSize);

	/*
	*	Checksum����
	*/
	unsigned int Checksum();

	/*
	*	VHD�ļ���Ϣ
	*/
	const char* ToString();

	/*
	*	�ͷ���Դ
	*/
	void Dispose();


public:
	/*
	*	LBAת��CHS
	*	LBA(Logical Block Addressing)����
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
	*	CHSת��LBA
	*	LBA = (C - CS) * HPC * SPT + (H - HS) * SPT + (S - SS)
	*	C, H and S are the cylinder number, the head number, and the sector number
	*	LBA is the logical block address
	*	HPC is the number of heads per cylinder
	*	SPT is the number of sectors per track
	*/
	static unsigned int CHS2LBA(const DiskGeometry* pDiskGeometry);

	/*
	*	CHS��ȡ
	*	VirtualSize is the current size of the fixed disk in bytes.
	*/
	static size_t CHS(DiskGeometry* pDiskGeometry, unsigned long long ullVirtualSize);

	/*
	*	UUID�ַ���Ϣ
	*	UUID ����
	*/
	static const std::string UUID(const unsigned char* pUUID);


private:
	/*
	*	������
	*	DataBuffer ����
	*	DataSize ���ݴ�С
	*/
	std::streamsize Read(char* szDataBuffer, std::streamsize llDataSize);
	/*
	*	д����
	*	DataBuffer ����
	*	DataSize ���ݴ�С
	*/
	std::streamsize Write(const char* szDataBuffer, std::streamsize llDataSize);


private:
	//VHD�ļ�Footer�ṹ��Ϣ
	HardDiskFooter m_stFooter;
	//VHD�ļ�Footer�ı���Ϣ
	std::string m_strFooter;

private:
	//VHD�ļ�������
	std::fstream m_oFile;
	std::string m_strFile;

};


#endif // !_H_VHD_X_H_
