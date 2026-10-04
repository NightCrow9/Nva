#!/bin/sh
set -eu
PORT="${1:-9000}"
IFACE="${2:-br-lan}"

opkg update || true
if [ -f /tmp/nva-udp-core_0.1.0-1_mipsel_24kc.ipk ]; then
  opkg install /tmp/nva-udp-core_0.1.0-1_mipsel_24kc.ipk || opkg install --force-reinstall /tmp/nva-udp-core_0.1.0-1_mipsel_24kc.ipk
fi

cat >/etc/init.d/nva-udp-core <<INIT
#!/bin/sh /etc/rc.common
START=99
STOP=10
USE_PROCD=1
start_service() {
    procd_open_instance
    procd_set_param command /usr/bin/nva-udp-core -b 0.0.0.0 -p ${PORT}
    procd_set_param respawn
    procd_set_param stdout 1
    procd_set_param stderr 1
    procd_close_instance
}
INIT
chmod +x /etc/init.d/nva-udp-core
/etc/init.d/nva-udp-core enable
/etc/init.d/nva-udp-core restart
logread -e nva-udp-core | tail -50 || true
