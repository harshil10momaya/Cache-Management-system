#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define CACHE_SIZE 5
#define MAX_REQUESTS 10

typedef struct {
    char filename[256];
    int frequency;
    time_t lastUsed;
} CacheBlock;

CacheBlock cache[CACHE_SIZE];
int cacheHits = 0, cacheMisses = 0;
pthread_mutex_t cacheMutex;

enum Policy { FIFO, LRU, LFU };
enum Policy policy = FIFO;

void clearCache() { memset(cache, 0, sizeof(cache)); }

int searchCache(const char *filename) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (strcmp(cache[i].filename, filename) == 0) {
            cache[i].lastUsed = time(NULL);
            return i;
        }
    }
    return -1;
}

void evictFIFO() { 
    for (int i = 1; i < CACHE_SIZE; i++)
        cache[i - 1] = cache[i];
    memset(&cache[CACHE_SIZE - 1], 0, sizeof(CacheBlock));
}

void evictLRU() {
    int lruIndex = 0;
    time_t oldest = cache[0].lastUsed;

    for (int i = 1; i < CACHE_SIZE; i++) {
        if (cache[i].lastUsed < oldest) {
            oldest = cache[i].lastUsed;
            lruIndex = i;
        }
    }

    for (int i = lruIndex; i < CACHE_SIZE - 1; i++)
        cache[i] = cache[i + 1];
    memset(&cache[CACHE_SIZE - 1], 0, sizeof(CacheBlock));
}

void evictLFU() {
    int minFreq = cache[0].frequency, minIndex = 0;
    for (int i = 1; i < CACHE_SIZE; i++) {
        if (cache[i].frequency < minFreq) {
            minFreq = cache[i].frequency;
            minIndex = i;
        }
    }
    for (int i = minIndex; i < CACHE_SIZE - 1; i++)
        cache[i] = cache[i + 1];
    memset(&cache[CACHE_SIZE - 1], 0, sizeof(CacheBlock));
}

void insertFile(const char* filename) {
    pthread_mutex_lock(&cacheMutex);
    int index = searchCache(filename);
    
    if (index != -1) {
        cacheHits++;
        cache[index].frequency++;
    } else {
        cacheMisses++;
        if (cache[CACHE_SIZE - 1].filename[0] != '\0') {
            if (policy == FIFO) evictFIFO();
            else if (policy == LRU) evictLRU();
            else if (policy == LFU) evictLFU();
        }
        strcpy(cache[CACHE_SIZE - 1].filename, filename);
        cache[CACHE_SIZE - 1].frequency = 1;
        cache[CACHE_SIZE - 1].lastUsed = time(NULL);
    }
    pthread_mutex_unlock(&cacheMutex);
}

void* requestFile(void* arg) {
    char filename[256];
    strcpy(filename, (char*)arg);
    free(arg);

    FILE *file = fopen(filename, "r");
    if (!file) {
        file = fopen(filename, "w");
        fprintf(file, "New file created.\n");
        fclose(file);
    }
    insertFile(filename);
    return NULL;
}

void writeStatsToFile() {
    FILE *fp = fopen("stats.csv", "w");
    fprintf(fp, "Cache Hits,%d\nCache Misses,%d\n", cacheHits, cacheMisses);
    fclose(fp);
}

void startCacheSimulation() {
    printf("Choose Policy: 1.FIFO 2.LRU 3.LFU\nEnter choice: ");
    int choice;
    scanf("%d", &choice);
    policy = (choice == 1) ? FIFO : (choice == 2) ? LRU : LFU;
    
    for (int i = 0; i < MAX_REQUESTS; i++) {
        char filename[256];
        printf("Enter file name: ");
        scanf("%s", filename);
        pthread_t thread;
        char* arg = malloc(strlen(filename) + 1);
        strcpy(arg, filename);
        pthread_create(&thread, NULL, requestFile, arg);
        pthread_join(thread, NULL);
    }

    writeStatsToFile();
    system("./shell.sh");
    clearCache();
}

int main() {
    pthread_mutex_init(&cacheMutex, NULL);
    startCacheSimulation();
    pthread_mutex_destroy(&cacheMutex);
    return 0;
}
