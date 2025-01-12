import requests
import json

def get_site_data(site="http://klokke.local/klokke"):
    data = requests.get(site)
    return data

def get_site_status(site="http://klokke.local/klokke"):
    status = get_site_data(site).status_code
    return status

def get_site_content(site="http://klokke.local/klokke"):
    content = get_site_data(site).text
    return content



if __name__ == "__main__":
    dicte = json.loads(get_site_content())
    print(dicte["diff"])
    