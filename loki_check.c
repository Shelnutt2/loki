/*
 * loki_check
 *
 * A utility to check if an aboot is supported by loki
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "loki.h"

int main(int argc, char **argv)
{
	int aboot_fd, i;
	unsigned long target, aboot_base;
	void *aboot, *ptr;
	struct target *tgt;
	struct stat st;

	if (argc != 2) {
		printf("Usage: %s [aboot.img] \n", argv[0]);
		return 1;
	}

	/* Open input files */
	aboot_fd = open(argv[1], O_RDONLY);
	if (aboot_fd < 0) {
		printf("[-] Failed to open %s for reading.\n", argv[2]);
		return 1;
	}

	/* Find the signature checking function via pattern matching */
	if (fstat(aboot_fd, &st)) {
		printf("[-] fstat() failed.\n");
		return 1;
	}

	aboot = mmap(0, (st.st_size + 0xfff) & ~0xfff, PROT_READ, MAP_PRIVATE, aboot_fd, 0);
	if (aboot == MAP_FAILED) {
		printf("[-] Failed to mmap aboot.\n");
		return 1;
	}

	target = 0;

	for (ptr = aboot; ptr < aboot + st.st_size - 0x1000; ptr++) {
		if (!memcmp(ptr, PATTERN1, 8) ||
			!memcmp(ptr, PATTERN2, 8) ||
			!memcmp(ptr, PATTERN3, 8)) {

			aboot_base = ABOOT_BASE_SAMSUNG;
			target = (unsigned long)ptr - (unsigned long)aboot + aboot_base;
			break;
		}

		if (!memcmp(ptr, PATTERN4, 8)) {

			aboot_base = ABOOT_BASE_LG;
			target = (unsigned long)ptr - (unsigned long)aboot + aboot_base;
			break;
		}

		if (!memcmp(ptr, PATTERN5, 8)) {

			aboot_base = ABOOT_BASE_G2;
			target = (unsigned long)ptr - (unsigned long)aboot + aboot_base;
			break;
		}
	}

	/* Do a second pass for the second LG pattern. This is necessary because
	 * apparently some LG models have both LG patterns, which throws off the
	 * fingerprinting. */

	if (!target) {
		for (ptr = aboot; ptr < aboot + st.st_size - 0x1000; ptr++) {
			if (!memcmp(ptr, PATTERN6, 8)) {

				aboot_base = ABOOT_BASE_LG;
				target = (unsigned long)ptr - (unsigned long)aboot + aboot_base;
				break;
			}
		}
	}

	if (!target) {
		printf("[-] Failed to find function to patch.\n");
		return 1;
	}

	tgt = NULL;

	for (i = 0; i < (sizeof(targets)/sizeof(targets[0])); i++) {
		if (targets[i].check_sigs == target) {
			tgt = &targets[i];
			break;
		}
	}

	if (!tgt) {
		printf("[-] Unsupported aboot image.\n");
		return 1;
	}
	printf("[+] Detected target %s %s build %s\n", tgt->vendor, tgt->device, tgt->build);
        return 0;
}
