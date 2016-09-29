#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <time.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/mii.h>
#include <linux/sockios.h>

struct ifreq ifr;
int do_getphy(int fd, char *ifname)
{
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(fd, SIOCGMIIPHY, &ifr) < 0) {
	return 1;
    }
    return 0;
}

int mdio_read(int fd, int location, unsigned short *value)
{
    int result = 0;
    struct mii_ioctl_data *mii = (struct mii_ioctl_data *)&ifr.ifr_data;
    mii->reg_num = location;
    if ((result = ioctl(fd, SIOCGMIIREG, &ifr)) < 0) {
	*value = 0;
    } else {
        *value = mii->val_out;
    }
    return result;
}

