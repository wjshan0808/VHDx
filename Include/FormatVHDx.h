#pragma once
#ifndef _H_FORMAT_VHD_H_
#define _H_FORMAT_VHD_H_



/*
*	大小端转换 2 Bytes
*/
#if !defined(S2B)
#define S2B(v) ((((v) & 0x00FF) << 0x08) | \
				(((v) & 0xFF00) >> 0x08))
#endif /* !S2B */

/*
*	大小端转换 4 Bytes
*/
#if !defined(S4B)
#define S4B(v) ((((v) & 0x000000FF) << 0x18) | \
				(((v) & 0x0000FF00) << 0x08) | \
				(((v) & 0x00FF0000) >> 0x08) | \
				(((v) & 0xFF000000) >> 0x18))
#endif /* !S4B */

/*
*	大小端转换 8 Bytes
*/
#if !defined(S8B)
#define S8B(v) ((((v) & 0x00000000000000FF) << 0x38) | \
				(((v) & 0x000000000000FF00) << 0x28) | \
				(((v) & 0x0000000000FF0000) << 0x18) | \
				(((v) & 0x00000000FF000000) << 0x08) | \
				(((v) & 0x000000FF00000000) >> 0x08) | \
				(((v) & 0x0000FF0000000000) >> 0x18) | \
				(((v) & 0x00FF000000000000) >> 0x28) | \
				(((v) & 0xFF00000000000000) >> 0x38))
#endif /* !S8B */



/*
*	Cylinder起始编号
*/
#if !defined(CS)
#define CS (0x00)
#endif /* !CS */

/*
*	Head起始编号
*/
#if !defined(HS)
#define HS (0x00)
#endif /* !HS */

/*
*	Sector起始编号
*/
#if !defined(SS)
#define SS (0x01)
#endif /* !SS */

/*
*	HPC is the number of heads per cylinder
*/
#if !defined(HPC)
#define HPC (16)	/*[1, 255]*/
#endif /* !HPC */

/*
*	SPT is the number of sectors per track
*/
#if !defined(SPT)
#define SPT (63)	/*[1, 63]*/
#endif /* !SPT */


/*
*	Sector长度
*/
#if !defined(SectorLength)
#define SectorLength (512)
#endif /* !SectorLength */



#pragma pack(push, 0x01)

/*
*	VHD Reference 
*	https://download.microsoft.com/download/f/f/e/ffef50a5-07dd-4cf8-aaa3-442c0673a029/Virtual%20Hard%20Disk%20Format%20Spec_10_18_06.doc
*	https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-vhdx/83e061f8-f6e2-4de1-91bd-5d518a43d477
*/

/*
*	All values in the file format, unless otherwise specified, are stored in network byte 
*	order (big endian). Also, unless otherwise specified, all reserved values should be 
*	set to zero.
*/


/*
*	The basic format of a dynamic hard disk is shown in the following table.
*		Dynamic Disk header fields
*			Copy of hard disk footer (512 bytes)
*			Dynamic Disk Header (1024 bytes)
*				BAT (Block Allocation table) 
*					Data Block 1
*					Data Block 2
*					…
*					Data Block n
*			Hard Disk Footer (512 bytes)
*	The Block Allocation Table (BAT) is a table of absolute sector offsets into the file 
*	backing the hard disk. It is pointed to by the 'Table Offset' field of the Dynamic Disk Header
*	For example, a 2-GB disk image that uses 2-MB blocks 
*	requires 1024 BAT entries. Each entry is four bytes long. All unused table entries 
*	are initialized to 0xFFFFFFFF.
*	The BAT is always extended to a sector boundary. The “Max Table Entries” field 
*	within the Dynamic Disk Header indicates how many entries are valid.
*/



/*
*	major/minor version
*/
typedef struct _stVersion
{
	unsigned short m_usMajor;/*The most-significant two bytes are for the major version*/
	unsigned short m_usMinor;/*The least-significant two bytes are the minor version*/
}Version;


/*
*	store an absolute byte offset in the file where the parent locator
*	for a differencing hard disk is stored. This field is used only for differencing
*	disks and should be set to zero for dynamic disks
*/
typedef struct _stParentLocatorEntry
{
	/*
	*	None (0x0)
	*	Wi2r (0x57693272) [deprecated]
	*	Wi2k (0x5769326B) [deprecated]
	*	W2ru (0x57327275) Unicode pathname (UTF-16) on Windows relative to the differencing disk pathname.
	*	W2ku (0x57326B75) Absolute Unicode (UTF-16) pathname on Windows.
	*	Mac (0x4D616320) (Mac OS alias stored as a blob)
	*	MacX(0x4D616358) A file URL with UTF-8 encoding conforming to RFC 2396.
	*/
	int m_iPlatformCode;/*describes which platform-specific format is used for the file locator*/

	int m_iPlatformDataSpace;/*stores the number of 512-byte sectors needed to store the parent hard disk locator*/
	int m_iPlatformDataLength;/*stores the actual length of the parent hard disk locator in bytes*/
	int m_iReserved;/*This field must be set to zero*/
	long long m_llPlatformDataOffset;/*stores the absolute file offset in bytes where the platform specific file locator data is stored*/

}ParentLocatorEntry;

