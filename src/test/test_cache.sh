echo "------------------------------------------------------------------ cache ------------------------------------------------------------------"
wrk -d 10 -t 1 -c 1 --latency -s ./get_cache.lua http://localhost:5055/
wrk -d 10 -t 1 -c 10 --latency -s ./get_cache.lua http://localhost:5055/
wrk -d 10 -t 1 -c 100 --latency -s ./get_cache.lua http://localhost:5055/
wrk -d 10 -t 5 -c 10 --latency -s ./get_cache.lua http://localhost:5055/
wrk -d 10 -t 5 -c 100 --latency -s ./get_cache.lua http://localhost:5055/
wrk -d 10 -t 10 -c 10 --latency -s ./get_cache.lua http://localhost:5055/
wrk -d 10 -t 10 -c 100 --latency -s ./get_cache.lua http://localhost:5055/

echo "----------------------------------------------------------------- no-cache -----------------------------------------------------------------"

wrk -d 10 -t 1 -c 1 --latency -s ./get_no_cache.lua http://localhost:5055/
wrk -d 10 -t 1 -c 10 --latency -s ./get_no_cache.lua http://localhost:5055/
wrk -d 10 -t 1 -c 100 --latency -s ./get_no_cache.lua http://localhost:5055/
wrk -d 10 -t 5 -c 10 --latency -s ./get_no_cache.lua http://localhost:5055/
wrk -d 10 -t 5 -c 100 --latency -s ./get_no_cache.lua http://localhost:5055/
wrk -d 10 -t 10 -c 10 --latency -s ./get_no_cache.lua http://localhost:5055/
wrk -d 10 -t 10 -c 100 --latency -s ./get_no_cache.lua http://localhost:5055/