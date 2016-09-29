#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <net/if.h>

#include <linux/mii.h>
#include <linux/sockios.h>

struct ifreq ifr;
int switch_getphy(char *ifname)
{
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    //if (ioctl(fd, SIOCGMIIPHY, &ifr) < 0) {
//	return 1;
  //  }
    return 0;
}
int switch_read(int fd, int location,int port, unsigned short *value)
{
    int result = 0;
    struct mii_ioctl_data *mii = (struct mii_ioctl_data *)&ifr.ifr_data;
    mii->phy_id = port;
    mii->reg_num = location;
    if ((result = ioctl(fd, SIOCGMIIREG, &ifr)) < 0) {
	*value = 0;
    } else {
        *value = mii->val_out;
    }
    return result;
}

int switch_write(int fd, int location,int port, unsigned short value)
{
    int result = 0;
    struct mii_ioctl_data *mii = (struct mii_ioctl_data *)&ifr.ifr_data;
    mii->phy_id = port;
    mii->reg_num = location;
    mii->val_in = value;
    if ((result = ioctl(fd, SIOCSMIIREG, &ifr)) < 0) {
    //    *value = 0;

    }
    return result;
}

