import requests

send_packets = []
prev_line = ''
with open('000002_upload_data.up.sql', 'r') as file:
    while True:
        line = file.readline()
        if line == '' or line is None:
            break
        if line.find('VALUES') != -1 and prev_line.find('`delivery`.`user`') != -1:
            data = line[8:-3].split(',')
            packet = {
                    "login": "",
                    "password": "",
                    "first_name": "",
                    "last_name": "",
                    "email": "",
                    "title": ""
            }
            for field in data:
                packet['login'] = data[1].replace('"', '')
                packet['password'] = data[2].replace('"', '')
                packet['first_name'] = data[3].replace('"', '')
                packet['last_name'] = data[4].replace('"', '')
                packet['email'] = data[5].replace('"', '')
                packet['title'] = data[6].replace('"', '')
            send_packets.append(packet)
        prev_line = line
for packet in send_packets:
    response = requests.post('http://localhost:5055/user',json=packet)
    print(response.json())
