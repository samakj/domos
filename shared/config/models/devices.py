from pydantic import BaseModel


class DeviceConfig(BaseModel):
    hostname: str
    ip: str


class DevicesConfig(BaseModel):
    sandbox: DeviceConfig