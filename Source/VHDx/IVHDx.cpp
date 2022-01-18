#include "IVHDx.h"
#include "VHDx.h"


/*
*	打开VHD文件
*/
VHDX_API int OpenVHD(IVHDx* pVHDx, const char* szFile)
{
	//检查
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//调用
	return pVHDx->OpenVHD(szFile);
}
/*
*	关闭VHD文件
*/
VHDX_API int CloseVHD(IVHDx* pVHDx)
{
	//检查
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//调用
	return pVHDx->CloseVHD();
}

/*
*	读取Footer信息
*/
VHDX_API long long ReadFooterInfo(IVHDx* pVHDx)
{
	//检查
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//调用
	return pVHDx->ReadFooterInfo();
}

/*
*	读数据
*	LBA(Logical Block Addressing)索引
*	DataBuffer 数据
*	DataSize 数据大小
*/
VHDX_API long long Read(IVHDx* pVHDx, unsigned int uiLBA, char* szDataBuffer, long long llDataSize)
{
	//检查
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//调用
	return pVHDx->Read(uiLBA, szDataBuffer, llDataSize);
}
/*
*	写数据
*	LBA(Logical Block Addressing)索引
*	DataBuffer 数据
*	DataSize 数据大小
*/
VHDX_API long long Write(IVHDx* pVHDx, unsigned int uiLBA, const char* szDataBuffer, long long llDataSize)
{
	//检查
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//调用
	return pVHDx->Write(uiLBA, szDataBuffer, llDataSize);
}

/*
*	Checksum计算
*/
VHDX_API unsigned int Checksum(IVHDx* pVHDx)
{
	//检查
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//调用
	return pVHDx->Checksum();
}

/*
*	VHD文件信息
*/
VHDX_API const char* ToString(IVHDx* pVHDx)
{
	//检查
	if (nullptr == pVHDx)
	{
		return nullptr;
	}

	//调用
	return pVHDx->ToString();
}

/*
*	释放资源
*/
VHDX_API void Dispose(IVHDx* pVHDx)
{
	//检查
	if (nullptr == pVHDx)
	{
		return;
	}

	//调用
	pVHDx->Dispose();
}


/*
*	创建VHDx对象
*/
VHDX_API IVHDx* NewVHDx(const char* szFile)
{
	return new VHDx(szFile);
}
