
#include "VHDx.h"
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iostream>


/*
*	��VHDx, ����VHDX, �����治��һ������
*	��ǰ�Ƕ�Fixed Hard Disk Image��
*	LBA(Logical Block Addressing)�ķ�ʽ��д����
*/
VHDx::VHDx(const char* szFile) :
	m_stFooter({ 0x00 })
	, m_strFooter("")
{
	if (nullptr != szFile)
	{
		m_strFile.assign(szFile);
	}
}
VHDx::~VHDx()
{
	CloseVHD();
}


/*
*	��VHD�ļ�
*/
int VHDx::OpenVHD(const char* szFile)
{
	if (m_oFile.is_open())
	{
		return 0x00;
	}

	//���ļ�
	if (nullptr != szFile)
	{
		m_strFile.assign(szFile);
	}

	//���ļ�(��������)
	m_oFile.open(m_strFile.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out);
	if (m_oFile.fail())
	{
		//��������
		return 0x00;
	}

	return 0x01;
}
/*
*	�ر�VHD�ļ�
*/
int VHDx::CloseVHD()
{
	//�ر��ļ�
	if (m_oFile.is_open())
	{
		m_oFile.close();
	}

	return 0x01;
}

/*
*	��ȡFooter��Ϣ
*/
long long VHDx::ReadFooterInfo()
{
	//���
	if (!m_oFile.is_open())
	{
		//�ļ�δ��
		return 0x00;
	}

	//��λFooterλ��
	std::streamoff spFooter = sizeof(HardDiskFooter);
	m_oFile.seekg(-spFooter, m_oFile.end);
	//return m_oFile.tellg();
	if (m_oFile.fail())
	{
		//��������
		return 0x00;
	}

	//��������Buffer
	char* szBuffer = new char[sizeof(HardDiskFooter) + 0x01]();

	//��ȡ����Buffer
	std::streamsize llDataSize = Read(szBuffer, sizeof(HardDiskFooter));
	if (sizeof(HardDiskFooter) == llDataSize)
	{
		//�������ݵ�Footer
		memcpy_s(&m_stFooter, sizeof(HardDiskFooter), szBuffer, sizeof(HardDiskFooter));
	}
	else
	{
		memset(&m_stFooter, 0x00, sizeof(HardDiskFooter));
	}

	//�ͷ�����Buffer
	delete[] szBuffer;

	return llDataSize;
}

/*
*	������
*	LBA(Logical Block Addressing)����
*	DataBuffer ����
*	DataSize ���ݴ�С
*/
long long VHDx::Read(unsigned int uiLBA, char* szDataBuffer, long long llDataSize)
{
	//���
	if (nullptr == szDataBuffer)
	{
		return 0x00;
	}
	if (0x00 >= llDataSize)
	{
		return 0x00;
	}

	//
	if (!m_oFile.is_open())
	{
		//�ļ�δ��
		return 0x00;
	}
	if (uiLBA > ((m_stFooter.m_ullCurrentSize / SectorLength) - 0x01))
	{
		//LBA��Ч
		return 0x00;
	}
	
	//Ԥ����ȡ���ȹ���
	llDataSize = ((llDataSize > static_cast<std::streamsize>(m_stFooter.m_ullCurrentSize - uiLBA * SectorLength))
		? (m_stFooter.m_ullCurrentSize - uiLBA * SectorLength)
		: llDataSize);

	//��λLBA����λ��
	std::streamoff spFooter = (uiLBA * SectorLength);
	m_oFile.seekg(spFooter, m_oFile.beg);
	//return m_oFile.tellg();
	if (m_oFile.fail())
	{
		//��������
		return 0x00;
	}

	//��ȡ����
	return Read(szDataBuffer, llDataSize);
}
/*
*	д����
*	LBA(Logical Block Addressing)����
*	DataBuffer ����
*	DataSize ���ݴ�С
*/
long long VHDx::Write(unsigned int uiLBA, const char* szDataBuffer, long long llDataSize)
{
	//���
	if (nullptr == szDataBuffer)
	{
		return 0x00;
	}
	if (0x00 >= llDataSize)
	{
		return 0x00;
	}

	//
	if (!m_oFile.is_open())
	{
		//�ļ�δ��
		return 0x00;
	}
	if (uiLBA > ((m_stFooter.m_ullCurrentSize / SectorLength) - 0x01))
	{
		//LBA��Ч
		return 0x00;
	}

	//Ԥ��д�볤�ȹ���
	llDataSize = ((llDataSize > static_cast<std::streamsize>(m_stFooter.m_ullCurrentSize - uiLBA * SectorLength))
		? (m_stFooter.m_ullCurrentSize - uiLBA * SectorLength)
		: llDataSize);

	//��λLBA����λ��
	std::streamoff spFooter = (uiLBA * SectorLength);
	m_oFile.seekp(spFooter, m_oFile.beg);
	//return m_oFile.tellp();
	if (m_oFile.fail())
	{
		//��������
		return 0x00;
	}

	//��ȡ����
	return Write(szDataBuffer, llDataSize);
}

