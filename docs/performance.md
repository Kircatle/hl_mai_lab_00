
#   1 threads and 1 connections

## cache

Running 10s test


  |Thread Stats|   Avg   |   Stdev  |   Max   |+/- Stdev|
  |------------|---------|----------|---------|---------|
  |  Latency   |200.50us |  74.10us |  2.89ms |  91.83% |
  |  Req/Sec   |  4.78k  | 572.03   |  6.00k  |  68.32% | 
  
  
#### 48041 requests in 10.10s, 14.71MB read

#### Requests/sec:   4756.75

#### Transfer/sec:      1.46MB

## no-cache

Running 10s test @ 

  |Thread Stats|   Avg  |    Stdev  |   Max  | +/- Stdev |
  |------------|--------|-----------|--------|-----------|
  |  Latency   |  0.86ms|  251.25us |  7.38ms|   76.53%  |
  |  Req/Sec   |  1.13k |  121.37   |  1.49k |   67.00%  |
  

#### 11300 requests in 10.00s, 3.57MB read

#### Requests/sec:   1129.79

#### Transfer/sec:    365.21KB


# 1 threads and 10 connections
## cache


  |Thread Stats|   Avg   |   Stdev  |   Max  | +/- Stdev|
  |------------|---------|----------|--------|----------|
  |  Latency   |  1.36ms |   2.13ms | 32.28ms|   96.86% |
  |  Req/Sec   |  9.31k  |   0.95k  | 10.52k |   65.35% |


     
#### 93514 requests in 10.10s, 28.72MB read
#### Requests/sec:   9258.87
#### Transfer/sec:      2.84MB

## no-cache

  |Thread Stats |  Avg   |   Stdev  |   Max  | +/- Stdev|
  |-------------|--------|----------|--------|----------|
  |  Latency    | 4.29ms |   8.19ms | 83.14ms|   93.30% |
  |  Req/Sec    | 4.00k  | 731.15   |  4.78k |   81.00% |


####  39858 requests in 10.00s, 12.58MB read
#### Requests/sec:   3983.92
#### Transfer/sec:      1.26MB

# 5 threads and 10 connections

## cache

  |Thread Stats|   Avg   |   Stdev  |   Max  | +/- Stdev|
  |------------|---------|----------|--------|----------|
  |  Latency   |  1.52ms |   2.86ms | 37.93ms|   96.50% |
  |  Req/Sec   |  1.83k  | 213.21   |  2.07k |   80.20% |



  90914 requests in 10.01s, 27.88MB read
Requests/sec:   9084.46
Transfer/sec:      2.79MB


## no-cache

  |Thread Stats |  Avg   |   Stdev |    Max   |+/- Stdev |
  |-------------|--------|---------|----------|----------|
  |  Latency    | 3.87ms |   6.79ms|  63.40ms |  94.26%  |
  |  Req/Sec    |797.85  |  127.89 |    0.97k |   75.20% |




#### 39726 requests in 10.01s, 12.37MB read
#### Requests/sec:   3968.47
#### Transfer/sec:      1.24MB

# 10 threads and 10 connections
## cache
  |Thread Stats |  Avg   |   Stdev |    Max   |+/- Stdev|
  |-------------|--------|---------|----------|---------|
  |  Latency    | 1.70ms |   3.64ms|  51.20ms |  96.03% | 
  |  Req/Sec    | 0.90k  | 167.49  |   4.67k  |  87.01% |



#### 89364 requests in 10.10s, 27.53MB read
#### Requests/sec:   8848.73
#### Transfer/sec:      2.73MB

## no-cache

  |Thread Stats|   Avg   |   Stdev |    Max   |+/- Stdev|
  |------------|---------|---------|----------|---------|
  |  Latency   |  3.99ms |   7.04ms|  69.04ms |  94.02% |
  |  Req/Sec   |390.82   |  61.56  | 494.00   |  77.90% |


#### 38949 requests in 10.01s, 12.11MB read
#### Requests/sec:   3889.32
#### Transfer/sec:      1.21MB

