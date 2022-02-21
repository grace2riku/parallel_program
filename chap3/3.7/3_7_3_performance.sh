#!/bin/bash
date
echo "EMPTY"
./3_7_3_performance_EMPTY
echo "MUTEX"
./3_7_3_performance_MUTEX
echo "RWLOCK"
./3_7_3_performance_RWLOCK
echo "RWLOCK_WR"
./3_7_3_performance_RWLOCK_WR
date