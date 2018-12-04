import json
from lib.t32_data_receiver import T32DataReceiver
from lib.t32_conn_manager import T32ConnManager
from pprint import pprint

with open('message_format.json') as data_file:
    data = json.load(data_file)

for key, value in data.items():
    print("%s=(%d)" % (key, len(value)))
    for key in value:
        print("  %s"%key)






