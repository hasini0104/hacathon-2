#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 256


void monitor_memory() {
    FILE *file;
    char line[MAX];
    long total = 0, free = 0, available = 0;

    file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        printf("Error opening /proc/meminfo\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "MemTotal: %ld kB", &total) == 1);
        if (sscanf(line, "MemFree: %ld kB", &free) == 1);
        if (sscanf(line, "MemAvailable: %ld kB", &available) == 1);
    }

    fclose(file);

    printf("\n===== SYSTEM MEMORY =====\n");
    printf("Total Memory     : %.2f MB\n", total / 1024.0);
    printf("Free Memory      : %.2f MB\n", free / 1024.0);
    printf("Available Memory : %.2f MB\n", available / 1024.0);
}


void track_processes() {
    FILE *fp;
    char path[50], line[256];
    int pid;

    printf("\n===== PROCESS MEMORY (>10MB) =====\n");

    for (pid = 1; pid < 5000; pid++) {
        sprintf(path, "/proc/%d/status", pid);
        fp = fopen(path, "r");

        if (fp != NULL) {
            while (fgets(line, sizeof(line), fp)) {
                if (strncmp(line, "VmRSS:", 6) == 0) {
                    int mem;
                    sscanf(line, "VmRSS: %d kB", &mem);
                    if (mem > 10240) { // >10MB
                        printf("PID: %d | Memory: %.2f MB\n", pid, mem / 1024.0);
                    }
                }
            }
            fclose(fp);
        }
    }
}


void simulate_leak() {
    printf("\nSimulating Memory Leak...\n");

    while (1) {
        int *leak = (int *)malloc(1000000 * sizeof(int)); // allocate memory
        sleep(1);
    }
}


void detect_leak() {
    FILE *file;
    char line[MAX];
    long prev = 0, current;

    while (1) {
        file = fopen("/proc/self/status", "r");
        if (file == NULL) return;

        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                sscanf(line, "VmRSS: %ld kB", &current);
            }
        }

        fclose(file);

        printf("\n===== LEAK DETECTION =====\n");
        printf("Memory Usage: %.2f MB\n", current / 1024.0);

        if (current > prev) {
            printf("⚠️ Possible Memory Leak Detected!\n");
        } else {
            printf("Stable\n");
        }

        prev = current;
        sleep(2);
    }
}

int main() {
    printf("🚀 Memory Usage Profiler & Leak Detector (C - OS Level)\n");

    while (1) {
        monitor_memory();
        track_processes();
        sleep(5);
    }

    return 0;
}