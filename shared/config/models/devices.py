from pydantic import BaseModel


class DeviceConfig(BaseModel):
    hostname: str
    ip: str
    ota_password: str


class DevicesConfig(BaseModel):
    sandbox: DeviceConfig
