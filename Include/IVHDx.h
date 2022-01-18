#pragma once
#ifndef _H_DEFINE_VHD_H_
#define _H_DEFINE_VHD_H_


/*
*	导入导出符号
*/
#ifdef VHDX_EXPORTS

#define VHDX_API __declspec(dllexport)

#else

#define VHDX_API __declspec(dllimport)

#endif


/*
*	C/C++
*/
#ifdef __cplusplus

#define EXTERN_C extern "C"

#else

#define EXTERN_C

#endif // __cplusplus




/*
*	C/C++
*/
#ifdef __cplusplus

/*
*	IVHDx
*/
struct IVHDx
{
	/*
	*	打开VHD文件
	*/
	virtual int OpenVHD(const char* szFile = nullptr) = 0;
	/*
	*	关闭VHD文件
	*/
	virtual int CloseVHD() = 0;

	/*
	*	读取Footer信息
	*/
	virtual long long ReadFooterInfo() = 0;

	/*
	*	读数据
	*	LBA(Logical Block Addressing)索引
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	virtual long long Read(unsigned int uiLBA, char* szDataBuffer, long long llDataSize) = 0;
	/*
	*	写数据
	*	LBA(Logical Block Addressing)索引
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	virtual long long Write(unsigned int uiLBA, const char* szDataBuffer, long long llDataSize) = 0;

	/*
	*	Checksum计算
	*/
	virtual unsigned int Checksum() = 0;

	/*
	*	VHD文件信息
	*/
	virtual const char* ToString() = 0;


	/*
	*	释放资源
	*/
	virtual void Dispose() = 0;
};

#else

/*
*	IVHDx
*/
typedef	struct _stIVHDx
{

}IVHDx;

#endif // __cplusplus


/*
*	extern "C" IVHD
*/
EXTERN_C
{
	/*
	*	打开VHD文件
	*/
	VHDX_API int OpenVHD(IVHDx* pVHDx, const char* szFile = nullptr);
	/*
	*	关闭VHD文件
	*/
	VHDX_API int CloseVHD(IVHDx* pVHDx);

	/*
	*	读取Footer信息
	*/
	VHDX_API long long ReadFooterInfo(IVHDx* pVHDx);

	/*
	*	读数据
	*	LBA(Logical Block Addressing)索引
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	VHDX_API long long Read(IVHDx* pVHDx, unsigned int uiLBA, char* szDataBuffer, long long llDataSize);
	/*
	*	写数据
	*	LBA(Logical Block Addressing)索引
	*	DataBuffer 数据
	*	DataSize 数据大小
	*/
	VHDX_API long long Write(IVHDx* pVHDx, unsigned int uiLBA, const char* szDataBuffer, long long llDataSize);

	/*
	*	Checksum计算
	*/
	VHDX_API unsigned int Checksum(IVHDx* pVHDx);

	/*
	*	VHD文件信息
	*/
	VHDX_API const char* ToString(IVHDx* pVHDx);

	/*
	*	释放资源
	*/
	VHDX_API void Dispose(IVHDx* pVHDx);
}


/*
*	extern "C" VHD
*/
EXTERN_C
{
	/*
	*	创建VHDx对象
	*/
	VHDX_API IVHDx* NewVHDx(const char* szFile = nullptr);
}

#endif // !_H_DEFINE_VHD_H_
