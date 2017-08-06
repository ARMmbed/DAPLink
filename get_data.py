import requests
x = requests.get('https://www.mbed.com/api/v1/boards/')
for thing in x.json():
    for listothings in thing['features']:
        if listothings['type'] == 'CMSIS-DAP':
                 print('- %s:\n  name: %s\n  product_code: %s\n  fw_name: \n  logoURL: %s\n  website: %s\n  instructions: \n    osx: \n    windows: \n    linux: \n' %(thing['name'], thing['name'], thing['productcode'],thing['logoURL'],thing['website']))