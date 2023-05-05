from pydantic import BaseModel


class NTPServers(BaseModel):
    uk: str
    uk0: str
    uk1: str
    uk2: str
    uk3: str


class NTPConfig(BaseModel):
    servers: NTPServers