/*
*	Checksum����
*/
unsigned int VHDx::Checksum()
{
	//���ֵ
	m_stFooter.m_uiChecksum = 0x00;

	//ת��Ϊ�ֽ�
	unsigned char arrHardDiskFooter[sizeof(HardDiskFooter) + 0x01] = { 0x00 };
	memcpy_s(arrHardDiskFooter, sizeof(HardDiskFooter), &m_stFooter, sizeof(HardDiskFooter));
	
	unsigned int uiChecksum = 0x00;
	for (int iCounter = 0; iCounter < sizeof(HardDiskFooter); ++iCounter)
	{
		uiChecksum += arrHardDiskFooter[iCounter];
	}
	uiChecksum = ~uiChecksum;

	//���ֵ
	m_stFooter.m_uiChecksum = S4B(uiChecksum);

	//����ֵ
	return uiChecksum;
}

/*
*	VHD�ļ���Ϣ
*/
const char* VHDx::ToString()
{
	//Footer��Ϣ
	m_strFooter.clear();
	{
		//Cookie
		m_strFooter += "Original Creator: ";
		m_strFooter += std::string(m_stFooter.m_szCookie, sizeof(m_stFooter.m_szCookie));
		m_strFooter += "\r\n";

		//Features
		m_strFooter += "Features: ";
		switch (S4B(m_stFooter.m_uiFeatures))
		{
		case 0x00000000:
			m_strFooter += "No features enabled";
			break;
		case 0x00000001:
			m_strFooter += "Temporary";
			break;
		case 0x00000002:
			m_strFooter += "Reserved";
			break;
		default:
			m_strFooter += std::to_string(S4B(m_stFooter.m_uiFeatures));
			break;
		}
		m_strFooter += "\r\n";

		//File Format Version
		m_strFooter += "File Format Version: ";
		m_strFooter += std::to_string(S2B(m_stFooter.m_stFileFormatVersion.m_usMajor));
		m_strFooter += ".";
		m_strFooter += std::to_string(S2B(m_stFooter.m_stFileFormatVersion.m_usMinor));
		m_strFooter += "\r\n";

		//Data Offset
		m_strFooter += "Data Offset: ";
		m_strFooter += std::to_string((long long)S8B(m_stFooter.m_ullDataOffset));
		m_strFooter += "\r\n";

		//Time Stamp
		m_strFooter += "Creation Time: ";
		{
			std::tm st = { 0x00, 0x00, 0x0C, 0x01, 0x00, 0x64, 0x00, 0x00, 0x00 };
			std::time_t uiSecs = std::mktime(&st) + S4B(m_stFooter.m_uiTimeStamp);
			localtime_s(&st, &uiSecs);
			char szTimeStamp[0x40] = { 0x00 };
			std::strftime(szTimeStamp, sizeof(szTimeStamp), "%c", &st);
			m_strFooter += std::string(szTimeStamp);
		}
		m_strFooter += "\r\n";

		//Creator Application
		m_strFooter += "Creator App: ";
		m_strFooter += std::string(m_stFooter.m_szCreatorApplication, sizeof(m_stFooter.m_szCreatorApplication));
		m_strFooter += "\r\n";

		//Creator Version
		m_strFooter += "Creator Version: ";
		m_strFooter += std::to_string(S2B(m_stFooter.m_stCreatorVersion.m_usMajor));
		m_strFooter += ".";
		m_strFooter += std::to_string(S2B(m_stFooter.m_stCreatorVersion.m_usMinor));
		m_strFooter += "\r\n";

		//Creator Host OS
		m_strFooter += "Creator Host OS: ";
		switch (S4B(m_stFooter.m_uiCreatorHostOS))
		{
		case 0x5769326B:
			m_strFooter += "Wi2k";
			break;
		case 0x4D616320:
			m_strFooter += "Mac";
			break;
		default:
			m_strFooter += std::to_string(S4B(m_stFooter.m_uiCreatorHostOS));
			break;
		}
		m_strFooter += "\r\n";

		//Original Size
		m_strFooter += "Original Size: ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullOriginalSize) / 1024 / 1024 / 1024) + "GB, ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullOriginalSize) / 1024 / 1024) + "MB, ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullOriginalSize) / 1024) + "KB, ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullOriginalSize)) + "Bytes";
		m_strFooter += "\r\n";

		//Current Size
		m_strFooter += "Current Size: ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullCurrentSize) / 1024 / 1024 / 1024) + "GB, ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullCurrentSize) / 1024 / 1024) + "MB, ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullCurrentSize) / 1024) + "KB, ";
		m_strFooter += std::to_string(S8B(m_stFooter.m_ullCurrentSize)) + "Bytes";
		m_strFooter += "\r\n";

		//Disk Geometry
		{
			m_strFooter += "Cylinder: ";
			m_strFooter += std::to_string(S2B(m_stFooter.m_stDiskGeometry.m_usCylinder));
			m_strFooter += "\r\n";
			m_strFooter += "Heads: ";
			m_strFooter += std::to_string(m_stFooter.m_stDiskGeometry.m_ucHeads);
			m_strFooter += "\r\n";
			m_strFooter += "Sectors per track: ";
			m_strFooter += std::to_string(m_stFooter.m_stDiskGeometry.m_ucSectors);
			m_strFooter += "\r\n";
		}
		//m_strFooter += "\r\n";

		//Disk Type
		m_strFooter += "Disk Type: ";
		switch (S4B(m_stFooter.m_uiDiskType))
		{
		case 0x00:
			m_strFooter += "None";
			break;
		case 0x01:
			m_strFooter += "Reserved (deprecated)";
			break;
		case 0x02:
			m_strFooter += "Fixed hard disk";
			break;
		case 0x03:
			m_strFooter += "Dynamic hard disk";
			break;
		case 0x04:
			m_strFooter += "Differencing hard disk";
			break;
		case 0x05:
			m_strFooter += "Reserved (deprecated)";
			break;
		case 0x06:
			m_strFooter += "Reserved (deprecated)";
			break;
		default:
			m_strFooter += std::to_string(S4B(m_stFooter.m_uiDiskType));
			break;
		}
		m_strFooter += "\r\n";

		//Checksum
		m_strFooter += "Checksum: ";
		m_strFooter += std::to_string(S4B(m_stFooter.m_uiChecksum));
		m_strFooter += "\r\n";

		//Unique ID
		m_strFooter += "Unique Identifier: ";
		m_strFooter += UUID(m_stFooter.m_arrUniqueId);
		m_strFooter += "\r\n";

		//Saved State
		m_strFooter += "Saved State: ";
		m_strFooter += std::to_string(m_stFooter.m_ucSavedState);
		m_strFooter += "\r\n";

		//Reserved
		m_strFooter += "Reserved: ";
		m_strFooter += "\r\n";

		//
	}

	//��ʽ����Ϣ
	return m_strFooter.c_str();
}

