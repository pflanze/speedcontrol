/*
 * SpeedControl - use SET STREAMING command to set the speed of DVD-drives
 *
 *
 * Copyright (c) 2004	Thomas Fritzsche <tf@noto.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*
 * Changes by Sami Farin, Christian Jaeger
 * (see history at https://github.com/pflanze/dvdspeedcontrol)
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/cdrom.h>

void dump_sense(unsigned char *cdb, struct request_sense *sense)
{
  int i;

  printf("Command failed: ");

  for (i=0; i<12; i++)
    printf("%02x ", cdb[i]);

    if (sense) {
      printf(" - sense: %02x.%02x.%02x\n",
              sense->sense_key, sense->asc, sense->ascq);
    } else {
      printf(", no sense\n");
    }
}

int main(int argc, char *argv[])
{
  char *device = "/dev/cdrom";
  int c, fd;
  int speed = 0;
  unsigned long rw_size;
  unsigned char buffer[28];
  struct cdrom_generic_command cgc;
  struct request_sense sense;
  extern char * optarg;

  while((c = getopt(argc,argv,"x:")) != EOF) {
    switch(c) {
      case 'x': speed = atoi(optarg); break;
      default:
        printf("Usage: %s [-x speed] [device]\n", argv[0]);
        return 1;
    }
  }

  if (argc > optind) device = argv[optind];

  fd = open(device, O_RDWR | O_NONBLOCK);
  if (fd < 0) {
    printf("%s: Can't open device %s: %s\n", argv[0], device, strerror(errno));
    return 1;
  }

  memset(&cgc, 0, sizeof(cgc));
  memset(&sense, 0, sizeof(sense));
  memset(&buffer, 0, sizeof(buffer));

  /* SET STREAMING command */
  cgc.cmd[0] = 0xb6;
  /* 28 byte parameter list length */
  cgc.cmd[10] = 28;

  cgc.sense = &sense;
  cgc.buffer = buffer;
  cgc.buflen = sizeof(buffer);
  cgc.data_direction = CGC_DATA_WRITE;
  cgc.quiet = 1;

  if(speed == 0) {
    /* set Restore Drive Defaults */
    buffer[0] = 4;
  }

  buffer[8] = 0xff;
  buffer[9] = 0xff;
  buffer[10] = 0xff;
  buffer[11] = 0xff;

  rw_size = 177 * speed;

  /* read size */
  buffer[12] = (rw_size >> 24);
  buffer[13] = (rw_size >> 16);
  buffer[14] = (rw_size >>  8);
  buffer[15] = rw_size;

  /* read time 1 sec. */
  buffer[18] = 0x03;
  buffer[19] = 0xE8;

  /* write size */
  buffer[20] = (rw_size >> 24);
  buffer[21] = (rw_size >> 16);
  buffer[22] = (rw_size >>  8);
  buffer[23] = rw_size;

  /* write time 1 sec. */
  buffer[26] = 0x03;
  buffer[27] = 0xE8;

  if (ioctl(fd, CDROM_SEND_PACKET, &cgc) != 0)
    if (ioctl(fd, CDROM_SELECT_SPEED, speed) != 0) {
      dump_sense(cgc.cmd, cgc.sense);
      printf("ERROR.\n");
      return 1;
    }

  printf("OK...\n");
  return 0;
}

