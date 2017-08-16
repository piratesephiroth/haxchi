#ifndef FSA_H
#define FSA_H

#define IOS_ERROR_UNKNOWN_VALUE     0xFFFFFFD6
#define IOS_ERROR_INVALID_ARG       0xFFFFFFE3
#define IOS_ERROR_INVALID_SIZE      0xFFFFFFE9
#define IOS_ERROR_UNKNOWN           0xFFFFFFF7
#define IOS_ERROR_NOEXISTS          0xFFFFFFFA

#define FLAG_IS_LINK                0x00010000
#define FLAG_IS_UNENCRYPTED         0x00800000
#define FLAG_IS_FILE                0x01000000
#define FLAG_IS_QUOTA               0x60000000
#define FLAG_IS_DIRECTORY           0x80000000

typedef struct
{
    u32 flag;
    u32 permission;
    u32 owner_id;
    u32 group_id;
    u32 size; // size in bytes
    u32 physsize; // physical size on disk in bytes
    u64 quota_size;
    u32 id;
    u64 ctime;
    u64 mtime;
    u8 attributes[48];
} FSStat;

typedef struct
{
    FSStat stat;
	char name[0x100];
} directoryEntry_s;

typedef struct
{
    u8 unknown[0x1E];
} FileSystemInfo;

typedef struct
{
    u8 unknown[0x28];
} DeviceInfo;

typedef struct
{
    u64 blocks_count;
    u64 some_count;
    u32 block_size;
} BlockInfo;

#define FSA_MOUNTFLAGS_BINDMOUNT (1 << 0)
#define FSA_MOUNTFLAGS_GLOBAL (1 << 1)

int FSA_Open();

int FSA_Mount(int fd, char* device_path, char* volume_path, u32 flags, char* arg_string, int arg_string_len);
int FSA_Unmount(int fd, char* path, u32 flags);
int FSA_FlushVolume(int fd, char* volume_path);

int FSA_GetInfo(int fd, char* device_path, int type, u32* out_data);
int FSA_GetStat(int fd, char *path, FSStat* out_data);

int FSA_MakeDir(int fd, char* path, u32 flags);
int FSA_OpenDir(int fd, char* path, int* outHandle);
int FSA_ReadDir(int fd, int handle, directoryEntry_s* out_data);
int FSA_RewindDir(int fd, int handle);
int FSA_CloseDir(int fd, int handle);
int FSA_ChangeDir(int fd, char* path);

int FSA_OpenFile(int fd, char* path, char* mode, int* outHandle);
int FSA_ReadFile(int fd, void* data, u32 size, u32 cnt, int fileHandle, u32 flags);
int FSA_WriteFile(int fd, void* data, u32 size, u32 cnt, int fileHandle, u32 flags);
int FSA_GetStatFile(int fd, int handle, FSStat* out_data);
int FSA_CloseFile(int fd, int fileHandle);
int FSA_SetPosFile(int fd, int fileHandle, u32 position);
int FSA_Remove(int fd, char *path);
int FSA_ChangeMode(int fd, char *path, int mode);
int FSA_ChangeOwner(int fd, char *path, u32 owner, u32 group);

int FSA_RawOpen(int fd, char* device_path, int* outHandle);
int FSA_RawRead(int fd, void* data, u32 size_bytes, u32 cnt, u64 sector_offset, int device_handle);
int FSA_RawWrite(int fd, void* data, u32 size_bytes, u32 cnt, u64 sector_offset, int device_handle);
int FSA_RawClose(int fd, int device_handle);

#endif