/*
*	�ͷ���Դ
*/
void VHDx::Dispose()
{
	delete this;
}


/*
*	������
*	DataBuffer ����
*	DataSize ���ݴ�С
*/
std::streamsize VHDx::Read(char* szDataBuffer, std::streamsize llDataSize)
{
	//���
	if (nullptr == szDataBuffer)
	{
		return 0x00;
	}
	if (0x00 >= llDataSize)
	{
		return 0x00;
	}

	//
	if (!m_oFile.is_open())
	{
		//�ļ�δ��
		return 0x00;
	}

	//��ȡ����
	m_oFile.clear();
	std::istream& iStream = m_oFile.read(szDataBuffer, llDataSize);
	if (iStream.fail())
	{
		//��������
		//return 0x00;
	}

	//��ȡ���ݴ�С
	return iStream.gcount();
}
/*
*	д����
*	DataBuffer ����
*	DataSize ���ݴ�С
*/
std::streamsize VHDx::Write(const char* szDataBuffer, std::streamsize llDataSize)
{
	//���
	if (nullptr == szDataBuffer)
	{
		return 0x00;
	}
	if (0x00 >= llDataSize)
	{
		return 0x00;
	}

	//
	if (!m_oFile.is_open())
	{
		//�ļ�δ��
		return 0x00;
	}

	//��ȡ����
	std::ostream& oStream = m_oFile.write(szDataBuffer, llDataSize);
	if (!oStream.good())
	{
		//��������
		return 0x00;
	}
	oStream.flush();

	//д�����ݴ�С
	return llDataSize;
}


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
size_t VHDx::LBA2CHS(DiskGeometry* pDiskGeometry, unsigned int uiLBA)
{
	//���
	if (nullptr == pDiskGeometry)
	{
		return 0x00;
	}

	/*
	*	C = LBA / (HPC * SPT) + CS
	*	H = (LBA / SPT) % HPC + HS
	*	S = LBA % SPT + SS
	*/
#define MOD 0
#if MOD
	//����CHS
	pDiskGeometry->m_usCylinder = uiLBA / (HPC * SPT) + CS;
	pDiskGeometry->m_ucHeads = (uiLBA / SPT) % HPC + HS;
	pDiskGeometry->m_ucSectors = uiLBA % SPT + SS;
#endif /* !MOD */


	//����CHS
	pDiskGeometry->m_usCylinder = uiLBA / (HPC * SPT) + CS;
	pDiskGeometry->m_ucHeads = uiLBA / SPT - (pDiskGeometry->m_usCylinder - CS) * HPC + HS;
	pDiskGeometry->m_ucSectors = uiLBA - (pDiskGeometry->m_usCylinder - CS) * HPC * SPT - (pDiskGeometry->m_ucHeads - HS) * SPT + SS;

	//
	return sizeof(DiskGeometry);
}
/*
*	CHSת��LBA
*	LBA = (C - CS) * HPC * SPT + (H - HS) * SPT + (S - SS)
*	C, H and S are the cylinder number, the head number, and the sector number
*	LBA is the logical block address
*	HPC is the number of heads per cylinder
*	SPT is the number of sectors per track
*/
unsigned int VHDx::CHS2LBA(const DiskGeometry* pDiskGeometry)
{
	//���
	if (nullptr == pDiskGeometry)
	{
		return 0x00;
	}

	//����LBA
	return (pDiskGeometry->m_usCylinder - CS) * HPC * SPT
		+ (pDiskGeometry->m_ucHeads - HS) * SPT
		+ (pDiskGeometry->m_ucSectors - SS);
}

