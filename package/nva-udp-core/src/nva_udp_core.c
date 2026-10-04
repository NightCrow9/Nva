#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#ifndef NVA_VERSION
#define NVA_VERSION "0.1.0"
#endif

static volatile int g_running = 1;

static void on_signal(int sig) {
    (void)sig;
    g_running = 0;
}

static void hexdump_preview(const uint8_t *buf, size_t len) {
    size_t n = len < 32 ? len : 32;
    for (size_t i = 0; i < n; i++) {
        printf("%02x", buf[i]);
        if (i + 1 < n) putchar(' ');
    }
    if (len > n) printf(" ...");
}

static int parse_packet(const uint8_t *buf, size_t len) {
    /*
     * 这里保留为本地协议解析入口。
     * 后续把 Windows 工程中纯数据结构/离线样例解析逻辑迁移到这里，
     * 不依赖 WinDivert、GUI、Windows API、.NET runtime。
     */
    if (len == 0) return 0;
    return 0;
}

int main(int argc, char **argv) {
    int port = 9000;
    const char *bind_ip = "0.0.0.0";

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-p") && i + 1 < argc) {
            port = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-b") && i + 1 < argc) {
            bind_ip = argv[++i];
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            printf("nva-udp-core %s\nUsage: %s [-b bind_ip] [-p port]\n", NVA_VERSION, argv[0]);
            return 0;
        }
    }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    if (inet_pton(AF_INET, bind_ip, &addr.sin_addr) != 1) {
        fprintf(stderr, "invalid bind ip: %s\n", bind_ip);
        close(fd);
        return 1;
    }

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return 1;
    }

    printf("nva-udp-core %s listening on %s:%d\n", NVA_VERSION, bind_ip, port);
    fflush(stdout);

    uint8_t buf[2048];
    while (g_running) {
        struct sockaddr_in peer;
        socklen_t peer_len = sizeof(peer);
        ssize_t n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&peer, &peer_len);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("recvfrom");
            break;
        }

        char ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &peer.sin_addr, ip, sizeof(ip));

        time_t now = time(NULL);
        struct tm tmv;
        localtime_r(&now, &tmv);
        char ts[32];
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tmv);

        parse_packet(buf, (size_t)n);

        printf("[%s] udp %s:%u len=%zd preview=", ts, ip, ntohs(peer.sin_port), n);
        hexdump_preview(buf, (size_t)n);
        putchar('\n');
        fflush(stdout);
    }

    close(fd);
    puts("stopped");
    return 0;
}
