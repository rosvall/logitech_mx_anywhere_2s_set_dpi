#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdbool.h>

typedef struct __attribute__((packed)) {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint16_t dpi;
} dpi_msg_t;

const int dpi_min = 10;
const int dpi_max = 100000;

static void
print_usage(const char * progname)
{
	fprintf(stderr, "Usage:\n" "%s HID_DEV DPI\n", progname);
}

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
	if (!is_chr_dev(path)) {
		fprintf(stderr, "ERROR: Not a character device: %s\n", path);
		return -1;
	}

	int err = 0;

	int fd = open(path, O_SYNC | O_RDWR);
	if (fd <= 0) {
		perror("open");
		return -1;
	}
	
	dpi_msg_t msg = {
		.a = 0x10,
		.b = 0xff,
		.c = 0x0a,
		.d = 0x38,
		.e = 0x00,
		.dpi = htons(dpi),
	};
	
	ssize_t written = write(fd, (void *)&msg, sizeof(msg));
	if (written != sizeof(msg)) {
		perror("write");
		err = 1;
		goto cleanup;
	}
	
	dpi_msg_t response = {0};
	ssize_t bytes_read = read(fd, &response, sizeof(response));
	if (bytes_read != sizeof(response)) {
		perror("read");
		err = 1;
	} else {
		uint16_t actual_dpi = ntohs(response.dpi);
		if (actual_dpi != dpi) {
			fprintf(stderr, "Failed to set DPI\n");
			err = 1;
		}
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
		print_usage(argv[0]);
		return -1;
	}
		
	const char * dev = argv[1];
	const int dpi = strtoul(argv[2], NULL, 10);
	
	if (dpi < dpi_min || dpi > dpi_max) {
		fprintf(stderr, "ERROR: DPI must be in range %i ~ %i\n", dpi_min, dpi_max);
		print_usage(argv[0]);
		return -1;
	}
	
	return set_dpi(dev, dpi);
}