/*
*	CHS��ȡ
*	VirtualSize is the current size of the fixed disk in bytes.
*/
size_t VHDx::CHS(DiskGeometry* pDiskGeometry, unsigned long long ullVirtualSize)
{
	//���
	if (nullptr == pDiskGeometry)
	{
		return 0x00;
	}

	// Total data sectors present in the disk image
	unsigned long long ullTotalSectors = ullVirtualSize / 512;
	// Cylinders * ucHeads
	unsigned long long ullCylinderTimesHeads;
	// Sectors per track on the disk
	unsigned char ucSectorsPerTrack;
	// Number of Heads present on the disk
	unsigned char ucHeads;
	// Cylinder on the disk
	unsigned short usCylinder;

	//                       C  * H  * S
	if (ullTotalSectors > 65535 * 16 * 255)
	{
		ullTotalSectors = 65535 * 16 * 255;
	}

	//
	if (ullTotalSectors >= 65535 * 16 * 63)
	{
		ucSectorsPerTrack = 255;
		ucHeads = 16;
		ullCylinderTimesHeads = ullTotalSectors / ucSectorsPerTrack;
	}
	else
	{
		ucSectorsPerTrack = 17;
		ullCylinderTimesHeads = ullTotalSectors / ucSectorsPerTrack;
		ucHeads = static_cast<unsigned char>((ullCylinderTimesHeads + 1023) / 1024);

		if (ucHeads < 4)
		{
			ucHeads = 4;
		}

		if (ullCylinderTimesHeads >= (ucHeads * 1024) || ucHeads > 16)
		{
			ucSectorsPerTrack = 31;
			ucHeads = 16;
			ullCylinderTimesHeads = ullTotalSectors / ucSectorsPerTrack;
		}

		if (ullCylinderTimesHeads >= (ucHeads * 1024))
		{
			ucSectorsPerTrack = 63;
			ucHeads = 16;
			ullCylinderTimesHeads = ullTotalSectors / ucSectorsPerTrack;
		}
	}
	usCylinder = static_cast<unsigned short>(ullCylinderTimesHeads / ucHeads);

	//��ֵ
	pDiskGeometry->m_ucHeads = ucHeads;
	pDiskGeometry->m_usCylinder = usCylinder;
	pDiskGeometry->m_ucSectors = ucSectorsPerTrack;

	//
	return sizeof(DiskGeometry);
}

