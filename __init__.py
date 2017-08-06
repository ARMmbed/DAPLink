import requests
x = requests.get('https://os.mbed.com/api/v3/platforms/')
for thing in x.json():
    for otherthing in thing['features']:
        if otherthing['name'] == 'DAPLink':
            print'- %s:\n  name: %s\n  product_code: %s\n  fw_name: \n  instructions: \n    windows: \n    osx: \n    linux: \n' % (thing['name'], thing['name'], thing['productcode'])
