from pydantic import BaseModel

class WifiCredentialsConfig(BaseModel):
    ssid: str
    password: str

class WifiConfig(BaseModel):
    credentials: list[WifiCredentialsConfig]