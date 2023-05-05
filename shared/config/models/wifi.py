from pydantic import BaseModel


class WifiCredentialsConfig(BaseModel):
    ssid: str
    password: str


class WifiNetworksConfig(BaseModel):
    the_vale: WifiCredentialsConfig
    purley_park: WifiCredentialsConfig


class WifiConfig(BaseModel):
    networks: WifiNetworksConfig
