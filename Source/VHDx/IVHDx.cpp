#include "IVHDx.h"
#include "VHDx.h"


/*
*	��VHD�ļ�
*/
VHDX_API int OpenVHD(IVHDx* pVHDx, const char* szFile)
{
	//���
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//����
	return pVHDx->OpenVHD(szFile);
}
/*
*	�ر�VHD�ļ�
*/
VHDX_API int CloseVHD(IVHDx* pVHDx)
{
	//���
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//����
	return pVHDx->CloseVHD();
}

/*
*	��ȡFooter��Ϣ
*/
VHDX_API long long ReadFooterInfo(IVHDx* pVHDx)
{
	//���
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//����
	return pVHDx->ReadFooterInfo();
}

/*
*	������
*	LBA(Logical Block Addressing)����
*	DataBuffer ����
*	DataSize ���ݴ�С
*/
VHDX_API long long Read(IVHDx* pVHDx, unsigned int uiLBA, char* szDataBuffer, long long llDataSize)
{
	//���
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//����
	return pVHDx->Read(uiLBA, szDataBuffer, llDataSize);
}
/*
*	д����
*	LBA(Logical Block Addressing)����
*	DataBuffer ����
*	DataSize ���ݴ�С
*/
VHDX_API long long Write(IVHDx* pVHDx, unsigned int uiLBA, const char* szDataBuffer, long long llDataSize)
{
	//���
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//����
	return pVHDx->Write(uiLBA, szDataBuffer, llDataSize);
}

/*
*	Checksum����
*/
VHDX_API unsigned int Checksum(IVHDx* pVHDx)
{
	//���
	if (nullptr == pVHDx)
	{
		return 0x00;
	}

	//����
	return pVHDx->Checksum();
}

/*
*	VHD�ļ���Ϣ
*/
VHDX_API const char* ToString(IVHDx* pVHDx)
{
	//���
	if (nullptr == pVHDx)
	{
		return nullptr;
	}

	//����
	return pVHDx->ToString();
}

/*
*	�ͷ���Դ
*/
VHDX_API void Dispose(IVHDx* pVHDx)
{
	//���
	if (nullptr == pVHDx)
	{
		return;
	}

	//����
	pVHDx->Dispose();
}


/*
*	����VHDx����
*/
VHDX_API IVHDx* NewVHDx(const char* szFile)
{
	return new VHDx(szFile);
}
