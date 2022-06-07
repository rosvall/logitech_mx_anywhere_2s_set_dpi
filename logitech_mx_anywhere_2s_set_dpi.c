#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdbool.h>

#define REPORT_ID_SHORT 0x10
#define HIDPP_RECEIVER_IDX     0xFF

#define DPI_MIN 10
#define DPI_MAX 100000

struct hidpp20_msg {
	uint8_t report_id;
	uint8_t device_idx;
	uint8_t sub_id;
	uint8_t address;
	uint8_t parameters[3];
} __attribute__((packed));


static bool
is_chr_dev(const char * path)
{
	struct stat st;
	int err = stat(path, &st);
	if (err) {
		perror("stat");
		return false;
	}
	
	return st.st_mode & S_IFCHR;
}

static int
set_dpi(const char * path, int dpi)
{
	int err = 0;

	int fd = open(path, O_SYNC | O_RDWR);
	if (fd <= 0) {
		perror("open");
		return -4;
	}
	
	struct hidpp20_msg msg = {
		.report_id = REPORT_ID_SHORT,
		.device_idx = HIDPP_RECEIVER_IDX,
		.sub_id = 0x0A,
		.address = 0x38,
		.parameters = {
			0x00,
			dpi>>8,
			dpi,
		},
	};
	
	ssize_t count;

	count = write(fd, &msg, sizeof(msg));
	if (count != sizeof(msg)) {
		perror("write");
		err = -5;
		goto cleanup;
	}
	
	count = read(fd, &msg, sizeof(msg));
	if (count != sizeof(msg)) {
		perror("read");
		err = -6;
		goto cleanup;
	}

	uint16_t actual_dpi = (msg.parameters[1]<<8) | msg.parameters[2];
	if (actual_dpi != dpi) {
		fprintf(stderr, "Failed to set DPI\n");
		err = -7;
	}
	
cleanup:
	err = close(fd);
	if (err) 
		perror("close");

	return err;
}

int
main(int argc, const char * argv[])
{
	if (argc != 3) {
		fprintf(stderr, "ERROR: Incorrect number af arguments\n");
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "%s <hid_dev> <dpi>\n", argv[0]);
		return -1;
	}
		
	const char * dev = argv[1];
	if (!is_chr_dev(dev)) {
		fprintf(stderr, "ERROR: Not a character device: %s\n", dev);
		return -2;
	}

	const int dpi = strtoul(argv[2], NULL, 10);
	
	if (dpi < DPI_MIN || dpi > DPI_MAX) {
		fprintf(stderr, "ERROR: DPI must be in range %i ~ %i\n", DPI_MIN, DPI_MAX);
		return -3;
	}
	
	return set_dpi(dev, dpi);
}