/*Dynamic Disk Footer Field Descriptions*/
typedef struct _stDynamicDiskHeader
{
	char m_szCookie[0x08];/*holds the value "cxsparse"*/
	/*
	*	 It is currently unused by existing formats and should be set to 0xFFFFFFFF
	*/
	long long m_llDataOffset;/*contains the absolute byte offset to the next structure in the hard disk image*/

	long long m_llTableOffset;/*stores the absolute byte offset of the Block Allocation Table (BAT) in the file*/

	short m_arrHeaderVersion[0x02];/*stores the version of the dynamic disk header*/

	/*
	*	This should be equal to the number of blocks in the disk (that is, the disk size divided by the block size).
	*/
	int m_iMaxTableEntries;/*holds the maximum entries present in the BAT*/
	/*
	*	A block is a unit of expansion for dynamic and differencing hard disks. It is 
	*	stored in bytes. This size does not include the size of the block bitmap
	*/
	int m_iBlockSize;/*the size of the data section of the block*/
	int m_iChecksum;/*holds a basic checksum of the dynamic header*/

	/*
	*	A differencing hard disk stores a 128-bit UUID of the parent hard disk
	*/
	unsigned char m_szParentUniqueId[0x10];/* used for differencing hard disks*/
	int m_iParentTimeStamp;/*stores the modification time stamp of the parent hard disk*/
	int m_iReserved;/*This field should be set to zero*/

	unsigned char m_szParentUnicodeName[0x200];/*contains a Unicode string (UTF-16) of the parent hard disk filename*/

	/*
	*	This field is used only for differencing
	*	disks and should be set to zero for dynamic disks
	*/
	ParentLocatorEntry m_arrParentLocatorEntries[0x08];/*store an absolute byte offset in the file where the parent locator for a differencing hard disk is stored*/

	/* contains zeroes. It is 256 bytes in size*/
	unsigned char m_szReserved[256];/*This must be initialized to zeroes*/

}DynamicDiskHeader;


/*
*	A BAT entry is 64 bits in length and is divided into bit fields
*/
typedef struct _stDiskBATEntry
{
	unsigned long long m_ullA			:0x03;/* Specifies how the associated data block or sector bitmap block is treated*/
	unsigned long long m_ullReserved	:0x11;/* This field MUST be set to 0.*/
	unsigned long long m_ullFileOffsetMB:0x2C;/* Specifies the offset within the file in units of 1 MB*/

}DiskBATEntry;


/*
*	This struct stores the cylinder, heads, and sectors per track value for the hard disk
*/
typedef struct _stDiskGeometry
{
	unsigned short m_usCylinder;/* stores the cylinder value for the hard disk*/
	unsigned char m_ucHeads;/* stores the heads value for the hard disk*/
	unsigned char m_ucSectors;/* stores the sectors per track value for the hard disk*/

}DiskGeometry;


/*Hard Disk Footer Field Descriptions*/
typedef struct _stHardDiskFooter
{
	char m_szCookie[0x08];/*used to uniquely identify the original creator.*/
	/*
	*	No features enabled 0x00000000
	*	Temporary 0x00000001
	*	Reserved 0x00000002
	*/
	unsigned int m_uiFeatures;/*used to indicate specific feature support*/

	/*
	*	This must match the file format specification. 
	*	For the current specification, this field must be initialized to 0x00010000
	*/
	Version m_stFileFormatVersion;

	/*
	*	This field is used for dynamic disks and differencing disks,
	*	For fixed disks, this field should be set to 0xFFFFFFFF
	*/
	unsigned long long m_ullDataOffset;/*holds the absolute byte offset, from the beginning of the file, to the next structure.*/

	unsigned int m_uiTimeStamp;/*stores the creation time of a hard disk image*/
	char m_szCreatorApplication[0x04];/* used to document which application created the hard disk*/
	Version m_stCreatorVersion;/* holds the major/minor version of the application that created the hard disk image*/

	/*
	*	Windows 0x5769326B (Wi2k)
	*	Macintosh 0x4D616320 (Mac)
	*/
	unsigned int m_uiCreatorHostOS;/*stores the type of host operating system this disk image is created on*/
	unsigned long long m_ullOriginalSize;/* stores the size of the hard disk in bytes, from the perspective of the virtual machine, at creation time*/
	unsigned long long m_ullCurrentSize;/*stores the current size of the hard disk, in bytes, from the perspective of the virtual machine*/

	/*
	*	This field stores the cylinder, heads, and sectors per track value for the hard disk
	*/
	DiskGeometry m_stDiskGeometry;

	/*
	*	None 0
	*	Reserved (deprecated) 1
	*	Fixed hard disk 2
	*	Dynamic hard disk 3
	*	Differencing hard disk 4
	*	Reserved (deprecated) 5
	*	Reserved (deprecated) 6
	*/
	unsigned int m_uiDiskType;
	unsigned int m_uiChecksum;/*holds a basic checksum of the hard disk footer*/

	/*
	*	used to associate a parent hard disk image with its differencing hard disk image(s).
	*/
	unsigned char m_arrUniqueId[0x10];/*used to identify the hard disk*/

	/* If the hard disk is in the saved state the value is set to 1 */
	unsigned char m_ucSavedState;/*holds a one-byte flag that describes whether the system is in saved state*/

	/* contains zeroes. It is 427 bytes in size*/
	unsigned char m_arrReserved[427];/**/

}HardDiskFooter;



#pragma pack(pop)

#endif // !_H_FORMAT_VHD_H_
