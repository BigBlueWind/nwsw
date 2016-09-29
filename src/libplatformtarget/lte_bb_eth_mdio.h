#ifndef LTE_BB_ETH_MDIO_H
#define LTE_BB_ETH_MDIO_H

extern int do_getphy(int fd, char *ifname);
extern int mdio_read(int fd, int location, unsigned short *value);
extern int switch_getphy(char *ifname);
extern int switch_read(int fd, int location,int port, unsigned short *value);
extern int switch_write(int fd, int location,int port, unsigned short value);
#endif
