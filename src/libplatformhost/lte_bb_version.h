#define ACTIVE_VERSION_FILENAME    "/etc/version.txt"
#define PASSIVE_VERSION_FILENAME   "/mnt/filesystem/etc/version.txt"
#define VERSION_SIZE    20
#define ACTIVE_SIDE     1
#define PASSIVE_SIDE    0
#define MOUNT_JFFS2_SIDEB       "mount -t jffs2 /dev/mtdblock5 /mnt/filesystem"
#define MOUNT_JFFS2_SIDEA       "mount -t jffs2 /dev/mtdblock3 /mnt/filesystem"
#define UMOUNT_JFFS2_SIDEB      "umount -t jffs2 /dev/mtdblock5"
#define UMOUNT_JFFS2_SIDEA      "umount -t jffs2 /dev/mtdblock3"

typedef enum e_Version
{
   VERSION_ERR_INVALID_INPUT=70,
   VERSION_ERR_FILE_OPEN,
   VERSION_SIDE_SUCCESS
}Version;

extern Version imageVersion  (UINT8 *ver, UINT32 side);
