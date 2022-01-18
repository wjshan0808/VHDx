// VHDxTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "IVHDx.h"
#include "FormatVHDx.h"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <iomanip>
#include <cwchar>


int main()
{

	char sz[1000] = {  };

	sprintf_s(sz, "S2B = %x\n", S2B(0x1234));
	std::cout << sz << std::endl;

	sprintf_s(sz, "S4B = %x\n", S4B(0x12345678));
	std::cout << sz << std::endl;

	char szA[128] = { 0 };
	sprintf_s(szA, "%llx", (0x1234567890abcdef));
	std::string strA(szA);
	sprintf_s(sz, "S8B = %s\n", std::string(strA.begin(), strA.end()).c_str());
	std::cout << sz << std::endl;

	char szB[128] = { 0 };
	sprintf_s(szB, "%llx", S8B(0x1234567890abcdef));
	std::string strB(szB);
	sprintf_s(sz, "S8B = %s\n", std::string(strB.begin(), strB.end()).c_str());
	std::cout << sz << std::endl;


	/*
		sizeof(time_t) = 8
		sizeof(char) = 1
		sizeof(unsigned char) = 1
		sizeof(short) = 2
		sizeof(unsigned short) = 2
		sizeof(int) = 4
		sizeof(unsigned int) = 4
		sizeof(long) = 4
		sizeof(unsigned long) = 4
		sizeof(long long) = 8
		sizeof(unsigned long long) = 8
	*/

	sprintf_s(sz, "sizeof(stHardDiskFooter) = %llu\n", sizeof(HardDiskFooter));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(stParentLocatorEntry) = %llu\n", sizeof(ParentLocatorEntry));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(stDynamicDiskHeader) = %llu\n", sizeof(DynamicDiskHeader));
	std::cout << sz << std::endl;

	sprintf_s(sz, "sizeof(char) = %llu\n", sizeof(char));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(unsigned char) = %llu\n", sizeof(unsigned char));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(short) = %llu\n", sizeof(short));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(unsigned short) = %llu\n", sizeof(unsigned short));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(int) = %llu\n", sizeof(int));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(unsigned int) = %llu\n", sizeof(unsigned int));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(long) = %llu\n", sizeof(long));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(unsigned long) = %llu\n", sizeof(unsigned long));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(long long) = %llu\n", sizeof(long long));
	std::cout << sz << std::endl;
	sprintf_s(sz, "sizeof(unsigned long long) = %llu\n", sizeof(unsigned long long));
	std::cout << sz << std::endl;



	//测试
	IVHDx* pVHDx = NewVHDx();
	pVHDx->OpenVHD("C:\\Users\\86182\\Desktop\\VHDx\\Data\\LEECHUNG.vhd");
	pVHDx->ReadFooterInfo();
	//VHD汇总信息
	sprintf_s(sz, "%s", pVHDx->ToString());
	std::cout << sz << std::endl;
	//写数据验证
	size_t llDataSize = pVHDx->Write(0x08, sz, std::strlen(sz));
	std::cout << llDataSize << " / " << std::strlen(sz) << std::endl;
	//读数据验证
	memset(sz, 0x00, sizeof(sz));
	pVHDx->Read(0x08, sz, llDataSize);
	std::cout << sz << std::endl;
	//校验值验证
	unsigned int uiChecksum = pVHDx->Checksum();
	sprintf_s(sz, "Re-Checksum = %u\n", (uiChecksum));
	std::cout << sz << std::endl;
	//释放资源
	pVHDx->Dispose();

	/*
	//静态测试
	{
		//CHS验证
		DiskGeometry stDiskGeometry;
		VHDx::CHS(&stDiskGeometry, 104857600);
		sprintf_s(sz, "Cylinder = %u\nHeads = %d\nSectors per track = %d\n"
			, stDiskGeometry.m_usCylinder, stDiskGeometry.m_ucHeads, stDiskGeometry.m_ucSectors);
		std::cout << sz << std::endl;
		//获取LBA
		unsigned int uiLBA = VHDx::CHS2LBA(&stDiskGeometry);
		sprintf_s(sz, "LBA = %u\n", uiLBA);
		std::cout << sz << std::endl;
		//获取CHS
		memset(&stDiskGeometry, 0x00, sizeof(stDiskGeometry));
		VHDx::LBA2CHS(&stDiskGeometry, uiLBA);
		sprintf_s(sz, "C = %u\nH = %d\nS = %d\n"
			, stDiskGeometry.m_usCylinder, stDiskGeometry.m_ucHeads, stDiskGeometry.m_ucSectors);
		std::cout << sz << std::endl;
	}*/

	std::cout << "Hello World !!!" << std::endl;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
