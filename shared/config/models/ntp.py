from pydantic import BaseModel

class NTPConfig(BaseModel):
    servers: list[str]