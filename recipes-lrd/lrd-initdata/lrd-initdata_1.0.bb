SUMMARY = "Laird Connectivity Init Configurations"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit allarch systemd

SRC_URI = " \
    file://file-cache.conf \
    file://modem \
    file://set-radio-mode \
    file://lrd.conf \
    file://lrdmwl.conf \
    file://lrd-profile.sh \
    file://overlayRoot.sh \
    file://fw_update \
    file://var-volatile-log-journal.mount \
    file://perm-mount \
    file://perm-mount.service \
    "

S = "${WORKDIR}"

FILES_${PN} += "${systemd_unitdir}/system ${sysconfdir} /perm /data"

SYSTEMD_SERVICE_${PN} = "perm-mount.service"
SYSTEMD_AUTO_ENABLE = "enable"

SYSTEMD_SERVICE_${PN}_append_lrdsecure = " var-volatile-log-journal.mount"

do_install () {
    install -D -m 0644 ${S}/file-cache.conf ${D}${sysconfdir}/sysctl.d/file-cache.conf
    install -D -m 0644 ${S}/lrd.conf ${D}${sysconfdir}/tmpfiles.d/lrd.conf
    install -D -m 0755 ${S}/modem ${D}${bindir}/modem
    install -D -m 0755 ${S}/set-radio-mode ${D}${bindir}/set-radio-mode
    install -D -m 0644 ${S}/lrdmwl.conf ${D}${sysconfdir}/modprobe.d/lrdmwl.conf
    install -D -m 0755 ${S}/lrd-profile.sh ${D}${sysconfdir}/profile.d/lrd-profile.sh
    install -D -m 0755 ${S}/overlayRoot.sh ${D}${sbindir}/overlayRoot.sh
    install -D -m 0755 ${S}/fw_update ${D}${sbindir}/fw_update
    install -d ${D}/perm
    install -D -m 0644 ${S}/perm-mount.service ${D}${systemd_unitdir}/system/perm-mount.service
    install -D -m 0755 ${S}/perm-mount ${D}${bindir}/perm-mount
}

do_install_append_lrdsecure () {
    install -d ${D}/data
    install -D -m 0644 ${S}/var-volatile-log-journal.mount ${D}${systemd_unitdir}/system/var-volatile-log-journal.mount
    rm -f ${D}${sbindir}/overlayRoot.sh
}