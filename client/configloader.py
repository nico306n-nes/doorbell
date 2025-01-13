import json

class Config_loader:
    configs = ""
    config = {}

    def __init__(self, config: str):
        with open(config) as file:
            self.configs = file.read()
            self.config = json.loads(self.configs)
        


if __name__ == "__main__":
    with open("config.json") as config:
        configs = config.read()
        print(configs)