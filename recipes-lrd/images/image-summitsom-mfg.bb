DESCRIPTION = "Summit SOM Radio Manufacturing Image"
LICENSE = "MIT"

inherit core-image extrausers

EXTRA_USERS_PARAMS = "usermod -P summit root;"

export IMAGE_BASENAME = "${PN}"

ROOTFS_POSTPROCESS_COMMAND += "clean_boot_dir;"

clean_boot_dir() {
    rm -rf ${IMAGE_ROOTFS}/boot/*
}

IMAGE_FEATURES = "\
	ssh-server-dropbear \
	allow-root-login \
	"

IMAGE_INSTALL += "\
	kernel-module-pac193x \
	packagegroup-radio-stack \
	ca-certificates \
	tzdata \
	ethtool \
	iproute2 \
	chrony \
	chronyc \
	packagegroup-fsl-tools-audio \
	lrd-automount \
	lrd-initdata \
	lrd-usbgadget \
	u-boot-fw-utils \
	swupdate \
	mpg123 \
	read-edid \
	fbida \
	"

# Diagnostic tools
IMAGE_INSTALL += "\
	hub-ctrl \
	iperf2 \
	iperf3 \
	htop \
	tcpdump \
	usbutils \
	pciutils \
	picocom \
	can-utils \
	stress-ng \
	echotest \
	"

# Radio manufacturing components
IMAGE_INSTALL += "\
	packagegroup-radio-stack-mfg \
	packagegroup-radio-stack-mfgbridge-8997 \
	mfg60n-lrt-som \
	sterling60-firmware-som \
	"