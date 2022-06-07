PROG := logitech_mx_anywhere_2s_set_dpi
UDEV_RULE := 99_logitech_mx_anywhere_2s.rules
UDEV_WRAPPER := logitech_mx_anywhere_2s_set_dpi_udev.sh


build: ${PROG}


install: ${PROG} ${UDEV_RULE} ${UDEV_WRAPPER}
	install -s -m 0755 -t /usr/local/bin ${PROG}
	install -m 0755 -t /usr/local/bin ${UDEV_WRAPPER}
	install -m 0644 -t /etc/udev/rules.d ${UDEV_RULE}

uninstall:
	rm -f /usr/local/bin/${PROG} /usr/local/bin/${UDEV_WRAPPER} /etc/udev/rules.d/${UDEV_RULE}

clean:
	rm -f ${PROG}

.PHONY: install uninstall clean

