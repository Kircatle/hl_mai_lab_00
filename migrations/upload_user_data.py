import requests

send_packets_user = []
prev_line = ''
with open('000003_upload_user_data.up.sql', 'r') as file:
    while True:
        line = file.readline()
        if line == '' or line is None:
            break
        if line.find('VALUES') != -1 and prev_line.find('`delivery`.`user`') != -1:
            data = line[8:-3].split(',')
            packet = {
                    "id": "",
                    "login": "",
                    "password": "",
                    "first_name": "",
                    "last_name": "",
                    "email": "",
                    "title": ""
            }
            for field in data:
                packet["id"] = data[0].replace('"', '')
                packet['login'] = data[1].replace('"', '')
                packet['password'] = data[2].replace('"', '')
                packet['first_name'] = data[3].replace('"', '')
                packet['last_name'] = data[4].replace('"', '')
                packet['email'] = data[5].replace('"', '')
                packet['title'] = data[6].replace('"', '')
            send_packets_user.append(packet)
        prev_line = line
for packet in send_packets_user:
    print(packet)
    response = requests.post('http://localhost:5055/user',json=packet)
    print(response.json())