/*
*	UUID�ַ���Ϣ
*	UUID ����
*/
const std::string VHDx::UUID(const unsigned char* pUUID)
{
	//�ַ���
	char szUUID[0x40] = { 0x00 };

	//����
	unsigned int   A = ((pUUID[0x03]) << 0x18 | (pUUID[0x02]) << 0x10 | (pUUID[0x01]) << 0x08 | (pUUID[0x00]));
	unsigned short B = ((pUUID[0x05]) << 0x08 | (pUUID[0x04]));
	unsigned short C = ((pUUID[0x07]) << 0x08 | (pUUID[0x06]));
	unsigned char  D = ((pUUID[0x08]));
	unsigned char  E = ((pUUID[0x09]));
	unsigned char  F = ((pUUID[0x0A]));
	unsigned char  G = ((pUUID[0x0B]));
	unsigned char  H = ((pUUID[0x0C]));
	unsigned char  I = ((pUUID[0x0D]));
	unsigned char  J = ((pUUID[0x0E]));
	unsigned char  K = ((pUUID[0x0F]));

	//��ʽ(8-4-4-4-12)
	sprintf_s(szUUID, "{%X-%X-%X-%X%X-%X%X%X%X%X%X}", A, B, C, D, E, F, G, H, I, J, K);

	//UUID�ַ�
	return std::string(szUUID, std::strlen(szUUID));
}

