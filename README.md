CACHE MANAGEMENT SYSTEM
This project implements a Cache Management System in C, integrating multithreading, semaphores, and shell scripting for automation and visualization. The system efficiently manages file access using cache replacement policies to optimize memory utilization and performance.
Key Features:
Multithreaded Implementation – Each file access request runs in a separate thread to simulate real-world parallel processing.
Semaphore Synchronization – Ensures safe concurrent access to cache, preventing data inconsistency.
Dynamic Cache Management – Supports three cache replacement policies:
FIFO (First In First Out) – Removes the oldest entry in the cache.
LRU (Least Recently Used) – Removes the least recently accessed entry.
LFU (Least Frequently Used) – Removes the least frequently accessed entry.
File Access Simulation – Loads requested files into cache, tracks cache hits/misses, and automatically creates files if they don’t exist.
Automated Memory Usage Analysis – A shell script gathers real-time memory statistics, logging RAM usage and available memory.
Performance Visualization with Python – The cache hit/miss ratio is analyzed and represented using pie charts with Matplotlib.
Session-Based Simulation – Allows users to run multiple cache simulations with different policies and view detailed performance metrics.
CSV Logging for Analysis – Cache statistics (hits, misses) are stored in stats.csv for further evaluation.
This project combines Operating Systems, File Handling, Synchronization, and Data Visualization, making it a valuable tool for understanding cache replacement policies and their impact on system performance.
