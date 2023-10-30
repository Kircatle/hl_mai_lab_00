local frandom = io.open("/dev/urandom", "rb")
local d = frandom:read(4)
math.randomseed(d:byte(1) + (d:byte(2) * 256) + (d:byte(3) * 65536) + (d:byte(4) * 4294967296))
ids = {
    "15d82ae6-1a47-473f-9423-fdf23e99164c",
    "670720e4-ed7c-48bc-a945-5fc3a87e21d3",
    "832035db-f5e6-46d6-bbe7-ca4ae2c4145d",
    "8b353ea6-bf9b-47eb-b9b8-6a1003a92615",
    "c9e0aff7-9b9c-4a24-b40a-e6c7e45f8402",
    "2e040d71-d9b7-4221-9bd5-3360c5e17a96",
    "6ea19130-6c03-4b21-8f0f-e7f5d8119fb9",
    "dab91bc4-cf02-44a1-a4fd-d936f3e59715",
    "fd378e59-87df-408e-85c8-46ff45c38b41"}
number =  math.random(1,9)
request = function()
    headers = {}
    headers["Content-Type"] = "application/json"
    headers["Cache-Control"] = "cache"
    body = ''
    return wrk.format("GET", "/user?id=".. ids[number], headers, body)
end