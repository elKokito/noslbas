#!/bin/sh
rm out-*
(perf stat --repeat 10 mpirun -np 2 ./src/exchng --lib blocking --width 10 --height 10 -v > /dev/null ) > perfstat_blocking_np2_wh10.txt 2>&1
rm out-*
(perf stat -ecache-misses --repeat 10 mpirun -np 2 ./src/exchng --lib blocking --width 10 --height 10 -v > /dev/null ) > perfstat_cachemiss_blocking_np2_wh10.txt 2>&1
rm out-*
(perf stat --repeat 10 mpirun -np 2 ./src/exchng --lib combined --width 10 --height 10 -v > /dev/null ) > perfstat_combined_np2_wh10.txt 2>&1
rm out-*
(perf stat -ecache-misses --repeat 10 mpirun -np 2 ./src/exchng --lib combined --width 10 --height 10 -v > /dev/null ) > perfstat_cachemiss_combined_np2_wh10.txt 2>&1
rm out-*
(perf stat --repeat 10 mpirun -np 2 ./src/exchng --lib buffered --width 10 --height 10 -v > /dev/null ) > perfstat_buffered_np2_wh10.txt 2>&1
rm out-*
(perf stat -ecache-misses --repeat 10 mpirun -np 2 ./src/exchng --lib buffered --width 10 --height 10 -v > /dev/null ) > perfstat_cachemiss_buffered_np2_wh10.txt 2>&1
rm out-*
(perf stat --repeat 10 mpirun -np 2 ./src/exchng --lib ordered --width 10 --height 10 -v > /dev/null ) > perfstat_ordered_np2_wh10.txt 2>&1
rm out-*
(perf stat -ecache-misses --repeat 10 mpirun -np 2 ./src/exchng --lib ordered --width 10 --height 10 -v > /dev/null ) > perfstat_cachemiss_ordered_np2_wh10.txt 2>&1
rm out-*
(perf stat --repeat 10 mpirun -np 2 ./src/exchng --lib async --width 10 --height 10 -v > /dev/null ) > perfstat_async_np2_wh10.txt 2>&1
rm out-*
(perf stat -ecache-misses --repeat 10 mpirun -np 2 ./src/exchng --lib async --width 10 --height 10 -v > /dev/null ) > perfstat_cachemiss_async_np2_wh10.txt 2>&1
rm out-*
